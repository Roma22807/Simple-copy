#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <memory>
off_t get_file_size(int fd) {
    return lseek(fd, 0, SEEK_END);
}
void printbar(off_t total, off_t copied) {
    if (total <= 0) return;
    float percent = (static_cast<float>(copied) / total) * 100;
    if (percent > 100) percent = 100;
    std::cout << "\r[" << std::string(percent / 10, '#')
    << std::string(10 - percent / 10, ' ') << "] "
    << static_cast<int>(percent) << "%" << std::flush;
}
int main(int args, char* argv[]) {
if (args < 2) {
    std::cerr << "ERROR: invalid input\nYou must type like that\n" << argv[0] <<  " -h / " << argv[0] << " -v <original file> <copy file>\n";
    return 1;
}
bool isVer = false;
std::string OR_f, CP_f;
for (int i = 1; i < args; ++i) {
std::string arg = argv[i];
if (arg == "-h") {
std::cout << "Help: this programm is copying the files\n";
std::cout << "Usage: " << argv[0] << " [-v] <original file> <copy file>\n";
    return 0;
} else if (arg == "-v") {
    std::cout << "Verbose mode: true\n";
    isVer = true;
    continue;
} else {
    if (OR_f.empty()) {
        OR_f = arg;
    } else if (CP_f.empty()) {
        CP_f = arg;
    }
}
}
if (OR_f.empty() || CP_f.empty()) {
    std::cerr << "ERROR: Missing source or destination file\n";
    return 1;
}
int ORG_file = open(OR_f.c_str(), O_RDONLY);
if (ORG_file == -1) {
    perror(("ERROR: Can't open source file: " + OR_f).c_str());
    return 1;
}
int COPY_file = open(CP_f.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0664);
if (COPY_file == -1) {
    close(ORG_file);
    perror(("ERROR: Can't create destination file: " + CP_f).c_str());
    return 1;
}
off_t total = get_file_size(ORG_file);
lseek(ORG_file, 0, SEEK_SET);
const size_t buffer_size = 4096;
std::unique_ptr<char[]> buffer = std::make_unique<char[]>(buffer_size);
ssize_t read_bytes;
off_t Copied_file = 0;
while ((read_bytes = read(ORG_file, buffer.get(), buffer_size)) > 0) {
    ssize_t written = write(COPY_file, buffer.get(), read_bytes);
    if (written != read_bytes) {
        perror("ERROR: No such write in file\n");
        close(COPY_file);
        close(ORG_file);
        return 1;
    }
    Copied_file += written;
    if (isVer) {
printbar(total, Copied_file);
    }
}
if (read_bytes == -1) {
    perror("ERROR: No such read in file\n");
        close(COPY_file);
        close(ORG_file);
        return 1;
}
if (isVer) {
    printbar(total, Copied_file);
    std::cout << "\nThe file " << OR_f << " was successfully copied to " << CP_f << "\n";
}
close(ORG_file);
close(COPY_file);
return 0;
}