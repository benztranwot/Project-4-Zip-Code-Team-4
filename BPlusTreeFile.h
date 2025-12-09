#pragma once
#ifndef BPLUS_TREE_FILE_H
#define BPLUS_TREE_FILE_H

#include <vector>
#include <string>
#include <ostream>
#include "BlockFile.h"
#include "BTreeBlocks.h"

struct ChildInfo {
    int largestKey;
    int childRBN;
};

class BPlusTreeFile {
public:
    explicit BPlusTreeFile(const std::string& filename);

    // Build B+tree index from leaf info (built from your sequence set)
    void buildFromLeaves(const std::vector<ChildInfo>& leaves);

    // Search by zip key
    //bool find(int zipKey, HeaderRecordPostalCodeItem& outRecord);

    // Dump tree structure for debugging
    void dumpTree(std::ostream& out);

    // Expose header (read-only) if needed
    //const BTreeHeader& getHeader() const { return header; }

private:
    BlockFile bf;
    BTreeHeader header;

    int buildIndexLevels(const std::vector<ChildInfo>& childInfo);
};

#endif
