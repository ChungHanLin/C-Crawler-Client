//
//  main.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/10.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include <stdio.h>
#include <time.h>

#include "struct.h"
#include "socket.h"
#include "log/log.h"
#include "curl/md5.h"
#include "curl/curl.h"
#include "batch/batch.h"
#include "batch/batch_struct.h"
#include "db/db_struct.h"
#include "db/url_db.h"
#include "db/fail_db.h"
#include "db/seen_db.h"

unsigned char *extract_content(char *source, char *pattern);

int find_hash_index(unsigned char *url);

int main(int argc, const char * argv[]) {
    int i, res_code, hash_index;
    int clientFD = create_socket();
    char path_msg[] = "@Path: ";
    char init_msg[] = "@Initialize: ";
    char recv_buffer[BUFFER_SIZE];
    char send_buffer[BUFFER_SIZE];
    unsigned char *buffer;
    
    time_t time_stamp;
    struct sockaddr_in client_info;
    struct tm *begin_time, *end_time;
    
    Batch batch;
    Link link;
    SeenDB seen_db;
    FailDB fail_db;
    UrlDB url_db;
    Log log;
    
    init_socket(&client_info, "127.0.0.1");
    
    connect_server(clientFD, &client_info);
    
    // Insert account name
    printf("Please insert your name: ");
    scanf("%s", send_buffer);

    // The server will ask you to insert account name for the first time (Better in english)
    if (send(clientFD, &send_buffer, BUFFER_SIZE, 0) < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    
    
//    while (true) {
        init_log(&log, send_buffer);
        init_link(&link);
        init_batch(&batch);
        create_seen_db(&seen_db);
        create_fail_db(&fail_db);
        create_url_db(&url_db);
        bzero(send_buffer, sizeof(send_buffer));
        bzero(recv_buffer, sizeof(recv_buffer));
        // record begin time
        time(&time_stamp);
        begin_time = localtime(&time_stamp);
        
        fprintf(stderr, "Begin Time: %s\n", asctime(begin_time));
        log.begin_time = strdup(asctime(begin_time));

        if (recv(clientFD, &recv_buffer, BUFFER_SIZE, 0) < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        
        fprintf(stderr, "Initialization... ");
        // start crawling form the init url
        if (strstr(recv_buffer, init_msg)) {
            // collect link from first url
            buffer = extract_content(recv_buffer, init_msg);

            if ((res_code = curl_site(buffer, &link, 0) != 0)) {
                fprintf(stderr, "ERRORL Url access failed.\n");
            }
            // force write seen_db
            hash_index = find_hash_index(buffer);
            seen_db.table[hash_index] = write_seen_db(&seen_db, hash_index, buffer);
            
            // insert link queue into the batch, then destruct link and also init new one.
            sort(&link);
            force_write_batch_task(&batch, &link);
        }
        // crawling from the file
        else if (strstr(recv_buffer, path_msg)) {
            fprintf(stderr, "crawling from the file\n");
            buffer = extract_content(recv_buffer, path_msg);
            // load link queue
            load_batch_task(&batch, buffer);
        }
        
        fprintf(stderr, "Done!\n");
        
        // start crawling from batch giving information
        for (i = 0; i < batch.batch_count; i++) {
            res_code = curl_site(batch.queue[i].url, &link, 0);
            hash_index = find_hash_index(batch.queue[i].url);
            if (res_code != 0) {
                fail_db.table[hash_index] = write_fail_db(&fail_db, hash_index, batch.queue[i].url);
                continue;
            }
            seen_db.table[hash_index] = write_seen_db(&seen_db, hash_index, batch.queue[i].url);
        }
        sort(&link);
        write_url_db(&url_db, link, seen_db, fail_db);
        
        // record end crawling time
        time(&time_stamp);
        end_time = localtime(&time_stamp);
        
        fprintf(stderr, "Finish Time: %s\n", asctime(end_time));
        fprintf(stderr, "Generating log file... ");
        // Writing log file
        
        log.finish_time = strdup(asctime(end_time));
        log.recent_crawled_cnt = seen_db.size;
        log.recent_fail_cnt = fail_db.size;
        log.collect_url_cnt = url_db.size;
        
        write_log_file(log);
        
        fprintf(stderr, "Done!\n");
        
        flush_seen_db(&seen_db, log.seen_path);
        flush_fail_db(&fail_db, log.fail_path);
        flush_url_db(&url_db, log.collect_path);
        
        destruct_link(&link);
        destruct_batch(&batch);
    
        sprintf(send_buffer, "@Done");
        if (send(clientFD, &send_buffer, BUFFER_SIZE, 0) < 0) {
            perror("send");
        }
//    }
    
    // Same account has multiple same attribute, so can be destructed before client ended.
    destruct_log(&log);
    
    close(clientFD);
    
    fprintf(stderr, "Done!\n");
    return 0;
}

unsigned char *extract_content(char *source, char *pattern) {
    char buffer[BUFFER_SIZE];
    int i, j;
    
    for (i = strlen(pattern), j = 0; source[i] != '\0'; i++, j++) {
        buffer[j] = source[i];
    }
    buffer[j] = '\0';
    unsigned char *content = strdup(buffer);
    return content;
}

int find_hash_index(unsigned char *url) {
    char *md5;
    encode_md5(url, &md5);
    int hash_index = hash_func(md5);
    free(md5);
    return hash_index;
}
