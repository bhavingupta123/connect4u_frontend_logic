#include "Scene/OfflineScene.h"
#include "Model/BoardModel.h"
#include "View/BoardView.h"
#include "Controller/GameController.h"

USING_NS_AX;

OfflineScene* OfflineScene::createScene() {
    return OfflineScene::create();
}

bool OfflineScene::init() {
    if (!Scene::init())
        return false;

    auto model = new BoardModel();
    auto view = BoardView::create(model->getCols(), model->getRows());
    this->addChild(view);

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    
    auto turnLabel = Label::createWithSystemFont("Player 1's Turn", "Arial", 28);
        turnLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 20));
        this->addChild(turnLabel, 99);

    auto gameOverLabel = Label::createWithSystemFont("", "Arial", 32);
    gameOverLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50));
    gameOverLabel->setVisible(false);
    this->addChild(gameOverLabel, 99);

    auto controller = new GameController(model, view);
    controller->startGame();

    controller->setOnGameOverCallback([gameOverLabel, turnLabel](int winner) {
        gameOverLabel->setString("Player " + std::to_string(winner) + " wins!");
        turnLabel->setVisible(false);
        gameOverLabel->setVisible(true);
    });
    
    controller->setOnTurnChangedCallback([turnLabel](int player) {
            turnLabel->setString("Player " + std::to_string(player) + "'s Turn");
        });

    return true;
}
