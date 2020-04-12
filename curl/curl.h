//
//  curl.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef curl_h
#define curl_h

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "../struct.h"
#include "../batch/batch_struct.h"

int curl_site(unsigned char *, Link *link, unsigned int depth);

void init_string(string *str);

void free_string(string *str);

size_t callback(char *source, size_t size, size_t nmeb, string *html_content);

#endif /* curl_h */
