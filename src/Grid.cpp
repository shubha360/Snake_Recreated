#include "../include/Grid.h"

Grid::Grid() {}

Grid::~Grid() {
    /*Evolve::ImageLoader::DeleteTexture(cellTexture_);*/
}

bool Grid::init(const Evolve::Size2D& windowSize) {

    numRows_ = windowSize.Height / CELL_SIZE;
    numColumns_ = windowSize.Width / CELL_SIZE;

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

void Grid::addSnakeCell(const int row, const int column) {
    grid_[row][column] = SNAKE_SIGN;
}

void Grid::addFoodCell(const int row, const int column) {
    grid_[row][column] = FRUIT_SIGN;
}

void Grid::addJackpotCells(const int row, const int column) {
    grid_[row][column] = JACKPOT_SIGN;
    grid_[row][column + 1] = JACKPOT_SIGN;
    grid_[row + 1][column] = JACKPOT_SIGN;
    grid_[row + 1][column + 1] = JACKPOT_SIGN;
}

void Grid::clearCell(const int row, const int column, bool isJackpot /*= false*/) {
    grid_[row][column] = EMPTY_SIGN;
    
    if (isJackpot) {
        grid_[row][column + 1] = EMPTY_SIGN;
        grid_[row + 1][column] = EMPTY_SIGN;
        grid_[row + 1][column + 1] = EMPTY_SIGN;
    }
}

bool Grid::isEmptyCell(const int row, const int column) const {
    return grid_[row][column] == EMPTY_SIGN;
}

bool Grid::isSnakeCell(const int row, const int column) const {
    return grid_[row][column] == SNAKE_SIGN;
}

bool Grid::isFoodCell(const int row, const int column) const {
    return grid_[row][column] == FRUIT_SIGN;
}

bool Grid::isJackpotCell(const int row, const int column) const {
    return grid_[row][column] == JACKPOT_SIGN;
}
