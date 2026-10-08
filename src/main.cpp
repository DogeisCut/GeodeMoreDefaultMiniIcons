#include <Geode/Geode.hpp>

using namespace geode::prelude;

static void aliasMini(std::string const& mode, std::string const& part) {
    auto cache = CCSpriteFrameCache::get();
    auto from = fmt::format("{}/{}_00_{}001.png", Mod::get()->getID(), mode, part);
    auto to = fmt::format("{}_00_{}001.png", mode, part);
    if (auto frame = cache->spriteFrameByName(from.c_str())) {
        cache->addSpriteFrame(frame, to.c_str());
    } else {
        log::warn("missing mini frame: {}", from);
    }
}

static void aliasAllMiniFrames() {
    for (auto mode : {"ship"/*, "dart", "swing", "jetpack"*/}) {
        aliasMini(mode, "");
        aliasMini(mode, "2_");
        aliasMini(mode, "glow_");
    }
    aliasMini("bird", "");
    aliasMini("bird", "2_");
    aliasMini("bird", "3_");
    aliasMini("bird", "glow_");
}

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        aliasAllMiniFrames();
        return true;
    }
};

static void setFrameIfExists(CCSprite* spr, std::string const& name) {
    if (!spr) return;
    if (auto frame = CCSpriteFrameCache::get()->spriteFrameByName(name.c_str())) {
        spr->setDisplayFrame(frame);
    }
}

#include <Geode/modify/PlayerObject.hpp>
class $modify(MyPlayerObject, PlayerObject) {
    enum class Gamemode {
        Cube,
        Ship,
        Ball,
        Ufo,
        Wave,
        Robot,
        Spider,
        Swing,
        Jetpack
    };

    bool isMini() {
        return m_vehicleSize < 1.f;
    }

    bool useDefaultMiniIcon() {
        return this->isMini() && m_defaultMiniIcon;
    }

    int getNewIconId(int frame) {
        return this->useDefaultMiniIcon() ? 0 : frame;
    }

    Gamemode currentGamemode() {
        if (m_isShip)
            return m_isPlatformer ? Gamemode::Jetpack : Gamemode::Ship;
        if (m_isBall)
            return Gamemode::Ball;
        if (m_isBird)
            return Gamemode::Ufo;
        if (m_isDart)
            return Gamemode::Wave;
        if (m_isRobot)
            return Gamemode::Robot;
        if (m_isSpider)
            return Gamemode::Spider;
        if (m_isSwing)
            return Gamemode::Swing;
        return Gamemode::Cube;
    }

    void updatePlayerShipFrame(int frame) {
        PlayerObject::updatePlayerShipFrame(frame);
		// it's not the prettiest solution but it'll have to do...
		if (frame == 0 || this->useDefaultMiniIcon()) {
			setFrameIfExists(m_vehicleSprite,          "ship_00_001.png");
			setFrameIfExists(m_vehicleSpriteSecondary, "ship_00_2_001.png");
			setFrameIfExists(m_vehicleGlow,            "ship_00_glow_001.png");
		}
    }
    void updatePlayerJetpackFrame(int frame) {
        PlayerObject::updatePlayerJetpackFrame(this->getNewIconId(frame));
    }
    void updatePlayerBirdFrame(int frame) {
        PlayerObject::updatePlayerBirdFrame(frame);
		if (frame == 0 || this->useDefaultMiniIcon()) {
			setFrameIfExists(m_vehicleSprite,          "bird_00_001.png");
			setFrameIfExists(m_vehicleSpriteSecondary, "bird_00_2_001.png");
			setFrameIfExists(m_birdVehicle, "bird_00_3_001.png");
			setFrameIfExists(m_vehicleGlow,            "bird_00_glow_001.png");
		}
    }
    void updatePlayerDartFrame(int frame) {
        PlayerObject::updatePlayerDartFrame(this->getNewIconId(frame));
    }
	// cant hook these
    // void updatePlayerRobotFrame(int frame) {
    //     PlayerObject::updatePlayerRobotFrame(this->getNewIconId(frame));
    // }
    // void updatePlayerSpiderFrame(int frame) {
    //     PlayerObject::updatePlayerSpiderFrame(this->getNewIconId(frame));
    // }
    void updatePlayerSwingFrame(int frame) {
        PlayerObject::updatePlayerSwingFrame(this->getNewIconId(frame));
    }

    void togglePlayerScale(bool enable, bool noEffects) {
        PlayerObject::togglePlayerScale(enable, noEffects);
        switch (currentGamemode()) {
        case Gamemode::Cube:
            // already handled by the game
            break;
        case Gamemode::Ship:
            this -> updatePlayerShipFrame(
                this->getNewIconId(GameManager::get() -> getPlayerShip()));
            break;
        case Gamemode::Ball:
            // already handled by the game
            break;
        case Gamemode::Ufo:
            this -> updatePlayerBirdFrame(
                this->getNewIconId(GameManager::get() -> getPlayerBird()));
            break;
        case Gamemode::Wave:
            this -> updatePlayerDartFrame(
                this->getNewIconId(GameManager::get() -> getPlayerDart()));
            break;
        case Gamemode::Robot:
			// working on it...
            // this -> updatePlayerRobotFrame(
            //     this->getNewIconId(GameManager::get() -> getPlayerRobot()));
            break;
        case Gamemode::Spider:
			// working on it...
            // this -> updatePlayerSpiderFrame(
            //     this->getNewIconId(GameManager::get() -> getPlayerSpider()));
            break;
        case Gamemode::Swing:
            this -> updatePlayerSwingFrame(
                this->getNewIconId(GameManager::get() -> getPlayerSwing()));
            break;
        case Gamemode::Jetpack:
            this -> updatePlayerJetpackFrame(
                this->getNewIconId(GameManager::get() -> getPlayerJetpack()));
            break;
        }
    }
};