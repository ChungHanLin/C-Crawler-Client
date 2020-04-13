//
//  log.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "log.h"

void init_log(Log *log, char *account) {
    char buffer[BUFFER_SIZE];
    char path[BUFFER_SIZE];
    FILE *fp;
    
    log->account = strdup(account);
    log->ip = strdup("127.0.0.1");
    log->port_num = PORT_NUM;
    
    sprintf(path, "../C-Crawler-Server/log/account/%s.log", account);
    fp = fopen(path, "r");
    
    // Log has existed, extrieve original content
    if (fp) {
        while (fgets(buffer, BUFFER_SIZE, fp)) {
            if (strstr(buffer, "Begin time: ")){
                // 紀錄上一次 start time, 輸入此次 "last crawled time" field
                log->last_crawled_time = extract_log_attribute(buffer, "Begin time: ");
            }
            else if (strstr(buffer, "Total crawled count: ")) {
                log->total_crawled_cnt = extract_number(buffer, "Total crawled count: ");
            }
            else if (strstr(buffer, "Total fail count: ")) {
                log->total_fail_cnt = extract_number(buffer, "Total fail count: ");
            }
            else if (strstr(buffer, "Success crawled link: ")) {
                log->seen_path = extract_log_attribute(buffer, "Success crawled link: ");
            }
            else if (strstr(buffer, "Fail crawled link: ")) {
                log->fail_path = extract_log_attribute(buffer, "Fail crawled link: ");
            }
            else if (strstr(buffer, "Collected link: ")) {
                log->collect_path = extract_log_attribute(buffer, "Collected link: ");
            }
        }
        fclose(fp);
    }
    else {
        log->total_crawled_cnt = 0;
        log->total_fail_cnt = 0;
        log->last_crawled_time = NULL;
        bzero(path, sizeof(path));
        sprintf(path, "../C-Crawler-Server/log/success/%s.log", account);
        log->seen_path = strdup(path);
        
        bzero(path, sizeof(path));
        sprintf(path, "../C-Crawler-Server/log/fail/%s.log", account);
        log->fail_path = strdup(path);
        
        bzero(path, sizeof(path));
        sprintf(path, "../C-Crawler-Server/log/collect/%s.log", account);
        log->collect_path = strdup(path);
    }
}

char *extract_log_attribute(char *source, char *target) {
    char buffer[BUFFER_SIZE];
    int i, j;
    
    for (i = strlen(target), j = 0; source[i] != '\n' && source[i] != '\0'; i++, j++) {
        buffer[j] = source[i];
    }
    buffer[j] = '\0';
    char *attribute = strdup(buffer);
    
    return attribute;
}

int extract_number(char *source, char *target) {
    int num = 0;
    int i;
    
    for (i = strlen(target); source[i] != '\n' && source[i] != '\0'; i++) {
        num = num * 10 + (source[i] - '0');
    }
    
    return num;
}

void write_log_file(Log log) {
    char path[BUFFER_SIZE];
    FILE *fp;
    sprintf(path, "../C-Crawler-Server/log/account/%s.log", log.account);
    
    fp = fopen(path, "w");
    fprintf(fp, "Account: %s\n", log.account);
    fprintf(fp, "Begin time: %s", log.begin_time);
    fprintf(fp, "Finish time: %s", log.finish_time);
    if (log.last_crawled_time) {
        fprintf(fp, "Last crawled time: %s\n", log.last_crawled_time);
    }
    fprintf(fp, "IP: %s\n", log.ip);
    fprintf(fp, "Port: %d\n", log.port_num);
    fprintf(fp, "Total crawled count: %d\n", log.total_crawled_cnt + log.recent_crawled_cnt);
    fprintf(fp, "Recent crawled count: %d\n", log.recent_crawled_cnt);
    fprintf(fp, "Success crawled link: %s\n", log.seen_path);
    fprintf(fp, "Total fail count: %d\n", log.total_fail_cnt + log.recent_fail_cnt);
    fprintf(fp, "Recent fail count: %d\n", log.recent_fail_cnt);
    fprintf(fp, "Fail crawled link: %s\n", log.fail_path);
    fprintf(fp, "Collected link count: %d\n", log.collect_url_cnt);
    fprintf(fp, "Collected link: %s\n", log.collect_path);
    fclose(fp);
}

void destruct_log(Log *log) {
    free(log->account);
    free(log->ip);
    free(log->seen_path);
    free(log->fail_path);
    free(log->collect_path);
    
    if (log->last_crawled_time) {
        free(log->last_crawled_time);
    }
}
