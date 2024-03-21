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

	Grid();
	~Grid();

	bool init(const int windowWidth, const int windowHeight);

	void printGrid(Evolve::TextureRenderer& renderer);

	void addSnakeCell(const int row, const int column);
	void addFruitCell(const int row, const int column);

	void clearCell(const int row, const int column);

	bool isEmptyCell(const int row, const int column) const;
	bool isSnakeCell(const int row, const int column) const;
	bool isFruitCell(const int row, const int column) const;

	inline int getNumRows() const { return numRows_; }
	inline int getNumColumns() const { return numRows_; }

private:
	int numRows_ = 0, numColumns_ = 0;
	std::vector<std::string> grid_;

	Evolve::TextureData cellTexture_;
};

