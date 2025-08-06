
#ifndef __BOARD_MODEL_H__
#define __BOARD_MODEL_H__

#include <vector>

class BoardModel {
public:
    BoardModel(int cols = 7, int rows = 6);
    void reset();
    bool applyMove(int col, int player);
    bool isValidMove(int col) const;
    int getCell(int col, int row) const;
    bool checkWin(int lastCol, int lastRow, int player, std::vector<std::pair<int, int>>& winCells) const;
    bool isFull() const;
    int getRows() const;
    int getCols() const;
private:
    int _cols, _rows;
    std::vector<std::vector<int>> _grid;
};

#endif // __BOARD_MODEL_H__
