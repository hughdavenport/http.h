# http.h: A HTTP stb-style header only library

[![patreon](https://img.shields.io/badge/patreon-FF5441?style=for-the-badge&logo=Patreon)](https://www.patreon.com/hughdavenport)
[![youtube](https://img.shields.io/badge/youtube-FF0000?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=dqw7B6eR9P8&list=PL5r5Q39GjMDfetFdGmnhjw1svsALW1HIY)

This repo contains a [stb-style](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt) header only library. This library depends on [url.h](https://github.com/hughdavenport/url.h) of atleast version `1.0.0`. Along side that, you need [http.h](https://github.com/hughdavenport/http.h/raw/refs/heads/main/http.h).

This was developed during a [YouTube series](https://www.youtube.com/watch?v=dqw7B6eR9P8&list=PL5r5Q39GjMDfetFdGmnhjw1svsALW1HIY) where I implement [bittorrent from scratch](https://github.com/hughdavenport/codecrafters-bittorrent-c), where [SHA-1](https://github.com/hughdavenport/sha1.h), and [URL parsing](https://github.com/hughdavenport/url.h/raw/refs/heads/main/url.h) and [HTTP communication](https://github.com/hughdavenport/http.h) is a necessary component.

To use the library, ensure [url.h](https://github.com/hughdavenport/url.h) has been setup then `#define HTTP_IMPLEMENTATION` exactly once (in your main.c may be a good place). You can `#include` the file as many times as you like.

An example file is shown below.
```c
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
```

Test by saving the above to a file `example.c` and then running this
```sh
cc example.c
./a.out http://httpbin.org/headers
```

It should give output similar to:
```
Different HTTP minor version 1.1
Content-Length: 379
{
  "headers": {
    "Accept": "*/*",
    "Host": "httpbin.org",
    "User-Agent": "curl/7.81.0",
    "X-Amzn-Trace-Id": "Root=1-67257586-6663a25640d0b042333ba5ee",
    "X-Http-H-Url": "https://github.com/hughdavenport/http.h",
    "X-Http-H-Version": "1.0.0",
    "X-Http-H-Version-Major": "1",
    "X-Http-H-Version-Minor": "0",
    "X-Http-H-Version-Patch": "0"
  }
}
```

I don't really guarantee the security of this, yet. Please feel free to try find bugs and report them.

Please leave any comments about what you liked. Feel free to suggest any features or improvements.

You are welcome to support me financially if you would like on my [patreon](https://www.patreon.com/hughdavenport).
