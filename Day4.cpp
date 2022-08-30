#include <iostream>
#include <fstream>

#include <string>
#include <regex>

#include <vector> 

struct Vector2 { 
    int x, y; 
    Vector2() { 
        x = 9; y = 9; // Default initialization to 9 meaning out of range... 
    }
    Vector2(int iX, int iY) { 
        x = iX; 
        y = iY; 
    }
}; 


struct BingoGrid { 
    int board[5][5]; // 25 element array that holds all the of the data for the bingoboards. 
    bool hasWon = false; // prevents this baord frmo being searched again... 
    int boardDimension = 5; 

    std::vector<int> relevancyList; 

    bool isRelevant(int toCheck) { 
        for (int entry: relevancyList) { 
            if (entry == toCheck) { 
                return true; 
            }
        }
        return false; 
    }

    Vector2 getPosition(int iNum) { 
        for (int x = 0; x < boardDimension; x++) { 
            for (int y = 0; y < boardDimension; y++) { 
                if (board[x][y] == iNum) { 
                    return Vector2(x, y); 
                }
            }
        }
        return Vector2(); 
    }

    void printBoard() { 
        for (int i = 0; i < boardDimension; i++)  {
            for (int j = 0; j < boardDimension; j++) { 
                std::cout << " " << board[i][j]; 
            }
            std::cout << std::endl;
        }
    }

    int processDraw(int drawNum) { // processes each draw from the system 
        Vector2 nPos = getPosition(drawNum); 

        if (nPos.x != 9) { // determines that this number is on the board... 
            relevancyList.push_back(drawNum); 

            int columnTally = 0;  
            int rowTally = 0; 
            for (int currentPos = 0; currentPos < boardDimension; currentPos++) { 
                columnTally += isRelevant(board[nPos.x][currentPos]); 
                rowTally += isRelevant(board[currentPos][nPos.y]); 
            }

            return std::max(columnTally, rowTally); 
        }
        return 0; 
    }

    int getScore() { 
        int sumUnm = 0; 
        for (int i = 0; i < boardDimension; i++) { 
            for (int j = 0; j < boardDimension; j++) { 
                if (!isRelevant(board[i][j])) { 
                    sumUnm += board[i][j]; 
                }
            }
        }
        return sumUnm * relevancyList.back(); 
    }
}; 

std::istream & operator >> (std::istream & cStream, BingoGrid & cGrid) { // formatted extraction of bingo board 
    for (int x = 0; x < 5; x++) { 
        for (int y = 0; y < 5; y++) { 
            cStream >> cGrid.board[x][y]; 
        }
    }
    return cStream; 
}

std::vector<int> scrapeIntegers(std::string inputStr) { 
    std::vector<int> returnVector; 

    std::regex numR("\\d+"); 
    std::sregex_iterator numData(inputStr.begin(), inputStr.end(), numR); 
    std::sregex_iterator endSeq;

    while (numData != endSeq) { 
        returnVector.push_back(std::stoi(numData->str())); 
        numData++; 
    }

    return returnVector; 
}


int winningScore(std::string filePath, bool solvePart) { 
    std::vector<int> guessList; 
    std::vector<BingoGrid> boards; 

    // Handle Input Acquisition 

    { // Input Extraction Is Now Working 
        std::ifstream inputReader(filePath); 
        if (!inputReader) { std::abort(); }

        std::string currentLine; 

        std::getline(inputReader, currentLine); 
        guessList = scrapeIntegers(currentLine); 

        while (!inputReader.eof()) { 
            BingoGrid newBoard; 
            inputReader >> newBoard; 
            boards.push_back(newBoard); 
        }
    }

    int lastScore = 0; 

    for (int i = 0; i < guessList.size(); i++) { 
        for (int j = 0; j < boards.size(); j++) { 
            // part 1 is to find the first win... 

            if (!boards[j].hasWon && boards[j].processDraw(guessList[i]) == 5) { // determines if the board is a win... 
                lastScore = boards[j].getScore(); 
                boards[j].hasWon = true; 

                if (solvePart) { 
                    return lastScore; 
                } 
            }
        }
    }
    return lastScore; 
}
int main() { 
    int part1_Score = winningScore("InputFolder/Day4Input.txt", true); 
    int part2_Score = winningScore("InputFolder/Day4Input.txt", false); 

    std::cout << "The score of the first winning board is : " << part1_Score << std::endl;
    std::cout << "The score of the last winning board is : " << part2_Score << std::endl;

    return 0; 
}

