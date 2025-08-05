#ifndef __BOARD_VIEW_H__
#define __BOARD_VIEW_H__

#include "axmol.h"
#include <vector>
#include <functional>

class BoardView : public ax::Node {
public:
    static BoardView* create(int cols, int rows);
    bool init(int cols, int rows);

    void drawDisc(int col, int row, int player);
    void highlightWinCells(const std::vector<ax::Vec2>& positions);
    void clearDiscs();
    void setupTouch(std::function<void(int)> callback);
    ax::Vec2 getCellPosition(int col, int row) const;
    void enableInput(bool enable);

private:
    int _cols, _rows;
    ax::Vec2 _gridPositions[10][10];
    ax::Sprite* _board;
    bool _inputEnabled = true;
    std::function<void(int)> _onColumnTapped;

    int getColumnFromPosition(const ax::Vec2& pos) const;
};

#endif // __BOARD_VIEW_H__
