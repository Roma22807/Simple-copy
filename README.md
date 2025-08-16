File Copy Utility - A C++ tool for copying files with progress tracking.

Build: g++ -o copy copy.cpp -std=c++17

Usage: ./copy [options]

Options: -h Show this help message -v Show progress bar during copy

Examples: ./copy file.txt backup.txt ./copy -v large_file.iso copy.iso

Notes:

    Uses low-level POSIX file operations
    Won't overwrite existing files
    Fixed 4KB buffer size
    Shows percentage progress in verbose mode
    Reports errors if files can't be opened

Just compile and run - no additional dependencies needed.
