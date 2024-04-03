#include "../include/Grid.h"

Grid::Grid() {}

Grid::~Grid() {
    /*Evolve::ImageLoader::DeleteTexture(cellTexture_);*/
}

bool Grid::init(const int windowWidth, const int windowHeight) {

    numRows_ = windowHeight / CELL_SIZE;
    numColumns_ = windowWidth / CELL_SIZE;

    grid_.resize(numRows_);

    for (auto& str : grid_) {
        str = std::string(numColumns_, EMPTY_SIGN);
    }

    /*Evolve::ImageLoader::LoadTextureFromImage("resources/images/cell.png", cellTexture_, 4);
    Evolve::ImageLoader::BufferTextureData(cellTexture_);
    Evolve::ImageLoader::FreeTexture(cellTexture_);*/

    return true;
}

//void Grid::printGrid(Evolve::TextureRenderer& renderer) {
//    static Evolve::UvDimension uv { 0.0f, 0.0f, 1.0f, 1.0f };
//
//    static Evolve::ColorRgba whiteColor { 255, 255, 255, 255 };
//    static Evolve::ColorRgba blackColor{ 0, 0, 0, 255 };
//
//    for (int row = 0; row < grid_.size(); row++) {
//
//        auto& currentRow = grid_[row];
//
//        for (int column = 0; column < currentRow.size(); column++) {
//
//            Evolve::RectDimension dims(
//                Evolve::Origin::BOTTOM_LEFT,
//                column * CELL_SIZE,
//                row * CELL_SIZE,
//                CELL_SIZE,
//                CELL_SIZE
//            );
//
//            renderer.draw(dims, uv, cellTexture_.id, whiteColor);
//        }
//    }
//}

void Grid::addSnakeCell(const size_t row, const size_t column) {
    grid_[row][column] = SNAKE_SIGN;
}

void Grid::addFruitCell(const size_t row, const size_t column) {
    grid_[row][column] = FRUIT_SIGN;
}

void Grid::addJackpotCells(const size_t row, const size_t column) {
    grid_[row][column] = JACKPOT_SIGN;
    grid_[row][column + 1] = JACKPOT_SIGN;
    grid_[row + 1][column] = JACKPOT_SIGN;
    grid_[row + 1][column + 1] = JACKPOT_SIGN;
}

void Grid::clearCell(const size_t row, const size_t column, bool isJackpot /*= false*/) {
    grid_[row][column] = EMPTY_SIGN;
    
    if (isJackpot) {
        grid_[row][column + 1] = EMPTY_SIGN;
        grid_[row + 1][column] = EMPTY_SIGN;
        grid_[row + 1][column + 1] = EMPTY_SIGN;
    }
}

bool Grid::isEmptyCell(const size_t row, const size_t column) const {
    return grid_[row][column] == EMPTY_SIGN;
}

bool Grid::isSnakeCell(const size_t row, const size_t column) const {
    return grid_[row][column] == SNAKE_SIGN;
}

bool Grid::isFruitCell(const size_t row, const size_t column) const {
    return grid_[row][column] == FRUIT_SIGN;
}

bool Grid::isJackpotCell(const size_t row, const size_t column) const {
    return grid_[row][column] == JACKPOT_SIGN;
}
