#include "BuildWord_GamePlay.h"
//#include "SimpleAudioEngine.h"
#include "AspectR.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "SoundM.h"
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"
#include "Math_Libs.h"
NS_GAF_BEGIN
class GAFObject;
class GAFAsset;
NS_GAF_END

USING_NS_GAF;

USING_NS_CC;

using namespace buildwords;

GamePlay* GamePlay::create(vector<buildword::LevelData> allLevels)
{
	GamePlay* obj = new GamePlay();
	if (obj->init())
	{
		obj->autorelease();
		obj->initOptions(allLevels);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return NULL;
}

GamePlay* GamePlay::createGame(std::vector<math::resource::Image*> images)
{
	GamePlay* obj = new GamePlay();
	if (obj->init())
	{
		obj->autorelease();
		obj->generateData(images);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return NULL;
}

void GamePlay::generateData(std::vector<math::resource::Image*> images) {
	string path = FileUtils::getInstance()->fullPathForFilename("config.json");
	rapidjson::Document doc;
	doc.Parse(FileUtils::getInstance()->getStringFromFile(path).c_str());

	vector<buildword::LevelData> totalLevels;
	//overall array
	auto levelArray = doc["data"].GetArray();
	for (auto i = 0; i < levelArray.Size(); i++) {
		auto level = buildword::LevelData();
		auto img_nature = math::resource::ResHelper::getImage(images, levelArray[i]["text_data"].GetString());

		level.word = img_nature->getText("name_1");
		level.wordAudio = img_nature->getAudios("name_1").front();

		auto char_text = img_nature->getTextPhonic("name_p");
		auto char_audios = img_nature->getAudioPhonic("name_p");
		//        auto char_audios = levelArray[i]["char_audio"].GetArray();
		for (int j = 0; j < char_audios.size(); j++) {
			//            level.charAudio.insert({ img_n->getText("name_1"),img_n->getAudios("name_1").front() });
			level.charAudio.insert({ char_text[j], char_audios[j] });
		}
		level.phonic_list = char_text;
		totalLevels.push_back(level);
	}
	initOptions(totalLevels);
}


void GamePlay::initOptions(vector<buildword::LevelData> allLevels)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->setPosition(Vec2::ZERO);
    this->setContentSize(Size(visibleSize.width,visibleSize.height));

    //assign
    _allLevels = allLevels;

    //background///////////
    auto bgSprite = Sprite::create("games/build_word/background.png");
    bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgSprite->setPosition(AspectR::getMidX(),AspectR::getMidY());
    bgSprite->setScale(((1.0/bgSprite->getBoundingBox().size.width)*this->getContentSize().width),
                       ((1.0/bgSprite->getBoundingBox().size.height)*this->getContentSize().height));
    this->addChild(bgSprite);
    ///////////////////////

    //Tray of biscuits/////
    _tray = Layout::create();
    _tray->setBackGroundImage("games/build_word/tray.png");
    _tray->setBackGroundImageScale9Enabled(true);
    _tray->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _tray->setContentSize(Size(AspectR::getWidth(1024 - 200),AspectR::getHeight(300)));
    _tray->setPosition(Vec2(AspectR::getMidX(), 0));
    this->addChild(_tray);
    ///////////////////////

    //background sound
    if(MJDEFAULT->getBoolForKey(key_background_music)){
          bg_audio_id = SoundM::playBg();
    }

    // speaker

    auto speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
    speaker->setPosition(Vec2(70,Director::getInstance()->getWinSize().height - 60));
    speaker->addClickEventListener([=](cocos2d::Ref* sender){
        if(canTouch)
            SoundM::playInstruction();
    });
    this->addChild(speaker);

    //starting game
    auto imgGameName = ImageView::create("games/build_word/gameName.png");
    this->addChild(imgGameName, 10000);
    imgGameName->setPosition(Director::getInstance()->getVisibleSize() / 2);

    imgGameName->runAction(Sequence::create(DelayTime::create(1.0f), FadeOut::create(0.5f)
                                            , CallFunc::create([=](){
        startGame();
    }), RemoveSelf::create(true), nullptr));
}

void GamePlay::startGame() {

	//determine the height of biscuits or biscuit back


	//auto width = ;
	//auto height = _tray->getContentSize().height - AspectR::getHeight(40);

	//getting current word
	auto currentWord = _allLevels[_currentIndex].phonic_list;

	////selecting types
	//auto selectedTypes = getRandomTypes(_allLevels[_currentIndex].charAudio.size());

	//selecting types
	auto selectedTypes = getRandomTypesByWords(currentWord);

	auto size = Size((_tray->getContentSize().width - AspectR::getWidth(40)) / (int)selectedTypes.size(), _tray->getContentSize().height - AspectR::getHeight(40));

	//create biscuits back & biscuits
	float overallWidth = 0;
	for (int i = 0; i < selectedTypes.size(); i++) {

		//current letter
//        string letter(1, currentWord[i]);
		string letter = currentWord[i];

		//instantiate biscuit back
		auto tileBack = ItemBack::create(letter, selectedTypes[i], size);
		_tilesBack.push_back(tileBack);

		//instantiate biscuit
		auto tileFront = ItemFront::create(letter, selectedTypes[i], size);
		_tiles.push_back(tileFront);

		//label text
		auto layout = Layout::create();
		//layout->setBackGroundColorType(BackGroundColorType::SOLID);
		//layout->setBackGroundColor(Color3B::BLUE);

		auto caption = Label::createWithTTF(letter, "fonts/monkeystories_medium.ttf", 100/*AspectR::getFontSize(100)*/);
		caption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		caption->setTextColor(Color4B::BLACK);
		caption->setOpacity(80);
		caption->setName("caption");
		layout->setContentSize(caption->getContentSize());
		caption->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
		layout->addChild(caption);

		_captionHolder.push_back(layout);

		//width of all biscuits
		overallWidth += tileBack->getContentSize().width;
	}

	//determine actual width to be taken for biscuits
	overallWidth += ((int)selectedTypes.size() + 1) * AspectR::getWidth(5);

	//set tray content size
	_tray->setContentSize(Size(overallWidth, _tray->getContentSize().height));

	//positioning
	float xx = AspectR::getWidth(5);
	for (int i = 0; i < _tilesBack.size(); i++) {

		//set position of back of biscuit
		auto backOfBiscuit = _tilesBack[i];
		backOfBiscuit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		backOfBiscuit->setPosition(Vec2(xx + backOfBiscuit->getContentSize().width / 2, _tray->getContentSize().height / 2));
		_tray->addChild(backOfBiscuit);

		//set position of caption
		auto captionLayout = _captionHolder[i];
		captionLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		captionLayout->setPosition(Vec2(backOfBiscuit->getPositionX(), backOfBiscuit->getPositionY()));
		_tray->addChild(captionLayout);

		//set position of biscuit
		auto biscuit = _tiles[i];
		biscuit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		biscuit->setPosition(backOfBiscuit->getPosition());
		biscuit->holder = backOfBiscuit;
		_tray->addChild(biscuit);



		//next position
		xx += backOfBiscuit->getContentSize().width + AspectR::getWidth(5);
	}

	//reset position
	_tray->setPosition(Vec2(AspectR::getMidX(), 0));

	SoundM::playTrayEnter();

	//sequence tray move to position
	_tray->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(1, Vec2(AspectR::getMidX(), AspectR::getHeight(320)))),
		CallFunc::create([=]() { SoundM::playInstruction(); }),
		DelayTime::create(0.5),
		CallFunc::create([=]() { shuffledAndMovePosition(); }), CallFunc::create([=] {
		//        if (!game_play_enable && _currentIndex >= 1) {
		//            this->pauseGame();
		//        }

	}), nullptr));


}

//void GamePlay::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(cocos2d::Director::getInstance()->getWinSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(layout->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(layout->getContentSize()/2);
//    text->setTextColor(Color4B::BLUE);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

/**
 * Get random type of item
 */
vector<int> GamePlay::getRandomTypes(int count) {

	//list of integer of biscuit types
	vector<int> types = { 2,3,5,6,7,8,9,10,11,12 };

	//shuffle
	static std::random_device mj_rd;
	static std::mt19937 mj_g(mj_rd());
	std::shuffle(types.begin(), types.end(), mj_g);

	vector<int> selected;
	for (int i = 0; i < count; i++) {
		selected.push_back(types[i]);
	}

	return selected;
}

vector<int> buildwords::GamePlay::getRandomTypesByWords(std::vector<std::string> words)
{
	// list of integer of biscuit types
	// word's length < 2
	vector<int> types = { 2,4,5,8,9,10,11 };
	// word's length >= 2
	vector<int> long_types = { 3,6,7,12 };

	mj::helper::shuffle(types);
	mj::helper::shuffle(long_types);

	vector<int> selected;
	int short_count = 0;
	int long_count = types.size();
	int index;

	types.insert(types.end(), long_types.begin(), long_types.end());

	for (auto word : words)
	{
		index = word.size() < 2 ? short_count++ : long_count++;
		index %= types.size();
		selected.push_back(types[index]);
	}
	return selected;
}

void GamePlay::shuffledAndMovePosition() {

	vector<int> arr;
	for (int i = 0; i < _tiles.size(); i++) arr.push_back(i);

	//shuffle
	static std::random_device mj_rd;
	static std::mt19937 mj_g(mj_rd());
	std::shuffle(arr.begin(), arr.end(), mj_g);

	//avilable max width
	float maxWidth = AspectR::getWidth(1024 - 20) / (int)_tiles.size();

	//starting pos
	float xx = -(AspectR::getWidth(1024 - 10) - _tray->getContentSize().width) / 2;
	float yy = AspectR::getHeight(768 - 320) / 2;

	//positioning shuffled biscuits
	for (int i = 0; i < _tiles.size(); i++) {
		auto selTile = _tiles[arr[i]];
		selTile->originalPos = Vec2(xx + maxWidth / 2, AspectR::getHeight(320) + RandomHelper::random_real(yy - selTile->getContentSize().height / 3.5, yy + selTile->getContentSize().height / 3.5));
		selTile->runAction(EaseBackIn::create(MoveTo::create(0.3, selTile->originalPos)));
		xx += maxWidth;
	}

	SoundM::playShuffleMove();

	canTouch = true;
}

void GamePlay::checkIfSolved() {

	//check if all biscuits are placed
	bool solved = true;
	for (int i = 0; i < _tiles.size(); i++)
		if (!_tiles[i]->placed) {
			solved = false;
			break;
		}

	if (solved) {

		//if solved

		canTouch = false;

		float delay = 2.0f;

		playEachLetterSound(delay);

		this->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create([=]() {
			//fade out image and show text only
			for (int i = 0; i < _tiles.size(); i++) {
				_tilesBack[i]->fadeOut();
				_tiles[i]->fadeOut();
				auto caption = ((Label*)_captionHolder[i]->getChildByName("caption"));
				caption->setTextColor(Color4B::WHITE);
				caption->setOpacity(255);
			}

			//move
			moveCaptionToCenter();

			//play again
			float d = 0.3 + 1;
			//playEachLetterSound(d);

			//play whole word
			this->runAction(Sequence::create(DelayTime::create(d + 0.3), CallFunc::create([=]() {
				for (int i = 0; i < _tiles.size(); i++) {
					Label* label;
					label = (Label*)_captionHolder[i]->getChildByName("caption");
					label->setTextColor(Color4B::BLUE);
				}
				auto audio_path = std::string(_allLevels[_currentIndex].wordAudio);
				auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
				cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
					for (int i = 0; i < _tiles.size(); i++) {
						Label* label;
						label = (Label*)_captionHolder[i]->getChildByName("caption");
						label->setTextColor(Color4B::WHITE);
					}
				});

			}), nullptr));

			//slide tray for round finish
			_tray->runAction(Sequence::create(DelayTime::create(d + 2),
				EaseBackOut::create(MoveTo::create(1, Vec2(AspectR::getMidX(), 0))),
				CallFunc::create([=]() { roundFinish(); }),
				nullptr));

		}), nullptr));

	}

}

void GamePlay::playEachLetterSound(float &delay) {
	//play letter sounds again
	for (int i = 0; i < _tiles.size(); i++) {
		Label* label;
		if (!_tiles[i]->hided) {
			label = _tiles[i]->caption();
		}
		else {
			label = (Label*)_captionHolder[i]->getChildByName("caption");
		}
		playEachLetterSound(delay, label);
		delay += 1;
	}
}

void GamePlay::playEachLetterSound(float &delay, Label* label) {
	label->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create([=]() {
		SoundM::playLetter(_allLevels[_currentIndex].charAudio.at(label->getString()));
		label->setTextColor(Color4B::BLUE);
	}), DelayTime::create(0.2f), CallFunc::create([=] {
		label->setTextColor(Color4B::WHITE);
	}), nullptr));
}

void GamePlay::moveCaptionToCenter()
{
	float widthOfCaptions = 0;
	for (int i = 0; i < _captionHolder.size(); i++) {
		widthOfCaptions += _captionHolder[i]->getContentSize().width;
	}

	float leftX = (_tray->getContentSize().width - widthOfCaptions) / 2;
	for (int i = 0; i < _captionHolder.size(); i++) {
		_captionHolder[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		_captionHolder[i]->runAction(MoveTo::create(0.5, Vec2(leftX, _captionHolder[i]->getPositionY())));
		leftX += _captionHolder[i]->getContentSize().width;
	}
}

void GamePlay::roundFinish() {

	_currentIndex++;

	//destroy and reset variables

	_tray->removeAllChildrenWithCleanup(true);

	_tiles.clear();

	_tilesBack.clear();

	_captionHolder.clear();

	_dragTile = nullptr;

	if (_currentIndex >= _allLevels.size()) {
		const string path = "gaf/build_word/eat_cake/eat_cake.gaf";
		GAFAsset *asset = GAFAsset::create(path);
		GAFObject *object = asset->createObject();
		object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		object->setPosition(Vec2(AspectR::getMidX(), AspectR::getMidY() - AspectR::getHeight(450)));
		object->setScale(0.6);
		this->addChild(object);

		object->start();

		this->unschedule("gaf_play_callback");
		auto anim_length = object->getTotalFrameCount();
		auto fps = object->getFps();
		this->scheduleOnce([=](float dt) {
			endGame();
		}, anim_length * 1.f / fps, "gaf_play_callback");

	}
	else {
		startGame();
	}

}

void GamePlay::endGame() {
	cocos2d::experimental::AudioEngine::stop(bg_audio_id);
	commonEndGame(END_GAME_TYPE::EGT_COMMON);
	//    cocos2d::experimental::AudioEngine::stopAll();
	//    cocos2d::experimental::AudioEngine::play2d("sounds/endgame/cheer.mp3");
	//
	//    auto screen_size = Director::getInstance()->getVisibleSize();
	//    auto touch_layout = cocos2d::ui::Layout::create();
	//    touch_layout->setContentSize(screen_size);
	//    touch_layout->setTouchEnabled(true);
	//    this->addChild(touch_layout, 1900000000);
	//    touch_layout->setBackGroundColor(Color3B::BLACK);
	//    touch_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	//    touch_layout->setOpacity(180);
	//
	//    auto monkey = GAFWrapper::createGAF("gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf");
	//    this->addChild(monkey, 1900000000);
	//    monkey->setPosition(cocos2d::Size(screen_size.width/2, screen_size.height - monkey->getContentSize().height/2));
	//    monkey->play(false, [=] (gaf::GAFObject *obj){
	//        this->unscheduleAllCallbacks();
	//        this->unscheduleUpdate();
	//        this->stopAllActions();
	//        cocos2d::experimental::AudioEngine::stopAll();
	//        if (this->getActivityDelegate()) {
	//            this->getActivityDelegate()->onActivityEnd(this);
	//        }
	//        this->removeFromParent();
	//    });
	//
	//    auto phaohoa = GAFWrapper::createGAF("gaf/end_game/phaoHoaNo_standalone/phaoHoaNo_standalone.gaf");
	//    this->addChild(phaohoa, 1900000000);
	//    phaohoa->setPosition(cocos2d::Size(screen_size.width*0.25, screen_size.height/2));
	//    phaohoa->play(true);
	//
	//    auto phaohoa2 = GAFWrapper::createGAF("gaf/end_game/phaoHoaNo_standalone/phaoHoaNo_standalone.gaf");
	//    this->addChild(phaohoa2, 1900000000);
	//    phaohoa2->setPosition(cocos2d::Size(screen_size.width*0.75, screen_size.height/2));
	//    phaohoa2->play(true);
	//
	//    auto phaogiay = GAFWrapper::createGAF("gaf/end_game/PhaoGiay_standalone/PhaoGiay_standalone.gaf");
	//    this->addChild(phaogiay, 1900000000);
	//    phaogiay->setPosition(screen_size/2);
	//    phaogiay->play(true);
}

void GamePlay::onEnter() {
    ms::BaseActivity::onEnter();

    //*** Event Listenter for DRAG ***//
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = [=](Touch* touch, Event* event) {
        if(!canTouch) return false;
        Vec2 pt = _tray->convertToNodeSpace(touch->getLocation());

        //check which biscuits to move
        for(int i=0;i<int(_tiles.size());i++) {
            if(!_tiles[i]->placed && _tiles[i]->getBoundingBox().containsPoint(pt)) {
                _dragTile = _tiles[i];
                SoundM::playDragStart();
                return true;
            }
        }
        return false;
    };
    _listener->onTouchMoved = [=](Touch* touch, Event* event) {
        Vec2 pt = _tray->convertToNodeSpace(touch->getLocation());
        if(_dragTile != NULL  && !_dragTile->placed) {
            //move biscuits
            _dragTile->setPosition(Vec2(_dragTile->getPosition().x + touch->getDelta().x,
                                        _dragTile->getPosition().y + touch->getDelta().y));
            //bring to front
            _dragTile->getParent()->reorderChild(_dragTile, _dragTile->getParent()->getChildrenCount()-1);
        }
    };
    _listener->onTouchEnded = [=](Touch* touch, Event* event) {
        Vec2 pt = _tray->convertToNodeSpace(touch->getLocation());

        if(_dragTile != NULL && !_dragTile->placed && _dragTile->holder->isOverlap(_dragTile->getBoundingBox())) {

            for(int i=0;i<int(_tiles.size());i++) {
                if(_tiles[i] == _dragTile) {

                    _dragTile = nullptr;

                    _tiles[i]->getParent()->reorderChild(_tiles[i], _tiles[i]->getParent()->getChildrenCount()-2);
                    //place biscuit
                    _tiles[i]->setPosition(_tiles[i]->holder->getPosition());

                    SoundM::playDrop();

                    //set so next time can't be moved
                    _tiles[i]->placed = true;

                    //drop animation and sound
                    _tiles[i]->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.3, 0.8)),
                                                          EaseBackOut::create(ScaleTo::create(0.3, 1)),
                                                          CallFunc::create([=]() {
                                                              float d = 0.1;
                                                              playEachLetterSound(d, _tiles[i]->caption());
                                                          }),nullptr));

                    //check if word solved
                    checkIfSolved();
                    break;
                }
            }

        } else {
            //return to original position
            canTouch = false;
            _dragTile->runAction(Sequence::create(EaseBackInOut::create(MoveTo::create(0.5,_dragTile->originalPos)), CallFunc::create([=]{
                canTouch = true;
            }), NULL));
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    //*********************//
}

void GamePlay::onExit() {
    Layout::onExit();
    mj::helper::purgeTextureData();
    _eventDispatcher->removeEventListener(_listener);
	//    cocos2d::experimental::AudioEngine::stopAll();

}
