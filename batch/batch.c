//
//  batch.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "batch.h"

void init_link(Link *link) {
    char a[] = "<a";
    
    link->link_count = 0;
    link->link_limit = BATCH_NUM;
    link->shift_pattern = (unsigned char *) strdup(a);
    link->store_content = (Connect *) malloc(sizeof(Connect) * link->link_limit);
    
    create_shift_table(link->shift_table, link->shift_pattern);
}

void init_batch(Batch *batch) {
    batch->batch_count = 0;
    batch->batch_limit = BATCH_NUM;
    batch->queue = (Connect *) malloc(sizeof(Connect) * batch->batch_limit);
}

int cmp(const void *a, const void *b) {
    Connect *x = (Connect *)a;
    Connect *y = (Connect *)b;
    int compare = strcmp(x->url, y->url);
    
    if (compare > 0) {
        return 1;
    }
    else {
        return -1;
    }
}

void sort(Link *link) {
    qsort(link->store_content, link->link_count, sizeof(Connect), cmp);
}

void load_batch_task(Batch *batch, unsigned char *path) {
    FILE *fp = fopen(path, "r");
    unsigned char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, fp)) {
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }
        
        batch->queue[batch->batch_count].url = strdup(buffer);
        batch->batch_count++;
        if (batch->batch_count == batch->batch_limit) {
            batch->batch_limit *= 2;
            batch->queue = (Connect *) realloc(batch->queue, batch->batch_limit * sizeof(Connect));
        }
    }
    
    fclose(fp);
}

void force_write_batch_task(Batch *batch, Link *link) {
    int i, j = 0;
    
    if (link->link_count != 0) {
        batch->queue[j].url = strdup(link->store_content[0].url);
        
        // Filtering same url, because after sorting same url will be put together.
        for (i = 1; i < link->link_count; i++) {
            if (strcmp(batch->queue[j].url, link->store_content[i].url) == 0) {
                continue;
            }
            j++;
            batch->queue[j].url = strdup(link->store_content[i].url);
            if (j == batch->batch_limit) {
                batch->batch_limit *= 2;
                batch->queue = (Connect *) realloc(batch->queue, batch->batch_limit * sizeof(Connect));
            }
        }
        batch->batch_count = j;
        destruct_link(&(*link));
        init_link(&(*link));
    }
}

void destruct_link(Link *link) {
    int i;
    for (i = 0; i < link->link_count; i++) {
        free(link->store_content[i].url);
    }
    
    free(link->shift_pattern);
    free(link->store_content);
}

void destruct_batch(Batch *batch) {
    int i;
    for (i = 0; i < batch->batch_count; i++) {
        free(batch->queue[i].url);
    }
    free(batch->queue);
}
