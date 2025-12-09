#ifndef BLOCK_POSTAL_CODE_H
#define BLOCK_POSTAL_CODE_H

#include "HeaderRecordPostalCodeItem.h"

class BlockPostalCode {
private:
    HeaderRecordPostalCodeItem data;
    BlockPostalCode* prev;
    BlockPostalCode* next;

public:
    BlockPostalCode();

    void setBlockItem(const HeaderRecordPostalCodeItem& item);
    HeaderRecordPostalCodeItem getBlockItem() const;

    void setPrevRBN(BlockPostalCode* p);
    void setNextRBN(BlockPostalCode* n);

    BlockPostalCode* getPrev() const;
    BlockPostalCode* getNext() const;
};

#endif
    