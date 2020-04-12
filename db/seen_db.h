//
//  seen_db.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef seen_db_h
#define seen_db_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "db_struct.h"

void create_seen_db(SeenDB *db);

DB *write_seen_db(SeenDB *db, int hash_index, unsigned char *url);

void flush_seen_db(SeenDB *db, char *path);

#endif /* seen_db_h */
