#pragma once
#ifndef BLOCK_FILE_H
#define BLOCK_FILE_H

#include <fstream>
#include <string>

class BlockFile {
public:
	BlockFile(const std::string& filename, int blockSize);

	void readBlock(int rbn, char* buffer);
	void writeBlock(int rbn, const char* buffer);

	int allocateNewBlock();

private:
	std::fstream file;
	int blockSize;

};
#endif
