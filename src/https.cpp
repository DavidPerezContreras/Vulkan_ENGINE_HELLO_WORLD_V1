#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 443 // HTTPS port
#define NUM_REQUESTS 1
#define BUFFER_SIZE 4096

// Struct to hold data for each request
typedef struct {
    const char *hostname;
    const char *request;
    char response[BUFFER_SIZE]; // Buffer to hold response
    int thread_id;
} RequestData;

// Function to handle each request using OpenSSL
void *handle_request(void *arg) {
    RequestData *data = (RequestData *)arg;

    SSL_library_init();  // Initialize the OpenSSL library
    SSL_load_error_strings(); // Load error strings for error reporting
    const SSL_METHOD *method = TLS_client_method();  // Define the TLS/SSL method
    SSL_CTX *ctx = SSL_CTX_new(method);  // Create a new SSL context

    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        pthread_exit(NULL);
    }

    SSL *ssl;
    int server;
    struct hostent *host;
    struct sockaddr_in addr;

    host = gethostbyname(data->hostname);
    if (host == NULL) {
        fprintf(stderr, "Error: Unable to resolve hostname\n");
        pthread_exit(NULL);
    }

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("socket");
        pthread_exit(NULL);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = *(long *)(host->h_addr);

    if (connect(server, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("connect");
        close(server);
        pthread_exit(NULL);
    }

    ssl = SSL_new(ctx);  // Create a new SSL structure for the connection
    SSL_set_fd(ssl, server);  // Associate the socket file descriptor with the SSL structure

    if (SSL_connect(ssl) == -1) {  // Initiate the TLS/SSL handshake
        ERR_print_errors_fp(stderr);
        close(server);
        pthread_exit(NULL);
    }

    SSL_write(ssl, data->request, strlen(data->request));  // Send the HTTP request over the SSL connection

    int bytes;
    char buf[BUFFER_SIZE];
    bytes = SSL_read(ssl, buf, sizeof(buf));  // Read the response into the buffer
    if (bytes > 0) {
        buf[bytes] = 0;
        strncpy(data->response, buf, BUFFER_SIZE - 1);
    } else {
        ERR_print_errors_fp(stderr);
    }

    SSL_free(ssl);  // Free the SSL structure
    close(server);  // Close the socket
    SSL_CTX_free(ctx);  // Free the SSL context

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_REQUESTS];
    RequestData request_data[NUM_REQUESTS];
    const char *hostname = "task.davidperezcontreras.com";
    const char *https_request = "GET / HTTP/1.1\r\nHost: task.davidperezcontreras.com\r\nConnection: close\r\n\r\n";

    // Initialize request data and create threads
    for (int i = 0; i < NUM_REQUESTS; i++) {
        request_data[i].hostname = hostname;
        request_data[i].request = https_request;
        request_data[i].thread_id = i;
        memset(request_data[i].response, 0, BUFFER_SIZE);

        if (pthread_create(&threads[i], NULL, handle_request, &request_data[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_REQUESTS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print all responses after all threads are completed
    for (int i = 0; i < NUM_REQUESTS; i++) {
        printf("Response from thread %d:\n%s\n", request_data[i].thread_id, request_data[i].response);
    }

    return 0;
}
