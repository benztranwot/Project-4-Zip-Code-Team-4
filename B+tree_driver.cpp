/**
 * @file main.cpp
 * @brief Builds a B+ tree index from USPS postal code data and allows ZIP-code lookups.
 *
 * This program:
 *  - Loads Block Sequence Set (BSS) records from a file.
 *  - Inserts ZIP codes into a B+ tree index.
 *  - Dumps the B+ tree to a file.
 *  - Allows runtime lookup of postal records using the B+ tree + sequence set.
 */

#include <string>
#include "BlockPostalCode.h"
#include "BlockSequenceSetPostalCode.h"
#include "PostalRecord.h"
#include "readHeaderPostalCodetoBSSBuffer.cpp"
#include <fstream>

#include "B+tree.cpp"

using namespace std;

/**
 * @brief Searches the Block Sequence Set (BSS) for a postal record by ZIP.
 *
 * Performs sequential traversal of the BSS linked blocks.  
 * Only used *after* the B+ tree confirms the ZIP exists.
 *
 * @param zip ZIP code to search for.
 * @param out Reference to PostalRecord where the result will be stored.
 * @param bss Reference to the Block Sequence Set containing postal blocks.
 * @return true If the ZIP was found in the sequence set.
 * @return false If the ZIP does not exist in the sequence set.
 */
bool lookupPostalRecord(int zip,
                        PostalRecord &out,
                        BlockSequenceSetPostalCode &bss)
{
    // Start with the head block (by value, just like in main)
    BlockPostalCode current = bss.getHead();

    while (true)
    {
        HeaderRecordPostalCodeItem item = current.getBlockItem();

        if (item.getZip() == zip)
        {
            out.zip = item.getZip();
            out.place = item.getPlace();
            out.state = item.getState();
            out.county = item.getCounty();
            return true;
        }

        // If there's no "next" block, we're done
        if (current.getNext() == nullptr)
        {
            break;
        }

        // Move to the next block (getNext() returns BlockPostalCode*)
        current = *current.getNext();
    }

    return false; // not found in the sequence set
}

/**
 * @brief Main function: builds B+ tree from postal codes and performs lookup.
 *
 * Steps:
 *  1. Builds a Block Sequence Set from a length-indicated record file.
 *  2. Inserts ZIP codes from each block into a B+ tree.
 *  3. Prints the B+ tree structure to `B+Tree_data.txt`.
 *  4. Performs user-driven ZIP lookups using:
 *     - B+ tree (index check)
 *     - Sequence set (record retrieval)
 *
 * @return int Program exit code.
 */
int main()
{
    int degree = 10; ///< B+ tree degree. Higher degree → shorter tree height.
    BPlusTree<int> tree(degree);

    string fileName = "us_postal_codes_length_indicated_header_record.txt"; ///< Input postal data file
    ofstream outputFile("B+Tree_data.txt"); ///< Output dump containing tree structure

    BlockSequenceSetPostalCode myBlockSequenceSetPostalCode; ///< Sequence set structure for all blocks
    string blockRecord;
    BlockPostalCode myBlock;

    /**
     * @brief Loads all postal header+records into the Block Sequence Set.
     */
    inputDatatoBlockSequenceSet(myBlockSequenceSetPostalCode, fileName);

    myBlock = myBlockSequenceSetPostalCode.getHead(); ///< First block pointer

    // Insert first block ZIP into B+ tree
    tree.insert(myBlock.getBlockItem().getZip());

    // Move to next block
    myBlock = *myBlock.getNext();

    /**
     * @brief Insert ZIP codes from all intermediate blocks.
     */
    while (myBlock.getNext() != nullptr)
    {
        tree.insert(myBlock.getBlockItem().getZip());
        myBlock = *myBlock.getNext();
    }

    // Insert last block ZIP
    tree.insert(myBlock.getBlockItem().getZip());

    // Save original std::cout buffer
    std::streambuf *originalCoutBuffer = std::cout.rdbuf();

    // Redirect std::cout to the output file
    std::cout.rdbuf(outputFile.rdbuf());

    /**
     * @brief Prints the B+ tree in a readable hierarchical structure to file.
     */
    tree.printTree();

    // Restore cout output
    std::cout.rdbuf(originalCoutBuffer);

    outputFile.close();

    cout << "B+tree builded successfully!" << endl;
    cout << "B+ tree file: B+Tree_data.txt" << endl;

    /**
     * @brief User search loop for interactive ZIP lookup.
     */
    int zip;
    while (true)
    {
        std::cout << "Enter ZIP to search (0 to quit): ";
        if (!(std::cin >> zip))
        {
            std::cout << "Input error, exiting...\n";
            break;
        }

        if (zip == 0)
        {
            break;
        }

        // B+ tree check
        if (tree.search(zip))
        {
            PostalRecord rec;

            // If ZIP exists, retrieve full record from sequence set
            if (lookupPostalRecord(zip, rec, myBlockSequenceSetPostalCode))
            {
                std::cout << "\nFOUND ZIP " << rec.zip << "\n"
                          << "Place:  " << rec.place << "\n"
                          << "State:  " << rec.state << "\n"
                          << "County: " << rec.county << "\n\n";
            }
            else
            {
                std::cout << "ZIP " << zip
                          << " FOUND in B+ tree\n\n";
            }
        }
        else
        {
            std::cout << "ZIP " << zip << " NOT FOUND in B+ tree\n\n";
        }
    }

    return 0;
}
