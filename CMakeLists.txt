cmake_minimum_required(VERSION 3.17)
project(Ex2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Ex2
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        AnomalyDetector.h
        MainTrain.cpp
        SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h
        timeseries.cpp
        timeseries.h)
