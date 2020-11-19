#include "timeseries.h"
#include "anomaly_detection_util.h"

//Constructor
TimeSeries::TimeSeries(const char* CSVfileName){
    numberOfColumns = 0;
    sizeOfColumn = 0;
    readFile(CSVfileName);
}

// The method parse a CSV file into a vector of pairs containing <string, vector<float>>
// Where each pair represents <column name, column values (represented as a float vector)>
void TimeSeries::readFile(const char* fileName){
    // Create an input filestream
    ifstream flightDataCSVFile(fileName);

    // Throw an exception in case file is not open
    if(!flightDataCSVFile.is_open()){
        throw runtime_error("Could not open file");
    }

    string currentLine, columnName;
    float val;
    int columnIndex;

    // Firstly - iterate the first line and extract the column names
    // Place every column name in the "first" part of each pair
    getline(flightDataCSVFile, currentLine);

    // Create a buffer from the current line
    stringstream columnNameBuffer(currentLine);

    // Extract each column name, each name is separated by ','
    while (getline(columnNameBuffer, columnName, ',')){
        // Add a pair of <columnName, vector<float>> to the flightData vector
        flightData.push_back({columnName, vector<float>{}});
    }

    // Iterate the CSV file line after line
    while(getline(flightDataCSVFile, currentLine)){
        // Create a buffer of the current line
        stringstream dataBuffer(currentLine);

        // The current column index
        columnIndex = 0;

        // Add each number to the matching vector in the matching pair
        while(dataBuffer >> val){
            // Add the number to the matching column - we use '.second' to access the vector in the pair
            flightData.at(columnIndex).second.push_back(val);

            // If the next char is a comma, skip it
            if(dataBuffer.peek() == ','){
                dataBuffer.ignore();
            }

            // Increment the column index
            columnIndex++;
        }

        //Increment the size of a column
        sizeOfColumn++;
    }

    numberOfColumns = flightData.size();

    // Close the CSV file
    flightDataCSVFile.close();
}

//The method returns the size of a column - not including the first line
int TimeSeries:: getNumberOfSamples() const{
    return sizeOfColumn;
}

//The method returns the number of columns in the file
int TimeSeries:: getNumberOfFeatures() const{
    return numberOfColumns;
}

//The method returns the matching column name
string TimeSeries::getFeatureNameByIndex(int index) const{
    if(flightData.empty() || index > flightData.size() - 1)
    {
        return NULL;
    }
    return flightData.at(index).first;
}

//The method returns a point of a specific correlated features
// feature - a specific column in the csv file (not including the feature name)
vector<float> TimeSeries:: getFeatureData(int index) const{
   return flightData.at(index).second;
}

//The method returns a vector of a specific sample
//sample - a specific row in the csv file (not including the first row)
Point TimeSeries:: getMatchingPoint(int index, string feature1, string feature2) const{
    vector<float> temp;

    for(int colIdx = 0; colIdx < numberOfColumns; colIdx++){
        if(flightData.at(colIdx).first == feature1 || flightData.at(colIdx).first == feature2){
            temp.push_back(flightData.at(colIdx).second.at(index));
        }
    }

    return Point(temp.at(0), temp.at(1));
}

