#ifndef __ONLINE_GAME_CONTROLLER_H__
#define __ONLINE_GAME_CONTROLLER_H__

#include "Model/BoardModel.h"
#include "View/BoardView.h"
#include "Controller/NetworkController.h"
#include "axmol.h"

class OnlineGameController {
public:
    static OnlineGameController* create(BoardModel* model,
                                        BoardView* view,
                                        NetworkController* network,
                                        ax::Label* statusLabel,
                                        const std::function<void()>& onGameOver);

    bool init(BoardModel* model,
              BoardView* view,
              NetworkController* network,
              ax::Label* statusLabel,
              const std::function<void()>& onGameOver);

private:
    void handleServerMessage(const std::string& msg);
    void sendMove(int col);
    void onTouchColumn(int col);
    void updateStatus(const std::string& text);

    BoardModel* _model;
    BoardView* _view;
    NetworkController* _network;
    ax::Label* _statusLabel;
    std::function<void()> _onGameOverCallback;

    int _playerId = -1;
    bool _isMyTurn = false;
    bool _gameOver = false;
};

#endif // __ONLINE_GAME_CONTROLLER_H__
