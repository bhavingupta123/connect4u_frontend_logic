#pragma once

#include "Model/BoardModel.h"
#include "View/BoardView.h"
#include "Controller/NetworkController.h"
#include <string>
#include <functional>

class OnlineGameController {
public:
    OnlineGameController(BoardModel* model, BoardView* view, NetworkController* network);
    void startGame();
    void handleServerMessage(const std::string& json);
    void setOnGameOverCallback(std::function<void(int)> callback);

private:
    void handleMoveFromServer(int col, int player);
    void sendMoveToServer(int col);
    void switchTurn();
    void enableInput(bool enable);

    BoardModel* _model;
    BoardView* _view;
    NetworkController* _network;

    int _playerId = -1;
    bool _isMyTurn = false;

    std::function<void(int)> _onGameOver;
};
