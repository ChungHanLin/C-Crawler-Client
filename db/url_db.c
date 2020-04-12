//
//  url_db.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "url_db.h"

void create_url_db(UrlDB *db) {
    db->size = 0;
    
    // Initial hash table
    db->table = (DB **) malloc (sizeof(DB *) * MAX_DB_HASH);
    
    int i;
    for (i = 0; i < MAX_DB_HASH; i++) {
        db->table[i] = NULL;
    }
}

void write_url_db(UrlDB *url_db, Link link, SeenDB seen_db, FailDB fail_db) {
    int i, hash_index;
    char *md5;
    unsigned char *url;
    
    url = link.store_content[0].url;
    
    for (i = 1; i < link.link_count; i++) {
        if (strcmp(url, link.store_content[i].url) == 0) {
            continue;
        }
        url = link.store_content[i].url;
        encode_md5(url, &md5);
        hash_index = hash_func(md5);
        free(md5);
        if (find_in_db(seen_db.table[hash_index], url) || find_in_db(fail_db.table[hash_index], url)) {
            continue;
        }
        url_db->table[hash_index] = insert(url_db->table[hash_index], link.store_content[i].url);
        url_db->size++;
    }
}

void flush_url_db(UrlDB *db, char *path) {
    int i;
    DB *cursor, *prev;
    FILE *fp = fopen(path, "w");
    
    for (i = 0; i < MAX_DB_HASH; i++) {
        if (db->table[i]) {
            cursor = db->table[i];
            while (cursor) {
                fprintf(fp, "%s\n", cursor->url);
                free(cursor->url);
                prev = cursor;
                cursor = cursor->next;
                free(prev);
            }
            db->table[i] = NULL;
        }
    }
    free(db->table);
    fclose(fp);
}
