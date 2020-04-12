//
//  tag.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef tag_h
#define tag_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "url.h"
#include "md5.h"
#include "../struct.h"
#include "../search.h"
#include "curl_struct.h"
#include "../batch/batch_struct.h"

bool extract_attribute(string html_content, Link *link, unsigned char *url, unsigned int depth);

void tag_init(Tag *tag);

void search_link(string html_content, unsigned char * c_url, unsigned int depth, Link **link, unsigned char *content_begin);

unsigned char *search_attr(string html_content, Shift shift, unsigned char *content_begin);

unsigned char *search_article(string html_content, Shift shift, unsigned char *close_tag);

int locate_content(unsigned char *data, int start_index, int end_index, unsigned char *content_begin);

void tag_destructor(Tag *tag);

void link_destructor(Link **link);

#endif /* tag_h */
