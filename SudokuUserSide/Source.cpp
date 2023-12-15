//Source.cpp Source File for Sudoku Generator Class
#include "Header.h"
#include <fstream>
#include <string>
#include <ostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>


using namespace std;

//Description: main function is the program director that starts and runs the program
//Pre-condition: n/a
//Post-condition: program runs
int main() {
    Sudoku sudoku;
    ofstream out;
    string outFile;
    ofstream hintFile;
    string hint;

    cout << "Where Would You Like the Answer to be Stored?" << endl;
    cin >> outFile;
    out.open(outFile);
    if (!out.is_open())
    {
        cerr << "File does not exist, program will now terminate";
        return 0;
    }

    cout << "Where Would You Like the Puzzle to be Stored?" << endl;
    cin >> hint;
    hintFile.open(hint);
    if (hintFile.is_open())
    {
        cerr << "File does not exist, program will now terminate";
        return 0;
    }
    sudoku.generatePuzzle();

    sudoku.drawHintLineSeparate(hint);

    cout << endl;


    sudoku.solve();

    sudoku.drawLineSeparate(outFile);

    out.close();
    hintFile.close();

    return 0;
}
