#include <iostream>
#include <fstream>

#include <string>
#include <regex> 

#include <numeric> 

#include <vector>

std::vector<int> stripIntegers(std::string filePath) { 
    std::vector<int> returnList; 

    std::string toStrip; 
    { 
        std::ifstream inputReader(filePath); 
        if (!inputReader) { 
            return returnList; 
        }
        std::getline(inputReader, toStrip); 

        inputReader.close(); 
    }
    std::regex numReg("\\d+"); 

    std::sregex_iterator strippedData(toStrip.begin(), toStrip.end(), numReg); 
    std::sregex_iterator endHolder; 
    
    while (strippedData != endHolder) { 
        std::smatch rNum = *strippedData; 
        returnList.push_back(std::stoi(rNum.str())); 
        strippedData++; 
    }

    return returnList; 
}

int getMedian(std::vector<int> inputData) { 
    if (inputData.size() >= 2) { 
        std::sort(inputData.begin(), inputData.end()); // sorts the set least -> greatest by default 
        return inputData[inputData.size() / 2]; 
    } 
    return 0; 
}

int getMean(std::vector<int> inputData) { 
    int sigma = std::accumulate(inputData.begin(), inputData.end(), 0); 
    return std::floor((double) sigma / inputData.size()); 
}

int getMostEfficient(std::string inputPath, bool solvePart) { 
    std::ifstream inputReader(inputPath); 

    if (!inputReader) { 
        exit(1); 
    }

    std::vector<int> hPositions = stripIntegers(inputPath);     
    int dPosition = solvePart ? getMedian(hPositions) : getMean(hPositions); 

    int fuelSum = 0; 

    std::for_each(
        hPositions.begin(), hPositions.end(), 
        [&] (const int cValue) { 
            int initCost = abs(cValue - dPosition); 

            if (solvePart) { 
                fuelSum += initCost; 
            } else { 
                int currentCost = 0; 
                for (int i = 0; i < initCost; i++) { 
                    currentCost += i + 1; 
                }
                fuelSum += currentCost; 
            }

        }
    ); 

    return fuelSum; 
}


int main() { 
    int part1_Solve = getMostEfficient("InputFolder/Day7Input.txt", true); 
    int part2_Solve = getMostEfficient("InputFolder/Day7Input.txt", false); 

    std::cout << "The most efficient fuel cost for part 1 is : " << part1_Solve << std::endl;
    std::cout << "The most efficient fuel cost for part 2 is : " << part2_Solve; 
    
    return 0; 
}
