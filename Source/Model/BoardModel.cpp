
#include "Model/BoardModel.h"

BoardModel::BoardModel(int cols, int rows) : _cols(cols), _rows(rows) {
    reset();
}

void BoardModel::reset() {
    _grid.assign(_cols, std::vector<int>(_rows, 0));
}

bool BoardModel::applyMove(int col, int player) {
    if (!isValidMove(col)) return false;
    for (int row = 0; row < _rows; ++row) {
        if (_grid[col][row] == 0) {
            _grid[col][row] = player;
            return true;
        }
    }
    return false;
}

bool BoardModel::isValidMove(int col) const {
    return col >= 0 && col < _cols && _grid[col][_rows - 1] == 0;
}

int BoardModel::getCell(int col, int row) const {
    return _grid[col][row];
}

bool BoardModel::checkWin(int lastCol, int lastRow, int player, std::vector<std::pair<int, int>>& winCells) const {
    const std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (auto [dx, dy] : directions) {
        std::vector<std::pair<int, int>> line = {{lastCol, lastRow}};
        for (int i = 1; i < 4; ++i) {
            int x = lastCol + i * dx, y = lastRow + i * dy;
            if (x < 0 || x >= _cols || y < 0 || y >= _rows || _grid[x][y] != player) break;
            line.emplace_back(x, y);
        }
        for (int i = 1; i < 4; ++i) {
            int x = lastCol - i * dx, y = lastRow - i * dy;
            if (x < 0 || x >= _cols || y < 0 || y >= _rows || _grid[x][y] != player) break;
            line.emplace_back(x, y);
        }
        if (line.size() >= 4) {
            winCells = line;
            return true;
        }
    }
    return false;
}

bool BoardModel::isFull() const {
    for (int col = 0; col < _cols; ++col)
        if (_grid[col][_rows - 1] == 0)
            return false;
    return true;
}

int BoardModel::getRows() const { return _rows; }
int BoardModel::getCols() const { return _cols; }
