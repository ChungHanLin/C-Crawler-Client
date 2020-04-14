//
//  curl.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "curl.h"

int curl_site(unsigned char *url, Link *link, unsigned int depth) {
    CURL *curl = curl_easy_init();
    CURLcode res;
    string html_content;
    bool success;
    
    // Initialize string
    init_string(&html_content);
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html_content);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            // Extract attribute
            fprintf(stderr, "Current: %s\n", url);
            success = extract_attribute(html_content, link, url, depth);
            
            if (!success) {
                // Add url in to the fail_db
                return -1;
            }
        }
        curl_easy_cleanup(curl);
    }
    free_string(&html_content);
    
    return res;
}

void init_string(string *str) {
    str->len = 0;
    str->data = (unsigned char *)malloc(str->len + 1);
    if (!str->data) {
        fprintf(stderr, "ERROR: Malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    str->data[0] = '\0';
}

void free_string(string *str) {
    free(str->data);
}


size_t callback(char *source, size_t size, size_t nmeb, string *html_content) {
    html_content->data = (unsigned char *) realloc (html_content->data, html_content->len + size * nmeb);
    if (!html_content->data) {
        fprintf(stderr, "ERROR: Realloc failed.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(html_content->data + html_content->len, source, size * nmeb);
    html_content->len += (size * nmeb);
    
    return size * nmeb;
}
