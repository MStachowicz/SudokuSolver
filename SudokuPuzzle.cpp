#include "SudokuPuzzle.h"
#include <Windows.h>
#include <fstream>
#include <algorithm>

SudokuPuzzle::SudokuPuzzle(void) :
	cellsSolved(0),
	nakedSingles(0),
	hiddenSingles(0),
	gridPasses(0),
	candidateValuesConsidered(0),
	preDefinedCells(0)
{}
SudokuPuzzle::~SudokuPuzzle(void)
{}

// Solves the puzzle in the file path specified and outputs it to the same directory. 
void SudokuPuzzle::Solve(char filenameIn[])
{
	// Initialising the grid by assigning all the cell groups their cell objects.
	this->InitialiseGrid(filenameIn);

	LARGE_INTEGER start, end, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	// Loop until the puzzle is solved.
	while (cellsSolved + preDefinedCells < 81)
	{
		int CellsSolvedBefore = cellsSolved;

		this->makePuzzlePass();
		gridPasses++;

		if (cellsSolved + preDefinedCells == CellsSolvedBefore)
		{
			cout << "Puzzle not solved" << endl;
			break; // No new cell values found on grid pass 
		}
	}

	QueryPerformanceCounter(&end);
	float time = (end.QuadPart - start.QuadPart) / (static_cast<float> (frequency.QuadPart));


	// Printing the complete grid on screen 
	for (int row = 0; row < 9; row++)
	{
		for (int column = 0; column < 9; column++)
		{
			Cell& pCell = *m_gridRows[row].GetCell(column);
			cout << "[" << pCell.getValue() << "]";
		}
		cout << endl;
	}
	cout << "Pre-defined cells: " << preDefinedCells
		<< "\nCells Solved: " << cellsSolved
		<< "\nTotal: " << preDefinedCells + cellsSolved
		<< "\nNaked singles: " << nakedSingles
		<< "\nHidden singles: " << hiddenSingles
		<< "\nGrid passes: " << gridPasses
		<< "\nCandidate Values Considered: " << candidateValuesConsidered
		<< endl;
	cout << "Time to solve = " << time << "\n\n" << endl;


	// Output the solved puzzle to the file directory.
	Output();
}

// Outputs the values of the cells after being solved to file.
void SudokuPuzzle::Output() const
{
	ofstream fout("sudoku_solution.txt"); // DO NOT CHANGE THE NAME OF THIS FILE
	if (fout.is_open())
	{
		for (int row = 0; row < 9; ++row)
		{
			for (int column = 0; column < 9; ++column)
			{
				Cell& pCell = *m_gridRows[row].GetCell(column);
				fout << pCell.getValue() << " ";
			}
			fout << endl;
		}
		fout.close();
	}
}

// Reads the sudoku puzzle from the file path supplied into the 3 cell group objects.
// Makes an initial pass over the grid to eliminate cell candidate values.
void SudokuPuzzle::InitialiseGrid(const string& FilePath)
{
	ifstream PuzzleStream(FilePath, ios_base::in);

	// The number of cells checked by the initialiseGrid method.
	unsigned int noCellsChecked = 1;
	// The grid block to write the cell to in the gridBlocks array.
	unsigned int block = 0;
	// The index within the block the cell will be contained in.
	unsigned int blockIndex = 0;
	// The offset by which the index will be adjusted.
	unsigned int blockIndexOffset = 0;

	//	bool isGivenValue;
	vector<int> PossibleValues = { 1,2,3,4,5,6,7,8,9 };

	int streamCellValue;

	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column, ++noCellsChecked)
		{
			// Read value from the stream into a variable.
			PuzzleStream >> streamCellValue;
			// Check if the value from the stream is a given value.
			if (streamCellValue != 0)
			{
				//isGivenValue = true;
				m_gridRows[row].SetCell(column, new Cell(streamCellValue, true));

				preDefinedCells += 1;
			}
			else // If the value is not given in the data file, build a candidate list of all the possible values
			{
				//isGivenValue = false;
				m_gridRows[row].SetCell(column, new Cell(streamCellValue, false, vector<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
			}

			m_gridColumns[column].SetCell(row, m_gridRows[row].GetCell(column));

			m_gridBlocks[block].SetCell(blockIndex + blockIndexOffset, m_gridRows[row].GetCell(column));

			// These conditionals change the variables used to setup the gridBlocks arrays.
			++blockIndex; // every cell		
			if (noCellsChecked % 27 == 0) // every 27 cells
			{
				block += 1;
				blockIndex = 0;
				blockIndexOffset = 0;
			}
			else if (noCellsChecked % 9 == 0) // every 9 cells
			{
				block -= 2;
				blockIndex = 0;
				blockIndexOffset += 3;
			}
			else if (noCellsChecked % 3 == 0) // every 3 cells
			{
				block += 1;
				blockIndex = 0;
			}
		}
	}

	// Initial clear up of all the cell's candidate lists
	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column, ++noCellsChecked)
		{
			Cell& pCell = *m_gridRows[row].GetCell(column);
			UpdateCellCandidateList(pCell, row, column);
		}
	}
}

// Passes over the whole sudoku puzzle cell by cell testing if the cell is a naked single or hidden single.
void SudokuPuzzle::makePuzzlePass()
{
	// For every cell in the puzzle
	for (int row = 0; row < 9; ++row)
	{
		for (int column = 0; column < 9; ++column)
		{
			// Reference to the cell being considered to ease readability.
			Cell& pCell = *m_gridRows[row].GetCell(column);

			if (IdentifyCellAsNakedSingle(pCell))
			{
				UpdateCellGroupCandidateLists(row, column);
			}
			else if (IdentifyCellAsHiddenSingle(pCell, row, column))
			{
				UpdateCellGroupCandidateLists(row, column);
			}
		}
	}
}

// Searches the triple context of the cell for cells with same value as candidates of cell.
void SudokuPuzzle::UpdateCellCandidateList(Cell& pCell, const int &row, const int &column)
{
	// If the cell has no set value.
	if (pCell.getValue() == 0)
	{
		for (int i = 0; i < pCell.getCandidateListSize(); i++)
		{
			candidateValuesConsidered++;

			// If the candidate value is already in the cell's row cellgroup.
			if (m_gridRows[row].isInCellGroup(pCell.candidateListvalue(i)))
			{
				// Remove the candidate value from the cell's candidate list.
				pCell.removeCandidate(pCell.candidateListvalue(i));
				// Decrement the loop as cell's further candidate list members 
				// were moved \down an index and the next would be skipped.
				i--;
			}
			// Searches the cell's column.
			else if (m_gridColumns[column].isInCellGroup(pCell.candidateListvalue(i)))
			{
				pCell.removeCandidate(pCell.candidateListvalue(i));
				i--;
			}
			// Searches the cell's block.
			else if (m_gridBlocks[CellGroup::identifyBlock(row, column)].isInCellGroup(pCell.candidateListvalue(i)))
			{
				pCell.removeCandidate(pCell.candidateListvalue(i));
				i--;
			}
		}
	}
}

// Remove the value supplied from the candidate lists of all the cells in the 3 supplied cellGroups.
void SudokuPuzzle::UpdateCellGroupCandidateLists(const int& row, const int& column)
{
	// Reference to the cell being considered to ease readability.
	Cell& pCell = *m_gridRows[row].GetCell(column);

	int temp = pCell.getValue();

	m_gridRows[row].removeCandidateFromCells(temp);
	m_gridColumns[column].removeCandidateFromCells(temp);
	m_gridBlocks[CellGroup::identifyBlock(row, column)].removeCandidateFromCells(temp);
}

// Searches if the cell candidate value is not present in the candidate list of other unknown cells in the triple context.
bool SudokuPuzzle::IdentifyCellAsHiddenSingle(Cell& pCell, const int& row, const int& column)
{
	// If the cell value was not found to be a naked single 
	if (pCell.getValue() == 0)
	{
		// Use the cell's remaining candidate list values to identify it as a hidden single
		for (int i = 0; i < pCell.getCandidateListSize(); i++)
		{
			if (!m_gridRows[row].doesAnyMemberCandidateListContainValue(pCell.candidateListvalue(i), pCell, candidateValuesConsidered))
			{
				pCell.setValueFromCandidateList(i);
				cellsSolved += 1;
				hiddenSingles += 1;
				return true;
			}
			else if (!m_gridColumns[column].doesAnyMemberCandidateListContainValue(pCell.candidateListvalue(i), pCell, candidateValuesConsidered))
			{
				pCell.setValueFromCandidateList(i);
				cellsSolved += 1;
				hiddenSingles += 1;
				return true;
			}
			else if (!m_gridBlocks[CellGroup::identifyBlock(row, column)].
				doesAnyMemberCandidateListContainValue(pCell.candidateListvalue(i), pCell, candidateValuesConsidered)) // have to find the index within the block via column and row
			{
				pCell.setValueFromCandidateList(i);
				cellsSolved += 1;
				hiddenSingles += 1;
				return true;
			}
			candidateValuesConsidered += 1;
		}
	}
	return false;
}

// If the cell has one remaining candidate value sets it as the cell value.
bool SudokuPuzzle::IdentifyCellAsNakedSingle(Cell & pCell)
{
	// check if the cell has only one candidate value remaining and sets it as the final value
	if (pCell.getCandidateListSize() == 1)
	{
		// Set the cell value
		pCell.setValueFromCandidateList(0);

		candidateValuesConsidered++;
		cellsSolved++;
		nakedSingles++;
		return true;
	}
	return false;
}
