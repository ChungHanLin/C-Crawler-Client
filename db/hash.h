//
//  hash.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef hash_h
#define hash_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db_struct.h"

// Temporarily
#define MAX_DB_HASH 1024

unsigned int hash_func(char *data);

DB *insert(DB *head, unsigned char *url);

bool find_in_db(DB *head, unsigned char *url);

#endif /* hash_h */
