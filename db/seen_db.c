//
//  seen_db.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "seen_db.h"

void create_seen_db(SeenDB *db) {
    db->size = 0;
    
    // Initial hash table
    db->table = (DB **) malloc (sizeof(DB *) * MAX_DB_HASH);
    
    // 先預設所有 hash table 指向 NULL
    int i;
    for (i = 0; i < MAX_DB_HASH; i++) {
        db->table[i] = NULL;
    }
}

DB *write_seen_db(SeenDB *db, int hash_index, unsigned char *url) {
    DB *head = insert(db->table[hash_index], url);
    
    db->size++;
    return head;
}

void flush_seen_db(SeenDB *db, char *path) {
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
