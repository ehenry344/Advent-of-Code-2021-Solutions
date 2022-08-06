#include <string> 
#include <iostream> 
#include <fstream> 

// Part 1 : 
struct SubmarineData { 
    int x = 0; 
    int y = 0; 
};
// Part 2 : 
struct RefinedSubmarineData { 
    int x = 0; 
    int y = 0; 
    int aim = 0; 
}; 

SubmarineData parseCommands(std::string filePath);
RefinedSubmarineData interpretCommands(std::string filePath); 


int main() { 
    SubmarineData testData = parseCommands("day2input.txt"); 
    RefinedSubmarineData newData = interpretCommands("day2input.txt"); 

    std::cout << (testData.x * testData.y) << std::endl; 
    std::cout << (newData.x * newData.y) << std::endl; 
    
    return 0; 
}

// Day 2 (Part 1) : 

SubmarineData parseCommands(std::string filePath) { 
    SubmarineData returnData; 

    std::ifstream fileReader(filePath); 

    if (fileReader.is_open()) { 
        std::string dir; 
        int increment;

        while (fileReader >> dir >> increment) { 
            switch (*dir.c_str()) { 
                case 'f': 
                    returnData.x += increment; 
                    break;
                case 'd': 
                    returnData.y += increment; 
                    break;
                default: 
                    returnData.y -= increment; 
                    break;
            }
        }
    }

    fileReader.close(); 

    return returnData; 
}

// Day 2 (Part 2) : 

RefinedSubmarineData interpretCommands(std::string filePath) { 
    RefinedSubmarineData returnData; 

    std::ifstream fileReader(filePath); 

    if (fileReader.is_open()) { 
        std::string dir;
        int increment; 
        while (fileReader >> dir >> increment) { 
            switch (*dir.c_str()) { 
                case 'f': 
                    returnData.x += increment; 
                    returnData.y += returnData.aim * increment;
                    break;
                case 'd': 
                    returnData.aim += increment; 
                    break;
                default: 
                    returnData.aim -= increment; 
                    break;
            }
        }
    }

    return returnData; 
}


