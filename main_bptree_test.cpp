/**
 * @file main_bptree_test.cpp
 * @brief Test driver for the BPlusTreeFile class.
 *
 * Usage:
 *   ./bptree_test bptree_postal.dat [zip1 zip2 ...]
 *
 * Behavior:
 *   - Reads the B+tree header and prints basic info.
 *   - Dumps the tree structure to std::cout.
 *   - Walks the leaf-level sequence set for a few blocks and prints ZIPs.
 *   - If ZIP arguments are provided, looks them up and prints full records.
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>   // std::atoi

#include "BPlusTreeFile.h"
#include "BlockFile.h"
#include "BTreeBlocks.h"

void printHeader(const BTreeHeader& hdr) {
    std::cout << "B+Tree Header\n"
        << "  blockSize    = " << hdr.blockSize << "\n"
        << "  rootRBN      = " << hdr.rootRBN << "\n"
        << "  firstLeafRBN = " << hdr.firstLeafRBN << "\n"
        << "  lastLeafRBN  = " << hdr.lastLeafRBN << "\n"
        << "  totalRecords = " << hdr.totalRecords << "\n"
        << "  treeHeight   = " << hdr.treeHeight << "\n\n";
}


void dumpLeafSequence(const std::string& filename,
    const BTreeHeader& hdr,
    int maxLeavesToShow = 5)
{
    if (hdr.firstLeafRBN < 0) {
        std::cout << "No leaf sequence (firstLeafRBN < 0)\n";
        return;
    }

    BlockFile bf(filename, BLOCK_SIZE);
    int currentRBN = hdr.firstLeafRBN;
    int leafCount = 0;

    char buffer[BLOCK_SIZE];

    std::cout << "Leaf-level sequence set (up to " << maxLeavesToShow
        << " leaves from the beginning):\n";

    while (currentRBN >= 0 && leafCount < maxLeavesToShow) {
        bf.readBlock(currentRBN, buffer);

        BlockHeader bh;
        std::memcpy(&bh, buffer, sizeof(BlockHeader));

        if (bh.type != BLOCK_LEAF) {
            std::cout << "  RBN " << currentRBN << " is not a leaf block!\n";
            break;
        }

        LeafBlock lb{};
        unpackLeafBlock(lb, buffer);

        std::cout << "  Leaf RBN=" << currentRBN
            << " prev=" << lb.prevRBN
            << " next=" << lb.nextRBN
            << " count=" << lb.recordCount
            << " | ZIPs: ";

        for (int i = 0; i < lb.recordCount; ++i) {
            std::cout << lb.records[i].zip << " ";
        }
        std::cout << "\n";

        currentRBN = lb.nextRBN;
        ++leafCount;
    }

    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
            << " <bptree_file> [zip1 zip2 ...]\n";
        return 1;
    }

    std::string btreeFilename = argv[1];

    // Open the B+tree file via the class
    BPlusTreeFile tree(btreeFilename);
    const BTreeHeader& hdr = tree.getHeader();

    // Print header information
    printHeader(hdr);

    if (hdr.rootRBN < 0) {
        std::cout << "B+Tree appears to be empty (rootRBN < 0).\n";
        return 0;
    }

    // Dump the tree structure (levels, index blocks, leaves)
    std::cout << "=== B+Tree Structure Dump ===\n";
    tree.dumpTree(std::cout);
    std::cout << "=============================\n\n";

    // Demonstrate sequence-set traversal
    dumpLeafSequence(btreeFilename, hdr, 5);

    // If ZIP codes are provided, look them up
    if (argc > 2) {
        std::cout << "=== Lookup Tests ===\n";
        for (int i = 2; i < argc; ++i) {
            int zipKey = std::atoi(argv[i]);
            HeaderRecordPostalCodeItem rec;

            std::cout << "Looking up ZIP " << zipKey << " ... ";

            if (tree.find(zipKey, rec)) {
                std::cout << "FOUND\n";
                std::cout << "  ZIP:    " << rec.getZip() << "\n"
                    << "  Place:  " << rec.getPlace() << "\n"
                    << "  State:  " << rec.getState() << "\n"
                    << "  County: " << rec.getCounty() << "\n"
                    << "  Lat:    " << rec.getLatitude()
                    << "  Lon: " << rec.getLongitude() << "\n\n";
            }
            else {
                std::cout << "NOT FOUND\n\n";
            }
        }
    }

    return 0;
}
