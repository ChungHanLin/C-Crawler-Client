//
//  search.h
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#ifndef search_h
#define search_h

#include <stdio.h>
#include "struct.h"

bool compare(char pattern_char, char text_char);

int boyer_moore(unsigned char *pattern, unsigned char *text, unsigned int *shift_table, int text_begin, int text_end);

void create_shift_table(unsigned int *shift_table, unsigned char *pattern);

#endif /* search_h */
