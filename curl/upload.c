//
//  upload.c
//  c_spider
//
//  Created by 林重翰 on 2020/4/14.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "upload.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    return size * nmemb;
}

int upload_to_db(Tag tag) {
    
    unsigned char *title = tag.title.store_content;
    unsigned char *url = tag.url.store_content;
    unsigned char *img = tag.img.store_content;
    unsigned char *content = tag.p.store_content;
    unsigned int title_len;

    if (!title) {
        title_len = 0;
    }
    else {
        title_len = strlen(title);
    }

    unsigned int img_len;

    if (!img) {
        img_len = 0;
    }
    else {
        img_len = strlen(img);
    }
    unsigned int content_len = 0;

    if (!content) {
        content_len = 0;
    }
    else {
        content_len = strlen(content);
    }

    unsigned int len = title_len + strlen(url) + img_len + content_len + 100;
    unsigned char *json = (unsigned char *) malloc (len);
    sprintf(json, "{\"title\":\"%s\",\"url\":\"%s\",\"img\":\"%s\",\"content\":\"%s\"}", title, url, img, content);
    
    CURL *curl = curl_easy_init();
    CURLcode res;
    struct curl_slist *plist = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9200/customer/es");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) 
		{
			fprintf(stderr, "ERROR: Index to elastic search failed.\n");
		}
        
    }
    curl_slist_free_all(plist);
    curl_easy_cleanup(curl);
    free(json);
    return 0;
}