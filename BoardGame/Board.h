#pragma once
#include <vector>

struct Position
{
	uint i = -1;
	uint j = -1;
	
	Position() = default;
	Position(uint i, uint j) : i(i), j(j) {}

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
	Board(uint nRows, uint nCols, TComponents defaultValue) : 
		nRows(nRows), 
		nCols(nCols),
		data(nRows * nCols, defaultValue)
	{
	}

	uint Rows() const { return nRows; }
	uint Cols() const { return nCols; }
	TComponents* operator [] (uint rowIndex)
	{
		return &data[0] + rowIndex * nCols;
	}
	const TComponents* operator [] (uint rowIndex) const
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
	const uint nRows;
	const uint nCols;
	std::vector<TComponents> data;
};

