#include "Cell.h"
#include <algorithm>
#include <iostream>

Cell::Cell() :
	m_value(0),
	m_given(false)
{}                                                                                                                                                                         
Cell::Cell(int pValue, bool pGiven) :
	m_value(pValue),
	m_given(pGiven)
{}
Cell::Cell(int pValue, bool pGiven, std::vector<int>& pCandidateList) : 
	m_value(pValue),
	m_given(pGiven),
	CandidateList(pCandidateList)
{}

Cell::~Cell() {}

// Returns the final value of the cell.
const int& Cell::getValue()	const						   { return m_value; }
// Returns whether value was given by file.
const bool& Cell::isGivenValue() const					   { return m_given; }
// Returns the size of candidate list of the cell.
const int& Cell::getCandidateListSize() const			   { return CandidateList.size(); }
// Return the value in the index of the cell candidate list.
const int& Cell::candidateListvalue(const int& pIndex)	   { return CandidateList[pIndex]; }


// Sets the cell value to a member of its candidate list and destroys its candidate list.
void Cell::setValueFromCandidateList(const int& index)
{
	m_value = CandidateList[index]; // set the final value.

	// using swap with empty vector to dealllocate memory
	// .clear and/or shrink_to_fit do not guarantee deallocation 
	std::vector<int> temp;
	CandidateList.swap(temp);
}
// Removes the value given from the candidate list of the cell.
void Cell::removeCandidate(int number)
{
	CandidateList.erase(std::remove(
		CandidateList.begin(),
		CandidateList.end(),
		number),
		CandidateList.end());
}
