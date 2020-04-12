//
//  md5.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef md5_h
#define md5_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 工作站其實有 <openssl/md5.h>
#include <CommonCrypto/CommonDigest.h>

void encode_md5(unsigned char *url, char **md5);

void hex_to_char(int c, char *x, char *y);

#endif /* md5_h */
