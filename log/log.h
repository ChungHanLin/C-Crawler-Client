//
//  log.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef log_h
#define log_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../socket.h"

typedef struct log {
    char *begin_time;
    char *finish_time;
    char *last_crawled_time;
    char *ip;
    char *account;
    char *seen_path;
    char *fail_path;
    char *collect_path;
    int port_num;
    int total_crawled_cnt;
    int recent_crawled_cnt;
    int total_fail_cnt;
    int recent_fail_cnt;
    int collect_url_cnt;
}Log;

void init_log(Log *log, char *account);

char *extract_log_attribute(char *source, char *target);

int extract_number(char *source, char *target);

void write_log_file(Log log);

void destruct_log(Log *log);

#endif /* log_h */
