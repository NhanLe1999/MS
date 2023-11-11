#include "Goat.h"
#include "HSAudioEngine.h"

Goat * Goat::createGAF(const std::string & gaf_name)
{
	auto wrapper = new Goat();
	if (wrapper && wrapper->init(gaf_name)) {
		wrapper->autorelease();
	}
	else {
		CC_SAFE_DELETE(wrapper);
		wrapper = nullptr;
	}

	return wrapper;
}

void Goat::jump(float height, std::function<void()> landCallback)
{
	if (m_isJumping)
		return;

	m_isJumping = true;
	// calculate v0
	auto v0 = sqrt(std::abs(2 * m_gravity * height));
	auto pos = getPosition();
	m_baseVerticalPos = pos.y;
	m_velocity = v0;
	m_landCallback = landCallback;
}

void Goat::setTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}

void Goat::update(float dt)
{
	GAFWrapper::update(dt);
	dt *= m_timeScale;
	if (m_isJumping) {
		m_velocity += (dt * m_gravity);
		//setSequence(m_velocity > 0 ? "jump" : "fall");
		if (m_velocity > 0)
			setSequence("jump");
		auto pos = getPosition();
		pos.y += m_velocity * dt;
		if (pos.y < m_baseVerticalPos) {
			m_isJumping = false;
			pos.y = m_baseVerticalPos;
			m_velocity = 0;
			setSequence("run");
			auto callback = m_landCallback;
			m_landCallback = nullptr;
			if (callback) {
				callback();
			}
		}
		setPosition(pos);
		if (m_velocity < 0 && pos.y - m_baseVerticalPos < 70)
			setSequence("fall");
	}
}

void Goat::onEnter()
{
	GAFWrapper::onEnter();
	scheduleUpdate();
}

void Goat::onExit()
{
	GAFWrapper::onExit();
	unscheduleUpdate();
}

Goat::Goat()
	:m_velocity(0),
	m_isJumping(false),
	m_gravity(-2000.0f),
	m_sequence("run"),
	m_landCallback(nullptr),
	m_timeScale(1)
{
}

void Goat::setSequence(std::string sequence)
{
	if (sequence == m_sequence)
		return;
	m_sequence = sequence;
	playSequence(sequence, sequence == "run");
	if (sequence == "run") {
		AUDIO_ENGINE->playSound("sounds/dinosaurchasing/goat_run.mp3", true);
	}
	else {
		AUDIO_ENGINE->stopSound("sounds/dinosaurchasing/goat_run.mp3");
	}
}
