#include "BlockPostalCode.h"

BlockPostalCode::BlockPostalCode()
    : prev(nullptr), next(nullptr)
{
}

void BlockPostalCode::setBlockItem(const HeaderRecordPostalCodeItem& item) {
    data = item;
}

HeaderRecordPostalCodeItem BlockPostalCode::getBlockItem() const {
    return data;
}

void BlockPostalCode::setPrevRBN(BlockPostalCode* p) {
    prev = p;
}

void BlockPostalCode::setNextRBN(BlockPostalCode* n) {
    next = n;
}

BlockPostalCode* BlockPostalCode::getPrev() const {
    return prev;
}

BlockPostalCode* BlockPostalCode::getNext() const {
    return next;
}
