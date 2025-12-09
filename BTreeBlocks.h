#pragma once
#ifndef BTREE_BLOCKS_H
#define BTREE_BLOCKS_H

#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <string>
#include "HeaderRecordPostalCodeItem.h"

// Choose a block size consistent with your project
const int BLOCK_SIZE = 512;

/*--- Block type and common header ---*/
enum BlockType : char {
    BLOCK_UNUSED = 0,
    BLOCK_INDEX = 'I',
    BLOCK_LEAF = 'L'
};

struct BlockHeader {
    BlockType type;
    int selfRBN;
};

/* --- B+Tree header (RBN 0) --- */
struct BTreeHeader {
    int blockSize;
    int rootRBN;
    int firstLeafRBN;
    int lastLeafRBN;
    int totalRecords;
    int treeHeight;
};

/* --- Disk-safe postal record (no std::string) --- */
struct PostalRecordOnDisk {
    int recordLength;
    int zip;
    char place[40];
    char state[3];     // e.g., "MN"
    char county[40];
    double latitude;
    double longitude;
};

// Conversion helpers between RAM object and disk record
inline PostalRecordOnDisk toDisk(const HeaderRecordPostalCodeItem& src) {
    PostalRecordOnDisk dst{};
    dst.recordLength = src.getRecordLength();
    dst.zip = src.getZip();

    strncpy_s(dst.place, src.getPlace().c_str(), sizeof(dst.place) - 1);
    dst.place[sizeof(dst.place) - 1] = '\0';

    strncpy_s(dst.state, src.getState().c_str(), sizeof(dst.state) - 1);
    dst.state[sizeof(dst.state) - 1] = '\0';

    strncpy_s(dst.county, src.getCounty().c_str(), sizeof(dst.county) - 1);
    dst.county[sizeof(dst.county) - 1] = '\0';

    dst.latitude = src.getLatitude();
    dst.longitude = src.getLongitude();

    return dst;
}

inline HeaderRecordPostalCodeItem fromDisk(const PostalRecordOnDisk& src) {
    HeaderRecordPostalCodeItem dst;
    dst.setRecordLength(src.recordLength);
    dst.setZip(src.zip);
    dst.setPlace(std::string(src.place));
    dst.setState(std::string(src.state));
    dst.setCounty(std::string(src.county));
    dst.setLatitude(src.latitude);
    dst.setLongitude(src.longitude);
    return dst;
}

/*--- Index block ---*/

struct IndexEntry {
    int largestKey;
    int childRBN;
};

// Compute max entries so IndexBlock fits in BLOCK_SIZE
const int INDEX_MAX_ENTRIES =
(BLOCK_SIZE - sizeof(BlockHeader) - sizeof(int)) / sizeof(IndexEntry);

struct IndexBlock {
    BlockHeader header;   /* type = BLOCK_INDEX */
    int keyCount;
    IndexEntry entries[INDEX_MAX_ENTRIES];
};

/* --- Leaf block (sequence-set page) --- */

// Choose max records to fit in block:
const int LEAF_MAX_RECORDS =
(BLOCK_SIZE - sizeof(BlockHeader) - 3 * sizeof(int)) /
sizeof(PostalRecordOnDisk);

struct LeafBlock {
    BlockHeader header;   /* type = BLOCK_LEAF */
    int prevRBN;
    int nextRBN;
    int recordCount;
    PostalRecordOnDisk records[LEAF_MAX_RECORDS];
};

// ---- Pack / unpack helpers ----

inline void packIndexBlock(const IndexBlock& ib, char* buffer) {
    std::memset(buffer, 0, BLOCK_SIZE);
    std::memcpy(buffer, &ib, sizeof(IndexBlock));
}

inline void unpackIndexBlock(IndexBlock& ib, const char* buffer) {
    std::memcpy(&ib, buffer, sizeof(IndexBlock));
}

inline void packLeafBlock(const LeafBlock& lb, char* buffer) {
    std::memset(buffer, 0, BLOCK_SIZE);
    std::memcpy(buffer, &lb, sizeof(LeafBlock));
}

inline void unpackLeafBlock(LeafBlock& lb, const char* buffer) {
    std::memcpy(&lb, buffer, sizeof(LeafBlock));
}

// header pack/unpack for block 0

inline void writeHeaderBlock(char* buffer, const BTreeHeader& hdr) {
    std::memset(buffer, 0, BLOCK_SIZE);
    std::memcpy(buffer, &hdr, sizeof(BTreeHeader));
}

inline void readHeaderBlock(const char* buffer, BTreeHeader& hdr) {
    std::memcpy(&hdr, buffer, sizeof(BTreeHeader));
}

#endif
