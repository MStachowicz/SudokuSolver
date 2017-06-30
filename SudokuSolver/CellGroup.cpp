#include "CellGroup.h"
#include <iostream>

CellGroup::CellGroup() : m_cells()
{}
CellGroup::~CellGroup()
{}

// Adds the address of a cell to the member array of cells in the CellGroup object at the specified index.
void CellGroup::SetCell(int index, Cell *cell)
{
	m_cells[index] = cell;
}
// Returns the address of the cell in the CellGroup member array of cells at the specified index.
Cell* CellGroup::GetCell(int index) const
{
	return m_cells[index];
}
// Identifies which block of the puzzle grid a cell belongs to based on the row and column.
int CellGroup::identifyBlock(const int & row, const int & column)
{
	if (row < 3)
	{
		if (column < 3)
		{
			return 0;
		}
		else if (column < 6)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if (row < 6)
	{
		if (column < 3)
		{
			return 3;
		}
		else if (column < 6)
		{
			return 4;
		}
		else
		{
			return 5;
		}
	}
	else
	{
		if (column < 3)
		{
			return 6;
		}
		else if (column < 6)
		{
			return 7;
		}
		else
		{
			return 8;
		}
	}
}
// Checks if the adress of the parameter cells matches.
bool CellGroup::IsSameCell(const Cell & a, const Cell & b) const
{
	if (&a == &b)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// Searches the cellgroup for a cell value same as parameter.
bool CellGroup::isInCellGroup(const int& Number) const
{
	// Iterates over all the cells in the cellgroup searching for a matching cell value
	for (int i = 0; i < 9; i++)
	{
		if (m_cells[i]->getValue() == Number)
		{
			return true;
		}
	}
	return false;
}
// Searches the candidate lists of the cells in the cellgroup for a given value (hidden singles)
bool CellGroup::doesAnyMemberCandidateListContainValue(const int& candidateToSearchFor, const Cell& pCell, int& candidatesConsidered)
{
	// Iterates over all the cells in the cellgroup.
	for (int CellGroupIndex = 0; CellGroupIndex < 9; CellGroupIndex++)
	{
		// Skips over the cell being considered as a hidden single.
		if (IsSameCell(*m_cells[CellGroupIndex], pCell))
			continue;

		//If the cell value is unknown
		if (m_cells[CellGroupIndex]->getValue() == 0)
		{
			// For all the values in its candidate list
			for (int CandidateIndex = 0; CandidateIndex < m_cells[CellGroupIndex]->getCandidateListSize(); CandidateIndex++)
			{
				candidatesConsidered++;
				if (m_cells[CellGroupIndex]->candidateListvalue(CandidateIndex) == candidateToSearchFor)
				{
					return true;
				}
			}
		}
	}
	return false;
}
// Removes the given value from all the cells in the cellgroup
void CellGroup::removeCandidateFromCells(const int & pValue)
{
	// Iterates over all the cells in the cellgroup.
	for (int i = 0; i < 9; i++)
	{
		if (m_cells[i]->getValue() == 0) // If cell has no set value
		{
			m_cells[i]->removeCandidate(pValue);
		}
	}
}
