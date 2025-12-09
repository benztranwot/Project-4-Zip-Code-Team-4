/*
File: BPlusTreeFile.cpp
*/

#include "BPlusTreeFile.h"
#include <queue>
#include <algorithm>  // std::min
#include <cstring>    // std::memcpy
#include <iostream>

BPlusTreeFile::BPlusTreeFile(const std::string& filename)
    : bf(filename, BLOCK_SIZE)
{
    // Try to read header from block 0
    char buffer[BLOCK_SIZE];
    bf.readBlock(0, buffer);

    BTreeHeader hdr{};
    std::memcpy(&hdr, buffer, sizeof(BTreeHeader));

    
    if (hdr.blockSize == BLOCK_SIZE && hdr.rootRBN >= 0) {
        header = hdr;
    }
    else {
        // File may be new or header not yet written: set defaults
        header.blockSize = BLOCK_SIZE;
        header.rootRBN = -1;
        header.firstLeafRBN = -1;
        header.lastLeafRBN = -1;
        header.totalRecords = 0;
        header.treeHeight = 0;
    }
}
void BPlusTreeFile::buildFromLeaves(const std::vector<ChildInfo>& leaves)
{
    if (leaves.empty()) {
        std::cerr << "No leaves provided — cannot build tree.\n";
        return;
    }

    
    IndexBlock root{};
    root.header.type = BLOCK_INDEX;
    root.keyCount = 0;

    // Allocate an RBN for the root
    int rootRBN = bf.allocateNewBlock();
    root.header.selfRBN = rootRBN;

    
    for (const auto& c : leaves) {
        if (root.keyCount >= INDEX_MAX_ENTRIES) {
            std::cerr << "Root overflow (static tree)!\n";
            break;
        }

        root.entries[root.keyCount].largestKey = c.largestKey;
        root.entries[root.keyCount].childRBN = c.childRBN;
        root.keyCount++;
    }

    
    char buf[BLOCK_SIZE];
    packIndexBlock(root, buf);
    bf.writeBlock(rootRBN, buf);

    
    header.rootRBN = rootRBN;
    header.firstLeafRBN = leaves.front().childRBN;
    header.lastLeafRBN = leaves.back().childRBN;
    header.totalRecords = (int)leaves.size();
    header.treeHeight = 1;

    
    char hbuf[BLOCK_SIZE];
    writeHeaderBlock(hbuf, header);
    bf.writeBlock(0, hbuf);
}
void BPlusTreeFile::dumpTree(std::ostream& out)
{
    out << "====== B+TREE DUMP ======\n";
    out << "Block Size: " << header.blockSize << "\n";
    out << "Root RBN: " << header.rootRBN << "\n";
    out << "First Leaf: " << header.firstLeafRBN << "\n";
    out << "Last Leaf: " << header.lastLeafRBN << "\n";
    out << "Tree Height: " << header.treeHeight << "\n";
    out << "Total Records: " << header.totalRecords << "\n\n";

    
    char buf[BLOCK_SIZE];
    bf.readBlock(header.rootRBN, buf);

    IndexBlock root{};
    unpackIndexBlock(root, buf);

    out << "---- ROOT INDEX BLOCK (RBN " << header.rootRBN << ") ----\n";
    out << "Key Count: " << root.keyCount << "\n";

    for (int i = 0; i < root.keyCount; i++) {
        out << "  Entry[" << i << "]  Key=" << root.entries[i].largestKey
            << "  ChildRBN=" << root.entries[i].childRBN << "\n";
    }
    out << "\n";

   
    int cur = header.firstLeafRBN;
    out << "---- LEAF SEQUENCE ----\n";

    while (cur != -1) {
        char lbuf[BLOCK_SIZE];
        bf.readBlock(cur, lbuf);

        LeafBlock leaf{};
        unpackLeafBlock(leaf, lbuf);

        out << "Leaf RBN " << cur << " | prev=" << leaf.prevRBN
            << " next=" << leaf.nextRBN
            << " count=" << leaf.recordCount << "\n";

        for (int i = 0; i < leaf.recordCount; i++) {
            out << "    ZIP: " << leaf.records[i].zip
                << "  Place: " << leaf.records[i].place << "\n";
        }

        cur = leaf.nextRBN;
    }
}
