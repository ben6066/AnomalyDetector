
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

// The method checks if a feature already exists in the featured correlations vector
bool SimpleAnomalyDetector::doesFeatureExist(string feature1, string feature2) {
    //vector is empty so no featured correlations exists
    if(cf.empty()){
        return false;
    }

    int size = cf.size();

    //check if a specific featured correlation exists
    for(int i = 0; i < size; i++){
        if(cf.at(i).feature1 == feature1 && cf.at(i).feature2 == feature2 ||
           cf.at(i).feature1 == feature2 && cf.at(i).feature2 == feature1){
            return true;
        }
    }

    return false;
}

// The method calculates all the required data in order to initialize a correlated feature
void SimpleAnomalyDetector::arrangeData(const TimeSeries& ts, int i, int j, float correlation){
    // The features names
    string featureName1 = ts.getFeatureNameByIndex(i);
    string featureName2 = ts.getFeatureNameByIndex(j);

    if(doesFeatureExist(featureName1, featureName2))
        return;

    //Vectors representing the features
    vector<float> feature1 = ts.getFeatureData(i);
    vector<float> feature2 = ts.getFeatureData(j);

    // The features columns as an arrays
    auto feature1AsArray = feature1.data();
    auto feature2AsArray = feature2.data();

    // Declaration of a vector containing Points
    vector<Point> pointsVector;

    // Declaration of a vector containing pointer to all the points in the 'pointsVector'
    vector<Point*> pointerToPointVector;

    // For loop. Insertion of all points into 'pointsVector'
    for(int k = 0; k < ts.getNumberOfSamples(); k++){
        pointsVector.push_back(Point(feature1AsArray[k], feature2AsArray[k]));
    }

    // For loop. insertion of all the pointers to the points in 'pointsVector'
    for(int k = 0; k < ts.getNumberOfSamples(); k++){
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
	int sizeOfColumn = ts.getNumberOfSamples();
	int numberOfColumns = ts.getNumberOfFeatures();

	//Vectors representing the features
	vector<float> feature1;
	vector<float> feature2;

	//Float arrays representing the features ('pearson' gets two float arrays)
	float* feature1AsArray;
	float* feature2AsArray;

	//For nested loop. go through all options of two different features
	for(int i = 0; i < numberOfColumns; i++){
	    maxCorrelation = 0;
        bestMatchingIndex = 0;

        //Get the matching feature data and convert it into an array
        feature1 = ts.getFeatureData(i);
        feature1AsArray = feature1.data();

        for(int j = 0; j < numberOfColumns; j++){
            //There's no point to check a feature with himself
            if(i == j)
                continue;

            //Get the matching feature data and convert it into an array
            feature2 = ts.getFeatureData(j);
            feature2AsArray = feature2.data();

            //Calculate the correlation using the 'person' function
            currentCorrelation = fabs(pearson(feature1AsArray, feature2AsArray, sizeOfColumn));

            //Check that the correlation is above 0.9 and save the biggest correlation
            if(currentCorrelation > minCorrelation && (currentCorrelation > maxCorrelation)){
                maxCorrelation = currentCorrelation;

                bestMatchingIndex = j;
            }
        }

        //Arrangement of data is only necessary in case that the correlation is above 0.9
        if(maxCorrelation > 0.9)
            arrangeData(ts, i, bestMatchingIndex, maxCorrelation);
	}
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<float> currentSample;
	vector<AnomalyReport> reportsVector;

	int numOfSamples = ts.getNumberOfSamples();
	int sampleResult;

	for(int i = 0; i < cf.size(); i++){
        for(int sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++){
            Point p = ts.getMatchingPoint(sampleIndex, cf.at(i).feature1, cf.at(i).feature2);

            sampleResult = detectSample(p, cf.at(i).lin_reg, cf.at(i).threshold);

            //Detected Error
            if(sampleResult == 1){
                AnomalyReport report(cf.at(i).feature1 + "-" + cf.at(i).feature2, sampleIndex + 1);
                reportsVector.push_back(report);
            }
        }
	}

	return reportsVector;
}

int SimpleAnomalyDetector::detectSample(Point point, Line linearLine, float threshold){
    float deviation = dev(point, linearLine);

    if(deviation > (threshold * 1.2)){
        return 1;
    }

    return 0;
}

