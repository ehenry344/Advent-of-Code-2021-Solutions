#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// Util Functions : 

std::string filterCommonBits(std::vector<std::string> inputSet, int bitPosition, int filterType);
std::vector<std::string> vectorizeData(std::string filePath); 

// Day 3 (Part 1) : 

int getPowerConsumption(std::string inputPath);

// Day 3 (Part 2) : 

int lifeSupportRating(std::string inputPath); 


int main()
{
    std::cout << lifeSupportRating("day3input.txt");


    return 0;
}

// Day 3 (Part 1) : 
int getPowerConsumption(std::string inputPath)
{
    std::vector<int> bitInfo;

    std::ifstream inputReader(inputPath); 
    int setSize = 0; 

    if (inputReader.is_open()) {
        std::string currentByte; 

        while (inputReader >> currentByte) 
        { 
            if (currentByte.length() > bitInfo.size()) { // Dynamically Allocate Necessary Space For Operation 
                bitInfo.resize(currentByte.length()); 
            }

            for (int i = 0; i < currentByte.length(); i++) { 
                bitInfo[i] += currentByte[i] == '1'; 
            }

            setSize++; 
        }

        std::string rateComponents[2]; 

        for (int i = 0; i < bitInfo.size(); i++) { 
            int decValue = ( setSize - bitInfo[i] )  < ( setSize / 2 ); 

            rateComponents[0] += std::to_string(decValue); 
            rateComponents[1] += std::to_string(!decValue); 
        }

        return (
            std::stoull(rateComponents[0], 0, 2) * std::stoull(rateComponents[1], 0, 2) 
        ); 
    }
    
    return 0; 
}

// Day 3 (Part 2) : 

int lifeSupportRating(std::string inputPath) { 
    std::vector<std::string> byteFormat = vectorizeData(inputPath); 

    std::string O2GenRating = filterCommonBits(byteFormat, 0, 1); 
    std::string CO2ScrubRating = filterCommonBits(byteFormat, 0, 0); 

    return (
        std::stoull(O2GenRating, 0, 2) * std::stoull(CO2ScrubRating, 0, 2)
    );
}

// Recursive Function To Filter Set By Common Bit 

std::string filterCommonBits(std::vector<std::string> inputSet, int bitPosition, int filterType) { 
    std::vector<std::string> outputVector; 

    // Determine What the Most Common Bit Is 

    int bitCounts[2]{0, 0}; 

    for (int i = 0; i < inputSet.size(); i++) { 
        int currentBit = (inputSet[i][bitPosition]) - '0'; 
        bitCounts[currentBit]++; 
    }

    int commonBit = 0; 

    if (bitCounts[0] == bitCounts[1]) { 
        commonBit = filterType; 
    } else { 
        if (filterType == 1) { 
            commonBit = bitCounts[1] > bitCounts[0]; 
        } else { 
            commonBit = bitCounts[1] < bitCounts[0]; 
        }
    }

    // Now Filter The Dataset 

    std::copy_if(
        inputSet.begin(), inputSet.end(), std::back_inserter(outputVector), 
        [=](std::string byte) { 
            return (byte[bitPosition] - '0') == commonBit;
        }
    ); 

    if (outputVector.size() > 1) { 
        return filterCommonBits(outputVector, ++bitPosition, filterType); 
    }

    return outputVector[0]; // Return Last Value :D  
}

std::vector<std::string> vectorizeData(std::string filePath) { 
    std::vector<std::string> outputVec; 

    std::ifstream inputReader(filePath); 

    if (inputReader.is_open()) { 
        std::string currentLine; 

        while (inputReader >> currentLine) { 
            outputVec.push_back(currentLine); 
        }
    }

    return outputVec; 
}
