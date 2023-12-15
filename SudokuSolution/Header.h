//Program Name:Sudoku Solver
//Programmer Name:Erika Valle-Baird
//Description:Puzzle Solver that takes in a file containing hints, uses recursion
//to solve.  Prints the answer and recursive solving tries to separate files.
//Date Created:08/07/2020

//Header.h Header File For SudokuSolver Class
#pragma once
#include <fstream>
#include <string>
#include <ostream>
#include <vector>
#include <iomanip>
#include <random>
#include <ctime>
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>


using namespace std;

//Variable Declarations Used Throughout Program
typedef vector<int> numbers;
typedef vector<numbers> Game;
//Counters Declared and Initialized
static int subGridCounter = 0;
static int rowCounter = 0;
static int columnCounter = 0;
static int recursiveCounter = 0;

class SudokuSolver {
    //Variable Declarations
    Game SudokuBoard;
    numbers checker;
    vector<string> FileInput;
    int rowCheck(int row);
    int columnCheck(int col);
    int subGridCheck(int row, int col);
public:
    int getSubGridCalls();
    int getRowCalls();
    int getColumnCalls();
    int getRecurrCalls();
    int RecursionToSolve(int row, int col,string solving);
    SudokuSolver(string hint);
    int solveSudoku(string solving);
    void print();
    void printAnswerToFile(string answer);
    void printRecursion(string solving, bool append);


};

//Description: Keeps track of calls to subGridCheck
//Pre-condition:  Initialized to zero
//Post-condition: Allows main to get amount of calls
int SudokuSolver::getSubGridCalls() {
    return subGridCounter;
}

//Description:  Keeps track of calls to rowCheck
//Pre-condition: Initialized to zero 
//Post-condition:  Allows main to get amount of calls
int SudokuSolver::getRowCalls() {
    return rowCounter;
}

//Description:  Keeps track of calls to colCheck
//Pre-condition:  Initialized to zero
//Post-condition:  Allows main to get amount of calls
int SudokuSolver::getColumnCalls() {
    return columnCounter;
}

//Description:  Keeps track of calls the recurance solver makes
//Pre-condition: Initialized to zero
//Post-condition:  Allows main to get amount of calls
int SudokuSolver::getRecurrCalls() {
    return recursiveCounter;
}


//Description: Class Constructor to recieve file input
//Pre-condition: Input File exists
//Post-condition: Data is loaded onto the table if data is valid,
//if not error messages displayed
SudokuSolver::SudokuSolver(string hint) {
    //Variable Declarations
    ifstream hintFile;
    hintFile.open(hint);
    checker.resize(10);
    int value;
    string line;
    while (getline(hintFile, line)) {
        FileInput.push_back(line);
    }
    hintFile.close();
    print();
    SudokuBoard.resize(9);
    for (int i = 0; i < FileInput.size(); ++i) {
        string input = FileInput[i];
        istringstream fileStream(input);

        for (int j = 0; j < 9; ++j) {
            if (!(fileStream >> value)) {
                cout << "ERROR With File Read" << endl;
                exit(1);
            }
            if (value >= 0 && value <= 9) {
                SudokuBoard[i].push_back(value);
            }
            else {
                cout << "ERROR Invalid File Element" << endl;
                exit(1);
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        if (!rowCheck(i)) {
            cout << "ERROR Invalid Row" << endl;
            exit(1);
        }
        if (!columnCheck(i)) {

            cout << "ERROR Invalid Column" << endl;
            exit(1);
        }
    }
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            if (!subGridCheck(i, j)) {
                cout << "ERROR Invalid SubGrid" << endl;
                exit(1);
            }
        }
    }
   
}

//Description: Checks if viable solution in row
//Pre-condition: Board Initialized with valid values
//Post-condition:  Returns valid or not to be placed in row
int SudokuSolver::rowCheck(int row) {
    //Variable Declaration and initialization
    int countRow = 1;
    for (int i = 0; i <= 9; i++)
        checker[i] = 0;
    for (int i = 0; i < 9; i++) {
        if (SudokuBoard[row][i] != 0) {
            if (checker[SudokuBoard[row][i]])
                return 0;
            checker[SudokuBoard[row][i]] = 1;
            ++countRow;
        }
    }
    rowCounter = countRow;
    return 1;
}

//Description: Checks if viable solution in column
//Pre-condition:  Board Initialized with valid values
//Post-condition: Returns valid or not to be placed in sub column
int SudokuSolver::columnCheck(int col) {
    //Variable Declaration and initialization
    int columnCount = 1;
    for (int i = 0; i <= 9; i++)
        checker[i] = 0;
    for (int i = 0; i < 9; i++) {
        if (SudokuBoard[i][col] != 0) {
            if (checker[SudokuBoard[i][col]])
                return 0;
            checker[SudokuBoard[i][col]] = 1;
            ++columnCount;
        }
    }
    columnCounter = columnCount;
    return 1;
}


//Description:  Checks if viable solution in sub grid
//Pre-condition:  Board Initialized with valid values
//Post-condition:  Returns valid or not to be placed in sub grid
int SudokuSolver::subGridCheck(int x, int y) {
    //Variable Declarations and initialization
    int row, col;
    int subGrid = 1;

    for (int i = 0; i <= 9; i++)
        checker[i] = 0;
    for (row = x; row < x + 3; row++) {
        for (col = y; col < y + 3; col++) {
            if (SudokuBoard[row][col] != 0) {
                if (checker[SudokuBoard[row][col]])
                    return 0;
                checker[SudokuBoard[row][col]] = 1;
                ++subGrid;
            }
        }
    }
    subGridCounter = subGrid;
    return 1;
}




//Description: Method that allows main to call and calls solving method
//Pre-condition: Called from main with output file name
//Post-condition: Calls method that uses recursion to solve 
int SudokuSolver::solveSudoku(string solving) { 
    //Output File Declaration and open for writing
    ofstream solveFile;
    solveFile.open(solving, ios_base::app);
    return RecursionToSolve(0, 0, solving); }


//Description: Method to solve puzzle using recursion
//Pre-condition: Valid viable table, output file created 
//Post-condition: Determines if puzzle can be solved.  If it can
//Solves it and outputs recursive method to output file 
int SudokuSolver::RecursionToSolve(int row, int col, string solving) {
    //Variable Declarations and initialization
    int recur = 1;
    ofstream solveFile;
    solveFile.open(solving, ios_base::app);
    while (row < 9 && SudokuBoard[row][col] != 0) {
        col++;
        if (col == 9) {
            row++;
            col = 0;            
        }     
    }
    
    if (row == 9) 
        return 1;
    for (int i = 1; i <= 9; i++) {
        SudokuBoard[row][col] = i;   
        if (rowCheck(row) && columnCheck(col) && subGridCheck(row - row % 3, col - col % 3)  && RecursionToSolve(row, col, solving)) {
            return 1;
        }
        if (i % 3 == 0) {
            ++recur;
            printRecursion(solving, true);
            }
        
    }
    SudokuBoard[row][col] = 0;
    recursiveCounter = recur;

    return 0;
   
}


//Description: Print method to print input file and answer to screen
//Pre-condition: Input File exists and is valid, puzzle is sovable
//Post-condition: Input file and answer to puzzle is printed to screen for user
void SudokuSolver::print() {
    for (int i = 0; i < 11; i++)
        cout << "--";
    cout << endl;

    for (int i = 0; i < SudokuBoard.size(); i++) {
        for (int j = 0; j < SudokuBoard[i].size(); j++) {
            if (SudokuBoard[i][j] == 0) 
                cout << "  ";
            else 
                cout << SudokuBoard[i][j] << " ";
            if (j == 2 || j == 5) 
                cout << "| ";
        }
        cout << endl;
        if (i == 2 || i == 5) {
            for (int k = 0; k < 11; k++) 
                cout << "--";
            cout << endl;
        }
    }
    cout << endl;
    cout << endl;
}


//Description:  Print method to print answer to puzzle to file
//Pre-condition: Output file exists
//Post-condition: Answer to puzzle written to output file
void SudokuSolver::printAnswerToFile(string answer) {
    //File Declaration and Open
    ofstream answerFile;
    answerFile.open(answer);
    for (int i = 0; i < 11; i++)
        answerFile << "--";
    answerFile << endl;

    for (int i = 0; i < SudokuBoard.size(); i++) {
        for (int j = 0; j < SudokuBoard[i].size(); j++) {
            if (SudokuBoard[i][j] == 0)
                answerFile << "  ";
            else
                answerFile << SudokuBoard[i][j] << " ";
            if (j == 2 || j == 5)
                answerFile << "| ";
        }
        answerFile << endl;
        if (i == 2 || i == 5) {
            for (int k = 0; k < 11; k++)
                answerFile << "--";
            answerFile << endl;
        }
    }
    answerFile << endl;
}



//Description: Print method to print Recursive Solving attempts while solving puzzle to file
//Pre-condition: Output file exists and is open for appending information 
//Post-condition: Recursive Solving attempts is written to file
void SudokuSolver::printRecursion(string solving, bool append) {
    //File Declaration and Open for writing
    ofstream solveFile;
    if (append) {
        solveFile.open(solving, ios_base::app);
    }
    else {
        solveFile.open(solving);
    }
    for (int i = 0; i < 11; i++)
        solveFile << "--";
    solveFile << endl;

    for (int i = 0; i < SudokuBoard.size(); i++) {
        for (int j = 0; j < SudokuBoard[i].size(); j++) {
            if (SudokuBoard[i][j] == 0)
                solveFile << "  ";
            else
                solveFile << SudokuBoard[i][j] << " ";
            if (j == 2 || j == 5)
                solveFile << "| ";
        }
        solveFile << endl;
        if (i == 2 || i == 5) {
            for (int k = 0; k < 11; k++)
                solveFile << "--";
            solveFile << endl;
        }
    }
    solveFile << endl;
}




