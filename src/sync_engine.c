#include "../include/sync_engine.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_BUFFER_SIZE (1024 * 1024)

int compare_and_copy(const char* src, const char* dst){
    struct stat src_stat, dst_stat;

    if(stat(src, &src_stat) != 0) return -1;

    if(stat(dst, &dst_stat) == 0){
        if(src_stat.st_mtime <= dst_stat.st_mtime){
            return 0;
        }
    }

    int in = open(src, O_RDONLY);
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(in < 0 || out < 0) return -2;

    char* buffer = (char*)malloc(DEFAULT_BUFFER_SIZE);
    if(!buffer){
        close(in);
        close(out);
        return -3;
    }

    ssize_t bytes_read, bytes_written;

    while((bytes_read = read(in, buffer, DEFAULT_BUFFER_SIZE)) > 0){
        bytes_written = write(out, buffer, bytes_read);

        if(bytes_written != bytes_read){
            free(buffer);
            close(in);
            close(out);
            return -4;
        }
    }

    if(bytes_read < 0){
        free(buffer);
        close(in);
        close(out);
        return -5;
    }

    free(buffer);
    close(in);
    close(out);

    return 0;
}