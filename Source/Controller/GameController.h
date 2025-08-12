#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "Model/BoardModel.h"
#include "View/BoardView.h"
#include <functional>

class GameController {
public:
    GameController(BoardModel* model, BoardView* view);
    void startGame();
    void handleMove(int col);
    void setOnGameOverCallback(std::function<void(int)> callback);
    void setOnTurnChangedCallback(std::function<void(int)> callback);

private:
    BoardModel* _model;
    BoardView* _view;
    int _currentPlayer;
    std::function<void(int)> _onGameOver;
    bool _gameOver = false;
    std::function<void(int)> _onTurnChanged;

    void switchPlayer();
};

#endif // __GAME_CONTROLLER_H__
