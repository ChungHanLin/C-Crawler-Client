//
//  url.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef url_h
#define url_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../struct.h"

bool legal_link_format(unsigned char *url);

int indexOfDomain(unsigned char *url);

bool get_full_link(unsigned char *s_url, unsigned char *t_url, unsigned char **store);

unsigned char *get_protocol(unsigned char *s_url, unsigned char *t_url);

unsigned char *get_server(unsigned char *s_url, unsigned char *t_url);

unsigned char *get_path(unsigned char *t_url);

#endif /* url_h */
