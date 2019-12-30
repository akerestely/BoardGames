#pragma once
#include <vector>

struct Position
{
	uint32_t i = -1;
	uint32_t j = -1;
	
	Position() = default;
	Position(uint32_t i, uint32_t j) : i(i), j(j) {}

	bool operator == (Position &other) const
	{
		return i == other.i && j == other.j;
	}
	bool operator != (Position &other) const
	{
		return !this->operator==(other);
	}

	bool Invalid() const { return !(~i && ~j); }
};

template<class TComponents>
class Board
{
public:
	Board(uint32_t nRows, uint32_t nCols, TComponents defaultValue) : 
		nRows(nRows), 
		nCols(nCols),
		data(nRows * nCols, defaultValue)
	{
	}

	uint32_t Rows() const { return nRows; }
	uint32_t Cols() const { return nCols; }
	TComponents* operator [] (uint32_t rowIndex)
	{
		return &data[0] + rowIndex * nCols;
	}
	const TComponents* operator [] (uint32_t rowIndex) const
	{
		return &data[0] + rowIndex * nCols;
	}
	TComponents& operator [](const Position &pos)
	{
		return data[pos.i * nCols + pos.j];
	}
	const TComponents& operator [](const Position &pos) const
	{
		return data[pos.i * nCols + pos.j];
	}

	bool operator == (const Board<TComponents> &other) const
	{
		return nRows == other.nRows && nCols == other.nCols && data == other.data;
	}

	//const std::vector<TComponents>& Data() const { return data; }

private:
	const uint32_t nRows;
	const uint32_t nCols;
	std::vector<TComponents> data;
};

