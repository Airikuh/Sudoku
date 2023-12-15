#pragma once
//Header.h Header File for Sudoku Generator
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
using namespace std;


const int SIZE = 9;
const int DELETE = 50;


class Sudoku {

public:
    void generatePuzzle();

    void copy();


    Sudoku();

    ~Sudoku() = default;

    void drawLineSeparate(string outFile);
    void drawHintLineSeparate(string hint);
    void drawSudoku(int sudokuSquares[][SIZE], string outFile);
    void drawSudokuHint(int sudokuSquares[][SIZE], string hint);

    char fill(const int x);
    int change;
    int sudokuBoard[SIZE][SIZE];
    int finalBoard[SIZE][SIZE];

    vector<bool> possibilities[9][81];


    bool checkSubGrid(int number, int x, int y);
    bool checkColumn(int number, int x);

    vector<bool> possibleSubGrid(int x, int y);
    vector<bool> possibleColumn(int x);
    vector<bool> possibleRow(int x);

    void singleSolved(int x, int y, int number);
    bool solutionCheck();
    int checkSingles();
    void checkUnknown(int x, int y);

    int possiblesCount(vector<bool> tryOut);



    bool solve();
};
