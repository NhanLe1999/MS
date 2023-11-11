#include "DienThe.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "ParseJSONHelper.hpp"
INITIALIZE_READER(DienThe);

std::once_flag DienThe_reader;

#define SOUND_CORRECT "sounds/DienThe/correct_%d.mp3"
#define SOUND_WRONG "sounds/DienThe/wrong_%d.mp3"
#define SOUND_TOUCH "sounds/DienThe/touch_%d.mp3"
#define SOUND_DROP "sounds/DienThe/drop_%d.mp3"
#define SOUND_MOVEBACK "sounds/DienThe/moveback_%d.mp3"

#define CSB_NAME "csb/game/DienThe/DienThe.csb"
#define _game_path "json/DienThe/"

DienThe* DienThe::createGame() {
	std::call_once(DienThe_reader, [] {
		REGISTER_CSB_READER(DienThe);
		});
	auto p = static_cast<DienThe*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void DienThe::didLoadFromCSB() {
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("config.json");
	rapidjson::Document document;
	document.Parse<0>(str.c_str());
	rapidjson::Value& json_data = document.FindMember("games")->value;
	this->generateData(json_data.GetArray());
	screen_size = Director::getInstance()->getVisibleSize();

	////    autoPlay = isAutoPlay;*/
	this->loadFrames();
}

void DienThe::generateData(rapidjson::Value value_data) {
	data_game.clear();
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
	auto str_icons = cocos2d::FileUtils::getInstance()->getStringFromFile("icons.json");
	rapidjson::Document document_icons;
	document_icons.Parse<rapidjson::kParseStopWhenDoneFlag>(str_icons.c_str());
	lang = math::lang::english_us;
	if (document_icons.IsArray()) {
		if (JSON_PARSE.getString("en", document_icons[0], "language") == "vi") {
			lang = math::lang::vietnamese;
		}
	}
	for (rapidjson::SizeType i = 0; i < value_data.Size(); i++) {
		DataGame data;
		// question
		std::string img_id = value_data[i].FindMember("question")->value.GetString();
		auto image = math::resource::ResHelper::getImage(images, img_id);
		data.ques.theme = value_data[i].FindMember("theme")->value.GetString();
		data.ques.audio = image->getAudios("name_1", lang).front();
		data.ques.text = image->getQuestionVMonkey("name_1", lang);
		// get dap an sai
		int count = 0;

		//sentence		
		rapidjson::Value value_sentences = value_data[i].FindMember("sentences")->value.GetArray();
		for (rapidjson::SizeType j = 0; j < value_sentences.Size(); j++) {
			Sentence sen;
			auto img_sen = math::resource::ResHelper::getImage(images, value_sentences[j].FindMember("sentence")->value.GetString());
			sen.text = img_sen->getText("name_1", lang);
			sen.audio = img_sen->getAudios("name_1", lang).front();
			std::vector<Word> _words;
			if (value_sentences[j].HasMember("words")) {
				rapidjson::Value value_words = value_sentences[j].FindMember("words")->value.GetArray();
				for (rapidjson::SizeType k = 0; k < value_words.Size(); k++) {
					auto img_word = math::resource::ResHelper::getImage(images, value_words[k].FindMember("word")->value.GetString());
					Word w;
					w.image_word = img_word;
					_words.push_back(w);
				}
			}
			sen.words = _words;

			if (count == 0)
			{
				wrong_answers.clear();
				if (value_data[i].HasMember("dap_an_sai")) {
					auto value_wrong_answer = value_data[i].FindMember("dap_an_sai")->value.GetArray();
					for (auto j = 0; j < value_wrong_answer.Size(); j++) {
						auto wImage = math::resource::ResHelper::getImage(images, value_wrong_answer[j].GetString());
						Word w;
						w.image_word = wImage;
						wrong_answers.push_back(w);
					}
					sen.word_wrong = wrong_answers;
					count++;
				}
			}
			data.sentences.push_back(sen);
		}
		data_game.push_back(data);
	}
	stages_number = data_game.size();
}

void DienThe::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->setTouchEnabled(true);
}

void DienThe::loadFrames() {
	auto bg = utils::findChild<ImageView*>(this, "bg");
	current_question = 0;
	root_layout = utils::findChild<Layout*>(this, "root_layout");
	btn_sound = utils::findChild<Layout*>(this, "sound_button");
	btn_sound->removeFromParent();
	_speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
	_speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
	this->addChild(_speaker);
	_speaker->setTouchEnabled(false);
	this->showQuestion();
}

void DienThe::showQuestion() {
	radio();
	number_paired = 0;
	if (data_game.front().ques.theme == "ngang") {
		box_slots = ImageView::create("games/DienThe/Layer-10.png");
		box_slots->setPosition(Vec2(screen_size.width / 2 - 200, screen_size.height * 0.4));
		root_layout->addChild(box_slots);
		box_items = ImageView::create("games/DienThe/Layer-12.png");
		box_items->setPosition(Vec2(screen_size.width / 2 + 300, screen_size.height * 0.4));

		root_layout->addChild(box_items);
	}
	else {
		box_slots = ImageView::create("games/DienThe/Layer-15.png");
		box_slots->setPosition(Vec2(screen_size.width / 2, screen_size.height / 2));
		root_layout->addChild(box_slots);
		box_items = ImageView::create("games/DienThe/Layer-17.png");
		box_items->setPosition(Vec2(screen_size.width / 2, screen_size.height * 0.1));
		root_layout->addChild(box_items);
	}
	texts_sentences.clear();
	// gen items
	for (auto sen : data_game[current_question].sentences) {
		for (auto img_w : sen.words) {
			// item
			auto item = DienThe_Item::createItem(img_w.image_word);
			item->setDelegate(this);
			m_items.push_back(item);
		}
	}
	for (auto sen : data_game[current_question].sentences) {
		if (!sen.word_wrong.empty()) {
			for (auto img_w : sen.word_wrong) {
				// item
				if (img_w.image_word) {
					auto item = DienThe_Item::createItem(img_w.image_word);
					item->setDelegate(this);
					m_items.push_back(item);
				}
			}
		}
	}
	// gen text 35
	int max_text = -1;
	for (int i = 0; i < data_game[current_question].sentences.size(); i++)
	{
		int m_text_size = data_game[current_question].sentences.at(i).text.size();
		if (m_text_size > max_text)
			max_text = m_text_size;
	}
	CCLOG("%s_________ nooo", data_game[current_question].sentences.at(0).text.c_str());
	float m_posX = max_text <= 35 ? 150 : 50;
	float posX = 50;
	float posY = box_slots->getBoundingBox().size.height - (4 - data_game[current_question].sentences.size()) * 50;

	auto sentencesSize = data_game[current_question].sentences.size();
	if (data_game[current_question].ques.theme == "doc") {
		posY = box_slots->getBoundingBox().size.height - (4 - data_game[current_question].sentences.size()) * ((sentencesSize > 2) ? 90 : 45);
		posX = m_posX;
	}
	for (auto sen : data_game[current_question].sentences) {
		string text_full = sen.text;
		std::vector<string> strs;
		int start = 0;
		for (int i = 0; i < sen.words.size(); i++) {
			string text_word = sen.words[i].image_word->getText("name_1", lang);
			int found = text_full.find(text_word, start);
			strs.push_back(text_full.substr(start, found - start));
			strs.push_back("*" + text_word);
			start = found + text_word.size();
		}

		strs.push_back(text_full.substr(start));
		std::vector<Text*>texts;
		float sizeBoxSlots = box_slots->getBoundingBox().size.width;
		int textPosY = 100;
		float sizeText = 0;

		for (auto str : strs) {
			str = trimText(str);
			if (str[0] != '*') {
				_quantityStr++;
				Text* text = Text::create(str, font_name, 29);
				_senLength = _senLength + text->getBoundingBox().size.width;
			}
		}
		auto slot = DienThe_Slot::createSlot(0);
		_senLength += strs.size()/2 * slot->getContentSize().width;
		if (_senLength >= sizeBoxSlots) {
			
			textFontsize = 29;
			_flag = false;
		}

		if (_flag) {
			for (auto str : strs) {
				str = trimText(str);
				if (str[0] != '*') {
					Text* text = Text::create(str, font_name, 30);
					text->setTextColor(Color4B::BLACK);
					text->setAnchorPoint(Vec2(0, 0));
					text->setPosition(Vec2(posX, posY));
					posX += text->getContentSize().width;
					box_slots->addChild(text);
					texts.push_back(text);
				}
				else {
					auto slot = DienThe_Slot::createSlot(0);
					slot->setAnchorPoint(Vec2(0, 0.5));
					slot->setPosition(Vec2(posX, posY + 15));
					posX += slot->getContentSize().width;
					box_slots->addChild(slot);
					slot->setName("slot");
					m_slots.push_back(slot);
				}

			}
			texts_sentences.push_back(texts);
			if (data_game[current_question].ques.theme == "doc") {
				posY -= 90;
				posX = m_posX;
			}
			else {
				posX = 100;
				posY -= 100;
			}
		}
		else
		{
			for (auto str : strs) {
				str = trimText(str);
				if (str[0] != '*') {
					Text* text = Text::create(str, font_name, textFontsize);
					_countBox++;
					_sizeQuestion = _sizeQuestion + text->getBoundingBox().size.width + (_countBox - 1) * 100;
					float m = _sizeQuestion - text->getBoundingBox().size.width;
					float sizeQuestion2 = _sizeQuestion + 150;
					if (sizeQuestion2 >= sizeBoxSlots - 50) {
						string textDefault = text->getString();
						int currentIndexMakeStringTooLong = 0;
						for (int i = 0; i < textDefault.size(); i++) {
							if (textDefault[i] == ' ' || i == textDefault.size() - 1) {
								string str = "";
								if (i == textDefault.size() - 1)
								{
									str = textDefault;
								}
								else
								{
									str = textDefault.substr(0, i + 1);
								}
								Text* text1 = Text::create(str, font_name, textFontsize);
								if (m + text1->getBoundingBox().size.width >= sizeBoxSlots - 100) {
									str = currentIndexMakeStringTooLong == 0? str : textDefault.substr(0, currentIndexMakeStringTooLong + 1);
									text1 = Text::create(str, font_name, textFontsize);
									text = text1;
									string m2 = currentIndexMakeStringTooLong != 0 ? textDefault.substr(currentIndexMakeStringTooLong + 1, textDefault.size() - 1) :
									 textDefault.substr(i + 1, textDefault.size() - 1);
									_text2 = Text::create(m2, font_name, textFontsize);
									_text2->setTextColor(Color4B::BLACK);
									_text2->setAnchorPoint(Vec2(0, 0));
									_text2->setPosition(Vec2(40, posY - 100));
									box_slots->addChild(_text2);
									textsSentences.push_back(_text2);
									_sizeQuestion = _text2->getBoundingBox().size.width + 200;
									_flag2 = false;
									_countBox = 0;
									break;
								}
								else
								{
									if (str == textDefault)
									{
										_flag2 = false;
										_countBox = 1;
										_sizeQuestion = 0;
									}
								}
								currentIndexMakeStringTooLong = i;
							}
						}
					}
					text->setTextColor(Color4B::BLACK);
					text->setAnchorPoint(Vec2(0, 0));
					text->setPosition(Vec2(posX - 10, posY));
					posX += text->getContentSize().width;
					box_slots->addChild(text);
					texts.push_back(text);
				}
				else {
					_slotItem++;
					auto slot = DienThe_Slot::createSlot(0);
					slot->setAnchorPoint(Vec2(0, 0.5));
					if (_flag2) {
						slot->setPosition(Vec2(posX - 10, posY + 10));
					}
					//New line
					else
					{
						if (_text2)
						{
							slot->setPosition(Vec2(40 + _text2->getBoundingBox().size.width, posY - 85));
							posX = _text2->getBoundingBox().size.width + 40;
							posY = posY - 100;
						}
						else
						{
							slot->setPosition(Vec2(40, posY - 85));
							posX = 40;
							posY = posY - 100;
							_text2 = Text::create(".", font_name, textFontsize);
							_flag2 = true;
						}
					}
					posX += slot->getContentSize().width;
					box_slots->addChild(slot);
					slot->setName("slot");
					m_slots.push_back(slot);
				}
			}
			texts_sentences.push_back(texts);
		}
	}

	showItemsAndSlots();

	// gen question
	auto text_question = utils::findChild<Text*>(this, "text_question");
	text_question->setFontName(font_name);
	text_question->setFontSize(35);
	text_question->setText(data_game[current_question].ques.text);
	// doc cau hoi va doan tho

	if (_flag3) {
		audio_id_question = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio, false, [=]() {
			for (auto a : data_game[current_question].sentences) {
				audios.push_back(a.audio);
				audio.push_back(a.audio);
			}
			if (audios.size() > 0) {
				this->playChain(audios);
			}
			});
	}
	else
	{
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			audio_id_question = AUDIO_ENGINE->playSound("", false, [=]() {
				for (auto a : data_game[current_question].sentences) {
					audios.push_back(a.audio);
					audio.push_back(a.audio);
				}
				if (audios.size() > 0) {
					this->playChain(audios);
				}
				});
			}), NULL));
	}
}
void DienThe::showItemsAndSlots() {

	for (int i = 0; i < m_slots.size(); i++) {
		m_slots[i]->setLinker(m_items[i]);
		m_slots[i]->onShow(0.25f);
	}
	m_items[0]->setName("item");

	mj::helper::shuffle(m_items);

	if (data_game[current_question].ques.theme == "ngang") {
		auto rect = box_items->getBoundingBox();
		rect.origin.x = rect.origin.x + 30;
		rect.size.width = rect.size.width - 60;
		rect.size.height = rect.size.height - 50;

		math::func::setPositionList(m_items, 2, rect);
	}
	else {
		if (m_items.size() >= 4) {
			box_items->setScaleX(1.5);
		}
		auto rect = box_items->getBoundingBox();
		rect.origin = Vec2(rect.origin.x + 30, rect.origin.y);
		rect.size = Size(rect.size.width - 60, rect.size.height);
		math::func::setPositionList(m_items, m_items.size(), rect);

	}
	for (auto item : m_items) {
		root_layout->addChild(item, 2);
		item->showItem(0.25f, [item] {
			item->setTouchHandleType(DienThe_ItemTouchType::TOUCH);
			item->setTouchEnable(false);
			});
	}

}
void DienThe::nextGame() {

}
std::string DienThe::trimText(std::string text) {
	auto _text = text;
	if (_text.size() >= 1) {
		if (_text[0] == ' ') {
			_text.erase(_text.begin());
		}
	}
	if (_text.size() >= 1) {
		if (_text[_text.size() - 1] == ' ') {
			_text.erase(_text.end() - 1);
		}
	}
	return _text;
}
void DienThe::onExitTransitionDidStart() {
	AUDIO_ENGINE->stopAllSound();
	cocos2d::ui::Layout::onExitTransitionDidStart();
	this->unscheduleAllCallbacks();
	this->unscheduleUpdate();
	this->stopAllActions();
}

void DienThe::showTutorial(float delay_time) {
	this->schedule([=](float) {
		_audio = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio);
		}, 13, 100, delay_time, "showTutorial");
}

void DienThe::stopTutorial() {
	this->unschedule("showTutorial");
	AUDIO_ENGINE->stopEffect(_audio);
}

void DienThe::guideGame() {
	this->schedule([=](float) {
		this->showDragGuideGame(root_layout->convertToWorldSpace((utils::findChild(root_layout, "item")->getPosition())), box_slots->convertToWorldSpace(utils::findChild(box_slots, "slot")->getPosition()) + Vec2(60, -10), 0);
		_audio = AUDIO_ENGINE->playSound(data_game[current_question].ques.audio);
		}, 13, 100, 10, "guide_game");
}

void DienThe::stopGuideGame() {
	this->unschedule("guide_game");
	AUDIO_ENGINE->stopEffect(_audio);
	if (Director::getInstance()->getRunningScene()->getChildByName("guide_layout")) {
		Director::getInstance()->getRunningScene()->removeChildByName("guide_layout");
	}
}

void DienThe::onFinishGameVMonkey() {
	MJMainGBase::onFinishGameVMonkey();
}

void DienThe::onClickedSlotDelegate(cocos2d::Ref* sender)
{

}

void DienThe::onClickedItem(cocos2d::Ref* sender)
{
	CCLOG("click");

}

void DienThe::onMoveBeginItem(cocos2d::Ref* sender)
{
	if (flag) {
		stopGuideGame();
		guideGame();
	}
	else
	{
		stopTutorial();
		showTutorial(10.0f);
	}
	CCLOG("MOVE ITEM");
	AUDIO_ENGINE->stopAllSound();
	for (auto text_sen : texts_sentences) {
		for (auto t : text_sen) {
			t->setTextColor(Color4B::BLACK);
		}
		for (auto t : textsSentences) {
			t->setTextColor(Color4B::BLACK);
		}
	}
	_speaker->setTouchEnabled(true);
	AUDIO_ENGINE->playSound(StringUtils::format(SOUND_TOUCH, cocos2d::random(1, 2)));
	auto item = static_cast<DienThe_Item*>(sender);
	item->setLocalZOrder(21);

}

void DienThe::onMoveEndedItem(cocos2d::Ref* sender, bool is_item_moved)
{
	CCLOG("MOVE END ITEM");
	auto item = static_cast<DienThe_Item*>(sender);
	if (is_item_moved) {
		item->setLocalZOrder(20);
		auto pos = item->getPosition();
		pos = box_slots->convertToNodeSpace(this->convertToWorldSpace(pos));
		DienThe_Slot* slot = nullptr;
		for (auto w : m_slots) {
			auto bounding = w->getBoundingBox();
			if (bounding.containsPoint(pos)) {
				slot = w;
			}
		}
		if (slot && (slot->getLinker() == item || slot->getLinker()->getImagePath() == item->getImagePath())) {
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_CORRECT, cocos2d::random(1, 3)));
			onPair(item, slot);
		}
		else {
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_WRONG, cocos2d::random(1, 3)));
			AUDIO_ENGINE->playSound(StringUtils::format(SOUND_MOVEBACK, cocos2d::random(1, 3)));
			item->backToStart();
		}
	}
}
void DienThe::onPair(DienThe_Item* item, DienThe_Slot* slot)
{
	flag = false;
	stopGuideGame();
	showTutorial(10.0f);
	++number_paired;
	item->setTouchHandleType(DienThe_ItemTouchType::NONE);
	auto pos = (item->getParent()->convertToNodeSpace(box_slots->convertToWorldSpace(slot->getPosition())));
	pos.x = pos.x + 5;
	item->setAnchorPoint(Vec2(0, 0.5));
	item->runAction(Sequence::create(MoveTo::create(0.25f, pos), CallFunc::create([=]() {
		slot->onHide();
		}), NULL));
	this->scheduleOnce([=](float) {
		AUDIO_ENGINE->playSound(StringUtils::format(SOUND_DROP, cocos2d::random(1, 3)));
		}, 0.1f, "drop_item");
	if (number_paired == m_slots.size()) {
		++current_question;
		if (current_question == stages_number)
		{
			stopTutorial();
			for (auto item : m_items) {
				item->setTouchEnable(false);
			}
			_speaker->setTouchEnabled(false);
			if (audios.size() > 0) {
				this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=] {
					this->playChain(audios, 0, [=]() {
						this->scheduleOnce([=](float) {
							onFinishGameVMonkey();
							}, 1.0f, "ending script");
						});
					}), NULL));
			}
		}
		else
		{
			_flag = true;
			_flag2 = true;
			_flag3 = false;
			_slotItem = -1;
			stopTutorial();
			_sizeQuestion = 0;
			for (auto item : m_items) {
				item->setTouchEnable(false);
			}
			_speaker->setTouchEnabled(false);
			this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
				for (auto t : texts_sentences[0]) {
					t->setTextColor(Color4B::RED);
				}
				for (auto t : textsSentences) {
					t->setTextColor(Color4B::RED);
				}
				AUDIO_ENGINE->playSound(audio[0], false, [=] {
					for (auto t : texts_sentences[0]) {
						t->setTextColor(Color4B::BLACK);
					}
					for (auto t : textsSentences) {
						t->setTextColor(Color4B::BLACK);
					}
					texts_sentences.clear();
					audios.clear();
					audio.clear();
					this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
						textsSentences.clear();
						flagText = false;
						root_layout->removeChild(box_items, true);
						for (auto slot : m_slots)
						{
							slot->removeFromParentAndCleanup(true);
						}
						m_slots.clear();
						m_images.clear();
						for (auto item : m_items)
						{
							item->removeFromParentAndCleanup(true);
						}
						m_items.clear();
						showQuestion();
						}), NULL));
					});
				}), NULL));
		}
	}
}
void DienThe::playChain(std::vector<std::string> paths, int startIndex, std::function<void()> _callback) {
	if (startIndex < paths.size() && paths.size() > 0) {
		for (auto t : texts_sentences[startIndex]) {
			t->setTextColor(Color4B::RED);
			if (textsSentences.size() > 0)
			{
				for (auto t : textsSentences) {
					t->setTextColor(Color4B::RED);
				}
			}
		}
		audio_id_question = AUDIO_ENGINE->playSound(paths[startIndex], false, [=]() {
			for (auto t : texts_sentences[startIndex]) {
				t->setTextColor(Color4B::BLACK);
				for (auto t : textsSentences) {
					t->setTextColor(Color4B::BLACK);
				}
			}
			playChain(paths, startIndex + 1, _callback);
			});
	}
	else {

		if (_callback) {
			_callback();
		}
		else {
			for (auto item : m_items) {
				item->setTouchEnable(true);
			}
			if (flag) {
				stopGuideGame();
				guideGame();
			}
			else
			{
				stopTutorial();
				showTutorial(10.0f);
			}
			_speaker->setTouchEnabled(true);
		}
	}
}

void DienThe::radio() {
	_speaker->addClickEventListener([=](cocos2d::Ref* sender) {
		stopGuideGame();
		stopTutorial();
		AUDIO_ENGINE->stopSound(data_game[current_question].ques.audio);
		_speaker->setTouchEnabled(false);
		this->playChain(audios, 0, [=]() {
			_speaker->setTouchEnabled(true);
			if (flag) {
				stopGuideGame();
				guideGame();
			}
			else
			{
				stopTutorial();
				showTutorial(10.0f);
			}
			});
		});
}

void DienThe::showDragGuideGame(Vec2 posStart, Vec2 posEnd, int loop, float delay_time, float dt, std::string gaf_guide_path, cocos2d::ui::Layout* parentLayout) {
	auto guide_layout_check = Director::getInstance()->getRunningScene()->getChildByName("guide_layout");
	if (guide_layout_check)
	{
		guide_layout_check->removeFromParent();
	}
	auto guide_layout = cocos2d::ui::Layout::create();
	guide_layout->setContentSize(screen_size);

	if (parentLayout == NULL) {
		Director::getInstance()->getRunningScene()->addChild(guide_layout, INT_MAX);
	}
	else {
		parentLayout->addChild(guide_layout, parentLayout->getZOrder());
	}

	guide_layout->setName("guide_layout");

	guide_layout->schedule([=](float) {
		auto hand = GAFWrapper::createGAF(gaf_guide_path);
		guide_layout->addChild(hand, 1900000000);
		hand->setPosition(posStart);
		hand->setName("_hand");
		hand->play(false, [=](gaf::GAFObject* obj) {
			hand->runAction(Sequence::create(MoveTo::create(0.6f, posEnd),
				CallFunc::create([=]() {
					hand->play(false, [=](gaf::GAFObject* obj) {
						hand->removeFromParent();
						});
					}), NULL));
			});
		}, dt, loop, delay_time, "animated_guide");
}