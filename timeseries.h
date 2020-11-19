#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <iostream>

using namespace std;

class TimeSeries{
    //Private Members
    int numberOfColumns;
    int sizeOfColumn;
    vector<pair<string, vector<float>>> flightData;

    //Private Methods
    void readFile(const char* fileName);

public:
    //Public Methods
	TimeSeries(const char* CSVfileName);
    int getNumberOfColumns() const;
    int getSizeOfColumn() const;
    string getColumnNameByIndex(int index) const;
    const float* getDataAsArray(int index) const;
};

#endif /* TIMESERIES_H_ */
