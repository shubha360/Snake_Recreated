#pragma once

#include <vector>
#include <string>

#include <Evolve/TextureRenderer.h>
#include <Evolve/ImageLoader.h>

class Grid {
public:
	static const int CELL_SIZE = 40;

	const char SNAKE_SIGN = 'S';
	const char FRUIT_SIGN = 'F';
	const char JACKPOT_SIGN = 'J';
	const char EMPTY_SIGN = ' ';

	Grid();
	~Grid();

	bool init(const Evolve::Size2D& windowSize);

	//void printGrid(Evolve::TextureRenderer& renderer);

	void addSnakeCell(const int row, const int column);
	void addFruitCell(const int row, const int column);
	void addJackpotCells(const int row, const int column);

	void clearCell(const int row, const int column, bool isJackpot = false);

	bool isEmptyCell(const int row, const int column) const;
	bool isSnakeCell(const int row, const int column) const;
	bool isFruitCell(const int row, const int column) const;
	bool isJackpotCell(const int row, const int column) const;

	inline int getNumRows() const { return numRows_; }
	inline int getNumColumns() const { return numColumns_; }

private:
	int numRows_ = 0, numColumns_ = 0;
	std::vector<std::string> grid_;

	//Evolve::TextureData cellTexture_;
};

