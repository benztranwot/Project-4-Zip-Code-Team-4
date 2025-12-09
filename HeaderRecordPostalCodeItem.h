/**
 * @file HeaderRecordPostalCodeItem.cpp
 * @brief Defines and manages individual HeaderRecord postal code items.
 * @author Mahad Farah, Kariniemi Carson, Tran Minh Quan, Rogers Mitchell, Asfaw Abel
 * @date 2025-10-17
 */
#ifndef HEADER_RECORD_POSTAL_CODE_ITEM_H
#define HEADER_RECORD_POSTAL_CODE_ITEM_H

#include <string>

class HeaderRecordPostalCodeItem {
private:
    int         recordLength;
    int         zip;
    std::string place;
    std::string state;
    std::string county;
    double      latitude;
    double      longitude;

public:
    // Constructors
    HeaderRecordPostalCodeItem();
    HeaderRecordPostalCodeItem(int recLen,
        int zipCode,
        const std::string& placeName,
        const std::string& stateCode,
        const std::string& countyName,
        double lat,
        double lon);

    // Getters
    int                 getRecordLength() const;
    int                 getZip()          const;
    const std::string& getPlace()        const;
    const std::string& getState()        const;
    const std::string& getCounty()       const;
    double              getLatitude()     const;
    double              getLongitude()    const;

    // Setters
    void setRecordLength(int len);
    void setZip(int zipCode);
    void setPlace(const std::string& placeName);
    void setState(const std::string& stateCode);
    void setCounty(const std::string& countyName);
    void setLatitude(double lat);
    void setLongitude(double lon);

    // For debugging / printing
    std::string getData() const;
};

#endif // HEADER_RECORD_POSTAL_CODE_ITEM_H
