//
//  curl_struct.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef curl_struct_h
#define curl_struct_h

typedef struct shift {
    unsigned char *store_content;   // 從 html content 擷取出的資料
    unsigned char *shift_pattern;         // shift pattern
    unsigned int shift_table[256];
}Shift;

typedef struct tag {
    Shift title;    // maybe h1 tag is another way to get the title of the article
    Shift img;  // meta
    Shift url;  // meta
    Shift p;   // maybe there are multiple p tag in an article
}Tag;

#endif /* curl_struct_h */
