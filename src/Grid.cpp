#include "../include/Grid.h"

Grid::Grid() {}

Grid::~Grid() {
    Evolve::ImageLoader::DeleteTexture(cellTexture_);
}

bool Grid::init(const int windowWidth, const int windowHeight) {

    numRows_ = windowHeight / CELL_SIZE;
    numColumns_ = windowWidth / CELL_SIZE;

    grid_.resize(numRows_);

    for (auto& str : grid_) {
        str = std::string(numColumns_, ' ');
    }

    Evolve::ImageLoader::LoadTextureFromImage("resources/images/cell.png", cellTexture_, 4);
    Evolve::ImageLoader::BufferTextureData(cellTexture_);
    Evolve::ImageLoader::FreeTexture(cellTexture_);

    return true;
}

void Grid::printGrid(Evolve::TextureRenderer& renderer) {
    static Evolve::UvDimension uv { 0.0f, 0.0f, 1.0f, 1.0f };

    static Evolve::ColorRgba whiteColor { 255, 255, 255, 255 };
    static Evolve::ColorRgba blackColor{ 0, 0, 0, 255 };

    for (int row = 0; row < grid_.size(); row++) {

        auto& currentRow = grid_[row];

        for (int column = 0; column < currentRow.size(); column++) {

            Evolve::RectDimension dims(
                Evolve::Origin::BOTTOM_LEFT,
                column * CELL_SIZE,
                row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE
            );

            renderer.draw(dims, uv, cellTexture_.id, whiteColor);
        }
    }
}

void Grid::addSnakeCell(const int row, const int column) {
    grid_[row][column] = 'S';
}

void Grid::addFruitCell(const int row, const int column) {
    grid_[row][column] = 'F';
}

void Grid::clearCell(const int row, const int column) {
    grid_[row][column] = ' ';
}

bool Grid::isEmptyCell(const int row, const int column) const {
    return grid_[row][column] == ' ';
}

bool Grid::isSnakeCell(const int row, const int column) const {
    return grid_[row][column] == 'S';
}

bool Grid::isFruitCell(const int row, const int column) const {
    return grid_[row][column] == 'F';
}
