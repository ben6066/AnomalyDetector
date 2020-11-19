#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <iostream>
#include "anomaly_detection_util.h"

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
    int getNumberOfFeatures() const;
    int getNumberOfSamples() const;
    string getFeatureNameByIndex(int index) const;
    vector<float> getFeatureData(int index) const;
    Point getMatchingPoint(int index, string feature1, string feature2) const;
};

#endif /* TIMESERIES_H_ */
