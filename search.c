//
//  search.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "search.h"

static inline int isAlpha(char c) {
    if (c >= 'a' && c <= 'z'){
        return 1;
    }
    else if (c >= 'A' && c <= 'Z') {
        return 2;
    }
    return 0;
}

inline bool compare(char pattern_char, char text_char) {
    int mode;
    if (pattern_char == text_char) {
        return true;
    }
    else if ((mode = isAlpha(pattern_char)) > 0) {
        if (mode == 1){
            return (pattern_char - 32) == text_char;
        }
        else {
            return (pattern_char + 32) == text_char;
        }
    }
    return false;
}

inline int boyer_moore(unsigned char *pattern, unsigned char *text, unsigned int *shift_table, int text_begin, int text_end) {
    int i, match_index, match_cnt, shift;
    int pattern_length = strlen(pattern);
    bool match = true;
    
    match_cnt = 0;
    
    for (i = text_begin; i < text_end - pattern_length + 1; i+=shift) {
        match = true;

        for (match_index = pattern_length - 1; match_index >= 0; match_index--) {
            if (!compare(pattern[match_index], text[i + match_index])) {
                match = false;
                break;
            }
        }
        if (match) {
            shift = 1;
            break;
        }
        else {
            shift = shift_table[text[i + pattern_length - 1]];
        }
    }
    if (!match) {
        return -1;
    }
    
    return i;
}

inline void create_shift_table(unsigned int *shift_table, unsigned char *pattern) {
    int prefix_index;
    int shift = 1;
    int pattern_length = strlen(pattern);

    int i;
    for (i = 0; i < 256; i++) {
        shift_table[i] = pattern_length;
    }

    for (prefix_index = pattern_length - 2; prefix_index >= 0; prefix_index--) {
        if (shift < shift_table[pattern[prefix_index]]) {
            shift_table[pattern[prefix_index]] = shift;
        }
        shift++;
    }
}
