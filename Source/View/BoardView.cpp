#include "View/BoardView.h"

using namespace ax;

BoardView* BoardView::create(int cols, int rows) {
    BoardView* view = new BoardView();
    if (view && view->init(cols, rows)) {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool BoardView::init(int cols, int rows) {
    if (!Node::init()) return false;
    _cols = cols;
    _rows = rows;

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();

    _board = Sprite::create("board_front.png");
    _board->setScale(0.7f);
    _board->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    float scaledW = _board->getContentSize().width * _board->getScaleX();
    float scaledH = _board->getContentSize().height * _board->getScaleY();

    Vec2 boardBottomLeft = Vec2(
        origin.x + (visibleSize.width - scaledW) / 2,
        origin.y + (visibleSize.height - scaledH) / 2
    );

    _board->setPosition(boardBottomLeft);
    this->addChild(_board, 2);

    float leftMarginRatio = 0.07f;
    float rightMarginRatio = 0.07f;
    float bottomMarginRatio = 0.11f;
    float topMarginRatio = 0.03f;

    float cellRegionW = scaledW * (1 - leftMarginRatio - rightMarginRatio);
    float cellRegionH = scaledH * (1 - bottomMarginRatio - topMarginRatio);

    float cellW = cellRegionW / _cols;
    float cellH = cellRegionH / _rows;

    float startX = boardBottomLeft.x + scaledW * leftMarginRatio;
    float startY = boardBottomLeft.y + scaledH * bottomMarginRatio;

    for (int col = 0; col < _cols; ++col) {
        for (int row = 0; row < _rows; ++row) {
            _gridPositions[col][row] = Vec2(
                startX + cellW * (col + 0.5f),
                startY + cellH * (row + 0.5f)
            );
        }
    }


    return true;
}

void BoardView::drawDisc(int col, int row, int player) {
    auto disc = DrawNode::create();
    disc->drawSolidCircle(Vec2::ZERO, 25, 0, 30, (player == 1) ? Color4F::RED : Color4F::BLUE);
    disc->setPosition(_gridPositions[col][row] + Vec2(0, 300));
    this->addChild(disc, 3);

    auto drop = MoveTo::create(0.3f, _gridPositions[col][row]);
    disc->runAction(drop);
}

void BoardView::highlightWinCells(const std::vector<Vec2>& positions) {
    for (auto pos : positions) {
        for (auto child : this->getChildren()) {
            if (child->getPosition().fuzzyEquals(pos, 1.0f)) {
                auto fade = Sequence::create(
                    FadeTo::create(0.3f, 100),
                    FadeTo::create(0.3f, 255),
                    nullptr
                );
                child->runAction(Repeat::create(fade, 2));
                break;
            }
        }
    }
}

void BoardView::clearDiscs() {
    this->removeAllChildrenWithCleanup(true);
    this->addChild(_board, 1);
}

void BoardView::setupTouch(std::function<void(int)> callback) {
    _onColumnTapped = std::move(callback);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (!_inputEnabled) return false;

        Vec2 location = touch->getLocation();
        int col = this->getColumnFromPosition(location);
        if (col >= 0 && col < _cols && _onColumnTapped) {
            _onColumnTapped(col);
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

int BoardView::getColumnFromPosition(const Vec2& pos) const {
    float cellWidth = (_board->getContentSize().width * _board->getScaleX()) / _cols;
    for (int col = 0; col < _cols; ++col) {
        float centerX = _gridPositions[col][_rows - 1].x;
        if (pos.x >= centerX - cellWidth / 2 && pos.x <= centerX + cellWidth / 2) {
            return col;
        }
    }
    return -1;
}

void BoardView::enableInput(bool enable) {
    _inputEnabled = enable;
}

ax::Vec2 BoardView::getCellPosition(int col, int row) const {
    if (col >= 0 && col < _cols && row >= 0 && row < _rows) {
        return _gridPositions[col][row];
    }
    return Vec2::ZERO;
}
