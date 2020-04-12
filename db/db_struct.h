//
//  db_struct.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef db_struct_h
#define db_struct_h

#include <stdio.h>

#include "../struct.h"
#include "../log/log.h"

// Using linked-list hash instead of linear-probing
typedef struct db{
    unsigned char *url;
    struct db *next;
}DB;

// Record the link that has been visited.
// Using "Hash" data strcture for storing data.
typedef struct seenDB {
    DB **table;
    unsigned int size;
}SeenDB;

// Link queue -> record depth as the end condition of the crawler
// Using "Hash" data structure for storing data.
typedef struct urlDB {
    DB **table;
    unsigned int size;
}UrlDB;

// Stroing access failed url.
typedef struct failDB {
    DB **table;
    unsigned int size;
}FailDB;

#endif /* db_struct_h */
