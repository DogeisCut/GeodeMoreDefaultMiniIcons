#include <Geode/Geode.hpp>

using namespace geode::prelude;

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
        PlayerObject::updatePlayerShipFrame(this->getNewIconId(frame));
    }
    void updatePlayerJetpackFrame(int frame) {
        PlayerObject::updatePlayerJetpackFrame(this->getNewIconId(frame));
    }
    void updatePlayerBirdFrame(int frame) {
        PlayerObject::updatePlayerBirdFrame(this->getNewIconId(frame));
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