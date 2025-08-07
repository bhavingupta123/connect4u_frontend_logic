#include "StatPopup.h"
#include "MatchResult.h"
#include "ui/UIScrollView.h"
#include "ui/UIButton.h"

using namespace ax;
using namespace ui;

void StatPopup::createAndShow(Node* parent, const std::vector<MatchResult>& stats) {
    auto popup = LayerColor::create(Color4B(0, 0, 0, 180));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    popup->setContentSize(visibleSize);

    auto bgNode = DrawNode::create();
    Vec2 origin(visibleSize.width / 2 - 360, visibleSize.height / 2 - 180);
    Vec2 dest(visibleSize.width / 2 + 360, visibleSize.height / 2 + 180);
    bgNode->drawSolidRect(origin, dest, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    popup->addChild(bgNode);

    float scrollWidth = 700, scrollHeight = 300;
    auto scrollView = ScrollView::create();
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(Size(scrollWidth, scrollHeight));
    float innerHeight = std::max(60.0f + stats.size() * 50.0f, scrollHeight);
    scrollView->setInnerContainerSize(Size(scrollWidth, innerHeight));
    scrollView->setPosition(Vec2(visibleSize.width / 2 - scrollWidth / 2,
                                 visibleSize.height / 2 - scrollHeight / 2));
    scrollView->setBounceEnabled(true);
    scrollView->setScrollBarEnabled(true);
    popup->addChild(scrollView);

    auto headerOpponent = Label::createWithSystemFont("Opponent", "Arial-BoldMT", 24);
    auto headerResult   = Label::createWithSystemFont("Result", "Arial-BoldMT", 24);
    float topY = innerHeight - 30;

    headerOpponent->setTextColor(Color4B::BLACK);
    headerResult->setTextColor(Color4B::BLACK);
    headerOpponent->setPosition(Vec2(200, topY));
    headerResult->setPosition(Vec2(500, topY));
    scrollView->addChild(headerOpponent);
    scrollView->addChild(headerResult);

    auto line = DrawNode::create();
    line->drawLine(Vec2(50, topY - 15), Vec2(650, topY - 15), Color4F::BLACK);
    scrollView->addChild(line);

    float y = topY - 50;
    for (const auto& s : stats) {
        auto opponentLabel = Label::createWithSystemFont(s.opponent, "Arial", 20);
        auto resultLabel   = Label::createWithSystemFont(s.result, "Arial", 20);
        opponentLabel->setTextColor(Color4B::BLACK);
        resultLabel->setTextColor(Color4B::BLACK);

        opponentLabel->setPosition(Vec2(200, y));
        resultLabel->setPosition(Vec2(500, y));

        scrollView->addChild(opponentLabel);
        scrollView->addChild(resultLabel);

        y -= 50;
    }

    auto closeBtn = MenuItemLabel::create(
        Label::createWithSystemFont("Close", "Arial-BoldMT", 24),
        [popup](Object*) { popup->removeFromParent(); });

    auto menu = Menu::create(closeBtn, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));
    popup->addChild(menu);

    parent->addChild(popup, 99);
}
