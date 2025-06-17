# NAS Sync Tool

A lightweight, C/C++-based file synchronization tool that mimics core NAS (Network Attached Storage) behavior. It monitors and synchronizes files from a source directory to a backup directory at a specified time interval.

## Features

- 🔄 One-way file sync: copies newer source files to the backup location.
- ⏱ Configurable sync interval using a simple config file.
- 🧠 Smart copy: only syncs if the source file has been modified.
- 🧵 Built using C for core performance and C++ for logic control.
- 📦 Ready for CLI use and easy to extend into a daemon or service.

## Setup

### 1. Folder Structure

```
NAS/
├── main.cpp               # C++ logic to read config and run the loop
├── config.txt             # Contains sync settings
├── src/
│   └── sync_engine.c      # File copy and comparison logic in C
├── include/
│   └── sync_engine.h      # Function declaration for sync logic
```

### 2. Config File (`config.txt`)

```
source=./data/source
backup=./data/backup
interval=5
```

- `source`: path to the folder or file to sync
- `backup`: path where backup files should be copied
- `interval`: time in seconds between sync checks

### 3. Compilation

```bash
g++ main.cpp src/sync_engine.c -Iinclude -o nas_app
```

Or to remove warnings:

```bash
gcc -c src/sync_engine.c -o sync_engine.o
g++ main.cpp sync_engine.o -Iinclude -o nas_app
```

### 4. Run

```bash
./nas_app
```

Output will show sync status every N seconds based on your config.

## Coming Soon

- 📁 Folder sync (multiple files)
- 📝 Logging to a file
- 🚦 Ctrl+C handling
- 🚀 Daemon mode for always-on syncing

## License

MIT — free to use and modify.

---

Built by Sohan 🚴‍♂️ fueled by code and curiosity.