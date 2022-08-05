#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 


int countIncreaseIntervals(std::string filePath); 
int slidingIncreaseIntervals(std:: string filePath); 

std::vector<int> vectorizeData(std::string filePath); 


int main() { 
    std::cout << slidingIncreaseIntervals("day1input.txt"); 

    return 0; 
}

// Utility Function To Store File Integers in Vector 

std::vector<int> vectorizeData(std::string filePath) { 
    std::vector<int> returnData; 

    std::ifstream inputReader(filePath); 

    if (inputReader.is_open()) { 
        while (!inputReader.eof()) { 
            int currentLine; 
            inputReader >> currentLine; 

            returnData.push_back(currentLine); 
        }
    }

    inputReader.close(); 

    return returnData; 
}

// AOC Pt. 1

int countIncreaseIntervals(std::string filePath) {
    int increaseIntervals = 0; 

    std::vector<int> depthVector = vectorizeData(filePath); 

    for (int currentPos = 0; currentPos < depthVector.size(); currentPos++) { 
        if (currentPos != 0 && depthVector[currentPos - 1] < depthVector[currentPos]) { 
            increaseIntervals++;
        }
    }

    return increaseIntervals; 
}

// AOC Pt. 2 

int slidingIncreaseIntervals(std::string filePath) { 
    int increaseIntervals = 0; 

    std::vector<int> depthData = vectorizeData(filePath); 

    if (depthData.size() < 3) { // Block Incompatible Data Sets 
        return 0; 
    }

    int previousWindow; 

    for (int i = 0; i < depthData.size() - 2; i++) { 
        int currentWindow = depthData[i] + depthData[i + 1] + depthData[i + 2]; 

        if (previousWindow && currentWindow > previousWindow) { 
            increaseIntervals++; 
        }

        previousWindow = currentWindow; 
    }

    return increaseIntervals; 
}
