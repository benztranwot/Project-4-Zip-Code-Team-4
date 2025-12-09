// HeaderRecordPostalCodeItem.cpp
#include "HeaderRecordPostalCodeItem.h"
#include <sstream>

// Default constructor
HeaderRecordPostalCodeItem::HeaderRecordPostalCodeItem()
    : recordLength(0),
    zip(0),
    place(""),
    state(""),
    county(""),
    latitude(0.0),
    longitude(0.0)
{
}

// Full constructor
HeaderRecordPostalCodeItem::HeaderRecordPostalCodeItem(int recLen,
    int zipCode,
    const std::string& placeName,
    const std::string& stateCode,
    const std::string& countyName,
    double lat,
    double lon)
    : recordLength(recLen),
    zip(zipCode),
    place(placeName),
    state(stateCode),
    county(countyName),
    latitude(lat),
    longitude(lon)
{
}

// Getters
int HeaderRecordPostalCodeItem::getRecordLength() const { return recordLength; }
int HeaderRecordPostalCodeItem::getZip() const { return zip; }
const std::string& HeaderRecordPostalCodeItem::getPlace()  const { return place; }
const std::string& HeaderRecordPostalCodeItem::getState()  const { return state; }
const std::string& HeaderRecordPostalCodeItem::getCounty() const { return county; }
double HeaderRecordPostalCodeItem::getLatitude()  const { return latitude; }
double HeaderRecordPostalCodeItem::getLongitude() const { return longitude; }

// Setters
void HeaderRecordPostalCodeItem::setRecordLength(int len) { recordLength = len; }
void HeaderRecordPostalCodeItem::setZip(int zipCode) { zip = zipCode; }
void HeaderRecordPostalCodeItem::setPlace(const std::string& name) { place = name; }
void HeaderRecordPostalCodeItem::setState(const std::string& code) { state = code; }
void HeaderRecordPostalCodeItem::setCounty(const std::string& name) { county = name; }
void HeaderRecordPostalCodeItem::setLatitude(double lat) { latitude = lat; }
void HeaderRecordPostalCodeItem::setLongitude(double lon) { longitude = lon; }

// Debug string
std::string HeaderRecordPostalCodeItem::getData() const
{
    std::ostringstream oss;
    oss << "len=" << recordLength
        << ", zip=" << zip
        << ", place=" << place
        << ", state=" << state
        << ", county=" << county
        << ", lat=" << latitude
        << ", lon=" << longitude;
    return oss.str();
}
        