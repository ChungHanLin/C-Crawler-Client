//
//  md5.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "md5.h"

void encode_md5(unsigned char *url, char **md5) {
    unsigned char digest[CC_MD5_DIGEST_LENGTH];
    *md5 = (char *) malloc (33);

    CC_MD5_CTX context;
    CC_MD5_Init(&context);
    CC_MD5_Update(&context, url, (CC_LONG)strlen(url));
    CC_MD5_Final(digest, &context);
    
    int i, c;
    for(i = 0; i < CC_MD5_DIGEST_LENGTH; i++) {
        c = (int) digest[i];
        hex_to_char(c, &(*md5)[i * 2], &(*md5)[(i * 2) + 1]);
    }
    (*md5)[32] = '\0';
}

void hex_to_char(int c, char *x, char *y) {
    int a = c / 16;
    int b = c % 16;
    
    if (a >= 10) {
        *x = 'a' + (a - 10);
    }
    else {
        *x = '0' + a;
    }
    
    if (b >= 10) {
        *y = 'a' + (b - 10);
    }
    else {
        *y = '0' + b;
    }
}
