//
//  BuggiesV2.h
//


#include "BuggiesV2.h"
#include "JsonDump.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"
#include "BuggiesV2_ChooseCar.h"

#define speed_car 20000
using namespace cocos2d;

BuggiesV2 * BuggiesV2::createGame(std::string json_file) {

	BuggiesV2 * obj = new BuggiesV2();
	if (obj->init())
	{
		obj->autorelease();
		obj->didLoadFromCSB(json_file);
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return nullptr;
}

void BuggiesV2::didLoadFromCSB(std::string json_file) {
	CCLOG(__FUNCTION__);
	auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
	rapidjson::Document value_data;
	bool is_convert_plist_2_json = false;
	if (is_convert_plist_2_json) {
		auto value_map = FileUtils::getInstance()->getValueMapFromFile(json_file);
		buffer_string = JsonDump::getInstance()->MapToString(value_map);
	}

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

	generateData(value_data.GetObject());

}

void BuggiesV2::generateData(rapidjson::Value value_data) {
	CCLOG(__FUNCTION__);
	if (value_data.HasMember("number_turnplay") && value_data["number_turnplay"].IsInt()) {
		number_turnplay = value_data["number_turnplay"].GetInt();
	}

	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

	if (value_data.HasMember("list_item") && value_data["list_item"].IsArray()) {
		auto list_item_data = value_data["list_item"].GetArray();
		for (int i = 0; i < (int)list_item_data.Size(); i++) {
			auto item_data = list_item_data[i].GetObject();
			BUGGIES_INFO item_info;
			item_info.level = item_data["level"].GetString();
			auto image_id = item_data["text"].GetString();
			auto image = math::resource::ResHelper::getImage(images, image_id);
			item_info.text = image->getText("name_1");
			item_info.audio_path_slow = image->getAudios("name_n")[0];
			item_info.audio_path_normal = image->getAudios("name_1")[0];

			auto list_charater_text = image->getTextPhonic("name_p");
			auto list_charater_audio = image->getAudioPhonic("name_p");

			for (int j = 0; j < (int)list_charater_text.size(); j++) {
				CHARACTER_INFO character_info;
				character_info.character = list_charater_text[j];
				character_info.audio_path = list_charater_audio[j];
				item_info.list_character.push_back(character_info);
			}
			genCharacterIndex(item_info);

			list_item_info.push_back(item_info);
		}
	}
}

void BuggiesV2::onEnter() {
	MJMainGBase::onEnter();
	this->setContentSize(screen_size);
	this->showChooseCarView();	
	AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bgm.mp3", true);
	
}

void BuggiesV2::onSpeakerClicked(cocos2d::Ref *sender) {
	if (!_isIntroSoundStop)
	{
		return;
	}
	unschedule(_scheduleReminderName);
	stopChainSpelling();
	_chainSpell.push_back(current_item_info.audio_path_normal);
	AUDIO_ENGINE->stopSound(current_item_info.audio_path_normal);
	for (auto character : current_item_info.list_character) {
		_chainSpell.push_back(character.audio_path);
		AUDIO_ENGINE->stopSound(character.audio_path);
	}
	AUDIO_ENGINE->playChain(_chainSpell);
	scheduleReminder();
}

void BuggiesV2::onExitTransitionDidStart() {
	unschedule(_scheduleReminderName);
	MJMainGBase::onExitTransitionDidStart();
}

void BuggiesV2::showChooseCarView() {
	auto choose_car = BuggiesV2_ChooseCar::createView(1);
	choose_car->setDelegate(this);
	choose_car->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	choose_car->setPosition(screen_size / 2);
	this->addChild(choose_car, 1000000);
}

//Call after car was chosen
void BuggiesV2::onChooseCar(std::string car_index, int map_index) {
	this->car_index = car_index;
	this->map_index = map_index;
	AUDIO_ENGINE->playEffect("sounds/buggiesv2/ting.mp3");
	this->showIntroGame();
}

void BuggiesV2::scheduleReminder()
{
	schedule([=](float) {
		onSpeakerClicked(this);
	}, 10, _scheduleReminderName);
}

void BuggiesV2::stopChainSpelling()
{
	
	//Stop chain spelling
	for (int i = 0; i < _chainSpell.size(); i++)
	{
		AUDIO_ENGINE->stopSound(_chainSpell[i]);
	}
	_chainSpell.clear();

}

void BuggiesV2::loadFrames() {
	CCLOG(__FUNCTION__);
	screen_size = cocos2d::Director::getInstance()->getWinSize();
	//this->setTouchEnabled(true);

	m_worldNode = cocos2d::ui::Layout::create();
	this->addChild(m_worldNode);
	m_worldNode->setTouchEnabled(true);

	m_worldNode = cocos2d::ui::Layout::create();
	this->addChild(m_worldNode);
	m_worldNode->setTouchEnabled(true);

	touch_layout = cocos2d::ui::Layout::create();
	touch_layout->setContentSize(screen_size);
	this->addChild(touch_layout, 100);
	touch_layout->setBackGroundColor(Color3B::BLACK);
	touch_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	touch_layout->setOpacity(100);
	touch_layout->setTouchEnabled(true);
	touch_layout->setVisible(false);

	auto bg = cocos2d::ui::ImageView::create(StringUtils::format("games/buggiesv2/bg_play_%d.png", map_index));
	bg->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	//bg->setScaleY(0.5f);
	bg->setPosition(Vec2(screen_size.width / 2, 0));
	m_worldNode->addChild(bg, 0, "bg");

	if (screen_size.width > bg->getContentSize().width) {
		bg->setScaleX(screen_size.width*1.0 / bg->getContentSize().width);
	}

	m_worldNode->setContentSize(Size(screen_size.width, bg->getContentSize().height));

	car = cocos2d::ui::ImageView::create(car_index.c_str());
	//car->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_worldNode->addChild(car, 200002);
	car->setRotation(-90);
	car->setPosition(Vec2(screen_size.width / 2 - 40, 300));

	auto motionstreak_size = 100;
	motion_streak = MotionStreak::create(0.75f, 5, motionstreak_size, Color3B::WHITE, StringUtils::format("games/buggiesv2/motionstreak_%d.png", map_index));
	m_worldNode->addChild(motion_streak, 1001);
	motion_streak->setPosition(car->getPosition());
	auto img_text_bg = cocos2d::ui::ImageView::create("games/buggiesv2/7.png");
	img_text_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	img_text_bg->setPosition(Vec2(screen_size.width / 2, -5));
    img_text_bg->ignoreContentAdaptWithSize(false);
    img_text_bg->setContentSize(Size(900, 140)) ;
	_imgTextBg = img_text_bg;
    this->addChild(_imgTextBg, 98);
    

	gachngang_layout = cocos2d::ui::Layout::create();
	this->addChild(gachngang_layout, 100);
	gachngang_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	gachngang_layout->setPosition(Vec2(screen_size.width / 2, 25));

	text_lb = cocos2d::ui::Text::create("", PHONIC_MANAGER->getGameFontName(), 100);
	this->addChild(text_lb, 102);
	text_lb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	text_lb->enableOutline(cocos2d::Color4B::BLACK, 3);

	auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(BuggiesV2::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(BuggiesV2::onTouchEnded, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(BuggiesV2::onTouchMoved, this);
	touch_listener->onTouchCancelled = CC_CALLBACK_2(BuggiesV2::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, bg);

	speaker = cocos2d::ui::Button::create("games/buggiesv2/loa icon.png");
	speaker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	speaker->setPosition(Vec2(40, screen_size.height - 40));
	this->addChild(speaker, 99);
	speaker->setTouchEnabled(true);
	speaker->addClickEventListener(CC_CALLBACK_1(BuggiesV2::onSpeakerClicked, this));

	this->scheduleOnce([=](float) {
		this->showText();
		this->showCharacter();
		AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_idle.mp3",true);
	}, 0.5f, "show_text");
	AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_intro.mp3", false, [=] {
		AUDIO_ENGINE->playEffect(list_item_info[index_item].audio_path_normal);
		_isIntroSoundStop = true;
	},0.5f);
	text_hint = cocos2d::ui::Text::create(current_character_info.character, PHONIC_MANAGER->getGameFontName(), 200);
	text_hint->setPosition(screen_size / 2);
	this->addChild(text_hint, 100);

	_minUnderPos.y = _imgTextBg->getContentSize().height + car->getContentSize().height / 2;
	

}

void BuggiesV2::showIntroGame() {
	CCLOG(__FUNCTION__);
	
	this->loadFrames();
	scheduleReminder();
}

bool BuggiesV2::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	CCLOG(__FUNCTION__);
	unschedule(_scheduleReminderName);
	stopChainSpelling();
	stopCarEffect();
	AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_Vinc.mp3",true);
	touch_point = m_worldNode->convertTouchToNodeSpace(touch);
	if (touch_point.y <= _minUnderPos.y)
	{
		touch_point.y = _minUnderPos.y;
	}
	if (car->getBoundingBox().containsPoint(touch_point) && !is_touch_car) {
		is_touch_car = true;
	}
	this->scheduleUpdate();
	return true;
}

void BuggiesV2::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
	CCLOG(__FUNCTION__);
	
	auto touch_end = m_worldNode->convertTouchToNodeSpace(touch);
	CCLOG("touch = %f x %f", touch_end.x, touch_end.y);

	scheduleReminder();

	is_touch_car = false;
	//this->unscheduleUpdate();
	//stopCarEffect();
	AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_Vdec.mp3", false, [=] {
		AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_idle.mp3", true);
	});
	/*if (is_touch_car) {
		
	}*/

}

void BuggiesV2::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
	touch_point = m_worldNode->convertTouchToNodeSpace(touch);
	if (touch_point.y <= _minUnderPos.y)
	{
		touch_point.y = _minUnderPos.y;
	}
}

void BuggiesV2::showHint(float delay_time, bool fade_out)
{
	if (current_item_info.level == "easy") {

	}
	else {
		this->scheduleOnce([=](float dt) {
			/*text_hint->setString(current_character_info.character);
			text_hint->runAction(Sequence::create(FadeIn::create(0.25f), DelayTime::create(1.25f),FadeOut::create(0.25f),NULL));
									Repeat::create(Sequence::create(CallFunc::create([=] {
						letter->setColor(Color3B::RED);
					}), DelayTime::create(0.5f), CallFunc::create([=] {
						letter->setColor(Color3B::WHITE);
					}), DelayTime::create(0.5f), NULL)

			*/
			AUDIO_ENGINE->playSound(current_character_info.audio_path);
			for (auto index : current_character_info.list_index) {
				auto letter = list_letter[index];
				if (letter) {
					auto repeat = fade_out ? 1000 : 3;
					letter->runAction(Sequence::create(FadeTo::create(0.25f, 255),
						Repeat::create(Sequence::create(FadeTo::create(0.25f, 100), DelayTime::create(0.25f),
							FadeTo::create(0.25f, 255),
							DelayTime::create(0.25f), NULL), repeat), FadeOut::create(0.25f), NULL));
				}
			}
			showHint(15.0f, true);
		}, delay_time, "hint_game");
	}

}

void BuggiesV2::hideHint()
{
	this->unschedule("hint_game");
	text_hint->stopAllActions();
	text_hint->setOpacity(0);

}

void BuggiesV2::runText()
{
	int move_path = cocos2d::random(1, 2);
	if (move_path == 1) {
		moveTextByCirclePath();
	}
	else {
		moveTextZigzagPath();
	}


}

Sequence * BuggiesV2::createMoveAnimation(std::vector<Vec2> st, float speed)
{
	Vector< FiniteTimeAction * > fta;
	//std::reverse(st.begin(), st.end());
	auto startPos = st.back();
	auto currentPos = st.back();
	st.pop_back();
	while (!st.empty()) {
		auto des = st.back();
		auto moveAction = MoveTo::create(1024.0f / screen_size.width * des.distance(currentPos) / speed, des);
		fta.pushBack(moveAction);
		currentPos = des;
		st.pop_back();
	}

	//so far, you have created a list of moving actions and a stop animation call at the end
	auto seq = Sequence::create(fta);
	return seq;
}

std::vector<Vec2> BuggiesV2::createCirclePath(Vec2 center_pos, float radius, int segment, float start_angle, float arc_angle, bool backToStart)
{
	std::vector<Vec2> vp;

	float step_angle = arc_angle / segment;

	for (int i = 0; i < segment; ++i) {
		Vec2 pos;
		auto angle = start_angle + i * step_angle;
		pos.x = center_pos.x + cos(CC_DEGREES_TO_RADIANS(angle)) * radius;
		pos.y = center_pos.y + sin(CC_DEGREES_TO_RADIANS(angle)) * radius;
		vp.push_back(pos);
	}
	if (backToStart) {
		vp.push_back(vp.front());
	}
	return vp;
}

std::vector<Vec2> BuggiesV2::createZigzagPath(Vec2 center_pos, float radius1, float radius2, int segment, float start_angle, float arc_angle, bool backToStart)
{
	std::vector<Vec2> vp;

	float step_angle = arc_angle / segment;

	for (int i = 0; i < segment; ++i) {
		Vec2 pos;
		auto angle = start_angle + i * step_angle;
		auto radius = (i & 1) ? radius1 : radius2;
		pos.x = center_pos.x + cos(CC_DEGREES_TO_RADIANS(angle)) * radius;
		pos.y = center_pos.y + sin(CC_DEGREES_TO_RADIANS(angle)) * radius;
		vp.push_back(pos);
	}
	if (backToStart) {
		vp.push_back(vp.front());
	}
	return vp;
}

void BuggiesV2::moveTextByCirclePath()
{
	auto avaible_size = m_worldNode->getContentSize();
	auto width_box = cocos2d::random(400, 600);
	auto min_h = 425;
	auto mid_point = Vec2(avaible_size.width / 2, (avaible_size.height + min_h) / 2);
	auto number_char = list_character.size();
	auto step_angle = 360 / number_char;
	std::vector<float> list_angle;
	for (int i = 0; i < number_char; i++) {
		auto item_angle = i * step_angle;
		list_angle.push_back(item_angle);
	}
	mj::helper::shuffle(list_angle);

	int segment = cocos2d::random(3, 32);
	//start play move animation now	
	for (int i = 0; i < number_char; i++) {
		auto item_angle = list_angle[i];
		auto pos = Vec2(mid_point.x + cos(item_angle) * width_box, mid_point.y + sin(item_angle) * width_box);
		list_character[i]->setPosition(pos);
		std::vector<cocos2d::Vec2> st = createCirclePath(mid_point, width_box, segment, item_angle);
		auto moveAnimation = createMoveAnimation(st);
		list_character[i]->runAction(RepeatForever::create(moveAnimation));
	}

}

void BuggiesV2::moveTextZigzagPath()
{
	auto avaible_size = m_worldNode->getContentSize();
	auto width_box = cocos2d::random(400, 600);
	auto min_h = 425;
	auto mid_point = Vec2(avaible_size.width / 2, (avaible_size.height + min_h) / 2);
	auto number_char = list_character.size();
	auto step_angle = 360 / number_char;
	std::vector<float> list_angle;
	for (int i = 0; i < number_char; i++) {
		auto item_angle = i * step_angle;
		list_angle.push_back(item_angle);
	}
	mj::helper::shuffle(list_angle);

	int segment = cocos2d::random(4, 8) * 2;
	//start play move animation now	
	for (int i = 0; i < number_char; i++) {
		auto item_angle = list_angle[i];
		auto pos = Vec2(mid_point.x + cos(item_angle) * width_box, mid_point.y + sin(item_angle) * width_box);
		list_character[i]->setPosition(pos);
		std::vector<cocos2d::Vec2> st = createZigzagPath(mid_point, width_box,width_box - 250, segment, item_angle);
		auto moveAnimation = createMoveAnimation(st);
		list_character[i]->runAction(RepeatForever::create(moveAnimation));
	}
}

void BuggiesV2::update(float dt) {

	auto car_position = car->getPosition();
	auto vec_distance = touch_point - car_position;
	auto move_distance = dt * speed_car;
	auto offset = vec_distance.getNormalized() *sqrtf(move_distance);

	auto destination = offset.length() < vec_distance.length() ? (offset + car_position) : (vec_distance + car_position);

	cocos2d::Size avaible_size = m_worldNode->getContentSize();

	/*destination.x = (destination.x < 0 ? 0 : (destination.x > avaible_size.width ? avaible_size.width : destination.x));
	destination.y = (destination.y < 0 ? 0 : (destination.y > avaible_size.height ? avaible_size.height : destination.y));*/

	destination.x = (destination.x < 0 ? 0 : (destination.x > avaible_size.width ? avaible_size.width : destination.x));
	destination.y = (destination.y < _minUnderPos.y ? _minUnderPos.y : (destination.y > avaible_size.height ? avaible_size.height : destination.y));

	if (destination != touch_point) {
		auto angle = atan2(vec_distance.y, vec_distance.x);
		angle = angle * (180 / M_PI);
		car->setRotation(-angle);
		car->setPosition(destination);
		motion_streak->setPosition(car->getPosition());
	}

	for (int i = 0; i < (int)list_character.size(); i++) {
		auto character_item = list_character[i];
		if (car->getBoundingBox().containsPoint(character_item->getPosition())) {
			//            CCLOG("touch hit %d = %s", index_character, current_character_info.character.c_str());
			//Collide letter
			if (character_item->item_info.character == current_character_info.character) {
				CCLOG("touch current");
				list_character.erase(list_character.begin() + i);
				this->mergeLetter(character_item);
				break;
			}
		}
	}

	updateCamera();
	//After reach destination point touch
	if (!is_touch_car && destination.distance(touch_point) < 20) {
		this->unscheduleUpdate();
		stopCarEffect();
		AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_Vdec.mp3", false, [=] {
			AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_idle.mp3",true);
		});
	}
}

//Call after got correct letter
void BuggiesV2::mergeLetter(BuggiesV2_Item *character_item) {
	CCLOG(__FUNCTION__);

	stopChainSpelling();

	hideHint();
	showHint();
	AUDIO_ENGINE->playEffect(character_item->item_info.audio_path);
	
	auto pos_goal = gachngang_layout->convertToWorldSpace(this->getPositionCharacterMove());
	pos_goal.y += character_item->getBoundingBox().size.height*0.3;
	character_item->stopAllActions();
	character_item->text->stopAllActions();
	auto scale = text_lb->getFontSize()*1.0f / character_item->text->getFontSize();
	character_item->onMergeCharacter();
	AUDIO_ENGINE->playEffect("sounds/buggiesv2/ting.mp3");

	character_item->runAction(Sequence::create(Spawn::create(MoveTo::create(0.3f, pos_goal), ScaleTo::create(0.3f, scale), NULL), CallFunc::create([=] {
		AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_pop.mp3");
		character_item->removeFromParent();
		for (int i = 0; i < current_character_info.list_index.size(); i++) {
			auto letter = list_letter[current_character_info.list_index[i]];
			letter->stopAllActions();
			letter->setColor(cocos2d::Color3B::WHITE);
			letter->setOpacity(255);
		}
		index_character++;
		this->onNextCharacter();
		
	}), NULL));
}

Vec2 BuggiesV2::getPositionCharacterMove() {
	CCLOG(__FUNCTION__);
	auto index = current_character_info.list_index[0];
	CCLOG("index = %d", index);
	auto rect_result = list_gachngang[index]->getBoundingBox();
	auto rect_num = current_character_info.list_index.size();
	if (rect_num > 1) {
		for (int i = 1; i < rect_num; i++) {
			index = current_character_info.list_index[i];
			CCLOG("index = %d", index);
			auto rect = list_gachngang[index]->getBoundingBox();
			rect_result = rect_result.unionWithRect(rect);
		}
	}
	return rect_result.origin + rect_result.size / 2;
}

void BuggiesV2::onNextCharacter() {
	CCLOG(__FUNCTION__);
	if (index_character >= (int)current_item_info.list_character.size()) {
		this->mergeWord();
	}
	else {
		for (int i = 0; i < (int)current_character_info.list_index.size(); i++) {
			list_gachngang[current_character_info.list_index[i]]->stopAllActions();
			list_gachngang[current_character_info.list_index[i]]->setColor(Color3B::WHITE);
		}
		current_character_info = current_item_info.list_character[index_character];
		CCLOG("touch hit %d = %s", index_character, current_character_info.character.c_str());
		this->runEffectGachNgang();
	}
}

void BuggiesV2::onNextWord() {
	CCLOG(__FUNCTION__);
	index_item++;
	if (index_item >= (int)list_item_info.size()) {
        AUDIO_ENGINE->stopAllAudio();
		this->onFinishGame();
	}
	else {
		AUDIO_ENGINE->playEffect(list_item_info[index_item].audio_path_normal);
		this->showText();
		this->showCharacter();
		car->runAction(FadeIn::create(0.5f));
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
			enableTouch();
			stopCarEffect();
			AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_idle.mp3",true);
		}), NULL));
	}
}

void BuggiesV2::updateCar(float dt) {
	motion_streak->setPosition(car->getPosition());
}

void BuggiesV2::updateCamera()
{


	// fix position

	auto car_pos = car->getPosition();
	CCLOG("car pos : %f %f", car_pos.x, car_pos.y);
	auto world_available_size = m_worldNode->getContentSize();
	if (car_pos.x < screen_size.width / 2) {
		m_camera.position.x = 0;
	}
	else if (car_pos.x > world_available_size.width - screen_size.width / 2) {
		m_camera.position.x = world_available_size.width - screen_size.width;
	}
	else {
		m_camera.position.x = car_pos.x - screen_size.width / 2;
	}

	if (car_pos.y < screen_size.height / 2) {
		m_camera.position.y = 0;
	}
	else if (car_pos.y > world_available_size.height - screen_size.height / 2) {
		m_camera.position.y = world_available_size.height - screen_size.height;
	}
	else {
		m_camera.position.y = car_pos.y - screen_size.height / 2;
	}
	m_worldNode->setPosition(-m_camera.position);
}

void BuggiesV2::showCharacter() {
	CCLOG(__FUNCTION__);
	for (int i = 0; i < (int)current_item_info.list_character.size(); i++) {
		BuggiesV2_Item::CHARACTER_INFO character_info;
		character_info.character = current_item_info.list_character[i].character;
		character_info.audio_path = current_item_info.list_character[i].audio_path;
		auto character_item = BuggiesV2_Item::createItem(character_info);
		m_worldNode->addChild(character_item, 1005);
		//character_item->runText();
		character_item->setScale(0);
		character_item->runAction(Sequence::create(DelayTime::create(0.1f*i), ScaleTo::create(0.5f, 1.0f), NULL));
		list_character.push_back(character_item);
	}

	runText();
	showHint();
}

void BuggiesV2::showText() {
	CCLOG(__FUNCTION__);
	list_letter.clear();
	list_gachngang.clear();
	list_pos_letter.clear();
	current_item_info = list_item_info[index_item];
	index_character = 0;
	current_character_info = current_item_info.list_character[index_character];
	auto width_gachngang = 81, w_add_gachngang = 5;
	auto num_gachngang = StringUtils::StringUTF8(current_item_info.text).length();
	for (int i = 0; i < num_gachngang; i++) {
		auto gachngang = cocos2d::ui::ImageView::create("games/buggiesv2/gachchan.png");
		gachngang_layout->addChild(gachngang);
		gachngang->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		gachngang->setPositionX(i*(width_gachngang + w_add_gachngang));
		gachngang->setScale(0, 1.0f);
		gachngang->runAction(ScaleTo::create(0.25f, 1.0f));
		list_gachngang.push_back(gachngang);
	}
	gachngang_layout->setContentSize(cocos2d::Size(num_gachngang*(width_gachngang + w_add_gachngang) - w_add_gachngang, 50));
	//    gachngang_layout->setPositionX(screen_size.width/2 - gachngang_layout->getContentSize().width/2);

	int letter_opacity = 0;
	if (current_item_info.level == "easy") {
		letter_opacity = 75;
	}
	text_lb->setScale(1.0f);
	text_lb->setString(current_item_info.text);
	auto pos_text = gachngang_layout->convertToWorldSpace(Vec2(0, list_gachngang[0]->getPositionY()));
	text_lb->setPosition(Vec2(screen_size.width / 2, pos_text.y + 10));
	for (int i = 0; i < (int)list_gachngang.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			auto pos_convert = text_lb->convertToNodeSpace(gachngang_layout->convertToWorldSpace(list_gachngang[i]->getPosition()));
			list_pos_letter.push_back(letter->getPosition() + Vec2(0, 0));
			CCLOG("y = %f", letter->getPositionY() - letter->getContentSize().height*0.5);
			letter->setPosition(Vec2(pos_convert.x + width_gachngang * 0.5, letter->getPositionY() - 23));
			letter->setOpacity(letter_opacity);
			list_letter.push_back(letter);
		}
	}

	this->runEffectGachNgang();
}
void BuggiesV2::runEffectGachNgang() {
	CCLOG(__FUNCTION__);
	for (int i = 0; i < (int)current_character_info.list_index.size(); i++) {
		auto index = current_character_info.list_index[i];
		CCLOG("index = %d", index);
		auto gach_ngang = list_gachngang[index];
		gach_ngang->runAction(Repeat::create(Sequence::create(CallFunc::create([=] {
			gach_ngang->setColor(Color3B::RED);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			gach_ngang->setColor(Color3B::WHITE);
		}), DelayTime::create(0.5f), NULL), 100000));
	}
}

void BuggiesV2::setColorForText(Color3B color_text) {
	CCLOG(__FUNCTION__);
	for (int i = 0; i < (int)list_letter.size(); i++) {
		auto letter = text_lb->getLetter(i);
		if (letter) {
			letter->setColor(color_text);
		}
	}
}

//Call after collect all word
void BuggiesV2::mergeWord() {
	CCLOG(__FUNCTION__);
	hideHint();
	this->disableTouch();
	this->resetCar();
	this->runAction(Sequence::create(DelayTime::create(0.65f), CallFunc::create([=] {
		auto duration = 0.5f;
		for (int i = 0; i < (int)list_gachngang.size(); i++) {
			auto gachngang = list_gachngang[i];
			gachngang->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([gachngang] {
				gachngang->removeFromParent();
			}), NULL));
		}
		list_gachngang.clear();
		AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_twinkle.mp3");
		text_lb->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(EaseSineIn::create(MoveTo::create(duration, Vec2(screen_size.width / 2, screen_size.height*0.4))), ScaleTo::create(duration, 1.7f), NULL), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playSyncText();

		}), NULL));
		for (int i = 0; i < (int)list_pos_letter.size(); i++) {
			list_letter[i]->runAction(Sequence::create(DelayTime::create(0.5f), MoveTo::create(duration, list_pos_letter[i]), NULL));
		}
	}), NULL));
}

//Playe after merge
void BuggiesV2::playSyncText()
{
	float dt = 0.0f;
	for (int i = 0; i < current_item_info.list_character.size(); ++i) {
		this->runAction(Sequence::create(DelayTime::create(dt), CallFunc::create([=] {
			AUDIO_ENGINE->playSound(current_item_info.list_character[i].audio_path);
			for (int index : current_item_info.list_character[i].list_index) {
				text_lb->getLetter(index)->runAction(Sequence::create(CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(Color3B::RED);
				}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
					text_lb->getLetter(index)->setColor(Color3B::WHITE);
				}), NULL));
			}
		}), NULL));
		dt += 0.75f;
	}
	this->scheduleOnce([=](float dt) {
		playSlowText();
	}, dt, "play sync text finnished");
}

void BuggiesV2::playSlowText() {
	CCLOG(__FUNCTION__);
	auto audio_path = std::string(current_item_info.audio_path_slow);
	auto audio_id = cocos2d::experimental::AudioEngine::play2d(audio_path);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		this->setColorForText(Color3B::RED);
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.3;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 20));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -20));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), DelayTime::create(0.5f), CallFunc::create([=] {
			this->playNormalText();
		}), NULL));
	}), NULL));
}
void BuggiesV2::playNormalText() {
	CCLOG(__FUNCTION__);
	auto audio_id = AUDIO_ENGINE->play(current_item_info.audio_path_normal);
	this->setColorForText(Color3B::RED);
	this->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=] {
		auto duration = cocos2d::experimental::AudioEngine::getDuration(audio_id);
		auto duration0 = duration * 0.4;
		duration -= duration0;
		auto scale_vl = text_lb->getScale();
		auto scale1 = ScaleTo::create(duration0, scale_vl - 0.1f);
		auto scale2 = ScaleTo::create(0.7*duration, scale_vl + 0.2f);
		auto move1 = MoveBy::create(0.7*duration, Vec2(0, 10));
		auto scale3 = ScaleTo::create(0.3*duration, scale_vl);
		auto move2 = MoveBy::create(0.3*duration, Vec2(0, -10));
		text_lb->runAction(Sequence::create(scale1, Spawn::create(scale2, move1, NULL), Spawn::create(scale3, move2, NULL), CallFunc::create([=] {
			this->setColorForText(Color3B::WHITE);
		}), DelayTime::create(1.0f), EaseBackIn::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height))), DelayTime::create(0.25f), CallFunc::create([=] {
			this->onNextWord();
		}), NULL));
	}), NULL));
}

void BuggiesV2::disableTouch() {
	touch_layout->setVisible(true);
}

void BuggiesV2::enableTouch() {
	touch_layout->setVisible(false);
}

void BuggiesV2::resetCar() {
	CCLOG(__FUNCTION__);
	stopCarEffect();
	AUDIO_ENGINE->playSound("sounds/buggiesv2/cb_bmw_Vdec.mp3");
	is_touch_car = false;
	this->unscheduleUpdate();
	auto pos_goal = Vec2(screen_size.width / 2 - 40, 300);
	auto vec_distance = pos_goal - car->getPosition();
	auto angle = atan2(vec_distance.y, vec_distance.x);
	angle = angle * (180 / M_PI);
	car->setRotation(-angle);
	auto duration = 1.0f / 1024 * pos_goal.distance(car->getPosition());
	
	car->runAction(Sequence::create(MoveTo::create(duration, pos_goal), CallFunc::create([=] {
		car->setRotation(-90);
		this->unschedule(schedule_selector(BuggiesV2::updateCar));
	}), FadeTo::create(0.5f, 100), NULL));

	m_worldNode->runAction(MoveTo::create(duration, Vec2(0, 0)));
	this->schedule(schedule_selector(BuggiesV2::updateCar), 0.075f);
}

void BuggiesV2::stopCarEffect()
{
	AUDIO_ENGINE->stopSound("sounds/buggiesv2/cb_bmw_idle.mp3");
	AUDIO_ENGINE->stopSound("sounds/buggiesv2/cb_bmw_Vdec.mp3");
	AUDIO_ENGINE->stopSound("sounds/buggiesv2/cb_bmw_Vinc.mp3");
}

void BuggiesV2::genCharacterIndex(BuggiesV2::BUGGIES_INFO &info)
{
	CCLOG(__FUNCTION__);
	// Gen character index	
	int start_index = 0;
	std::queue<int> underline_index;
	for (int i = 0; i < info.list_character.size(); ++i) {
		auto phonic_data = info.list_character[i];
		for (int j = 0; j < phonic_data.character.size(); ++j) {
			if (phonic_data.character[j] == '_') {
				//underline_index
				underline_index.push(start_index + j);
			}
			else {
				if (underline_index.empty() || underline_index.front() > start_index) {
					info.list_character[i].list_index.push_back(start_index + j);
				}
				else {
					info.list_character[i].list_index.push_back(underline_index.front());
					underline_index.pop();
				}
			}
		}
		start_index += phonic_data.character.size();
	}
}
