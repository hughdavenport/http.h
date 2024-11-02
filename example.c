/* Depends on https://github.com/hughdavenport/url.h for URL parsing */
#define URL_IMPLEMENTATION
#include "url.h"

#define HTTP_IMPLEMENTATION
#include "http.h"

#include <stdio.h>
#include <sysexits.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s url\n", argv[0]);
        return EX_USAGE;
    }

    HttpUserAgent agent = {         // These options can be set
        .name = NULL,               //  or the argument passed
        .cookie_jar = NULL,         //  as NULL. The library will
    };                              //  do as you expect ;)

    int ret = EX_DATAERR;
    HttpHeaders headers = {0};
    if (!add_http_headers(&headers,
            HTTP_HEADER("X-http-h-version", HTTP_H_VERSION),
            HTTP_HEADER("X-http-h-url", "https://github.com/hughdavenport/http.h")
    )) {
        fprintf(stderr, "Could not add headers\n");
        goto end;
    }
    if (!add_http_headers(&headers,
            HTTP_HEADER("X-http-h-version-major", HTTP_H_VERSION_MAJOR_S),
            HTTP_HEADER("X-http-h-version-minor", HTTP_H_VERSION_MINOR_S),
            HTTP_HEADER("X-http-h-version-patch", HTTP_H_VERSION_PATCH_S)
    )) {
        fprintf(stderr, "Could not add headers\n");
        goto end;
    }

    URL url = {0};
    if (!parse_url(argv[1], argv[1] + strlen(argv[1]), &url)) {
        fprintf(stderr, "Invalid URL: %s.\n", argv[1]);
        goto end;
    }

    ret = EX_UNAVAILABLE;
    HttpResponse response = {0};
    if (!send_http_request(&url, &agent, &headers, &response)) {
        fprintf(stderr, "Error occurred while sending request.\n");
        goto end;
    }

    if (response.body) printf("%s", response.body);

    ret = EX_OK;
end:
    free_http_headers(&headers);
    free_http_response(&response);
    return ret;
}
