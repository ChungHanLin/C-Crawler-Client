//
//  struct.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/10.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef struct_h
#define struct_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

#define BATCH_NUM 1024
#define MAX_LINK_DEPTH 2

typedef unsigned int bool;

typedef struct string {
    unsigned char *data;
    size_t len;
}string;

#endif /* struct_h */
