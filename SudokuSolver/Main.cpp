#include "SudokuPuzzle.h"

// The starting point for the sudoku solver implementation.
int main(int argc, char **) {
	
	//SudokuPuzzle puzzle;
	//SudokuPuzzle puzzle1;
	//SudokuPuzzle puzzle2;
	SudokuPuzzle puzzle3;

	//puzzle.Solve("sudoku_puzzle.txt");
	//puzzle1.Solve("sudoku_puzzle2.txt");
	//puzzle2.Solve("sudoku_puzzle3.txt");
	puzzle3.Solve("sudoku_puzzle4.txt");

	Cell temp;
	temp.~Cell();

	system("pause");

	return 0;
}
