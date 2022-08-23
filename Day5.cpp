#include <iostream>
#include <fstream>

#include <string>
#include <regex> 

#include <vector> 
#include <unordered_map>

struct Point { 
    int x, y; 

    Point(int inputX, int inputY) { 
        x = inputX; y = inputY; 
    }
}; 

struct PointHash { 
    std::size_t operator() (const Point & pHash) const { 
        return pHash.x ^ pHash.y; 
    }
}; 

struct IsEqual { 
    bool operator() (const Point & p1, const Point & p2) const { 
        return p1.x == p2.x && p1.y == p2.y; 
    }
}; 
bool isStraight(const Point p1, const Point p2) { 
    return (p1.x == p2.x) || (p1.y == p2.y); 
}

std::vector<int> extractIntegers(std::string rawLine) { 
    std::vector<int> returnVector; 

    std::regex numPattern("\\d+"); 
    std::sregex_iterator numIterator(rawLine.begin(), rawLine.end(), numPattern); 

    for (int i = 0; i < 4; i++) { 
        std::smatch cMatch = *numIterator;

        returnVector.push_back(stoi(cMatch.str())); 

        numIterator++; 
    }

    return returnVector; 
} 

int getOverlapMax(std::string inputPath, bool isP1) { 
    std::ifstream inputReader(inputPath); 
    if (!inputReader) { 
        return 0; 
    }

    std::unordered_map<Point, int, PointHash, IsEqual> overlapContainer; 

    std::string currentLine; 
    while (std::getline(inputReader, currentLine)) { 
        std::vector<int> extractedValues = extractIntegers(currentLine); 

        if (extractedValues.size() > 0) {
            Point p1 (extractedValues[0], extractedValues[1]); 
            Point p2 (extractedValues[2], extractedValues[3]); 

            if (isP1 && !isStraight(p1, p2)) { 
                continue; 
            }

            int deltaX = (p1.x - p2.x) == 0 ? 0 : (p1.x - p2.x) < 0 ? 1 : -1; 
            int deltaY = (p1.y - p2.y) == 0 ? 0 : (p1.y - p2.y) < 0 ? 1 : -1; 

            while (p1.x != p2.x + deltaX || p1.y != p2.y + deltaY) { 
                auto findIter = overlapContainer.find(p1);

                if (findIter != overlapContainer.end()) { 
                    findIter->second += 1; 
                } else { 
                    overlapContainer.insert({p1, 1}); 
                }

                p1.x += deltaX;
                p1.y += deltaY; 
            }
        }
    }

    int doubleCross = std::count_if(
        overlapContainer.begin(), overlapContainer.end(), 
        
        [] (const std::pair<Point, int> & e1) { 
            return e1.second >= 2; 
        }
    );
    return doubleCross; 
}

int main() { 
    int part1 = getOverlapMax("day5input.txt", true); 
    int part2 = getOverlapMax("day5input.txt", false); 

    std::cout << "Advent of Code Day 5 (Hydrothermal Vents)" << std::endl;
    std::cout << "Part 1 Answer : " << part1 << std::endl;
    std::cout << "Part 2 Answer : " << part2; 

    return 0; 
}
