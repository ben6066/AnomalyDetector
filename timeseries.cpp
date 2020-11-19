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
int TimeSeries:: getSizeOfColumn() const{
    return sizeOfColumn;
}

//The method returns the number of columns in the file
int TimeSeries:: getNumberOfColumns() const{
    return numberOfColumns;
}

//The method returns the matching column name
string TimeSeries::getColumnNameByIndex(int index) const{
    if(flightData.empty() || index > flightData.size() - 1)
    {
        return NULL;
    }
    return flightData.at(index).first;
}

//The method returns a pointer to a specific column as an array
const float* TimeSeries:: getDataAsArray(int index) const{
    if(index > flightData.size() - 1){
        return NULL;
    }

   return flightData.at(index).second.data();
}

