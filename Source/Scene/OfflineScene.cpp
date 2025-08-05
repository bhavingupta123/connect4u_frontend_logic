
#include "Scene/OfflineScene.h"
#include "Model/BoardModel.h"
#include "View/BoardView.h"
#include "Controller/GameController.h"

USING_NS_AX;

OfflineScene* OfflineScene::createScene() {
    return OfflineScene::create();
}

bool OfflineScene::init() {
    if (!Scene::init()) return false;

    auto model = new BoardModel();
    auto view = BoardView::create(model->getCols(), model->getRows());
    this->addChild(view);

    auto controller = new GameController(model, view);
    controller->startGame();
    controller->setOnGameOverCallback([](int winner) {
        messageBox(("Player " + std::to_string(winner) + " wins!").c_str(), "Game Over");
    });

    return true;
}
