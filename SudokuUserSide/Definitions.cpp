//Definitions.cpp Souce File For Sudoku Class
#include "Header.h"
#include <fstream>
#include <string>
#include <ostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;

typedef mt19937 RandGenerator;
RandGenerator random;

//Now class definitions
//Description: this will create an 2 random 2d sudoku board of size 9x9
//Pre-condition: none
//Post-condition: 2 9x9 arrays are created and ready for input
Sudoku::Sudoku() {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j) {
            sudokuBoard[i][j] = 0;
            finalBoard[i][j] = 0;
        }
    change = 0;
    random.seed(time(nullptr));
}



//Description: checks subgrid for validity of number given from sudokugenerator
//Pre-condition: number is given from possiblesubgrid
//Post-condition: return false if invalid or true if valid number
bool Sudoku::checkSubGrid(int number, int x, int y) {
    int mid[2] = { (x * 3 + 1), (y * 3 + 1) };
    for (int i = mid[0] - 1; i <= mid[0] + 1; ++i) {
        for (int j = mid[1] - 1; j <= mid[1] + 1; ++j) {
            if (finalBoard[i][j] == 0)
                return false;
            if (number == finalBoard[i][j])
                return true;
        }
    }
    return false;
}

//Description: checks number against numbers in column to determine validity
//Pre-condition: generatepuzzle is active and going through cycle and possible column is sending integers for checks
//Post-condition: return true if number is valid or false if not
bool Sudoku::checkColumn(int number, int x) {
    for (int i = 0; i < SIZE; ++i) {
        if (finalBoard[i][x] == 0)
            break;
        if (finalBoard[i][x] == number)
            return true;
    }
    return false;
}
//Description: generate possible integers to go into checksubgrid where it will check for validity
//Pre-condition: generatepuzzle is active and going through cycle
//Post-condition:
vector<bool> Sudoku::possibleSubGrid(int x, int y) {
    vector<bool> check(9, true);
    int mid[2] = { x * 3 + 1, y * 3 + 1 };
    for (int i = mid[0] - 1; i <= mid[0] + 1; ++i) {
        for (int j = mid[1] - 1; j <= mid[1] + 1; ++j) {
            if (this->sudokuBoard[i][j] != 0)
                check[this->sudokuBoard[i][j] - 1] = false;
        }
    }
    return check;
}
//Description: generate possible integers to go into checkcolumn where it will check for validity
//Pre-condition: generatepuzzle is active and going through cycle 
//Post-condition: return false if invalid and true if it is possible integer for row
vector<bool> Sudoku::possibleColumn(int x) {
    vector<bool> check(9, true);
    for (int i = 0; i < SIZE; ++i) {
        if (this->sudokuBoard[i][x] != 0)
            check[this->sudokuBoard[i][x] - 1] = false;
    }
    return check;
}
//Description: generate possible integers to go into check where it will check for validity
//Pre-condition: generatepuzzle is active and going through cycle
//Post-condition: return false if invalid and true if it is possible integer for row
vector<bool> Sudoku::possibleRow(int i) {
    vector<bool> check(9, true);
    for (int j = 0; j < SIZE; ++j) {
        if (this->sudokuBoard[i][j] != 0)
            check[this->sudokuBoard[i][j] - 1] = false;
    }
    return check;
}

//Description: this is the core of the program. calls other functions and creates the puzzle
//Pre-condition: 2 vectors created from sudoku()
//Post-condition: final board is completed  
void Sudoku::generatePuzzle() {
    vector<bool> numbers(9, true);
    for (int i = 0; i < SIZE; ++i) {
        vector<bool> row = numbers;
        int j = 0;
        while (j < SIZE) {
            if (this->possibilities[i][j].size() == 0) {
                this->possibilities[i][j] = row;
            }
            if (possiblesCount(this->possibilities[i][j]) == 0) {
                this->possibilities[i][j].resize(0);
                --j;
                this->possibilities[i][j][finalBoard[i][j] - 1] = false;
                row[finalBoard[i][j] - 1] = true;
                finalBoard[i][j] = 0;
            }
            else {
                uniform_int_distribution<uint32_t> tempGenerator(0, possiblesCount(this->possibilities[i][j]) - 1);
                int number = tempGenerator(random) + 1;
                int pickedNumber = 0;
                while (true) {
                    if (this->possibilities[i][j][pickedNumber])
                        --number;
                    if (number == 0)
                        break;
                    ++pickedNumber;
                }
                if (checkSubGrid(pickedNumber + 1, i / 3, j / 3) || checkColumn(pickedNumber + 1, j)) {
                    this->possibilities[i][j][pickedNumber] = false;
                }
                else {
                    finalBoard[i][j] = pickedNumber + 1;
                    row[pickedNumber] = false;
                    ++j;
                }
            }
        }
    }
    copy();
}
//Description: 
//Pre-condition:
//Post-condition:
void Sudoku::singleSolved(int x, int y, int number) {
    int mid[2] = { (x / 3) * 3 + 1, (y / 3) * 3 + 1 };
    for (int i = mid[0] - 1; i <= mid[0] + 1; ++i) {
        for (int j = mid[1] - 1; j <= mid[1] + 1; ++j) {
            this->possibilities[i][j][number] = false;
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        this->possibilities[i][y][number] = false;
        this->possibilities[x][i][number] = false;
    }
    this->change++;
    this->sudokuBoard[x][y] = number + 1;
}
//Description:
//Pre-condition:
//Post-condition: 
int Sudoku::checkSingles() {
    int solved = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (this->sudokuBoard[i][j] == 0 &&
                possiblesCount(this->possibilities[i][j]) == 1) {
                for (int x = 0; x < 9; ++x) {
                    if (this->possibilities[i][j][x]) {
                        solved++;
                        singleSolved(i, j, x);
                        break;
                    }
                }
            }
        }
    }
    return solved;
}

//Description: function checks temp input to see if it can be validated and be put into board
//Pre-condition: integer was passed on to this function for check
//Post-condition: passes on whether the integer is valid or not
void Sudoku::checkUnknown(int x, int y) {
    int mid[2] = { (x / 3) * 3 + 1, (y / 3) * 3 + 1 };

    int temp[SIZE] = { 0 };
    for (int i = mid[0] - 1; i <= mid[0] + 1; ++i)
        for (int j = mid[1] - 1; j <= mid[1] + 1; ++j)
            if ((i != x) && (j != y)) {
                for (int k = 0; k < 9; ++k) {
                    if (this->possibilities[i][j][k]) temp[k]++;
                }
            }
    for (int i = 0; i < SIZE; ++i) {
        if (temp[i] == 1) {
            singleSolved(x, y, i);
        }
    }
    fill_n(temp, SIZE, 0);
    int column[SIZE] = { 0 };
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (this->possibilities[x][i][j]) temp[j]++;
            if (this->possibilities[i][y][j]) column[j]++;
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        if (temp[i] == 1) {
            singleSolved(x, y, i);
        }
        else if (column[i] == 1) {
            singleSolved(x, y, i);
        }
    }
}
//Description: function will attempt to solve the hint grid and will go through and check to solution after complete to make sure it solved correctly
//Pre-condition: sudoku hint file is ready
//Post-condition: sudoku is solved and will 
bool Sudoku::solve() {
    this->change = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            this->possibilities[i][j] = vector<bool>(9, false);
        }
    }
    while (true) {
        int undo = this->change;
        vector<bool> column[SIZE];
        vector<bool> row[SIZE];
        for (int j = 0; j < SIZE; ++j) {
            column[j] = possibleColumn(j);
        }
        for (int i = 0; i < SIZE; ++i) {
            row[i] = possibleRow(i);
            for (int j = 0; j < SIZE; ++j) {
                vector<bool> subGrid = possibleSubGrid(i / 3, j / 3);
                for (int k = 0; k < 9; ++k) {
                    this->possibilities[i][j][k] = (row[i][k] &&
                        column[j][k] &&
                        subGrid[k]);
                }
            }
        }
        if (checkSingles() == 0) {
            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    if (this->sudokuBoard[i][j] != 0) continue;
                    checkUnknown(i, j);
                }
            }
        }
        if (undo == this->change)
            break;
    }
    return solutionCheck();
}
//Description: copies boards to a temp location and tries to solve the hint board
//Pre-condition: board is created and passed on from generateboard
//Post-condition: solved or not solved
void Sudoku::copy() {
    int x[DELETE];
    int y[DELETE];
    while (true) {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                this->sudokuBoard[i][j] = this->finalBoard[i][j];
            }
        }
        uniform_int_distribution<uint32_t> temp(0, 8);
        for (int i = 0; i < DELETE; ++i) {
            x[i] = temp(random);
            y[i] = temp(random);
            while (this->sudokuBoard[x[i]][y[i]] == 0) {
                x[i] = temp(random);
                y[i] = temp(random);
            }
            this->sudokuBoard[x[i]][y[i]] = 0;
        }
        if (this->solve())
            break;
    }
    for (int i = 0; i < DELETE; ++i) {
        this->sudokuBoard[x[i]][y[i]] = 0;
    }
}
//Description: this will check the solution file once completed and sent from solve() before sending it to file to display
//Pre-condition: solve is finished processing and solving sudokuhintboard file
//Post-condition: return true if solved correctly. if unsolved return false
bool Sudoku::solutionCheck() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (this->sudokuBoard[i][j] != this->finalBoard[i][j]) {
                return false;
            }
        }
    }
    return true;
}

//Discription: function checks to see if count is valid during generating sudoku board to make sure its in the 9x9 grid
//Pre-condition: generateboard is active and cycling through to create board
//Post-condition: returns # per cycles it goes through to make sure # is within range of board
int  Sudoku::possiblesCount(vector<bool> tryOut) {
    int numberChecker = 0;
    for (int i = 0; i < tryOut.size(); ++i) {
        if (tryOut[i]) ++numberChecker;
    }
    return numberChecker;

}



//Description: this will draw lines to separate board to make it more visible
//Pre-condition: completed sudoku board
//Post-condition: completed board with separated lines
void Sudoku::drawLineSeparate(string outFile) {
    ofstream out;
    out.open(outFile);
    drawSudoku(this->sudokuBoard, outFile);
}

//Description: this will draw lines to separate board to make it more visible
//Pre-condition: completed sudoku board
//Post-condition: completed board with separated lines
void Sudoku::drawHintLineSeparate(string hint) {
    ofstream hintFile;
    hintFile.open(hint);

    drawSudokuHint(this->sudokuBoard, hint);

}

//Description: function is called by drawsudoku and it prints 0 for none hint integers
//Pre-condition: drawsudokuhint is activated and creating hint file
//Post-condition: 0 for empty spots and hint number where needed
char  Sudoku::fill(const int x) {
    if (x == 0) {
        return '0';
    }
    else {
        return x + '0';
    }
}


//Description: creates squares and outputs them to outfile
//Pre-condition: sudokuboard is created and ready to output to file
//Post-condition: squares created and output to file and to screen
void  Sudoku::drawSudoku(int sudokuSquares[][SIZE], string outFile) {
    ofstream out;
    out.open(outFile);
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << fill(sudokuSquares[i][j]) << " ";
            out << fill(sudokuSquares[i][j]) << " ";
        }
        cout << endl;
        out << endl;
    }
}

//Description: function will create sudoku hint file by removing a predetermined number of integers from sudoku grid
//Pre-condition: sudoku grid is created and complete
//Post-condition: predetermined number of integers removed from sudoku grid and a new grid created with hints only
void  Sudoku::drawSudokuHint(int sudokuSquares[][SIZE], string hint) {
    ofstream hintFile;
    hintFile.open(hint);
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << fill(sudokuSquares[i][j]) << " ";
            hintFile << fill(sudokuSquares[i][j]) << " ";
        }
        cout << endl;
        hintFile << endl;
    }
}
