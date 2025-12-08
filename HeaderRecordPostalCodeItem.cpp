/**
 * @file HeaderRecordPostalCodeItem.cpp
 * @brief Defines and manages individual HeaderRecord postal code items.
 * @author Mahad Farah, Kariniemi Carson, Tran Minh Quan, Rogers Mitchell, Asfaw Abel
 * @date 2025-10-17
 */

#include "HeaderRecordPostalCodeItem.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

/**
 * @brief Default constructor initializing member variables to default values.
 * zip is set to 0, place, state, and county are set to empty strings,
 * and latitude and longitude are set to 0.0.
 * This ensures that a HeaderRecordPostalCodeItem object starts with a known state.
 * @note This constructor can be used to create an empty HeaderRecordPostalCodeItem object,
 * which can later be populated with actual data using the setter methods.
 * @see HeaderRecordPostalCodeItem(int, const string&, const string&, const string&, double, double)
 * @see setZip(int)
 * @see setPlace(const string&)
 * @see setState(const string&)
 * @see setCounty(const string&)
 * @see setLatitude(double)
 * @see setLongitude(double)
 * @see printInfo() const
 */
HeaderRecordPostalCodeItem::HeaderRecordPostalCodeItem()
{
    recordLength = 0;
    zip = 0;
    place = "";
    state = "";
    county = "";
    latitude = 0;
    longitude = 0;
}

/**
 * @brief Parameterized constructor to initialize a HeaderRecordPostalCodeItem with specific values.
 * @param z The ZIP code (integer).
 * @param p The place name (string).
 * @param s The state name (string).
 * @param c The county name (string).
 * @param lat The latitude (double).
 * @param lon The longitude (double).
 * This constructor allows for the creation of a fully initialized HeaderRecordPostalCodeItem object.
 * @note Ensure that the provided values are valid and meaningful for the postal code entry.
 */
HeaderRecordPostalCodeItem::HeaderRecordPostalCodeItem(int r, int z, const string &p, const string &s, const string &c, double lat, double lon)
{
    recordLength = r;
    zip = z;
    place = p;
    state = s;
    county = c;
    latitude = lat;
    longitude = lon;
}

int HeaderRecordPostalCodeItem::getRecordLength() const
{
    return recordLength;
}

/**
 * @brief Get the ZIP code of the postal code item.
 * @return The ZIP code as an integer.
 */
int HeaderRecordPostalCodeItem::getZip() const
{
    return zip;
}

/**
 * @brief Get the place name of the postal code item.
 * @return The place name as a string.
 */
string HeaderRecordPostalCodeItem::getPlace() const
{
    return place;
}

/**
 * @brief Get the state name of the postal code item.
 * @return The state name as a string.
 */
string HeaderRecordPostalCodeItem::getState() const
{
    return state;
}

/**
 * @brief Get the county name of the postal code item.
 * @return The county name as a string.
 * @note County names may vary in format and length depending on the region.
 * Ensure that the county name is correctly formatted for display or processing.
 */
string HeaderRecordPostalCodeItem::getCounty() const
{
    return county;
}

/**
 * @brief Get the latitude of the postal code item.
 * @return The latitude as a double.
 * @note Latitude values are typically in the range of -90 to 90 degrees.
 */
double HeaderRecordPostalCodeItem::getLatitude() const
{
    return latitude;
}

/**
 * @brief Get the longitude of the postal code item.
 * @return The longitude as a double.
 * @note Longitude values are typically in the range of -180 to 180 degrees.
 */
double HeaderRecordPostalCodeItem::getLongitude() const
{
    return longitude;
}

string HeaderRecordPostalCodeItem::getData() const
{
    string zipCodeData = to_string(getZip()) + "," + getPlace() + "," + getState() + "," + getCounty() + "," + to_string(getLatitude()) + "," + to_string(getLongitude());
    return zipCodeData;
}

void HeaderRecordPostalCodeItem::setRecordLength(int newRecordLength)
{
    recordLength = newRecordLength;
}

/**
 * @brief Set the ZIP code of the postal code item.
 * @param newZip The new ZIP code to be set (integer).
 * @note Ensure that the new ZIP code is a valid integer value.
 */
void HeaderRecordPostalCodeItem::setZip(int newZip)
{
    zip = newZip;
}

/**
 * @brief Set the place name of the postal code item.
 * @param newPlace The new place name to be set (string).
 * @note Ensure that the new place name is a valid string value.
 */
void HeaderRecordPostalCodeItem::setPlace(const string &newPlace)
{
    place = newPlace;
}

/**
 * @brief Set the state name of the postal code item.
 * @param newState The new state name to be set (string).
 * @note Ensure that the new state name is a valid string value.
 */
void HeaderRecordPostalCodeItem::setState(const string &newState)
{
    state = newState;
}

/**
 * @brief Set the county name of the postal code item.
 * @param newCounty The new county name to be set (string).
 * @note Ensure that the new county name is a valid string value.
 */
void HeaderRecordPostalCodeItem::setCounty(const string &newCounty)
{
    county = newCounty;
}

/**
 * @brief Set the latitude of the postal code item.
 * @param newLat The new latitude to be set (double).
 * @note Ensure that the new latitude is within the valid range of -90 to 90 degrees.
 * Invalid latitude values may lead to incorrect geographical representations.
 */
void HeaderRecordPostalCodeItem::setLatitude(double newLat)
{
    latitude = newLat;
}

/**
 * @brief Set the longitude of the postal code item.
 * @param newLon The new longitude to be set (double).
 * @note Ensure that the new longitude is within the valid range of -180 to 180 degrees.
 * Invalid longitude values may lead to incorrect geographical representations.
 */
void HeaderRecordPostalCodeItem::setLongitude(double newLon)
{
    longitude = newLon;
}

/**
 * @brief Print the postal code item's information in a formatted manner.
 * The information includes ZIP code, place name, state, county, latitude, and longitude.
 * The output is aligned in columns for better readability.
 * @note This method uses standard output (cout) to display the information.
 */
void HeaderRecordPostalCodeItem::printInfo() const
{
    cout << left << setw(5) << recordLength
         << setw(10) << zip
         << setw(20) << place
         << setw(10) << state
         << setw(30) << county
         << setw(12) << latitude
         << setw(12) << longitude
         << endl;
}