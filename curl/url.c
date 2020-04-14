//
//  url.c
//  C-Crawler-Client
//
//  Created by 林重翰 on 2020/4/11.
//  Copyright © 2020 林重翰. All rights reserved.
//

#include "url.h"

static inline int indexOf(unsigned char *url, char *str) {
    int i, j, k;
    for (i = 0; i < strlen(url); i++) {
        k = i;
        j = 0;
        if (url[k] == str[j]) {
            while (url[k] == str[j]) {
                k++;
                j++;
            }
            if (j == strlen(str)) {
                return k;
            }
        }
    }

    return -1;
}

inline int indexOfDomain(unsigned char *url) {
    int i;
    if ((i = indexOf(url, ".com")) != -1) {
        return i;
    }
    else if ((i = indexOf(url, ".net")) != -1) {
        return i;
    }
    else if ((i = indexOf(url, ".edu")) != -1) {
        return i;
    }
    else if ((i = indexOf(url, ".org")) != -1) {
        return i;
    }
    else if ((i = indexOf(url, ".ly")) != -1) {
        return i;
    }

    return -1;
}

bool legal_link_format(unsigned char *url) {
    if (strstr(url, "javascript:")) {
        return false;
    }
    else if (strstr(url, "mailto")) {
        return false;
    }
    else if (strstr(url, "#")) {
        return false;
    }
    else if (strlen(url) == 0) {
        return false;
    }
    else if (strlen(url) == 1 && url[0] == ' ') {
        return false;
    }

    return true;
}

bool get_full_link(unsigned char *s_url, unsigned char *t_url, unsigned char **store) {
    // get protocol -> http, https, ftp, sftp
    unsigned char *protocol = get_protocol(s_url, t_url);
    unsigned char *server = get_server(s_url, t_url);

    unsigned char *path = get_path(t_url);
    
    if (!protocol || !server || !path) {
        return false;
    }
    
    *store = (unsigned char *) malloc ((strlen(protocol) + strlen(server) + strlen(path) + 1) * sizeof(unsigned char));

    // return server;
    sprintf(*store, "%s%s%s", protocol, server, path);
    free(protocol);
    free(server);
    free(path);
    
    return true;
}

unsigned char *get_protocol(unsigned char *s_url, unsigned char *t_url) {
    int i, index = indexOf(t_url, "://");
    unsigned char *protocol = (unsigned char *) malloc (8 * sizeof(unsigned char));
    if (index == -1) {
        index = indexOf(s_url, "://");
        if (index == -1) {
            return NULL;
        }
        for (i = 0; i < index; i++) {
           protocol[i] = s_url[i];
        }
    }
    else {
        for (i = 0; i < index; i++) {
           protocol[i] = t_url[i];
        }
    }
    protocol[i] = '\0';
    return protocol;
}

unsigned char *get_server(unsigned char *s_url, unsigned char *t_url) {
    int i, j, protocolIndex, domainIndex = indexOfDomain(t_url);
    unsigned char *server;
    if (domainIndex == -1) {
        // fprintf(stderr, "參照 source url: ");
        protocolIndex = indexOf(s_url, "://");
        domainIndex = indexOfDomain(s_url);
        server = (unsigned char *) malloc ((domainIndex - protocolIndex + 1) * sizeof(unsigned char));
        j = 0;
        for (i = protocolIndex ; i < domainIndex; i++, j++) {
            server[j] = s_url[i];
        }
    }
    else {
        // fprintf(stderr, "參照 target url: ");
        protocolIndex = indexOf(t_url, "//");
        j = 0;
        if (protocolIndex == -1) {
            domainIndex = indexOf(t_url, "/");
            i = 0;
        }
        else {
            i = protocolIndex;
        }
        server = (unsigned char *) malloc ((domainIndex - i + 1) * sizeof(unsigned char));
        for(; i < domainIndex; i++, j++) {
            server[j] = t_url[i];
        }
    }
    server[j] = '\0';

    return server;
}

unsigned char *get_path(unsigned char *t_url) {
    int i, j, domainIndex = indexOfDomain(t_url);
    unsigned char *path;

    if (domainIndex == -1){
        if (t_url[0] == '/') {
            path = (unsigned char *) malloc ((strlen(t_url) + 1) * sizeof(unsigned char));
            strcpy(path, t_url);
        }
        else {
            path = (unsigned char *) malloc ((strlen(t_url) + 2) * sizeof(unsigned char));
            sprintf(path, "/%s", t_url);
        }
    }
    else {
        path = (unsigned char *) malloc ((strlen(t_url) - domainIndex + 1) * sizeof(unsigned char));
        for (i = domainIndex, j = 0; i < strlen(t_url) ; i++, j++) {
            path[j] = t_url[i];
        }
        path[j] = '\0';
    }

    return path;
}
