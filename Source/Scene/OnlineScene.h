#pragma once

#include "axmol.h"
#include "View/BoardView.h"

class OnlineScene : public ax::Scene {
public:
    static OnlineScene* createScene();
    virtual bool init();

    CREATE_FUNC(OnlineScene);

private:
    ax::Label* _statusLabel;
    ax::MenuItemLabel* _rematchButton;
    ax::MenuItemLabel* _homeButton;
    ax::Menu* _gameOverMenu;

    void showGameOverOptions();
    void showRematchOfferPopup();
    void hideGameOverOptions();
    void handleServerMessage(const std::string& msg);
};
