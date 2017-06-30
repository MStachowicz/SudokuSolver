#pragma once

#include <iostream>
#include <fstream>
#include "CellGroup.h"
#include "Cell.h"

using namespace std;

class SudokuPuzzle
{
public:
	SudokuPuzzle(void);
	~SudokuPuzzle(void);
	
	// Solves the puzzle in the file path specified and outputs it to the same directory. 
	void Solve(char filenameIn[]);

private:
	// Outputs the values of the cells after being solved to file.
	void Output() const;
	// Reads the sudoku puzzle from the file path supplied into the 3 cell group objects.
	// Makes an initial pass over the grid to eliminate cell candidate values.
	void InitialiseGrid(const string& FilePath);
	// Passes over the whole sudoku puzzle cell by cell testing if the cell is a naked single or hidden single.
	void makePuzzlePass();
	// Searches the triple context of the cell for cells with same value as candidates of cell.
	void UpdateCellCandidateList(Cell& pCell, const int &row, const int &column);
	// Remove the parameter value from the candidate lists of all the cells
	void UpdateCellGroupCandidateLists(const int& row, const int& column);
	// Searches if the cell candidate value is not present in the candidate list of other unknown 
	// cells in the triple context.
	bool IdentifyCellAsHiddenSingle(Cell& pCell, const int& row, const int& column);
	// Checks if the cell has one remaining candidate value and sets it as the cell value.
	bool IdentifyCellAsNakedSingle(Cell& pCell);



	// Array of nine row cellgroups.
	CellGroup m_gridRows[9];
	// Array of nine column cellgroups.
	CellGroup m_gridColumns[9];
	// Array of nine block cellgroups.
	CellGroup m_gridBlocks[9];

	// Number of cells solved in the sudoku puzzle.
	int cellsSolved;
	// Number of naked singles found.
	int nakedSingles;
	// Number of hidden singles found.
	int hiddenSingles;
	// Number of times passed through whole puzzle grid.
	int gridPasses;
	// Number of candidate values considered.
	int candidateValuesConsidered;
	// Number of cells pre-defined by puzzle file.
	int preDefinedCells;
};