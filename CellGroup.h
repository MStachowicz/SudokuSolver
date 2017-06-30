#pragma once

#include "CellGroup.h"
#include "Cell.h"

class CellGroup
{
public:
	CellGroup();
	~CellGroup();

	// Adds the address of a cell to the member array of cells in the CellGroup object at the specified index.
	void SetCell(int index, Cell *cell);
	// Returns the address of the cell in the CellGroup member array of cells at the specified index.
	Cell* GetCell(int index) const;
	// Identifies which block of the puzzle grid a cell belongs to based on the row and column.
	static int identifyBlock(const int& row, const int& column);
	// Checks if the adress of the parameter cells matches.
	bool IsSameCell(const Cell& a, const Cell& b) const;
	// Searches the cellgroup for a cell value same as parameter.
	bool isInCellGroup(const int& Number) const;
	// Searches the candidate lists of the cells in the cellgroup for a given value (hidden singles)
	bool doesAnyMemberCandidateListContainValue(const int& Number, const Cell & pCell, int& candidatesConsidered);
	// Removes the given value from all the cells in the cellgroup
	void removeCandidateFromCells(const int& pValue);

private:
	// All the cells in the cellgroup.
	Cell* m_cells[9];
};