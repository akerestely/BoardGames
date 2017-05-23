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
	Board(uint nRows, uint nCols) : nRows(nRows), nCols(nCols)
	{
		data.resize(nRows * nCols, TComponents::None);
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

	//const std::vector<TComponents>& Data() const { return data; }

private:
	const uint nRows;
	const uint nCols;
	std::vector<TComponents> data;
};

