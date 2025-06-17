#ifndef SYNC_ENGINE_H
#define SYNC_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

int compare_and_copy(const char* src, const char* dst);
int sync_folder(const char* source_dir, const char* backup_dir);

#ifdef __cplusplus
}
#endif

#endif // SYNC_ENGINE_H