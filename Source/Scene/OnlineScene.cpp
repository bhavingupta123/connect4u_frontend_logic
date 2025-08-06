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

    _statusLabel = Label::createWithSystemFont("Connected", "Arial", 24);
    _statusLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50));
    this->addChild(_statusLabel);

    _model = new BoardModel(7, 6);
    _view = BoardView::create(_model->getCols(), _model->getRows());
    this->addChild(_view);

    network = NetworkController::create("ws://localhost:8080/ws");
    network->retain();

    _controller = OnlineGameController::create(_model, _view, network, _statusLabel, [this]() {
        showGameOverOptions();
    });

    _controller->setOnRematchOfferCallback([this]() {
        showRematchOfferPopup();
    });

    _controller->setOnRematchDeclinedCallback([this]() {
        showRematchDeclinedPopup();
    });

    _controller->setOnRematchAcceptedCallback([this]() {
        _rematchAccepted = true;
        hideGameOverOptions();
        resetBoardForRematch();
    });

    return true;
}

void OnlineScene::showGameOverOptions() {
    auto visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    _rematchButton = MenuItemLabel::create(
        Label::createWithSystemFont("Rematch", "Arial", 24),
        [this](ax::Object*) {
            _rematchButton->setEnabled(false);
            network->sendRematchRequest();
        });

    _homeButton = MenuItemLabel::create(
        Label::createWithSystemFont("Go to Home", "Arial", 24),
        [this](ax::Object*) {
            network->close();
            network->release();
            network = nullptr;
            _director->replaceScene(FirstScene::createScene());
        });

    auto menu = Menu::create(_rematchButton, _homeButton, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));
    this->addChild(menu, 10, "GameOverMenu");
}

void OnlineScene::hideGameOverOptions() {
    this->removeChildByName("GameOverMenu");
}

void OnlineScene::showRematchOfferPopup() {
    auto visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    auto accept = MenuItemLabel::create(
        Label::createWithSystemFont("Accept Rematch", "Arial", 24),
        [this](Object*) {
            network->sendRaw(R"({"type":"rematch_accept"})");
            hideRematchPopup();
        });

    auto decline = MenuItemLabel::create(
        Label::createWithSystemFont("Decline Rematch", "Arial", 24),
        [this](Object*) {
            network->sendRaw(R"({"type":"rematch_decline"})");
            hideRematchPopup();
        });

    _rematchPopup = Menu::create(accept, decline, nullptr);
    _rematchPopup->alignItemsVerticallyWithPadding(20);
    _rematchPopup->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100));
    this->addChild(_rematchPopup, 20, "RematchPopup");
}

void OnlineScene::hideRematchPopup() {
    this->removeChildByName("RematchPopup");
}

void OnlineScene::showRematchDeclinedPopup() {
    if (_rematchAccepted)
        return;
    
    auto label = Label::createWithSystemFont("Rematch was not accepted", "Arial", 24);
    label->setPosition(_director->getVisibleOrigin() + _director->getVisibleSize() / 2);
    this->addChild(label, 10);

    label->runAction(Sequence::create(
        FadeIn::create(0.2f),
        DelayTime::create(2.0f),
        FadeOut::create(0.8f),
        CallFunc::create([this]() {
            network->close();
            network->release();
            network = nullptr;
            _director->replaceScene(FirstScene::createScene());
        }),
        nullptr));
}

void OnlineScene::resetBoardForRematch() {
    _model->reset();
    _view->reset();
    _controller->resetState();
}
