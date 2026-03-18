#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "Geode/utils/random.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/BoomScrollLayer.hpp>
#include <Geode/binding/GJChestSprite.hpp>

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
        dy = 375.f + random::generate(-15.f, 15.f);

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

        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

        // edit title
        auto title = (CCSprite*) getChildByIDRecursive("title-label");
        auto titleGlorp = CCSprite::create("glorp.png"_spr);
        title->addChild(titleGlorp);

        titleGlorp->setPosition({138.f, 21.f});
        titleGlorp->setScaleX(6.15);
        titleGlorp->setScaleY(1.225);

        // shops
        auto shopGlorp1 = CCSprite::create("glorp.png"_spr);
        getChildByIDRecursive("scratch-shop")->getChildByID("store-sprite")->addChild(shopGlorp1);
        shopGlorp1->setPosition({24.f, 46.f});
        shopGlorp1->setRotation(-13.f);
        shopGlorp1->setScale(1.4);

        auto shopGlorp2 = CCSprite::create("glorp.png"_spr);
        getChildByIDRecursive("creator-shop")->getChildByID("store-sprite")->addChild(shopGlorp2);
        shopGlorp2->setPosition({22.f, 46.f});
        shopGlorp2->setRotation(4.f);
        shopGlorp2->setScale(1.25);

        auto shopGlorp3 = CCSprite::create("glorp.png"_spr);
        getChildByIDRecursive("mechanic-shop")->getChildByID("store-sprite")->addChild(shopGlorp3);
        shopGlorp3->setPosition({21.f, 46.f});
        shopGlorp3->setScaleX(1.45);
        shopGlorp3->setScaleY(1.25);

        auto shopGlorp4 = CCSprite::create("glorp.png"_spr);
        getChildByIDRecursive("diamond-shop")->getChildByID("store-sprite")->addChild(shopGlorp4);
        shopGlorp4->setPosition({26.f, 45.f});
        shopGlorp4->setRotation(7.f);
        shopGlorp4->setScale(1.275);

        // yellow glorp
        auto yellowGlorp = CCSprite::create("glorp_yellow.png"_spr);
        auto wraith = getChildByIDRecursive("page4");
        wraith->addChild(yellowGlorp);
        yellowGlorp->setPosition({282.5, 180.f});
        yellowGlorp->setScale(3.4);

        wraith->getChildByID("chests-menu")->getChildByID("chest10")->getChildByType<GJChestSprite>(0)->setOpacity(0);

        // main glorp
        auto glorpLayer = CCLayer::create();
        glorpLayer->setID("glorp-page"_spr);

        auto glorplingLayer = CCLayer::create();
        glorplingLayer->setID("glorpling-layer"_spr);
        glorpLayer->addChild(glorplingLayer);

        auto menu = CCMenu::create();
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
