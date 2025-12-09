/**
 * @file PostalRecord.h
 * @brief Defines the PostalRecord struct used to store ZIP code information.
 *
 * This file contains the definition of the PostalRecord structure, which holds
 * information about a postal region, including the ZIP code, place name, state,
 * and county. It is intended for use in applications that process or store
 * geographic or postal data.
 */

#ifndef POSTAL_RECORD_H
#define POSTAL_RECORD_H

#include <string>

/**
 * @struct PostalRecord
 * @brief Represents a record containing postal region information.
 *
 * The PostalRecord struct stores key postal attributes such as ZIP code, place
 * name, state, and county. It can be used as a basic data container for mapping
 * or searching postal information.
 */
struct PostalRecord
{
    /**
     * @brief ZIP code (5-digit integer).
     */
    int zip;

    /**
     * @brief Name of the city or locality associated with the ZIP code.
     */
    std::string place;

    /**
     * @brief U.S. state abbreviation (e.g., "MN", "CA").
     */
    std::string state;

    /**
     * @brief County name associated with the ZIP code.
     */
    std::string county;
};

#endif // POSTAL_RECORD_H
