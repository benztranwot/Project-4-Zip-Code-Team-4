#include <string>
#include "BlockPostalCode.h"
#include "BlockSequenceSetPostalCode.h"
#include "PostalRecord.h"
#include "readHeaderPostalCodetoBSSBuffer.cpp"
#include <fstream>

#include "B+tree.cpp"

using namespace std;

// Postal record lookup
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

int main()
{
    int degree = 10; ///< I choose higher degree to create a shorter tree lead to reducing the time complexity of operations
    BPlusTree<int> tree(degree);

    string fileName = "us_postal_codes_length_indicated_header_record.txt"; ///< Input file containing header + length-indicated records
    ofstream outputFile("B+Tree_data.txt");                                 ///< Output file for block sequence set

    BlockSequenceSetPostalCode myBlockSequenceSetPostalCode; ///< The full Block Sequence Set structure
    string blockRecord;
    BlockPostalCode myBlock;

    inputDatatoBlockSequenceSet(myBlockSequenceSetPostalCode, fileName); ///< Populate BSS from data file

    myBlock = myBlockSequenceSetPostalCode.getHead(); ///< Pointer to first block

    // outputFile << blockRecord << endl;
    tree.insert(myBlock.getBlockItem().getZip());

    myBlock = *myBlock.getNext(); ///< Move to next block

    // Loop through middle blocks
    while (myBlock.getNext() != nullptr)
    {
        // outputFile << blockRecord << endl;
        tree.insert(myBlock.getBlockItem().getZip());

        myBlock = *myBlock.getNext(); ///< Advance to next block
    }

    // outputFile << blockRecord << endl;
    tree.insert(myBlock.getBlockItem().getZip());

    // Save the original std::cout buffer
    std::streambuf *originalCoutBuffer = std::cout.rdbuf();

    // Redirect std::cout to the file
    std::cout.rdbuf(outputFile.rdbuf());

    // Sequencing print B+tree
    tree.printTree();

    // Restore std::cout's original buffer
    std::cout.rdbuf(originalCoutBuffer);

    outputFile.close();

    cout << "B+tree builded successfully!" << endl;
    cout << "B+ tree file: B+Tree_data.txt" << endl;

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

        if (tree.search(zip)) // B+ tree index says "this ZIP exists"
        {
            PostalRecord rec;
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