#include "GymGame.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "MJHelper.h"

#define bg_game "games/gymwiththemonkey/MonkeyRun.png"
#define audio_intro "sounds/gymwiththemonkey/Monkey is getting in shape. Tap on the correct word to help Monkey run..mp3"

USING_NS_CC;

//GymGame * GymGame::createScene()
//{
//	return GymGame::create();
//}

GymGame* GymGame::createGame(std::string json_file, bool isAutoPlay)
{
	auto result = GymGame::create();
	if (json_file.length() > 5)
		result->m_configPath = json_file;
	return result;
}

GymGame::~GymGame()
{

}


bool GymGame::init()
{
	m_gameState = GymNS::GameState::NONE;

	if (!MJMainGBase::init())
		return false;

	if (!initBackground())
		return false;

	if (!initCloud())
		return false;

	if (!initMonkey())
		return false;

	if (!initEndingNode())
		return false;

	return true;
}

void GymGame::startGame()
{
	AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/Monkey Run.mp3");
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bgSprite = Sprite::create("games/gymwiththemonkey/bg.png");
	bgSprite->setName("bg_gymwwith");
	if (bgSprite->getContentSize().width / visibleSize.width < bgSprite->getContentSize().height / visibleSize.height) {
		bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width);
	}
	else {
		bgSprite->setScale(visibleSize.height / bgSprite->getContentSize().height);
	}

	bgSprite->setPosition(visibleSize / 2);
	this->addChild(bgSprite);
	auto backgound_game = Sprite::create(bg_game);
	if (backgound_game) {
		backgound_game->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		backgound_game->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
		backgound_game->setName("gymwithmonkey.png");
		this->addChild(backgound_game, 19000000000);
	}
	this->runAction(Sequence::create(DelayTime::create(2.5f), CallFunc::create([=] {
		AUDIO_ENGINE->playSound(audio_intro);
		this->removeChildByName("gymwithmonkey.png");
		this->removeChildByName("bg_gymwwith");
		if (MJDEFAULT->getBoolForKey(key_background_music)) {
			AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/gym_bgm.mp3");
		}
		m_monkey->reset();
		AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/gym_jog.mp3");
		m_monkeyState = GymNS::MonkeyState::FADE_OUT;
		setMonkeyState(GymNS::MonkeyState::FADE_IN);
		for (int i = 0; i < 3; i++) {
			m_clouds[i]->setLetter("");
		}
		parseGameData();
		if (m_tickGroup) {
			m_tickGroup->removeFromParent();
			m_tickGroup = nullptr;
		}
		initTickGroup();
		m_tickGroup->resetTicks();
		this->runAction(Sequence::create(DelayTime::create(3.5f), CallFunc::create([=] {
			setGameState(GymNS::GameState::INTRO);
			}), NULL));
		}), NULL));
}

void GymGame::onSelectLetter(std::string letter)
{
	AUDIO_ENGINE->stopSound("sounds/gymwiththemonkey/tap on.mp3");
	CCLOG("clicked cloud");
	stopGuideGame();
	if (getGameState() != GymNS::GameState::PLAYING)
	{
		if (getGameState() != GymNS::GameState::GUIDING)
		{
			return;
		}
	}
	// unschedule idle guiding
	InActiveMode(false);
	unschedule("guide_schedule");
	setGameState(GymNS::GameState::WAITING);
	if (letter.compare(m_correctLetter)) {
		//wrong answer
		AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/gymwiththemonkey/gym_wrong%d.mp3", cocos2d::random(1, 2)), false, ([=] {
			}));
		setMonkeyState(GymNS::MonkeyState::FALL);
		for (auto i = 0; i < 3; i++) {
			if (!m_clouds[i]->getLetter().compare(letter)) {
				if (CONFIG_MANAGER->isVietnameseStories())
					AUDIO_ENGINE->playSound(m_clouds[i]->m_audio);
				setGameState(GymNS::GameState::WAITING);
				m_clouds[i]->playAnimation(false, [=]() {
					setGameState(GymNS::PLAYING);
					});
				break;

			}
			};
	}
	else {//correct
		this->runAction(Sequence::create(CallFunc::create([=] {
			AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/gymwiththemonkey/gym_correct.mp3"));
			}), DelayTime::create(0.75f),
				CallFunc::create([=] {
				AUDIO_ENGINE->playSound(cocos2d::StringUtils::format("sounds/gymwiththemonkey/gym_cheer%d.mp3", cocos2d::random(1, 3)));
					}), NULL));
		if (CONFIG_MANAGER->isVietnameseStories())
			AUDIO_ENGINE->playSound(m_current_game.audio);
		for (auto i = 0; i < 3; i++) {
			if (!m_clouds[i]->getLetter().compare(m_correctLetter)) {
				m_clouds[i]->playAnimation(true, [=] {
					if (m_tickGroup->isFull()) {
						this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
							setGameState(GymNS::GameState::ENDING);
							}), NULL));
					}
					else {
						for (int i = 0; i < 3; i++) {
							m_clouds[i]->setClickAbleStatus(false);
						}
						updateRunAnimation();
						this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
							nextQuestion();
							}), NULL));
					}
					});
			}
			else {

				m_clouds[i]->getLabel()->runAction(FadeOut::create(0.15f));

			}
			m_clouds[i]->setClickAbleStatus(false);
		}
		m_tickGroup->pushTick();
	}
}

void GymGame::setGameState(GymNS::GameState state)
{
	if (state == m_gameState)
		return;
	std::vector<std::string> chain;
	switch (state)
	{
	case GymNS::GameState::INTRO: // play intro, change state to guiding after intro is done
		if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
			this->scheduleOnce([=](float dt) {
				nextQuestion();
				}, 1.5f, "delay_intro");
		}
		else {
			this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=] {
				AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/intro.mp3", false, [=]() {
					nextQuestion();
					});
				}), NULL));
		}
		break;
	case GymNS::GameState::GUIDING:
		chain.clear();
		if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
			chain.push_back(hoc_am_hoc_dau == "hoc_am" ? "sounds/gymwiththemonkey/tap on_vi_am.mp3" : "sounds/gymwiththemonkey/tap on_vi_van.mp3");
		}
		else {
			chain.push_back("sounds/gymwiththemonkey/tap on.mp3");
		}
		chain.push_back(m_current_game.audio);
		AUDIO_ENGINE->playChain(chain, [=]() {
			setGameState(GymNS::GameState::PLAYING);
			});
		// 		play the guide audio, repeat each 7s 
		// 				this->schedule([=](float) {
		// 					//AUDIO_ENGINE->playSound("guide.mp3", false);
		// 					setGameState(GymNS::GameState::GUIDING);
		// 				}, 7, "guide_schedule");
		break;
	case GymNS::GameState::PLAYING:
		InActiveMode(true);
		break;
	case GymNS::GameState::ENDING:
		stopMonkeySounds();
		transitionMonkeyState(m_monkeyState, GymNS::FADE_OUT);
		AUDIO_ENGINE->playSound(CONFIG_MANAGER->isGameVMonkey() ? "sounds/gymwiththemonkey/good_job_vi.mp3" : "sounds/gymwiththemonkey/i_feel_so_good.mp3", false, [=]() {
			for (int i = 0; i < 3; i++) {
				m_clouds[i]->setLetter("");
			}
			setGameState(GymNS::GameState::ENDED);
			});
		break;
	case GymNS::GameState::ENDED:
		scheduleOnce([=](float) {
			AUDIO_ENGINE->stopSound("sounds/gymwiththemonkey/gym_bgm.mp3"); // avoid iterator error
			}, 0.1, "stop_sound");
		if (!CONFIG_MANAGER->isGameVMonkey())
			AUDIO_ENGINE->play("sounds/gymwiththemonkey/good_job.mp3", false);
		AUDIO_ENGINE->play("sounds/gymwiththemonkey/ending.mp3", false);
		m_endMonkey->reset();
		if (CONFIG_MANAGER->isGameVMonkey()) {
			scheduleOnce([=](float) {
				MJMainGBase::onCloseGame();
				}, 1.5f, "end");
		}
		else {
			m_endMonkey->play(false, [=](gaf::GAFObject* obj) {
				// return to gamelist
				scheduleOnce([=](float) {
					MJMainGBase::onCloseGame();
					}, 1.5f, "end");

				});
		}
		break;
	default:
		break;
	}
	m_gameState = state;
	m_gameNode->setVisible(state != GymNS::GameState::ENDED);
	m_endingNode->setVisible(!m_gameNode->isVisible());
}


void GymGame::stopMonkeySounds()
{
	AUDIO_ENGINE->stopSound("sounds/gymwiththemonkey/gym_walk.mp3");
	AUDIO_ENGINE->stopSound("sounds/gymwiththemonkey/gym_jog.mp3");
	AUDIO_ENGINE->stopSound("sounds/gymwiththemonkey/gym_run.mp3");
}

void GymGame::setMonkeyState(GymNS::MonkeyState state)
{
	if (state == m_monkeyState)
		return;
	stopMonkeySounds();
	m_monkeyState = state;
	switch (m_monkeyState)
	{
	case GymNS::FADE_IN:
		m_monkey->playSequence("fade_in", false, [=](gaf::GAFObject* obj, const std::string& callback_name) {
			transitionMonkeyState(m_monkeyState, GymNS::WALK);
			});
		break;
	case GymNS::FALL:
		AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/gym_fall.mp3");
		m_monkey->playSequence("fall", false, [=](gaf::GAFObject* obj, const std::string& sqN) {
			updateRunAnimation();
			});
		break;
	case GymNS::WALK:
		m_treadmill->playSequence("walk", true);
		m_monkey->playSequence("walk", true);
		AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/gym_walk.mp3", true);
		setCloudsTexture(0);
		break;
	case GymNS::JOG:
		m_treadmill->playSequence("jog", true);
		m_monkey->playSequence("jog", true);
		AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/gym_jog.mp3", true);
		setCloudsTexture(1);
		break;
	case GymNS::RUN:
		m_treadmill->playSequence("run", true);
		m_monkey->playSequence("run", true);
		AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/gym_run.mp3", true);
		setCloudsTexture(2);
		break;
	case GymNS::FADE_OUT:
		m_monkey->playSequence("fade_out", false);
		break;
	default:
		break;
	}
}

void GymGame::transitionMonkeyState(GymNS::MonkeyState lastState, GymNS::MonkeyState state)
{
	if (lastState == state)
		return;
	std::string transition1 = "";
	std::string transition2 = "";

	switch (lastState)
	{
	case GymNS::FADE_IN:
		transition1 = "turning on";
		break;
	case GymNS::FALL:
		transition1 = "rejoin";
		switch (state)
		{
		case GymNS::WALK:
			transition2 = "towalk";
			break;
		case GymNS::JOG:
			transition2 = "tojog";
			break;
		case GymNS::RUN:
			transition2 = "torun";
			break;
		default:
			break;
		}

		break;

	case GymNS::WALK:
		if (state == GymNS::JOG)
			transition1 = "walkjog";
		break;
	case GymNS::JOG:
		if (state == GymNS::RUN)
			transition1 = "jogrun";
		break;
	case GymNS::RUN:
		break;
	default:
		break;
	}
	if (transition1 != "") {
		m_monkey->playSequence(transition1, false, [=](gaf::GAFObject* obj, const std::string& sqN) {
			if (transition2 != "") {
				m_monkey->playSequence(transition2, false, [=](gaf::GAFObject* obj, const std::string& sqN) {
					setMonkeyState(state);
					});
			}
			else {
				setMonkeyState(state);
			}
			});
	}
	else {
		setMonkeyState(state);
	}
}


void GymGame::updateRunAnimation()
{

	if (m_tickGroup->getTickCount() >= m_tickGroup->getNumberOfTicks() / 3 * 2) {
		transitionMonkeyState(m_monkeyState, GymNS::MonkeyState::RUN);
	}
	else if (m_tickGroup->getTickCount() >= m_tickGroup->getNumberOfTicks() / 3) {
		transitionMonkeyState(m_monkeyState, GymNS::MonkeyState::JOG);
	}
	else {
		transitionMonkeyState(m_monkeyState, GymNS::MonkeyState::WALK);
	}
}

void GymGame::parseGameData()
{

	std::vector<math::resource::Image*> m_images = math::resource::ResHelper::getImages("icons.json");

	auto buffer_string = FileUtils::getInstance()->getStringFromFile(m_configPath);
	rapidjson::Document value_data;
	//bool is_convert_plist_2_json = true;
	//if (is_convert_plist_2_json) {
	//	auto value_map = FileUtils::getInstance()->getValueMapFromFile(m_configPath);
	//	buffer_string = JsonDump::getInstance()->MapToString(value_map);
	//}

	value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
	if (value_data.HasParseError()) {
		CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
		return;
	}
	// log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
	rapidjson::StringBuffer rj_buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
	value_data.Accept(writer);
	CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
	if (value_data.HasMember("option_vm_phonic")) {
		hoc_am_hoc_dau = value_data["option_vm_phonic"].GetString();
	}
	number_tick = value_data["number"].GetInt();
	for (rapidjson::SizeType i = 0; i < value_data["games"].Size(); i++) {
		SingleGame game;
		auto correctId = value_data["games"][i]["correctAnswer"].GetString();
		auto correctImage = math::resource::ResHelper::getImage(m_images, correctId);
		game.correctAnswer = correctImage->getText("name_1");
		game.audio = correctImage->getAudios("name_1").front();
		//game.answers.push_back(game.correctAnswer);
		auto jsonAnswer = value_data["games"][i]["answers"].GetArray();
		for (rapidjson::SizeType j = 0; j < jsonAnswer.Size(); j++) {
			std::string text = jsonAnswer[j].GetString();
			std::string audio = "";
			for (auto img : m_images) {
				if (img->getText("name_1") == text) {
					audio = img->getAudios("name_1").front();
					break;
				}
			}
			game.answers.push_back({ text,audio });

		}
		mj::helper::shuffle(game.answers);
		if (game.answers.size() > 2) {
			game.answers.resize(2);
		}
		game.answers.push_back({ game.correctAnswer,game.audio });

		m_gameData.push_back(game);
	}
}

bool GymGame::nextQuestion()
{
	if (game_index >= m_gameData.size()) {
		game_index = 0;
	}
	m_current_game = m_gameData.at(game_index);
	++game_index;
	mj::helper::shuffle(m_current_game.answers);
	m_correctLetter = m_current_game.correctAnswer;
	for (int i = 0; i < 3; i++) {
		m_clouds[i]->getLabel()->setOpacity(255);
		m_clouds[i]->setLetter(m_current_game.answers[i].first, true);
		m_clouds[i]->m_audio = m_current_game.answers[i].second;
		m_clouds[i]->setVisible(true);
		m_clouds[i]->setClickAbleStatus(false);
	}
	setGameState(GymNS::GameState::GUIDING);
	return true;
}

void GymGame::setLetters(std::string answer[3], std::string correctAnswer)
{
}

void GymGame::setCloudsTexture(int textureColor)
{
	for (auto i = 0; i < 3; i++) {
		m_clouds[i]->setTextureColor(textureColor);
	}
}

void GymGame::InActiveMode(bool active)
{
	if (active) {
		this->scheduleOnce([=](float dt) {
			setGameState(GymNS::GameState::GUIDING);
			}, 7.0f, "in-activemode");
	}
	else {
		this->unschedule("in-activemode");
	}
}

GymNS::GameState GymGame::getGameState()
{
	return m_gameState;
}

void GymGame::onEnter()
{
	MJMainGBase::onEnter();
	startGame();
}

void GymGame::onExit()
{
	MJMainGBase::onExit();
}

GymGame::GymGame() :
	m_endingNode(nullptr),
	m_endMonkey(nullptr),
	m_gameNode(nullptr),
	m_monkey(nullptr),
	m_tickGroup(nullptr),
	m_configPath("json/gymwiththemonkey/config.json"),
	game_index{ 0 }
{
}

void GymGame::onClickedSpeaker(cocos2d::Ref* sender)
{
	if (m_gameState == GymNS::PLAYING) {
		InActiveMode(false);
		std::vector<std::string> chain;
		if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
			chain.push_back(hoc_am_hoc_dau == "hoc_am" ? "sounds/gymwiththemonkey/tap on_vi_am.mp3" : "sounds/gymwiththemonkey/tap on_vi_van.mp3");
		}
		else {
			//chain.push_back("sounds/gymwiththemonkey/tap on.mp3");
		}
		chain.push_back(m_current_game.audio);
		AUDIO_ENGINE->playChain(chain, [=] {
			setGameState(GymNS::GameState::PLAYING);
			});
	}
	guideGame();
}

bool GymGame::initBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bgSprite = Sprite::create("games/gymwiththemonkey/bg.png");
	if (!bgSprite)
		return false;
	if (bgSprite->getContentSize().width / visibleSize.width < bgSprite->getContentSize().height / visibleSize.height) {
		bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width);
	}
	else {
		bgSprite->setScale(visibleSize.height / bgSprite->getContentSize().height);
	}

	bgSprite->setPosition(visibleSize / 2);
	this->addChild(bgSprite);

	m_gameNode = cocos2d::Node::create();
	this->addChild(m_gameNode);

	m_endingNode = cocos2d::Node::create();
	m_endingNode->setVisible(false);
	this->addChild(m_endingNode);

	speaker = cocos2d::ui::Button::create("games/gymwiththemonkey/loa icon.png");
	speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	speaker->setPosition(Vec2(75, visibleSize.height - 50));
	m_gameNode->addChild(speaker, 100);
	speaker->setTouchEnabled(true);
	speaker->addClickEventListener(CC_CALLBACK_1(GymGame::onClickedSpeaker, this));

	return true;
}

bool GymGame::initTickGroup()
{
	m_tickGroup = TickGroup::create();
	m_tickGroup->setNumber(number_tick);
	if (!m_tickGroup)
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	const cocos2d::Size offset(350.0f, -120.0f);
	m_tickGroup->setPosition(visibleSize / 2 + offset);
	m_tickGroup->setScale(0.7f);
	m_gameNode->addChild(m_tickGroup);

	return true;
}

bool GymGame::initCloud()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	cocos2d::Size offset[]{
		cocos2d::Size(-372, 198.0f),
		cocos2d::Size(-72.0f, 238.0f),
		cocos2d::Size(228.0f, 218.0f),
	};

	float angle[] = { -18.0f,0.0f,18.0f };
	for (int i = 0; i < 3; i++) {
		cloud = LetterCloud::create();
		cloud->setPosition(visibleSize / 2 + offset[i]);
		cloud->setListener(this);
		cloud->setName("cloud" + to_string(i));
		cloud->setRotation(angle[i]);
		m_gameNode->addChild(cloud);
		m_clouds[i] = cloud;
	}

	return true;
}

bool GymGame::initMonkey()
{

	auto monkey = GAFWrapper::createGAF("gaf/gymwiththemonkey/gym_monkey/gym_monkey.gaf");
	if (!monkey)
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Size offset(-400, -120);
	monkey->setPosition(visibleSize / 2 + offset);
	monkey->setScale(0.8f);
	m_gameNode->addChild(monkey);
	m_monkey = monkey;

	auto treadmill = GAFWrapper::createGAF("gaf/gymwiththemonkey/gym_treadmill/gym_treadmill.gaf");
	if (!treadmill)
		return false;
	treadmill->setPosition(visibleSize / 2 + offset);
	treadmill->setScale(0.8f);
	m_gameNode->addChild(treadmill);
	m_treadmill = treadmill;


	return true;
}

bool GymGame::initEndingNode()
{
	auto endMonkey = GAFWrapper::createGAF("gaf/gymwiththemonkey/gym_ending/gym_ending.gaf");
	if (!endMonkey)
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Size offset(0, -230.0f);
	endMonkey->setPosition(visibleSize / 2 + offset);
	endMonkey->setScale(0.84f);
	m_endingNode->addChild(endMonkey);
	m_endMonkey = endMonkey;
	return true;
}

void GymGame::guideGame() {
	this->schedule([=](float) {
		soundGuidingId = -1;
		soundQuesId = -1;
		soundGuidingId = AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/tap on.mp3", false, [=] {
			soundQuesId = AUDIO_ENGINE->playSound(m_current_game.audio);
			});
		}, 10, 100, 10, "guidegame");
}

void GymGame::stopGuideGame() {
	this->unschedule("guidegame");
	if (cocos2d::experimental::AudioEngine::getState(soundGuidingId) == cocos2d::experimental::AudioEngine::AudioState::PLAYING) {
		AUDIO_ENGINE->stopSound("sounds/gymwiththemonkey/tap on.mp3");
	}
	if (cocos2d::experimental::AudioEngine::getState(soundQuesId) == cocos2d::experimental::AudioEngine::AudioState::PLAYING) {
		AUDIO_ENGINE->stopSound(m_current_game.audio);
	}
}