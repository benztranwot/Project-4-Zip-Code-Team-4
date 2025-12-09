#ifndef POSTAL_RECORD_H
#define POSTAL_RECORD_H

#include <string>

struct PostalRecord
{
    int zip;
    std::string place;
    std::string state;
    std::string county;
};

#endif // POSTAL_RECORD_H