//
//  tag.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "tag.h"

static inline bool isTail(unsigned char *data, int index) {
    if (data[index] == '<') {
        return true;
    }
    else if (data[index] == '"') {
        return true;
    }
    else if (data[index] == '/' && data[index] == '>') {
        return true;
    }
    
    return false;
}

static inline bool isEscape(unsigned char c) {
    if ((c >= 0 && c <= 31) || c == 127) {
        return true;
    }
    return false;
}

static inline bool isSpace(unsigned char c) {
    if (c == 32) {
        return true;
    }
    return false;
}

static inline unsigned int find_close_tag(unsigned char *data, int start_index) {
    while (data[start_index] != '>') {
        start_index++;
    }
    
    return ++start_index;
}

bool extract_attribute(string html_content, Link *link, unsigned char *url, unsigned int depth) {
    if (html_content.len == 0) {
        return false;
    }
    Tag tag;
    
    tag_init(&tag);
    
    tag.title.store_content = search_article(html_content, tag.title, (unsigned char *) "</title>");
    tag.img.store_content = search_attr(html_content, tag.img, (unsigned char *) "content=\"");
    tag.url.store_content = search_attr(html_content, tag.url, (unsigned char *) "content=\"");
    tag.p.store_content = search_article(html_content, tag.p, (unsigned char *) "</p>");
    if (tag.p.store_content == '\0') {
        tag_destructor(&tag);
        return false;
    }
    
    if (depth + 1 < MAX_LINK_DEPTH) {
        search_link(html_content, url, depth, &link,(unsigned char *) "href=\"");
    }
    tag_destructor(&tag);
    
    return true;
}

// Unsure if it`s compatible to the others website or not
// <title> tag
void tag_init(Tag *tag) {
    char title[] = "<title";
    char img[] = "<meta property=\"og:image\"";
    char url[] = "<meta property=\"og:url\"";
    char p[] = "<p";
    
    tag->title.shift_pattern = (unsigned char *) strdup(title);
    tag->img.shift_pattern = (unsigned char *) strdup(img);
    tag->url.shift_pattern = (unsigned char *) strdup(url);
    tag->p.shift_pattern = (unsigned char *) strdup(p);
    
    create_shift_table(tag->title.shift_table, tag->title.shift_pattern);
    create_shift_table(tag->img.shift_table, tag->img.shift_pattern);
    create_shift_table(tag->url.shift_table, tag->url.shift_pattern);
    create_shift_table(tag->p.shift_table, tag->p.shift_pattern);
}

unsigned char *search_attr(string html_content, Shift shift, unsigned char *content_begin) {
    // first time start index is at 0
    int start_index = boyer_moore(shift.shift_pattern, html_content.data, shift.shift_table, 0, (int) html_content.len);
    int i;
    unsigned int content_limit = 1024, content_size = 0;
    unsigned char *tag_content = (unsigned char *) malloc (content_limit);
    
    if (start_index == -1) {
        return NULL;
    }
    
    start_index = locate_content(html_content.data, start_index, (int) html_content.len, content_begin);
    
    for (i = start_index; !isTail(html_content.data, i); i++){
        tag_content[content_size] = html_content.data[i];
        content_size++;

        if (content_size == content_limit) {
            content_limit *= 2;
            tag_content = (unsigned char *)realloc(tag_content, content_limit);
            if (!tag_content) {
                fprintf(stderr, "ERROR: Realloc failed.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    tag_content[content_size] = '\0';
    
    return tag_content;
}

int locate_content(unsigned char *data, int start_index, int end_index, unsigned char *content_begin) {
    int index, str_index;
    bool find_content = true;
    
    for (index = 0; data[index + start_index] != '>' && (start_index + index < end_index); index++) {
        find_content = true;
        if (data[index + start_index] == content_begin[0]) {
            for(str_index = 0; str_index < strlen(content_begin); str_index++) {
                if (data[index + start_index + str_index] != content_begin[str_index]) {
                    find_content = false;
                    break;
                }
            }
            if (find_content && str_index == strlen(content_begin)) {
                return index + start_index + str_index;
            }
        }
    }
    
    return -1;
}

unsigned char *search_article(string html_content, Shift shift, unsigned char *close_tag) {
    // 可能會有很多段文章，因此將內文一次回傳
    // 先定位第一次的位置，確認至少有內文存在，否則回傳 NULL
    int start_index = 0, end_index, cursor;
    unsigned int article_limit = 1024, article_size = 0;
    unsigned char *article = (unsigned char *) malloc (article_limit);
    unsigned char c;
    bool strip_space = false;
    Shift close_shift;
    close_shift.shift_pattern = close_tag;
    create_shift_table(close_shift.shift_table, close_shift.shift_pattern);
    
    while ((start_index = boyer_moore(shift.shift_pattern, html_content.data, shift.shift_table, start_index, (int) html_content.len)) != -1) {
        
        c = html_content.data[start_index + (strlen(close_tag) - 2)];
        if (c == '>') {
            start_index = start_index + (strlen(close_tag) - 1);
        }
        else if (c == ' ') {
            start_index = find_close_tag(html_content.data, start_index);
        }
        else {
            start_index++;
            continue;
        }
        end_index = boyer_moore(close_shift.shift_pattern, html_content.data, close_shift.shift_table, start_index, (int) html_content.len);
        if (end_index == -1) {
            // 應該不可能出現
            fprintf(stderr, "ERROR: Html source error.\n");
            return '\0';
        }
        cursor = start_index;
        strip_space = false;
        while (cursor < end_index) {
            while (html_content.data[cursor] == '<') {
                cursor = find_close_tag(html_content.data, cursor);
            }
            if (isEscape(html_content.data[cursor])) {
                cursor++;
                continue;
            }
            else if (isSpace(html_content.data[cursor])) {
                if (!strip_space) {
                    strip_space = true;
                }
                else {
                    cursor++;
                    continue;
                }
            }
            else {
                strip_space = false;
            }
            article[article_size] = html_content.data[cursor];
            cursor++;
            article_size++;
            if (article_size == article_limit) {
                article_limit *= 2;
                article = (unsigned char *) realloc(article, article_limit);
                if (!article) {
                    fprintf(stderr, "ERROR: Realloc failed.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        if (strcmp(close_tag, "</title>") == 0) {
            break;
        }
    }
    article[article_size] = '\0';
    
    return article;
}

void search_link(string html_content, unsigned char *s_url, unsigned int depth, Link **link, unsigned char *content_begin) {
    int start_index = 0, cursor, i;
    bool status;
    unsigned char *t_url;
    unsigned int t_url_limit, t_url_size;
    
    while ((start_index = boyer_moore((*link)->shift_pattern, html_content.data, (*link)->shift_table, start_index, (int) html_content.len)) != -1) {
        
        cursor = locate_content(html_content.data, start_index, (int) html_content.len, content_begin);
        if (cursor == -1) {
            start_index++;
            continue;
        }
        else {
            start_index = cursor;
        }
        t_url_limit = 1024;
        t_url_size = 0;
        t_url = (unsigned char *) malloc (t_url_limit);
        
        for (i = start_index; !isTail(html_content.data, i); i++){
            t_url[t_url_size] = html_content.data[i];
            t_url_size++;

            if (t_url_size == t_url_limit) {
                t_url_limit *= 2;
                t_url = (unsigned char *)realloc(t_url, t_url_limit);
                if (!t_url) {
                    fprintf(stderr, "ERROR: Realloc failed.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        t_url[t_url_size] = '\0';
        // check if the url is legal or not
        if (!legal_link_format(t_url)) {
            // skip this round and free memory
            free(t_url);
            continue;
        }
        status = get_full_link(s_url, t_url, &(*link)->store_content[(*link)->link_count].url);
        if (status) {
            (*link)->link_count++;
            if ((*link)->link_count == (*link)->link_limit) {
                (*link)->link_limit *= 2;
                (*link)->store_content = (Connect *) realloc((*link)->store_content, (*link)->link_limit * sizeof(Connect));
            }
        }
        
        free(t_url);
    }
}


void tag_destructor(Tag *tag) {
    free(tag->title.store_content);
    free(tag->title.shift_pattern);
    
    free(tag->img.store_content);
    free(tag->img.shift_pattern);
    
    free(tag->url.store_content);
    free(tag->url.shift_pattern);
    
    free(tag->p.store_content);
    free(tag->p.shift_pattern);
}


