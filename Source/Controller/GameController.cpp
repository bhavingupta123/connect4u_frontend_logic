
#include "Controller/GameController.h"

GameController::GameController(BoardModel* model, BoardView* view): _model(model), _view(view), _currentPlayer(1) {}

void GameController::startGame() {
    _model->reset();
    _view->clearDiscs();
    _currentPlayer = 1;
    _gameOver = false;
    
    if (_onTurnChanged)
            _onTurnChanged(_currentPlayer);
    
    _view->setupTouch([this](int col) {
        handleMove(col);
    });
}

void GameController::handleMove(int col) {
    if(_gameOver)
        return;
    if (!_model->isValidMove(col))
        return;

    for (int row = 0; row < _model->getRows(); ++row) {
        if (_model->getCell(col, row) == 0) {
            _model->applyMove(col, _currentPlayer);
            _view->drawDisc(col, row, _currentPlayer);

            std::vector<std::pair<int, int>> winCells;
            if (_model->checkWin(col, row, _currentPlayer, winCells)) {
                std::vector<ax::Vec2> positions;
                for (auto& p : winCells) {
                    positions.push_back(_view->getCellPosition(p.first, p.second));
                }
                _view->highlightWinCells(positions);
                _gameOver = true;
                _view->enableInput(false);
                if (_onGameOver)
                    _onGameOver(_currentPlayer);
            } else {
                switchPlayer();
            }

            break;
        }
    }
}

void GameController::switchPlayer() {
    _currentPlayer = 3 - _currentPlayer;
    
    if (_onTurnChanged)
            _onTurnChanged(_currentPlayer);
}

void GameController::setOnGameOverCallback(std::function<void(int)> callback) {
    _onGameOver = callback;
}

void GameController::setOnTurnChangedCallback(std::function<void(int)> callback) {
    _onTurnChanged = callback;
}
