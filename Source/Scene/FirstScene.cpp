
#include "Scene/FirstScene.h"
#include "Scene/OfflineScene.h"
#include "Scene/OnlineScene.h"

USING_NS_AX;

FirstScene* FirstScene::createScene() {
    return FirstScene::create();
}

bool FirstScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    auto label = Label::createWithSystemFont("Connect 4", "Arial", 36);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - 100));
    this->addChild(label);

    auto offlineButton = MenuItemLabel::create(Label::createWithSystemFont("Play Offline", "Arial", 30),
        [this](Object*) {
            _director->replaceScene(OfflineScene::createScene());
        });

    auto onlineButton = MenuItemLabel::create(Label::createWithSystemFont("Play Online", "Arial", 30),
        [this](Object*) {
            _director->replaceScene(OnlineScene::createScene());
        });

    auto menu = Menu::create(offlineButton, onlineButton, nullptr);
    menu->alignItemsVerticallyWithPadding(30);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    this->addChild(menu);

    return true;
}

