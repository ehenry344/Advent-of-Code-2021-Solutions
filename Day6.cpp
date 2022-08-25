#include <iostream>
#include <fstream> 

#include <string>
#include <regex>

#include <vector>

class SimulationStruct { 
    std::vector<long long int> dataHolder; 
    int expP; 

    public: 
        SimulationStruct(std::vector<int> initData, int structureSize, int expPos) { 
            dataHolder.resize(structureSize); 

            std::for_each(
                initData.begin(), initData.end(),
                [&] (const int toIns) { 
                    dataHolder.at((structureSize - 1) - toIns) += 1; 
                }
            );

            expP = expPos; // holds the positions in which elements will be flowed into to model an exponential growth 
        }

        void ShiftStructure() { 
            std::vector<long long int> copyVec = dataHolder; // makes a presumed shallow copy of the dataholder to manipulate and assign to dataHolder
            // Handle the Shifting Of Data 
            for (int i = 0; i < dataHolder.size(); i++) { 
                if (i == dataHolder.size() - 1) {
                    copyVec[0] = dataHolder[i]; 
                    copyVec[expP] += dataHolder[i]; 
                } else { 
                    copyVec[i + 1] = dataHolder[i]; 
                }
            }
            dataHolder = copyVec; 
        }

        long long int SummateStructure() { 
            long long int returnData = 0; 

            std::for_each( // Just uses for_each loop with lambda expressions to add to the sum 
                dataHolder.begin(), dataHolder.end(), 
                [&] (const long long int toAdd) { 
                    returnData += toAdd; 
                }
            ); 

            return returnData; 
        }
    private: 
        void printData() { 
            for (int i = 0; i < dataHolder.size(); i++) { 
                std::cout << i << " " << dataHolder[i] << std::endl;
            }
        }
}; 

std::vector<int> stripIntegers(std::string filePath) { 
    std::vector<int> returnList; 

    std::string toStrip; 
    { 
        std::ifstream inputReader(filePath); 
        if (!inputReader) { 
            return returnList; 
        }
        std::getline(inputReader, toStrip); 
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

long long int simLanternFishCycles(std::string filePath, int cycleNum) { 
    SimulationStruct cycleManager(stripIntegers(filePath), 9, 2);

    for (int i = 0; i < cycleNum; i++) { 
        cycleManager.ShiftStructure(); // Shifts the structure according to the # of cycles 
    }

    return cycleManager.SummateStructure(); 
}

int main() { 
    int cycleNum = 80; 
    long long int cycleSum = simLanternFishCycles("InputFolder/Day6Input.txt", cycleNum); 

    std::cout << "The # of Lantern Fish after " << cycleNum << " cycles is : " << cycleSum << std::endl;

    return 0; 
}
