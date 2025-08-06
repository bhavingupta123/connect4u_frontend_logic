#pragma once

#include "axmol.h"
#include <functional>

class BoardModel;
class BoardView;
class NetworkController;
class OnlineGameController;

class OnlineScene : public ax::Scene {
public:
    static OnlineScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(OnlineScene);

private:
    void showGameOverOptions();
    void hideGameOverOptions();
    void showRematchOfferPopup();
    void hideRematchPopup();
    void showRematchDeclinedPopup();
    void resetBoardForRematch();

    BoardModel* _model = nullptr;
    BoardView* _view = nullptr;
    NetworkController* network = nullptr;
    OnlineGameController* _controller = nullptr;

    ax::Label* _statusLabel = nullptr;
    ax::MenuItemLabel* _rematchButton = nullptr;
    ax::MenuItemLabel* _homeButton = nullptr;
    ax::Menu* _rematchPopup = nullptr;
    
    bool _rematchAccepted = false;
};
