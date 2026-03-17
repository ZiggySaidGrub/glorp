#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "Geode/utils/random.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/BoomScrollLayer.hpp>

using namespace geode::prelude;

#define GRAVITY 500

class Glorpling : public CCSprite {
protected:
    bool init() {
        
        if (!CCSprite::initWithFile(random::generate(0.f, 1.f) < 1.f/1000.f ? "33.png"_spr : "glorp.png"_spr)) {
            return false;
        }

        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        setPosition(screenSize / 2);

        dx = random::generate(-100.f, 100.f);
        dy = 375.f;

        scheduleUpdate();

        return true;
    }

public:
    float dx = 0.f;
    float dy = 0.f;

    static Glorpling* create() {
        auto ret = new Glorpling();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    void update(float delta) {
        dy -= GRAVITY * delta;

        setPositionX(getPositionX() + (dx * delta));
        setPositionY(getPositionY() + (dy * delta));
        
        if (getPositionY() < 0 - getScaledContentSize().height) {
            removeFromParent();
        }
    }
};

#include <Geode/modify/SecretRewardsLayer.hpp>
class $modify(GlorpSecretRewardsLayer, SecretRewardsLayer) {
    bool init(bool fromShop) {
        if (!SecretRewardsLayer::init(fromShop)) return false;
        
        auto glorpLayer = CCLayer::create();
        glorpLayer->setID("glorp-page"_spr);

        auto glorplingLayer = CCLayer::create();
        glorplingLayer->setID("glorpling-layer"_spr);
        glorpLayer->addChild(glorplingLayer);

        auto menu = CCMenu::create();
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        menu->setAnchorPoint({ 0.f, 0.f });
        menu->setScale(5.5);
        menu->setPosition(screenSize / 2);

        auto glorpSpr = CCSprite::create("glorp.png"_spr);
        auto glorpBtn = CCMenuItemSpriteExtra::create(glorpSpr, this, menu_selector(GlorpSecretRewardsLayer::glorpPressed));
        glorpBtn->setAnchorPoint({0.5, 0.5});
        glorpBtn->setPosition({ 0.f, 0.f });

        menu->addChild(glorpBtn);

        glorpLayer->addChild(menu);

        auto scrollLayer = (BoomScrollLayer*) getChildByIDRecursive("scroll-layer");
        scrollLayer->addPage(glorpLayer);

        return true;
    }

    void glorpPressed(CCObject* sender) {
        auto glorplingLayer = getChildByIDRecursive("glorpling-layer"_spr);
        int spawnCount = Mod::get()->getSettingValue<int>("glorpling-spawn-count");
        for (int i = 0; i < spawnCount; i++) {
            auto glorpling = Glorpling::create();
            glorplingLayer->addChild(glorpling);
        }
    }
};
