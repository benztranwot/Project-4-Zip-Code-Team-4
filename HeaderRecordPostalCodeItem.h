/**
 * @file HeaderRecordPostalCodeItem.h
 * @brief Defines and manages individual HeaderRecord postal code items.
 * @author Mahad Farah, Kariniemi Carson, Tran Minh Quan, Rogers Mitchell, Asfaw Abel
 * @date 2025-10-17
 */

#ifndef HEADER_RECORD_POSTAL_CODE_ITEM
#define HEADER_RECORD_POSTAL_CODE_ITEM

using namespace std;

class HeaderRecordPostalCodeItem
{
private:
    int recordLength;
    int zip;          /**< ZIP code */
    string place;     /**< Place name */
    string state;     /**< State abbreviation */
    string county;    /**< County name */
    double latitude;  /**< Latitude coordinate */
    double longitude; /**< Longitude coordinate */

public:
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
    HeaderRecordPostalCodeItem();

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
    HeaderRecordPostalCodeItem(int r, int z, const string &p, const string &s, const string &c, double lat, double lon);

    int getRecordLength() const;

    /**
     * @brief Get the ZIP code of the postal code item.
     * @return The ZIP code as an integer.
     */
    int getZip() const;

    /**
     * @brief Get the place name of the postal code item.
     * @return The place name as a string.
     */
    string getPlace() const;

    /**
     * @brief Get the state name of the postal code item.
     * @return The state name as a string.
     */
    string getState() const;

    /**
     * @brief Get the county name of the postal code item.
     * @return The county name as a string.
     * @note County names may vary in format and length depending on the region.
     * Ensure that the county name is correctly formatted for display or processing.
     */
    string getCounty() const;

    /**
     * @brief Get the latitude of the postal code item.
     * @return The latitude as a double.
     * @note Latitude values are typically in the range of -90 to 90 degrees.
     */
    double getLatitude() const;

    /**
     * @brief Get the longitude of the postal code item.
     * @return The longitude as a double.
     * @note Longitude values are typically in the range of -180 to 180 degrees.
     */
    double getLongitude() const;

    string getData() const;

    void setRecordLength(int newRecordLength);

    /**
     * @brief Set the ZIP code of the postal code item.
     * @param newZip The new ZIP code to be set (integer).
     * @note Ensure that the new ZIP code is a valid integer value.
     */
    void setZip(int newZip);

    /**
     * @brief Set the place name of the postal code item.
     * @param newPlace The new place name to be set (string).
     * @note Ensure that the new place name is a valid string value.
     */
    void setPlace(const string &newPlace);

    /**
     * @brief Set the state name of the postal code item.
     * @param newState The new state name to be set (string).
     * @note Ensure that the new state name is a valid string value.
     */
    void setState(const string &newState);

    /**
     * @brief Set the county name of the postal code item.
     * @param newCounty The new county name to be set (string).
     * @note Ensure that the new county name is a valid string value.
     */
    void setCounty(const string &newCounty);

    /**
     * @brief Set the latitude of the postal code item.
     * @param newLat The new latitude to be set (double).
     * @note Ensure that the new latitude is within the valid range of -90 to 90 degrees.
     * Invalid latitude values may lead to incorrect geographical representations.
     */
    void setLatitude(double newLat);

    /**
     * @brief Set the longitude of the postal code item.
     * @param newLon The new longitude to be set (double).
     * @note Ensure that the new longitude is within the valid range of -180 to 180 degrees.
     * Invalid longitude values may lead to incorrect geographical representations.
     */
    void setLongitude(double newLon);

    /**
     * @brief Print the postal code item's information in a formatted manner.
     * The information includes ZIP code, place name, state, county, latitude, and longitude.
     * The output is aligned in columns for better readability.
     * @note This method uses standard output (cout) to display the information.
     */
    void printInfo() const;
};

#include "HeaderRecordPostalCodeItem.cpp"
#endif