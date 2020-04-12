//
//  fail_db.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef fail_db_h
#define fail_db_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "../struct.h"
#include "db_struct.h"

void create_fail_db(FailDB *db);

DB *write_fail_db(FailDB *db, int hash_index, unsigned char *url);

void flush_fail_db(FailDB *db, char *path);

#endif /* fail_db_h */
