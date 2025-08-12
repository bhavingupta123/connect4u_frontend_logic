#include "Scene/FirstScene.h"
#include "Scene/OfflineScene.h"
#include "Scene/OnlineScene.h"
#include "axmol.h"
#include "utils/GameData.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UIScale9Sprite.h"
#include "Controller/StatsController.h"
#include "utils/GameData.h"


USING_NS_AX;

FirstScene* FirstScene::createScene() {
    return FirstScene::create();
}

void FirstScene::promptForNameThenStartOnline() {
    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();

    if (!gameData.playerName.empty()) {
        _director->replaceScene(OnlineScene::createScene());
        return;
    }

    auto overlay = LayerColor::create(Color4B(0, 0, 0, 180));
    overlay->setContentSize(visibleSize);
    this->addChild(overlay, 100);


    auto titleLabel = Label::createWithSystemFont("Enter Your Name", "Arial", 28);
    titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 90));
    overlay->addChild(titleLabel);


    auto inputBox = ui::EditBox::create(Size(300, 60), ui::Scale9Sprite::create());
    inputBox->setPlaceHolder("Name");
    inputBox->setFontColor(Color3B::WHITE);
    inputBox->setFontSize(24);
    inputBox->setMaxLength(12);
    inputBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    inputBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 30));
    overlay->addChild(inputBox);

    auto startBtn = MenuItemLabel::create(Label::createWithSystemFont("Play Online", "Arial", 26),
        [this, overlay, inputBox](Object*) {
            std::string name = inputBox->getText();
            if (!name.empty()) {
                gameData.playerName = name;
                _director->replaceScene(OnlineScene::createScene());
            }
        });

    auto cancelBtn = MenuItemLabel::create(Label::createWithSystemFont("Cancel", "Arial", 26),
        [overlay](Object*) {
            overlay->removeFromParent();
        });


    auto menu = Menu::create(startBtn, cancelBtn, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 40));
    overlay->addChild(menu);
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
            promptForNameThenStartOnline();
        });
    
    auto statsButton = MenuItemLabel::create(Label::createWithSystemFont("View Stats", "Arial", 30),
        [this](Object*) {
            if (gameData.playerName.empty()) {
                promptForNameThenShowStats();
            } else {
                StatsController::showStats(this, gameData.playerName);
            }
        });

    auto menu = Menu::create(offlineButton, onlineButton, statsButton, nullptr);

    menu->alignItemsVerticallyWithPadding(30);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    this->addChild(menu);

    return true;
}

void FirstScene::promptForNameThenShowStats() {
    auto visibleSize = _director->getVisibleSize();

    auto overlay = LayerColor::create(Color4B(0, 0, 0, 180));
    overlay->setContentSize(visibleSize);
    this->addChild(overlay, 100);

    auto titleLabel = Label::createWithSystemFont("Enter Your Name", "Arial", 28);
    titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 90));
    overlay->addChild(titleLabel);

    auto inputBox = ui::EditBox::create(Size(300, 60), ui::Scale9Sprite::create());
    inputBox->setPlaceHolder("Name");
    inputBox->setFontColor(Color3B::WHITE);
    inputBox->setFontSize(24);
    inputBox->setMaxLength(12);
    inputBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    inputBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 30));
    overlay->addChild(inputBox);

    auto viewBtn = MenuItemLabel::create(Label::createWithSystemFont("View Stats", "Arial", 26),
        [this, overlay, inputBox](Object*) {
            std::string name = inputBox->getText();
            if (!name.empty()) {
                gameData.playerName = name;
                overlay->removeFromParent();
                StatsController::showStats(this, gameData.playerName);
            }
        });

    auto cancelBtn = MenuItemLabel::create(Label::createWithSystemFont("Cancel", "Arial", 26),
        [overlay](Object*) {
            overlay->removeFromParent();
        });

    auto menu = Menu::create(viewBtn, cancelBtn, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 40));
    overlay->addChild(menu);
}

