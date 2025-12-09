/* File: main_build_bptree.cpp */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>


#include "HeaderRecordPostalCodeItem.h"
#include "BlockSequenceSetPostalCode.h"
#include "BlockPostalCode.h"
#include "BTreeBlocks.h"
#include "BlockFile.h"
#include "BPlusTreeFile.h"
#include "readHeaderPostalCodetoBSSBuffer.h"

int main() {

    std::string inputFile = "us_postal_codes_length_indicated_header_record.txt";

    // 1. Load CSV → BSS in RAM
    BlockSequenceSetPostalCode bss;
    // Debug: Show working directory and input file check
    std::cout << "Current working directory: "
        << std::filesystem::current_path() << "\n";

    std::cout << "Looking for input file: " << inputFile << "\n";

    if (!std::filesystem::exists(inputFile)) {
        std::cerr << "❌ ERROR: Input file not found in working directory.\n";
        std::cerr << "Make sure this file is located here:\n"
            << std::filesystem::current_path() << "\n";
        return 0;
    }

    // If it exists, continue normally
    int count = inputDatatoBlockSequenceSet(bss, inputFile);


    if (count == 0) {
        std::cerr << "No leaves loaded — exiting.\n";
        return 0;
    }

    // Open B+Tree file
    std::string btreeFilename = "bptree_postal.dat";
    BlockFile bf(btreeFilename, BLOCK_SIZE);

    // Reserve RBN 0 for header
    bf.allocateNewBlock();

    // Build leaves & build leafInfo vector
    std::vector<ChildInfo> leafInfo;

    BlockPostalCode* current = bss.getHead();
    int prevRBN = -1;
    bool first = true;

    while (current != nullptr) {

        HeaderRecordPostalCodeItem rec = current->getBlockItem();
        int zip = rec.getZip();

        LeafBlock lb{};
        lb.header.type = BLOCK_LEAF;
        lb.prevRBN = prevRBN;
        lb.nextRBN = -1;
        lb.recordCount = 1;
        lb.records[0] = toDisk(rec);

        int rbn = bf.allocateNewBlock();
        lb.header.selfRBN = rbn;

        // Fix linking of previous leaf
        if (!first) {
            char tmpBuf[BLOCK_SIZE];
            bf.readBlock(prevRBN, tmpBuf);
            LeafBlock prevLB{};
            unpackLeafBlock(prevLB, tmpBuf);
            prevLB.nextRBN = rbn;
            packLeafBlock(prevLB, tmpBuf);
            bf.writeBlock(prevRBN, tmpBuf);
        }

        // Write new leaf block
        char buf[BLOCK_SIZE];
        packLeafBlock(lb, buf);
        bf.writeBlock(rbn, buf);

        leafInfo.push_back({ zip, rbn });

        prevRBN = rbn;
        first = false;
        current = current->getNext();
    }

    // Build the B+Tree index
    BPlusTreeFile tree(btreeFilename);
    tree.buildFromLeaves(leafInfo);

    // Dump tree
    std::ofstream out("BPlusTree_dump.txt");
    tree.dumpTree(out);
    out.close();

    std::cout << "Tree built successfully.\n";
    return 0;
}
    
