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

	bool init(const int windowWidth, const int windowHeight);

	//void printGrid(Evolve::TextureRenderer& renderer);

	void addSnakeCell(const size_t row, const size_t column);
	void addFruitCell(const size_t row, const size_t column);
	void addJackpotCells(const size_t row, const size_t column);

	void clearCell(const size_t row, const size_t column, bool isJackpot = false);

	bool isEmptyCell(const size_t row, const size_t column) const;
	bool isSnakeCell(const size_t row, const size_t column) const;
	bool isFruitCell(const size_t row, const size_t column) const;
	bool isJackpotCell(const size_t row, const size_t column) const;

	inline size_t getNumRows() const { return numRows_; }
	inline size_t getNumColumns() const { return numColumns_; }

private:
	size_t numRows_ = 0, numColumns_ = 0;
	std::vector<std::string> grid_;

	//Evolve::TextureData cellTexture_;
};

