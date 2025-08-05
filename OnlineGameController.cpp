#include "OnlineGameController.h"
#include "rapidjson/document.h"

using namespace rapidjson;

OnlineGameController::OnlineGameController(BoardModel* model, BoardView* view, NetworkController* network)
    : _model(model), _view(view), _network(network) {}

void OnlineGameController::startGame() {
    _model->reset();
    _view->clearDiscs();

    _view->setupTouch([this](int col) {
        if (!_isMyTurn || !_model->isValidMove(col)) return;

        for (int row = 0; row < _model->getRows(); ++row) {
            if (_model->getCell(col, row) == 0) {
                _model->applyMove(col, _playerId + 1);
                _view->drawDisc(col, row, _playerId + 1);
                sendMoveToServer(col);
                _isMyTurn = false;
                break;
            }
        }
    });

    _network->setOnMessageCallback([this](const std::string& msg) {
        handleServerMessage(msg);
    });

    _network->setOnOpenCallback([this]() {
        _network->sendRaw(R"({"mode":"online"})");
    });
}

void OnlineGameController::handleServerMessage(const std::string& json) {
    Document d;
    d.Parse(json.c_str());
    if (!d.IsObject() || !d.HasMember("type")) return;

    std::string type = d["type"].GetString();

    if (type == "match_found") {
        _playerId = d["player"].GetInt();
        _isMyTurn = (_playerId == 0);
        enableInput(_isMyTurn);

    } else if (type == "bot_mode") {
        _playerId = d["player"].GetInt();
        _isMyTurn = true;
        enableInput(true);

    } else if (type == "move") {
        int col = d["column"].GetInt();
        int player = d["player"].GetInt();

        if (player == _playerId) return;

        handleMoveFromServer(col, player);

    } else if (type == "game_over") {
        int winner = d["winner"].GetInt();
        enableInput(false);
        if (_onGameOver) _onGameOver(winner);
    }
}

void OnlineGameController::handleMoveFromServer(int col, int player) {
    for (int row = 0; row < _model->getRows(); ++row) {
        if (_model->getCell(col, row) == 0) {
            _model->applyMove(col, player + 1);
            _view->drawDisc(col, row, player + 1);
            break;
        }
    }

    if (player != _playerId) {
        _isMyTurn = true;
        enableInput(true);
    }
}

void OnlineGameController::sendMoveToServer(int col) {
    _network->sendMove(col);
}

void OnlineGameController::enableInput(bool enable) {
    _view->enableInput(enable);
}

void OnlineGameController::setOnGameOverCallback(std::function<void(int)> callback) {
    _onGameOver = std::move(callback);
}
