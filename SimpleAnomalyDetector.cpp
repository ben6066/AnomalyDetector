
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector(){
	// TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

// The method inserts the correlated features according to the struct and adds them to the class member vector
void SimpleAnomalyDetector::insertData(string name1, string name2, float correlation, Line l, float threshold){
    correlatedFeatures data;

    // correlated features insertion
    data.feature1 = name1;
    data.feature2 = name2;
    data.corrlation = correlation;
    data.lin_reg = l;
    data.threshold = threshold;

    // struct insertion into the vector
    cf.push_back(data);
}

// The method calculates all the required data in order to initialize a correlated feature
void SimpleAnomalyDetector::arrangeData(const TimeSeries& ts, int i, int j, float correlation){
    // The features names
    string featureName1 = ts.getColumnNameByIndex(i);
    string featureName2 = ts.getColumnNameByIndex(j);

    // The features columns as an arrays
    auto feature1 = (float*)ts.getDataAsArray(i);
    auto feature2 = (float*)ts.getDataAsArray(j);

    // Declaration of a vector containing Points
    vector<Point> pointsVector;

    // Declaration of a vector containing pointer to all the points in the 'pointsVector'
    vector<Point*> pointerToPointVector;

    // For loop. Insertion of all points into 'pointsVector'
    for(int k = 0; k < ts.getSizeOfColumn(); k++){
        pointsVector.push_back(Point(feature1[k], feature2[k]));
    }

    // For loop. insertion of all the pointers to the points in 'pointsVector'
    for(int k = 0; k < ts.getSizeOfColumn(); k++){
        pointerToPointVector.push_back(&pointsVector.at(k));
    }

    // A pointer to the dynamic array 'pointerToPointVector'
    // The function .data() returns the array allocated in the heap
    Point** ppPointer = pointerToPointVector.data();

    // Creation of a linear line by using the linear_reg function
    Line linearLine = linear_reg(ppPointer, pointsVector.size());

    // Dev variables
    float currentDev = 0;
    float maxDev = 0;

    // For loop. find the biggest deviation between the line and the point
    for(int k = 0; k < pointsVector.size(); k++){
        currentDev = dev(pointsVector.at(k), linearLine);

        if(currentDev > maxDev){
            maxDev = currentDev;
        }
    }

    // Insertion of all the data required into the correlated features vector
    insertData(featureName1, featureName2, correlation, linearLine, maxDev);
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	float minCorrelation = 0.9;
	float currentCorrelation;
	float maxCorrelation;
	int bestMatchingIndex = 0;
	int sizeOfColumn = ts.getSizeOfColumn();
	int numberOfColumns = ts.getNumberOfColumns();

	const float* feature1;
	const float* feature2;

	for(int i = 0; i < numberOfColumns; i++){
	    maxCorrelation = 0;
        bestMatchingIndex = 0;

        for(int j = i + 1; j < numberOfColumns; j++){


            feature1 = ts.getDataAsArray(i);
            feature2 = ts.getDataAsArray(j);

            currentCorrelation = fabs(pearson((float*)feature1, (float*)feature2, sizeOfColumn));

            if(currentCorrelation > minCorrelation && (currentCorrelation > maxCorrelation)){
                maxCorrelation = currentCorrelation;

                bestMatchingIndex = j;
            }
        }

        if(bestMatchingIndex != 0 && i != bestMatchingIndex){
                arrangeData(ts, i, bestMatchingIndex, maxCorrelation);
        }
	}
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	// TODO Auto-generated destructor stub
	return vector<AnomalyReport>();
}

