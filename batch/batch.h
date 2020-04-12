//
//  batch.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef batch_h
#define batch_h

#include <stdio.h>

#include "../struct.h"
#include "../search.h"
#include "batch_struct.h"

#define BUFFER_SIZE 1024

void init_link(Link *link);

void init_batch(Batch *batch);

int cmp(const void *a, const void *b);

void sort(Link *link);

void load_batch_task(Batch *batch, unsigned char *path);

void force_write_batch_task(Batch *batch, Link *link);

void destruct_link(Link *link);

void destruct_batch(Batch *batch);

#endif /* batch_h */
