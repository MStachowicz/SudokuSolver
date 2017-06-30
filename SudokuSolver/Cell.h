#pragma once
#include <vector>

class Cell
{
public:

	Cell();
	Cell(int pValue, bool pGiven);
	Cell(int pValue, bool pGiven, std::vector<int>& pCandidateList);
	~Cell();


	// Returns the final value of the cell.
	const int& getValue() const;
	// Returns whether value was given by file.
	const bool& isGivenValue() const;
	// Returns the size of candidate list of the cell.
	const int& getCandidateListSize() const;
	// Return the value in the index of the cell candidate list.
	const int& candidateListvalue(const int& pIndex);


	// Sets the final value of the cell from its candidate list and then destroys its candidate list.
	void setValueFromCandidateList(const int& index);
	// Removes the value given from the candidate list of the cell
	void removeCandidate(int number);


private:
	// The final value of the cell.
	unsigned int m_value;
	// If value was given by file.
	bool m_given;
	// int vector containing the candidate values for cell.
	std::vector<int> CandidateList;
	// int array to test operations on.
	//int array[] = { 0,1,2,3,4,5,6,7,8 };
};