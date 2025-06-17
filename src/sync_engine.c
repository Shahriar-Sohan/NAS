#include "../include/sync_engine.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_BUFFER_SIZE (1024 * 1024)

int compare_and_copy(const char *src, const char *dst)
{
    struct stat src_stat, dst_stat;

    if (stat(src, &src_stat) != 0)
        return -1;

    if (stat(dst, &dst_stat) == 0) {
        if (src_stat.st_mtime <= dst_stat.st_mtime) {
            return 0;
        }
    }

    int in = open(src, O_RDONLY);
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (in < 0 || out < 0)
        return -2;

    char *buffer = (char *)malloc(DEFAULT_BUFFER_SIZE);
    if (!buffer) {
        close(in);
        close(out);
        return -3;
    }

    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(in, buffer, DEFAULT_BUFFER_SIZE)) > 0) {
        bytes_written = write(out, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            free(buffer);
            close(in);
            close(out);
            return -4;
        }
    }

    if (bytes_read < 0) {
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

int sync_folder(const char *source_dir, const char *backup_dir)
{
    DIR *dir = opendir(source_dir);
    if (!dir)
        return -10;

    int total_entries = 0;
    off_t total_bytes = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char src_path[512];
        snprintf(src_path, sizeof(src_path), "%s/%s", source_dir, entry->d_name);

        struct stat path_stat;
        if (stat(src_path, &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
            total_bytes += path_stat.st_size;
            total_entries++;
        } else if (S_ISDIR(path_stat.st_mode)) {
            total_entries++;
        }
    }

    if (total_entries == 0) {
        printf("No files to sync\n");
        closedir(dir);
        return 0;
    }

    rewinddir(dir);
    int processed_entries = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char src_path[512], dst_path[512];
        snprintf(src_path, sizeof(src_path), "%s/%s", source_dir, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", backup_dir, entry->d_name);

        struct stat path_stat;
        if (stat(src_path, &path_stat) != 0) {
            processed_entries++;
            continue;
        }

        int result = 0;
        if (S_ISDIR(path_stat.st_mode)) {
            if (mkdir(dst_path, 0755) != 0 && errno != EEXIST) {
                processed_entries++;
                continue;
            }
            result = sync_folder(src_path, dst_path);
        } else if (S_ISREG(path_stat.st_mode)) {
            result = compare_and_copy(src_path, dst_path);
            if (result == 0) {
                printf("Synced: %s\n", entry->d_name);
            }
        }

        processed_entries++;
    }
    closedir(dir);
    return 0;
}