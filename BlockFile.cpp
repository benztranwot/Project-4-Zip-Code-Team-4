#include "BlockFile.h"
#include <cstring>   // for std::memset

BlockFile::BlockFile(const std::string& filename, int blockSize)
    : blockSize(blockSize)
{
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        // Create the file if it doesn't exist
        file.clear();
        file.open(filename, std::ios::out | std::ios::binary);
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
}

void BlockFile::readBlock(int rbn, char* buffer) {
    file.seekg(static_cast<std::streamoff>(rbn) * blockSize);
    file.read(buffer, blockSize);
}

void BlockFile::writeBlock(int rbn, const char* buffer) {
    file.seekp(static_cast<std::streamoff>(rbn) * blockSize);
    file.write(buffer, blockSize);
    file.flush();
}

int BlockFile::allocateNewBlock() {
    file.seekp(0, std::ios::end);
    std::streamoff bytes = file.tellp();
    int rbn = static_cast<int>(bytes / blockSize);

    char* zero = new char[blockSize]();
    file.write(zero, blockSize);
    file.flush();
    delete[] zero;

    return rbn;
}
