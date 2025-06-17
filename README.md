# sync_engine

`sync_engine` is the core C module responsible for file synchronization logic in your NAS project. It handles efficient file comparison, copying, and folder syncing with performance in mind.

---

## Features

- Compare source and destination files using file metadata (`stat`).
- Copy files only if the source is newer, avoiding unnecessary writes.
- Recursively sync folders with support for subdirectories.
- Uses a dynamically allocated buffer (default 1MB) for high-performance file I/O.
- Graceful error handling with detailed status codes.

---

## Files

- `sync_engine.c` — Implementation of file sync functions.
- `sync_engine.h` — Header file with function declarations and constants.

---

## Main Functions

### `int compare_and_copy(const char* src, const char* dst)`

- Compares modification times of source and destination files.
- Copies the source file to the destination if newer or missing.
- Returns:
  - `1` if file copied successfully
  - `0` if no copy was necessary (destination up to date)
  - Negative values on errors (e.g., -1 file missing, -2 open error)

### `int sync_folder(const char* source_dir, const char* backup_dir)`

- Recursively synchronizes all files and directories from source to backup.
- Uses `compare_and_copy` internally.
- Prints progress and sync status to the console.
- Returns `0` on success or negative error codes.

---

## Buffer Size

- Uses a **default buffer size of 1 MB** for file copying.
- This can be modified by changing `DEFAULT_BUFFER_SIZE` in `sync_engine.c`.

---

## Compilation

The `sync_engine` module is compiled alongside the main NAS application:

```bash
gcc -c src/sync_engine.c -o sync_engine.o
g++ main.cpp sync_engine.o -Iinclude -o nas_app
```

---

## Usage

Your main NAS app (e.g., `main.cpp`) calls `sync_folder()` periodically based on a config interval. This module handles all the file I/O efficiently and prints syncing progress.

---

## Error Handling

The module returns detailed error codes from file operations, which you can catch and log or display in your frontend or CLI.

---

## Future Improvements

- Add file deletion syncing for true bidirectional sync.
- Support for file versioning or conflict resolution.
- Logging sync operations to a file instead of console.
- Integration with a REST API backend for remote control.

---

## License

MIT License — free to use, modify, and distribute.

---

Built by Sohan 🚴‍♂️ fueled by code and curiosity.