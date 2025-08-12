#include "Controller/OnlineGameController.h"
#include "rapidjson/document.h"
#include "utils/GameData.h"

using namespace rapidjson;

OnlineGameController* OnlineGameController::create(BoardModel* model, BoardView* view,
                                                   NetworkController* network,
                                                   ax::Label* statusLabel,
                                                   const std::function<void()>& onGameOver) {
    auto* controller = new OnlineGameController();
    if (controller && controller->init(model, view, network, statusLabel, onGameOver)) {
        return controller;
    }
    delete controller;
    return nullptr;
}

bool OnlineGameController::init(BoardModel* model,
                                BoardView* view,
                                NetworkController* network,
                                ax::Label* statusLabel,
                                const std::function<void()>& onGameOver) {
    _model = model;
    _view = view;
    _network = network;
    _statusLabel = statusLabel;
    _onGameOverCallback = onGameOver;

    _view->setupTouch([this](int col) {
        onTouchColumn(col);
    });

    _network->setOnMessageCallback([this](const std::string& msg) {
        handleServerMessage(msg);
    });

    _network->setOnOpenCallback([this]() {
        std::string name = gameData.playerName;
        _network->sendRaw(R"({"mode":"online","name":")" + name + R"("})");
    });

    return true;
}

void OnlineGameController::onTouchColumn(int col) {
    if (!_isMyTurn || _gameOver || !_model->isValidMove(col)) return;
    sendMove(col);
}

void OnlineGameController::sendMove(int col) {
    _network->sendMove(col);
}

void OnlineGameController::handleServerMessage(const std::string& msg) {
    Document d;
    d.Parse(msg.c_str());

    if (!d.HasMember("type") || !d["type"].IsString())
        return;
    std::string type = d["type"].GetString();

    if (type == "match_found" || type == "bot_mode") {
        _playerId = d["player"].GetInt();
        _isMyTurn = (_playerId == 0);
        _view->enableInput(_isMyTurn);
        if (type == "bot_mode") {
            updateStatus("Playing vs Bot. " + std::string(_isMyTurn ? "Your turn!" : "Bot's turn."));
        } else {
            updateStatus("Match found! " + std::string(_isMyTurn ? "Your turn!" : "Opponent's turn."));
        }
    }
    else if (type == "move") {
        int col = d["column"].GetInt();
        int player = d["player"].GetInt();
        for (int row = 0; row < _model->getRows(); ++row) {
            if (_model->getCell(col, row) == 0) {
                _model->applyMove(col, player + 1);
                _view->drawDisc(col, row, player + 1);
                break;
            }
        }
        _isMyTurn = (player != _playerId);
        _view->enableInput(_isMyTurn);
        updateStatus(_gameOver ? "Game Over!" : (_isMyTurn ? "Your turn" : "Opponent's turn"));
    }
    else if (type == "game_over") {
        _gameOver = true;
        int winner = d["winner"].GetInt();
        _view->enableInput(false);
        std::string result;
        if (winner == _playerId)
            result = "You win!";
        else if (winner == 1 - _playerId)
            result = "You lose!";
        else
            result = "Draw!";
        
        updateStatus("Game Over - " + result);
        
        if (d.HasMember("winning_cells") && d["winning_cells"].IsArray()) {
                std::vector<std::pair<int, int>> winningCells;
                for (auto& cell : d["winning_cells"].GetArray()) {
                    if (cell.IsArray() && cell.Size() == 2) {
                        int col = cell[0].GetInt();
                        int row = cell[1].GetInt();
                        winningCells.emplace_back(col, row);
                    }
                }
                _view->highlightWinningCells(winningCells);
            }
        
        if (_onGameOverCallback) {
            _onGameOverCallback();
        }
    }
    else if (type == "rematch_offer") {
        if (_onRematchOffer) {
            _onRematchOffer();
        }
    }
    else if (type == "rematch_declined") {
        if (_onRematchDeclined) {
            _onRematchDeclined();
        }
    }
    else if (type == "rematch_start") {
        if (_onRematchAccepted) {
                _onRematchAccepted();
        }
    }

}

void OnlineGameController::setOnRematchAcceptedCallback(std::function<void()> callback) {
    _onRematchAccepted = std::move(callback);
}

void OnlineGameController::updateStatus(const std::string& text) {
    if (_statusLabel) {
        _statusLabel->setString(text);
    }
}

void OnlineGameController::resetState() {
    _isMyTurn = (_playerId == 0);
    _gameOver = false;
    _view->enableInput(_isMyTurn);
    updateStatus(_isMyTurn ? "Your turn" : "Opponent's turn");
}
