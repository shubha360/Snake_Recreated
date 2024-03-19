#include "../include/Grid.h"

Grid::Grid() {}

Grid::~Grid() {}

bool Grid::init(const int windowWidth, const int windowHeight) {

    grid_.resize(windowHeight / CELL_SIZE);

    for (auto& str : grid_) {
        str = std::string(windowWidth / CELL_SIZE, ' ');
    }

    return true;
}

//void Grid::printGrid(Evolve::ShapeRenderer& shapeRenderer) {
//    static Evolve::ColorRgba SNAKE_COLOR      {   0, 200, 0, 255 };
//    static Evolve::ColorRgba FRUIT_COLOR      { 255, 255,   0, 255 };
//    static Evolve::ColorRgba JACKPOT_COLOR    { 255,   0, 255, 255 };
//
//    for (int row = 0; row < grid_.size(); row++) {
//
//        auto& currentRow = grid_[row];
//
//        for (int column = 0; column < currentRow.size(); column++) {
//            if (currentRow[column] == 'S') {
//
//                Evolve::RectDimension dims(
//                    Evolve::Origin::TOP_LEFT,
//                    column * CELL_SIZE,
//                    row * CELL_SIZE,
//                    CELL_SIZE, 
//                    CELL_SIZE
//                );
//
//                shapeRenderer.drawRectangle(dims, SNAKE_COLOR);
//            }
//        }
//    }
//}
