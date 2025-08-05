#include "Scene/OnlineScene.h"
#include "Model/BoardModel.h"
#include "View/BoardView.h"
#include "Controller/NetworkController.h"
#include "Controller/OnlineGameController.h"
#include "Scene/FirstScene.h"

USING_NS_AX;

OnlineScene* OnlineScene::createScene() {
    return OnlineScene::create();
}

bool OnlineScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    // Status label to show game state
    _statusLabel = Label::createWithSystemFont("Connected", "Arial", 24);
    _statusLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                   origin.y + visibleSize.height - 50));
    this->addChild(_statusLabel);

    auto model = new BoardModel(7, 6);
    auto view = BoardView::create(model->getCols(), model->getRows());
    this->addChild(view);

    auto network = NetworkController::create("ws://localhost:8080/ws");
    network->retain();

    auto controller = OnlineGameController::create(model, view, network, _statusLabel, [this]() {
        showGameOverOptions();
    });

    return true;
}

void OnlineScene::showGameOverOptions() {
    auto visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    _rematchButton = MenuItemLabel::create(
        Label::createWithSystemFont("Rematch", "Arial", 24),
        [this](ax::Object*) {
            _director->replaceScene(OnlineScene::createScene());
        });

    _homeButton = MenuItemLabel::create(
        Label::createWithSystemFont("Go to Home", "Arial", 24),
        [this](ax::Object*) {
            _director->replaceScene(FirstScene::createScene());
        });

    auto menu = Menu::create(_rematchButton, _homeButton, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2 - 50));
    this->addChild(menu, 10, "GameOverMenu");
}

void OnlineScene::hideGameOverOptions() {
    this->removeChildByName("GameOverMenu");
}
