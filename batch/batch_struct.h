//
//  batch_struct.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef batch_struct_h
#define batch_struct_h

typedef struct connect {
    unsigned char *url;
}Connect;

typedef struct link {
    unsigned char *shift_pattern;
    unsigned int shift_table[256];
    Connect *store_content;
    unsigned int link_count;
    unsigned int link_limit;
}Link;

typedef struct batch {
    Connect *queue;
    unsigned int batch_count;
    unsigned int batch_limit;
}Batch;

#endif /* batch_struct_h */
