#include "Header.h"

using namespace std;

//Description: Function to display hint file to screen
//Pre-condition: Input file exists and is valid 
//Post-condition: Calls Method to print
void hintFileToScreen(SudokuSolver display) {
    cout << "Hint File: "<< endl;
	display.print();
}


//Description: Function to display solved puzzle to screen
//Pre-condition: Output files exist and have been named via user
//Post-condition: Calls method to solve sudoku, prints to screen
//and output file
void solvedPuzzle(SudokuSolver display,string answer,string solving) {
	ofstream solveFile;
	solveFile.open(solving);
	ofstream answerFile;
	answerFile.open(answer);

	if (!display.solveSudoku(solving)) {
		cout << "ERROR Unable to Solve Puzzle! " << endl;
		exit(0);
	}
	cout << endl;
	cout << " SOLVED! " << endl;
	cout << endl;
	display.print();
	cout << endl;
	display.printAnswerToFile(answer);

}


//Description: Main function for program
//Pre-condition: None
//Post-condition: Puzzle will be solved or not, information
//written to necessary output files, statistics determined
int main() {
	//Variable Declarations
	string hint;
	string answer;
	string solving;
	ifstream hintFile;
	ofstream answerFile;
	ofstream solveFile;
	int counterCalls;
	int totalCalls;


	cout << "What is the Name of the File that Contains the Hints: " << endl;
	cin >> hint;
	cout << endl;

	cout << "Where Would You Like to Save the Answer File to: " << endl;
	cin >> answer;
	cout << endl;

	cout << "Where Would You Like to Save the Recursion Solving File to: " << endl;
	cin >> solving;
	cout << endl;

	hintFile.open(hint);
	if (!hintFile.is_open()) {
		ofstream errorWrite;
		errorWrite.open("error.txt");
		cout << "ERROR NO SUCH FILE";
		errorWrite << "ERROR NO SUCH FILE";
		return 0;
	}

	answerFile.open(answer);
	SudokuSolver solve(hint);

	cout << endl;
	cout << "Sudoku Hint File " << endl;
	cout << endl;

	hintFileToScreen(solve);
	
	solvedPuzzle(solve,answer,solving);

	
	cout << endl;


	hintFile.close();
	answerFile.close();
	counterCalls = subGridCounter + rowCounter + columnCounter;


	cout << "The Sudoku Solver Made " << subGridCounter << " Calls to subGridCheck " << endl;
	cout << "The Sudoku Solver Made " << rowCounter << " Calls to rowCheck " << endl;
	cout << "The Sudoku Solver Made " << columnCounter << " Calls to colCheck" << endl;
	cout << "The Sudoku Solver Made " << counterCalls << " Total Calls to Methods For rowCheck, colCheck, and subGridCheck " << endl;
	cout << endl;
	cout << "The Sudoku Solver Made " << recursiveCounter << " Recursive Calls, Resulting in ";
	totalCalls = counterCalls * recursiveCounter;
	cout << totalCalls << " Total Calls to Solve this Puzzle " << endl;

	return 0;

}