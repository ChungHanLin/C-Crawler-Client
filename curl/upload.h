//
//  upload.h
//  c_spider
//
//  Created by 林重翰 on 2020/4/14.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef upload_h
#define upload_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "../struct.h"
#include "curl_struct.h"

int upload_to_db(Tag tag);

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

#endif