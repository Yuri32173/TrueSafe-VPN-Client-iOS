#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define kHTTPS_DNS "www.apache.org"
#define kHTTPS_Port 443
#define kHTTPS_URI "/"

void printError(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
}

void printSSLError(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, ERR_error_string(ERR_get_error(), NULL));
}

int main() {
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    int sockfd = -1, n;
    char request[512];
    char response[4096];

    SSL_library_init();
    ERR_load_crypto_strings();
    ERR_load_SSL_strings();

    ctx = SSL_CTX_new(TLS_client_method());
    if (ctx == NULL) {
        printSSLError("SSL_CTX_new");
        goto EXIT;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printError("socket");
        goto EXIT;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(kHTTPS_Port);
    if (inet_pton(AF_INET, kHTTPS_DNS, &serveraddr.sin_addr) <= 0) {
        printError("inet_pton");
        goto EXIT;
    }

    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        printError("connect");
        goto EXIT;
    }

    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        printSSLError("SSL_new");
        goto EXIT;
    }

    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) <= 0) {
        printSSLError("SSL_connect");
        goto EXIT;
    }

    snprintf(request, sizeof(request), "GET %s HTTP/1.0\r\n\r\n", kHTTPS_URI);
    if (SSL_write(ssl, request, strlen(request)) < 0) {
        printSSLError("SSL_write");
        goto EXIT;
    }

    while ((n = SSL_read(ssl, response, sizeof(response)-1)) > 0) {
        response[n] = '\0';
        printf("%s", response);
    }

    if (n < 0) {
        printSSLError("SSL_read");
        goto EXIT;
    }

EXIT:
    if (ssl != NULL) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    if (ctx != NULL) {
        SSL_CTX_free(ctx);
    }
    if (sockfd >= 0) {
        close(sockfd);
    }

    return 0;
}
