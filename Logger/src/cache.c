#include "../lib/cache.h"

cache init_cache() {
    cache ca;
    ca.index_of_near_word = 0;
    ca.is_on_cache = 0;
    return ca; 
}

FILE* file_handler(FILE *fp, const char* path, file_operation fo){
    if(fp == NULL) {
        return NULL; 
    }

    switch (fo) {
        case FILE_OPEN:
            fp = fopen(path,"r");
            if(fp == NULL) {
                return NULL;
            }
            printf("--- FILE %s OPENED ---\n", path);
            break;

        case FILE_CLOSE:
            fclose(fp);
            printf("--- FILE %s CLOSED ---\n", path);
            break;

        default:
            perror("!!! INVALID OPTION !!!");
            exit(EXIT_FAILURE);
    }

    return fp; 
}