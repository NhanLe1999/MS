//
//  Thanh_Matching3C.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#include "Thanh_Matching3C.h"
#include "Math_Libs.h"
#define line_color cocos2d::Color4F(cocos2d::Color3B(243, 104, 35))
#define line_weight 8

Thanh_Matching3C* Thanh_Matching3C::createQuiz()
{
	Thanh_Matching3C* quiz = new Thanh_Matching3C();
	if (quiz->_initQuiz()) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE(quiz);
	return NULL;
}

bool Thanh_Matching3C::init() {
	return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_Matching3C::initQuiz(mj::model::Game game)
{

	Math_Quiz::initQuiz(game);

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
	rapidjson::Document document;
	document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;

	//TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember("game")->value;

	str = json_game.FindMember("game_type")->value.GetString();
	if (str == "GOG") gameType = GameType::GOG;
	else if (str == "OGO") gameType = GameType::OGO;
	else {
		if (cocos2d::random(0, 1)) gameType = GameType::GOG;
		else gameType = GameType::OGO;
	}
	number_row = json_game.FindMember("number_row")->value.GetInt();

	object_size = cocos2d::Size( json_game.FindMember( "object_size" )->value.FindMember( "width" )->value.GetInt(),
								 json_game.FindMember( "object_size" )->value.FindMember( "height" )->value.GetInt() );
	group_size = cocos2d::Size( json_game.FindMember( "group_size" )->value.FindMember( "width" )->value.GetInt(),
								json_game.FindMember( "group_size" )->value.FindMember( "height" )->value.GetInt() );

	object_bg = math::resource::ResHelper::createImage(getUIPath("common/drag.png"));
	group_bg = groupBG();
	//TODO: OBJECT CONFIG
	rapidjson::Value& json_object = json_data.FindMember("object")->value;

	object_reuse = json_object.FindMember("resource_reuse")->value.GetBool();

	str = json_object.FindMember("object_1")->value.FindMember("display")->value.GetString();
	if (str == "name") object_option_1 = ObjectOption::text;
	else if (str == "number") object_option_1 = ObjectOption::number;
	else if (str == "image") object_option_1 = ObjectOption::image;
	str = json_object.FindMember("object_2")->value.FindMember("display")->value.GetString();
	if (str == "name") object_option_2 = ObjectOption::text;
	else if (str == "number") object_option_2 = ObjectOption::number;
	else if (str == "image") object_option_2 = ObjectOption::image;


	//TODO: GROUP CONFIG
	rapidjson::Value& json_group = json_data.FindMember("group")->value;

	group_reuse = json_group.FindMember("resource_reuse")->value.GetBool();

	rapidjson::Value& group_1 = json_group.FindMember("group_1")->value;
	ele_group_1_size = group_size / 3;
	str = group_1.FindMember("group_type")->value.GetString();
	if (str == "frame") type_group_1 = math::group::GroupType::frame;
	else if(str == "custom") type_group_1 = math::group::GroupType::custom;
	else if(str == "queue") type_group_1 = math::group::GroupType::queue;

	rapidjson::Value& group_2 = json_group.FindMember("group_2")->value;
	ele_group_2_size = group_size / 3;
	str = group_2.FindMember("group_type")->value.GetString();
	if (str == "frame") type_group_2 = math::group::GroupType::frame;
	else if (str == "custom") type_group_2 = math::group::GroupType::custom;
	else if (str == "queue") type_group_2 = math::group::GroupType::queue;

	//TODO:
	//lay tat ca icon trong media_set
	// object icon
	number_list = getNumbers(0, 10);

	// group icon
	auto icons_list = math::resource::ResHelper::getImages(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()), "", image_prefix);
	for (math::resource::Image* img : icons_list) {
		if (img->getComparisionValue("role") == 0) {
			icons_group_0.push_back(img);
		}
		else if (img->getComparisionValue("role") == 1) {
			icons_group_1.push_back(img);
		}
	}

	/*---> Lay xong data cua game*/

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	parseQuestion();
	/*
	 if (random_sorted) {
	 random_shuffle(list_questiondata.begin(), list_questiondata.end());
	 }
	 */
	 // gen obj & group
	genObjectGroup();
	//Math_Pool::getInstance()->resetImages();
}

void Thanh_Matching3C::onStart() {
	CCLOG("%s override", __func__);
	if (gameType == GameType::GOG) {
		// show list group 1
		for (Math_Group *group : list_group_1) {
			addChildNode(group, 2);
			group->onShow(0, math::animation::No_Effect);
			Matching3CPair *mp = new Matching3CPair();
			mp->node = group;
			mp->panel = 1;
			my_pair.push_back(mp);  
		}
		// show object list
		for (Math_Object *object : list_object_1) {
			addChildNode(object, 2);
			object->onShow(0, math::animation::ScaleUpAndRotation);
			Matching3CPair *mp = new Matching3CPair();
			mp->node = object;
			mp->panel = 2;
			my_pair.push_back(mp);
		}
		// show list group 2
		for (Math_Group *group : list_group_2) {
			addChildNode(group, 2);
			group->onShow(0, math::animation::No_Effect);
			Matching3CPair *mp = new Matching3CPair();
			mp->node = group;
			mp->panel = 3;
			my_pair.push_back(mp);
		}
	}
	else if (gameType == GameType::OGO) {
		// show list object 1
		for (Math_Object *object : list_object_1) {
			addChildNode(object, 2);
			object->onShow(0, math::animation::ScaleUpAndRotation);
			Matching3CPair *mp = new Matching3CPair();
			mp->node = object;
			mp->panel = 1;
			my_pair.push_back(mp);
		}
		// show list group
		for (Math_Group *group : list_group_1) {
			addChildNode(group, 2);
			group->onShow(0, math::animation::No_Effect);
			Matching3CPair *mp = new Matching3CPair();
			mp->node = group;
			mp->panel = 2;
			my_pair.push_back(mp);
		}
		// show list object 2
		for (Math_Object *object : list_object_2) {
			addChildNode(object, 2);
			object->onShow(0, math::animation::ScaleUpAndRotation);
			Matching3CPair *mp = new Matching3CPair();
			mp->node = object;
			mp->panel = 3;
			my_pair.push_back(mp);
		}
	}
	for (Matching3CPair *mp : my_pair) {
		mp->draw = cocos2d::DrawNode::create();
		addChildNode(mp->draw, 1);
		mp->draw->setPosition(cocos2d::Vec2(0, 0));
		mp->start = cocos2d::DrawNode::create();
		addChildNode(mp->start, 0);
		mp->start->drawDot(cocos2d::Vec2(0, 0), line_weight / 2, cocos2d::Color4F(line_color));
		mp->end = cocos2d::DrawNode::create();
		addChildNode(mp->end, 0);
		mp->end->drawDot(cocos2d::Vec2(0, 0), line_weight / 2, line_color);
		mp->avg = cocos2d::DrawNode::create();
		addChildNode(mp->avg, 0);
		mp->disable = false;
		if (mp->panel == 1) {
			mp->plug_right = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/3.png"));
			addChildNode(mp->plug_right, 1);
			mp->plug_right->setPosition(mp->node->getPosition() + cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->plug_right->getContentSize().width / 2, 0));
			mp->start->setPosition(mp->node->getPosition() + cocos2d::Vec2(mp->node->getContentSize().width / 2, 0));
			mp->end->setPosition(mp->plug_right->getPosition() - cocos2d::Vec2(mp->plug_right->getContentSize().width / 2, 0));
			mp->avg->setPosition((mp->start->getPosition() + mp->end->getPosition()) / 2);
			mp->start_plug_position = mp->plug_right->getPosition();
		}
		else if (mp->panel == 2) {
			mp->plug_left = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/4.png"));
			addChildNode(mp->plug_left, 1);
			mp->plug_left->setPosition(mp->node->getPosition() - cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->plug_left->getContentSize().width / 2, 0));
			mp->plug_right = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/3.png"));
			addChildNode(mp->plug_right, 1);
			mp->plug_right->setPosition(mp->node->getPosition() + cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->plug_right->getContentSize().width / 2, 0));
			mp->start->setPosition(mp->node->getPosition() + cocos2d::Vec2(mp->node->getContentSize().width / 2, 0));
			mp->end->setPosition(mp->plug_right->getPosition() - cocos2d::Vec2(mp->plug_right->getContentSize().width / 2, 0));
			mp->avg->setPosition((mp->start->getPosition() + mp->end->getPosition()) / 2);
			mp->start_plug_position = mp->plug_right->getPosition();
		}
		else if (mp->panel == 3) {
			mp->light = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/2.png"));
			addChildNode(mp->light, 1);
			mp->light->setRotation(180);
			mp->light->setPosition(mp->node->getPosition() + cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->light->getContentSize().width / 2, 0));
			mp->plug_left = cocos2d::ui::ImageView::create(getUIPath("gameui/electriclight/4.png"));
			addChildNode(mp->plug_left, 1);
			mp->plug_left->setPosition(mp->node->getPosition() - cocos2d::Vec2(mp->node->getContentSize().width / 2 + mp->plug_left->getContentSize().width / 2, 0));
		}
	}

	//Gen question
	showQuestionBox();

	// touch
	auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(Thanh_Matching3C::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(Thanh_Matching3C::onTouchEnded, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(Thanh_Matching3C::onTouchMoved, this);
	touch_listener->onTouchCancelled = CC_CALLBACK_2(Thanh_Matching3C::onTouchCancelled, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
	this->scheduleUpdate();
}

void Thanh_Matching3C::startQuestion() {
	CCLOG("%s override", __func__);
	if (this->getQuestion()->getData().index == 0) {
		enableControlQuiz();

		touch_disable = false;
		game_over = false;
	}
}

void Thanh_Matching3C::onExplanation(bool correct) {
	CCLOG("%s override", __func__);

}

void Thanh_Matching3C::onComplete() {
	CCLOG("%s override", __func__);

	//Math_Pool::getInstance()->resetAnswers();

	float delay = showConratulation(0.3);

	float m_delay = delay - 0.75;
	if (m_delay < 0) {
		m_delay = 0;
	}
	for (Matching3CPair *mp : my_pair) {
		if (mp->panel == 1 || mp->panel == 3) {
			if (gameType == GameType::OGO) {
				((Math_Object*)mp->node)->onHide(m_delay + 0.2, math::animation::ScaleUpAndRotation);
			}
			else if (gameType == GameType::GOG) {
				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::CallFunc::create([=]() {
					((Math_Group*)mp->node)->setDisable();
				}), nullptr));
			}
		}
		else {
			if (gameType == GameType::OGO) {
				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::CallFunc::create([=]() {
					((Math_Group*)mp->node)->setDisable();
				}), nullptr));
			}
			else if (gameType == GameType::GOG) {
				((Math_Object*)mp->node)->onHide(m_delay + 0.2, math::animation::ScaleUpAndRotation);
			}
		}
		if (mp->draw != nullptr) {
			mp->draw->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
				mp->draw->clear();
				mp->draw->removeFromParentAndCleanup(true);
			}), nullptr));
		}
		if (mp->start != nullptr) {
			mp->start->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
				mp->start->clear();
				mp->start->removeFromParentAndCleanup(true);
			}), nullptr));
		}
		if (mp->avg != nullptr) {
			mp->avg->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
				mp->avg->clear();
				mp->avg->removeFromParentAndCleanup(true);
			}), nullptr));
		}
		if (mp->end != nullptr) {
			mp->end->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.4), cocos2d::CallFunc::create([=]() {
				mp->end->clear();
				mp->end->removeFromParentAndCleanup(true);
			}), nullptr));
		}
		if (mp->light != nullptr) {
			mp->light->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.6), cocos2d::CallFunc::create([=]() {
				mp->light->removeFromParentAndCleanup(true);
			}), nullptr));
		}
		if (mp->plug_left != nullptr) {
			mp->plug_left->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.6), cocos2d::CallFunc::create([=]() {
				mp->plug_left->removeFromParentAndCleanup(true);
			}), nullptr));
		}
		if (mp->plug_right != nullptr) {
			mp->plug_right->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(m_delay - 0.2), cocos2d::FadeOut::create(0.6), cocos2d::CallFunc::create([=]() {
				mp->plug_right->removeFromParentAndCleanup(true);
			}), nullptr));
		}
	}

	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}

void Thanh_Matching3C::onAnswerDone(bool correct)
{
	bool done = true;
	for (int i = 0; i < status.size(); i++) {
		if (status[i] != 0) done = false;
	}
	this->onExplanation(done);
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_Matching3C::autoTesting() {

	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("%s-%d", __func__, testing_step++));
}

void Thanh_Matching3C::autoTestingRun(float dt) {

}

#pragma mark - gen Group & Object
void Thanh_Matching3C::genObjectGroup()
{
	bool num[10] = { false };

	for (int i = 0; i < number_row; i++)
	{
		int random = cocos2d::random(1, 9);
		while (num[random])
		{
			random = cocos2d::random(1, 9);
		}
		num[random] = true;
		value_list.push_back(random);
	}

	std::random_device rd;
	std::mt19937 g(rd());

	switch (gameType) {
	case GameType::GOG:
	{
		// collum a
		list_group_1 = createGroupList(icons_group_0, type_group_1, ele_group_1_size / 1.2);
		std::shuffle(list_group_1.begin(), list_group_1.end(), g);
		math::func::setPositionList(list_group_1, 1, cocos2d::Rect(0.025, 0.05, 0.2, 0.8));

		// collum b
		list_object_1 = createObjectList(number_list, object_option_1);
		std::shuffle(list_object_1.begin(), list_object_1.end(), g);
		math::func::setPositionList(list_object_1, 1, cocos2d::Rect(0.35, 0.05, 0.2, 0.8));
		for (int i = number_row; i < number_row * 2; i++)
		{
			// status = 2 sẽ nối được 2 lần. chỉ cột giữa mới có
			status[i]++;
		}

		// collum c
		if (group_reuse) list_group_2 = createGroupList(icons_group_1, type_group_2, ele_group_2_size / 1.2);
		else list_group_2 = createGroupList(icons_group_1, type_group_2, ele_group_2_size / 1.2);
		std::shuffle(list_group_2.begin(), list_group_2.end(), g);
		math::func::setPositionList(list_group_2, 1, cocos2d::Rect(0.7, 0.05, 0.2, 0.8));
		break;
	}
	case GameType::OGO:
	{
		// collum a
		list_object_1 = createObjectList(number_list, object_option_1);
		std::shuffle(list_object_1.begin(), list_object_1.end(), g);
		math::func::setPositionList(list_object_1, 1, cocos2d::Rect(0.025, 0.05, 0.2, 0.8));

		// collum b
		list_group_1 = createGroupList(icons_group_0, type_group_1, ele_group_1_size / 1.2);
		std::shuffle(list_group_1.begin(), list_group_1.end(), g);
		math::func::setPositionList(list_group_1, 1, cocos2d::Rect(0.35, 0.05, 0.2, 0.8));
		for (int i = number_row; i < number_row * 2; i++)
		{
			// status = 2 sẽ nối được 2 lần. chỉ cột giữa mới có
			status[i]++;
		}

		// collum c
		if (object_reuse) list_object_2 = createObjectList(number_list, object_option_2);
		else list_object_2 = createObjectList(number_list, object_option_2);
		std::shuffle(list_object_2.begin(), list_object_2.end(), g);
		math::func::setPositionList(list_object_2, 1, cocos2d::Rect(0.7, 0.05, 0.2, 0.8));
		break;
	}
	default:
		break;
	}
}
std::vector<Math_Group*> Thanh_Matching3C::createGroupList(std::vector<math::resource::Image*> img_list, math::group::GroupType type, cocos2d::Size element_size)
{
	std::vector<Math_Group*> vector_group;
	int random = cocos2d::random(0, (int)img_list.size() - 1);

	for (int k = 0; k < value_list.size(); k++)
	{
		Math_Group* group = Math_Pool::getInstance()->getGroup();

		group->setObjectSize(element_size);
		group->setGroupSize(group_size);

		std::vector<Math_Object*> _group_list;
		for (int i = 0; i < value_list[k]; i++) {
			Math_Object* object = Math_Pool::getInstance()->getObject();
			object->setEnable(obj_id++, math::common::deny, img_list[random], group_bg);
			object->setBackgroundVisible(false);
			object->setObjectSize(element_size);
			_group_list.push_back(object);
		}

		group->setEnable(gr_id++, _group_list, type, 9);

		if (type == math::group::GroupType::custom) {
			group->setBackground(group_bg, 20);
			group->setAlignObjects(math::group::GroupAlign::random);
			group->setGroupSize( cocos2d::Size( group_size.width + 30, group_size.height + 30 ) );
			group->updatePosition(false, false);
		}
		else if (type == math::group::frame) { 
			group->setGroupFrames(3, 3, math::resource::ResHelper::createImage(""), group_bg);
		}
		else if (type == math::group::queue) { 
			group->setGroupQueue(cocos2d::Size(element_size.width*1.2*3 + 30, element_size.height*1.2*3 + 30));
			group->setBackground(group_bg, 20); 
		} 
		_groups.push_back(group);
		vector_group.push_back(group);

		list_obj.push_back(group);
		status.push_back(1);
	}
	return vector_group;
}
std::vector<Math_Object*> Thanh_Matching3C::createObjectList(std::vector<math::resource::Image*> img_list, ObjectOption obj_option)
{
	math::common::TouchPermission touch_per = math::common::TouchPermission::touch;

	std::vector<Math_Object*> vector_object;
	for (int i = 0; i < value_list.size(); i++)
	{
		for (math::resource::Image* img : img_list)
		{
			if (img->getComparisionValue("number") == value_list[i])
			{
				Math_Object* obj = Math_Pool::getInstance()->getObject();
				obj->setEnable(obj_id++, touch_per, img, object_bg);
				obj->setObjectSize(object_size); 

				switch (obj_option) {
				case ObjectOption::text:
				{
					obj->setImageVisible(false);
					obj->setValueText(img->getText("name_1", m_lang->getLanguage()));
					obj->fitValueTextSize();
					obj->showValueTextWithAnimation(true);
					break;
				}
				case ObjectOption::number:
				{
					obj->setImageVisible(false);
					obj->setValueText(cocos2d::StringUtils::toString(img->getComparisionValue("number")));
					obj->fitValueTextSize();
					obj->showValueTextWithAnimation(true);
					obj->getLabelValue()->setFontSize(object_size.height*0.75);
					break;
				}
				// image k can lam gi

				default:
					break;
				}

				obj->getLabelValue()->disableEffect();
				_objects.push_back(obj);
				vector_object.push_back(obj);
				list_obj.push_back(obj);
				status.push_back(1);
				break;
			}
		}
	}
	return vector_object;
}

#pragma mark - Add Event
void Thanh_Matching3C::update(float dt)
{
	if (game_over) return;
	for (Matching3CPair *mp : my_pair) {
		if (mp != nullptr && mp->plug_right != nullptr) {
			mp->draw->clear();
			mp->draw->setLineWidth(line_weight);
			cocos2d::Vec2 end_pos;
			end_pos = mp->plug_right->getPosition() - cocos2d::Vec2(mp->plug_right->getContentSize().width / 2, 0);
			mp->draw->drawQuadBezier(mp->start->getPosition(), mp->avg->getPosition(), end_pos, 50, line_color);
		}
	}
}
bool Thanh_Matching3C::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	CCLOG("%s override", __func__);
	if (touch_disable) return false;
	touch_to_plug = false;
	touch_to_image = false;
	for (Matching3CPair *mp : my_pair) {
		if (mp->disable == false && mp->plug_right != nullptr) {
			cocos2d::Vec2 locationInNode = mp->plug_right->convertToNodeSpace(touch->getLocation());
			cocos2d::Size size = mp->plug_right->getBoundingBox().size;
			cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				mp_current = mp;
				mp->plug_right->setLocalZOrder(4);
				mp->draw->setLocalZOrder(3);
				touch_to_plug = true;
				return true;
			}
		}
		if (mp->disable == false) {
			cocos2d::Vec2 locationInNode = mp->node->convertToNodeSpace(touch->getLocation());
			cocos2d::Size size = mp->node->getBoundingBox().size;
			cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				mp_current = mp;
				mp->node->setScale(1.1);
				touch_to_image = true;
				return true;
			}
		}
	}
	CCLOG("play audio: Oh");
	return false;
}

void Thanh_Matching3C::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	CCLOG("%s override", __func__);
	if (touch_to_plug) {
		mp_current->plug_right->setPosition(_panel->convertToNodeSpace(touch->getLocation()));
		mp_current->end->setPosition(mp_current->plug_right->getPosition() - cocos2d::Vec2(mp_current->plug_right->getContentSize().width / 2, 0));
		mp_current->avg->setPosition((mp_current->start->getPosition() + mp_current->end->getPosition()) / 2);
		return;
	}
}
void Thanh_Matching3C::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	CCLOG("%s override", __func__);
	touch_disable = true;
	if (touch_to_plug) {
		start_mp = mp_current;
		for (Matching3CPair *mp : my_pair) {
			if (mp->disable == false) {
				cocos2d::Vec2 locationInNode = mp->node->convertToNodeSpace(touch->getLocation());
				cocos2d::Size size = mp->node->getBoundingBox().size;
				cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);
				if (rect.containsPoint(locationInNode)) {
					end_mp = mp;
					break;
				}
				if (mp->plug_left != nullptr) {
					locationInNode = mp->plug_left->convertToNodeSpace(touch->getLocation());
					size = mp->plug_left->getBoundingBox().size;
					rect = cocos2d::Rect(0, 0, size.width, size.height);
					if (rect.containsPoint(locationInNode)) {
						end_mp = mp;
						break;
					}
				}
			}
		}
		if (end_mp != nullptr && start_mp->panel == end_mp->panel - 1) {
			int a = -1, b = -2;
			switch (gameType)
			{
			case GameType::OGO:
				if (start_mp->panel == 1)
					a = ((Math_Object*)start_mp->node)->getComparisionValue().number;
				else if (start_mp->panel == 2)
					a = ((Math_Group*)start_mp->node)->getNumberObjects();

				if (end_mp->panel == 2)
					b = ((Math_Group*)end_mp->node)->getNumberObjects();
				else  if (end_mp->panel == 3)
					b = ((Math_Object*)end_mp->node)->getComparisionValue().number;
				break;
			case GameType::GOG:
				if (start_mp->panel == 1)
					a = ((Math_Group*)start_mp->node)->getNumberObjects();
				else if (start_mp->panel == 2)
					a = ((Math_Object*)start_mp->node)->getComparisionValue().number;

				if (end_mp->panel == 2)
					b = ((Math_Object*)end_mp->node)->getComparisionValue().number;
				else  if (end_mp->panel == 3)
					b = ((Math_Group*)end_mp->node)->getNumberObjects();
				break;
			default:
				break;
			}
			if (a == b) {
				cocos2d::Vec2 pos;
				pos = end_mp->plug_left->getPosition() - cocos2d::Vec2(end_mp->plug_left->getContentSize().width / 2 + start_mp->plug_right->getContentSize().width / 4, 0);
				float dt = cocos2d::ccpDistance(touch->getLocation(), pos) / 400;
				if( dt > 1 ) dt = 1;
				auto avg_pos = (start_mp->start->getPosition() + end_mp->plug_left->getPosition() -
								 cocos2d::Vec2(end_mp->plug_left->getContentSize().width / 2 + start_mp->plug_right->getContentSize().width*3/4, 0)) / 2;
				start_mp->avg->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.1), 
																				  cocos2d::EaseElasticOut::create( cocos2d::MoveTo::create( dt * 3, avg_pos ), 0.3 )));
				start_mp->end->setVisible(false);
				start_mp->plug_right->runAction(cocos2d::MoveTo::create(dt/2, pos));

				this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::CallFunc::create([=]() {
					touch_disable = false;
					start_mp->draw->setLocalZOrder(0);
					start_mp->plug_right->setLocalZOrder(0);
					if (end_mp->light != nullptr) {
						for (Matching3CPair *p : my_pair) {
							if (p->disable == true && p->panel == 1) {
								if ((GameType::GOG == gameType && ((Math_Group*)p->node)->getNumberObjects() == a)
									|| (GameType::OGO == gameType && ((Math_Object*)p->node)->getComparisionValue().number == a))
								{
									end_mp->light->loadTexture(getUIPath("gameui/electriclight/1.png"));
									break;
								}
							}
						}
					}
					else {
						for (Matching3CPair *p : my_pair) {
							if (p->disable == true && p->panel == 3) {
								if ((GameType::GOG == gameType && ((Math_Group*)p->node)->getNumberObjects() == a)
									|| (GameType::OGO == gameType && ((Math_Object*)p->node)->getComparisionValue().number == a))
								{
									p->light->loadTexture(getUIPath("gameui/electriclight/1.png"));
									break;
								}
							}
						}
					}
					if (start_mp->panel != 2) start_mp->disable = true;
					if (end_mp->panel != 2) end_mp->disable = true;
					bool done = true;
					for (Matching3CPair *mp : my_pair) {
						if (mp->disable == false && mp->panel != 2) {
							done = false;
						}
					}
					if (done) {
						this->runAction( cocos2d::Sequence::createWithTwoActions( cocos2d::DelayTime::create( 0.5 ), cocos2d::CallFunc::create( [=](){
							this->nextQuestion();
						})));
						game_over = true;
						touch_disable = true;
					}
				}), nullptr));
				return;
			}
		}
		float dt = cocos2d::ccpDistance(touch->getLocation(), start_mp->start_plug_position) / 400;
		if( dt > 0.6 ) dt = 0.6;
		start_mp->avg->runAction(cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(dt, start_mp->start->getPosition()), 0.5));
		start_mp->end->runAction(cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(dt, start_mp->start->getPosition()), 0.5));
		start_mp->plug_right->runAction(cocos2d::EaseElasticOut::create(cocos2d::MoveTo::create(dt, start_mp->start_plug_position), 0.5));
		this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(dt), cocos2d::CallFunc::create([=]() {
			start_mp->plug_right->setLocalZOrder(1);
			start_mp->draw->setLocalZOrder(0);
			touch_disable = false;
		}), nullptr));
		return;
	}
}
void Thanh_Matching3C::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event)
{
	CCLOG("%s override", __func__);

}

#pragma mark - on this game
bool Thanh_Matching3C::isTouchedOnObject(cocos2d::Vec2 position, cocos2d::Node *object) {
	cocos2d::Vec2 locationInNode = object->convertToNodeSpace(position);
	cocos2d::Size size = object->getBoundingBox().size;
	cocos2d::Rect rect = cocos2d::Rect(0, 0, size.width, size.height);

	if (rect.containsPoint(locationInNode)) return true;
	return false;
}

void Thanh_Matching3C::setSelected(bool selected, cocos2d::Node* object)
{
	if (selected == true)
	{
		object->setScale(1.1);
		object->setLocalZOrder(4);
	}
	else
	{
		object->setScale(1.0);
		object->setLocalZOrder(2);
	}
}

void Thanh_Matching3C::connect(int i, int j)
{
	cocos2d::Vec2 pos_a;
	cocos2d::Vec2 pos_b;
	if (i > j)
	{
		pos_a = getPositionDraw(i, Direction::left);
		pos_b = getPositionDraw(j, Direction::right);
	}
	else
	{
		pos_a = getPositionDraw(i, Direction::right);
		pos_b = getPositionDraw(j, Direction::left);
	}
	int index_color = i / number_row != 1 ? i : j;
	drawer[i]->clear();
	drawer[index_color]->drawLine(pos_a, pos_b, color[index_color%number_row]);
	drawer[index_color]->drawDot(pos_a, 5, color[index_color%number_row]);
	drawer[index_color]->drawDot(pos_b, 5, color[index_color%number_row]);
	drawer[index_color]->setLineWidth(10);
	drawer[index_color]->setLocalZOrder(1);
	setSelected(false, list_obj[i]);
	setSelected(false, list_obj[j]);
	status[i]--;
	status[j]--;
}

cocos2d::Vec2 Thanh_Matching3C::getPositionDraw(int i, Direction direction)
{
	switch (direction)
	{
	case Direction::left:
		return list_obj[i]->getPosition() - cocos2d::Vec2(list_obj[i]->getContentSize().width / 2, 0);
	case Direction::right:
		return list_obj[i]->getPosition() + cocos2d::Vec2(list_obj[i]->getContentSize().width / 2, 0);
	}
}

bool Thanh_Matching3C::isCorrect(int i, int j)
{
	if (i == j) return false;

	int a, b;
	// lay gia tri node i
	a = getValue(i);

	// lay gia tri node j
	b = getValue(j);

	return (a == b);
}
int Thanh_Matching3C::getValue(int i)
{
	for (Math_Group *gr : _groups)
	{
		if (gr == (Math_Group*)list_obj[i]) return gr->getNumberObjects();
	}
	for (Math_Object *obj : _objects)
	{
		if (obj == (Math_Object*)list_obj[i]) return obj->getComparisionValue().number;
	}
	return -1;
}
