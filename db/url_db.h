//
//  url_db.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef url_db_h
#define url_db_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "fail_db.h"
#include "db_struct.h"
#include "../struct.h"
#include "../curl/md5.h"
#include "../batch/batch_struct.h"

void create_url_db(UrlDB *db);

void write_url_db(UrlDB *url_db, Link link, SeenDB seen_db, FailDB fail_db);

void flush_url_db(UrlDB *db, char *path);

#endif /* url_db_h */
