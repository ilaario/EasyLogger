#include "../lib/cache.h"

int main(int argc, char const *argv[]) {
    FILE* fp = file_handler(fp, argv[1],FILE_OPEN);
    cache ca = init_cache();
    file_handler(fp,argv[1],FILE_CLOSE);
    int bytesToRead = atoi(argv[2]);
    return 0;
}
