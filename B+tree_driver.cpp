#include <string>
#include "BlockPostalCode.h"
#include "BlockSequenceSetPostalCode.h"
#include "readHeaderPostalCodetoBSSBuffer.cpp"
#include <fstream>

#include "B+tree.cpp"

using namespace std;

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

    tree.printTree();

    // Restore std::cout's original buffer
    std::cout.rdbuf(originalCoutBuffer);

    outputFile.close();

    return 0;
}