//
//  MSGradeBox.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#include "MSGradeBox.h"
#include "StoryDataModel.h"
#include "MSGradeBoxDelegate.h"
#include "MJDefinitions.h"
#include "StoryLanguageManager.h"
#include "MSEventLogger.h"
#include "StoryConfigManager.h"
#include "MJHelper.h"

USING_NS_CC;
INITIALIZE_READER(MSGradeBox);

std::once_flag ms_gradebox_reader;

cocos2d::Data ms_gradebox_node_data;

MSGradeBox * MSGradeBox::createBox(bool is_new,int i_gradeID)
{
    std::call_once(ms_gradebox_reader, []
    {
        REGISTER_CSB_READER(MSGradeBox);
        auto csb_name = mj::helper::getCSBName("csb/new_ui/GradeBox.csb");
        ms_gradebox_node_data =  FileUtils::getInstance()->getDataFromFile(csb_name);
    });
    
    auto box = reinterpret_cast<MSGradeBox *>(CSLoader::createNode(ms_gradebox_node_data));
    
    if (box)
    {
        box->didLoadFromCSB(is_new,i_gradeID);
        return box;
    }
    
    CC_SAFE_DELETE(box);
    
    return nullptr;
}

void MSGradeBox::didLoadFromCSB(bool is_new,int i_gradeID)
{
    _is_showing_grades  = _is_animating = false;
    _selected_grade_id  = (i_gradeID==UNDEFINED_GRADE_ID ? ms::GradeManager::getInstance().getCurrentGradeId():i_gradeID);
    _grades_info        = ms::GradeManager::getInstance().getGradesInfo();
    m_scope             = ms::GradeManager::getInstance().getScope();
    //loadGrades();
    CCLOG("xxx MSGradeBox::didLoadFromCSB|");
    
    for(auto grade :_grades_info )
    {
        CCLOG("xxx grade.name %s lang:%d",grade.name.c_str(),LANGUAGE_MANAGER->getDisplayLangId());
    }
    
    auto listener = EventListenerCustom::create(key_event_grade_changed, [=](EventCustom * e) {

        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);

        //e->userData() = [current scope]|[current grade id] , ex : 1|1
        if(dataVec.size()>=2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));

        }
        //auto grade_id = reinterpret_cast<int *>(e->getUserData());
        //this->onReceivedEventGradeChanged(* grade_id);
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [=](EventCustom *)
    {
        ms::GradeManager::getInstance().setScope(m_scope);

        ms::GradeManager::getInstance().updateGrades();

        _grades_info        = ms::GradeManager::getInstance().getGradesInfo();

        this->loadGrades();
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_go_to_parents, [this](EventCustom * e)
    {
        this->hideGrades();
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_gradebox_tap_from_sidebar, [this](EventCustom * e)
    {
        this->hideGrades();
    }), this);

}

void MSGradeBox::onEnter()
{
    ui::Layout::onEnter();
    
    loadGrades();
}

void MSGradeBox::loadGrades()
{
    CCLOG("xxx MSGradeBox::loadGrades()");
    
    auto model = utils::findChild<ui::Widget *>(this, "model");
    _model_size = model->getContentSize();
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    listview->removeAllItems();
    listview->setItemsMargin(15);
    listview->setPadding(0, 15, 0, 15);
    listview->setScrollBarEnabled(false);
    listview->setTouchEnabled(true);
    
    auto model_comingsoon = utils::findChild<ui::Widget *>(this, "model_coming");
    
    if(auto comingSoonTxt = utils::findChild<ui::Text *>(model_comingsoon, "name_coming"))
    {
        comingSoonTxt->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        
        comingSoonTxt->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.comming.soon"));
    }
    
    if(auto comingSoonNameTxt = utils::findChild<ui::Text *>(model_comingsoon, "name"))
    {
        comingSoonNameTxt->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    }

    auto model_header = utils::findChild<ui::Widget *>(this, "model_header");
    
    auto h = 0.f;
    
    auto saved_story_grade_id = -1;
    
    if(auto currentNameTxt = utils::findChild<ui::Text *>(this, "name_current"))
    {
        currentNameTxt->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    }
    
    for (auto grade : _grades_info)
    {
        auto item = model->clone();
        
        item->getChildByName<ui::Text *>("name")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");

        if (ms::GradeManager::getInstance().isGradeLocked(grade.grade_id))
        {
            item = model_comingsoon->clone();
            
            //grayscale
            auto grayScaleShader = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, ccPositionTexture_GrayScale_frag);
            grayScaleShader->retain();
            
            grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
            grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
            grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            
            grayScaleShader->link();
            CHECK_GL_ERROR_DEBUG();
            
            grayScaleShader->updateUniforms();
            CHECK_GL_ERROR_DEBUG();
            
            item->getVirtualRenderer()->setGLProgram(grayScaleShader);
            item->getVirtualRenderer()->getGLProgram()->use();
            
            item->setEnabled(false);
            item->setBright(false);
        }
        
        if (/*ms::GradeManager::getInstance().getScope() == ms::GradeManager::Scope::STORIES && */grade.grade_id != -1)
        {
            //item is level info, need to get actual grade info
//            auto level_info = StoryDataModel::getInstance()->getLevelInfoById(grade.grade_id, LANGUAGE_MANAGER->getCurrentLangId());
//            auto grade_info = StoryDataModel::getInstance()->getGradeInfoById(level_info.grade_id, LANGUAGE_MANAGER->getCurrentLangId());
            auto grade_id = ms::GradeManager::getInstance().getGradeIdByLevelId(grade.grade_id);
            auto grade_info = StoryDataModel::getInstance()->getGradeInfoById(grade_id, LANGUAGE_MANAGER->getCurrentLangId());
            if (grade_id != saved_story_grade_id)
            {
                saved_story_grade_id = grade_id;
                auto header = model_header->clone();
                header->setCascadeOpacityEnabled(true);
                header->setName("header");
                header->getChildByName<ui::Text *>("name")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
                header->getChildByName<ui::Text *>("name")->setString(ms::GradeManager::getInstance().getNameByLanguage(grade_info.name, LANGUAGE_MANAGER->getDisplayLangId()));
                listview->pushBackCustomItem(header);
                h += header->getContentSize().height + listview->getItemsMargin();
            }
        }
        
        CCLOG("xxx grade.name %s lang:%d",grade.name.c_str(),LANGUAGE_MANAGER->getDisplayLangId());

        item->setCascadeColorEnabled(false);
        item->setPropagateTouchEvents(true);
        item->setSwallowTouches(false);
        item->setTag(grade.grade_id);
        item->getChildByName<ui::Text *>("name")->setString(ms::GradeManager::getInstance().getNameByLanguage(grade.name, LANGUAGE_MANAGER->getDisplayLangId()));
        listview->pushBackCustomItem(item);
        h += item->getContentSize().height + listview->getItemsMargin();
        
        if (grade.grade_id == _selected_grade_id)
        {
            if(auto currentNameTxt = utils::findChild<ui::Text *>(this, "name_current"))
            {
                currentNameTxt->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
                
                currentNameTxt->setString(ms::GradeManager::getInstance().getNameByLanguage(grade.name, LANGUAGE_MANAGER->getDisplayLangId()));
            }
        }
    }
    
    listview->setContentSize(Size(this->getContentSize().width, MIN(h - listview->getItemsMargin() + listview->getTopPadding() + listview->getBottomPadding(), Director::getInstance()->getVisibleSize().height - 70)));
    
    listview->setTouchEnabled(listview->getContentSize().height == Director::getInstance()->getVisibleSize().height - 70);

    for (auto c : listview->getItems())
    {
        c->setVisible(false);
    }
    
    listview->setVisible(false);
    
    auto background = utils::findChild(this, "background");
    background->setColor(Color3B::WHITE);
}

ui::Widget::ccWidgetClickCallback MSGradeBox::onLocateClickCallback(const std::string & callback_name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(MSGradeBox, onSelect),
        CLICK_MAP(MSGradeBox, onSelectedGrade),
        CLICK_MAP(MSGradeBox, onOverlay)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSGradeBox::onSelect(Ref * sender)
{
    mj::helper::playButtonFX();

    if (_is_animating)
    {
        return;
    }
    
    _is_showing_grades ? hideGrades() : showGrades();
}

void MSGradeBox::showGrades()
{
    if (_is_showing_grades)
    {
        return;
    }
    
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_gradebox_show);
    
    utils::findChild(this, "overlay")->setVisible(true);
    utils::findChild(this, "listview")->setVisible(true);
    
    auto name_current = utils::findChild<ui::Text *>(this, "name_current");
//    name_current->runAction(TintTo::create(0.5, Color3B::BLACK));
    
    if(name_current)
    {
       name_current->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    }
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    listview->setItemsMargin(15);
    listview->doLayout();
    
    for (auto c : listview->getItems())
    {
        if (c->getTag() == _selected_grade_id)
        {
//            c->getChildByName<ui::Text *>("name")->setTextColor(Color4B::YELLOW);
            int color = 0x5CC203;
            
            if(CONFIG_MANAGER->isAppVNStories())
            {
                color = 7041530;
            }
            
            c->setColor(mj::helper::colorFromHex(color));
        }
    }
    
    _is_showing_grades = true;
    
    auto background = utils::findChild(this, "background");
    _is_animating = true;
    auto start_value = this->getContentSize().height;
    auto stop_value = start_value + listview->getContentSize().height + 10;
    
//    auto list_height = MIN(_grades_info.size() * _model_size.height + 30, Director::getInstance()->getVisibleSize().height - 100);
    
//    background->runAction(TintTo::create(0.5, Color3B::WHITE));
//    listview->setContentSize(Size(background->getContentSize().width, list_height));
    background->runAction(Sequence::create(EaseExponentialOut::create(ResizeTo::create(0.5, Size(background->getContentSize().width, stop_value))), CallFunc::create([=]{
        _is_animating = false;
    }), nullptr));
    
    auto overlay = utils::findChild(this, "overlay");
    overlay->setOpacity(0);
    overlay->runAction(FadeTo::create(.5f, 225));
    
    // hieunt: sửa nhanh - LỖI: ở một số màn có tai thỏ android, iphonex thì mask bị khuyết một khoảng nhỏ ở bên trái
    overlay->setContentSize(overlay->getContentSize()+Size(200, 200));
    overlay->setPosition(overlay->getPosition()-Point(100, 100));
    
    auto items = listview->getItems();
    for (auto i = 0; i < items.size(); ++i)
    {
        auto item = items.at(i);
        item->setVisible(true);
        item->setOpacity(0.9);
        item->runAction(Sequence::create(DelayTime::create(i * .025f), Spawn::create(FadeIn::create(.2f), EaseBackOut::create(ScaleTo::create(.2f, 1)), nullptr), NULL));
    }
    mj::helper::setGlobalZorderForNode(this, 1);
    _delegate ? _delegate->onSelectGradeBox() : (void) nullptr;
}

void MSGradeBox::hideGrades()
{
    if (!_is_showing_grades)
    {
        return;
    }
    
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_gradebox_hide);
    
    auto name_current = utils::findChild<ui::Text *>(this, "name_current");
    
    if(name_current)
    {
       name_current->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    }
    
    _is_showing_grades = false;
    auto background = utils::findChild(this, "background");
    auto start_value = background->getContentSize().height;
    auto stop_value = this->getContentSize().height;
    _is_animating = true;
    
    auto overlay = utils::findChild(this, "overlay");
    overlay->runAction(Sequence::create(DelayTime::create(0.2), FadeTo::create(.25f, 0), nullptr));
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    
//    background->runAction(TintTo::create(0.5, Color3B(38, 176, 230)));
    background->runAction(Sequence::create(DelayTime::create(0.2), EaseCircleActionOut::create(ResizeTo::create(0.5, Size(background->getContentSize().width, stop_value))), CallFunc::create([=]{
        _is_animating = false;
        listview->jumpToTop();
        utils::findChild(this, "overlay")->setVisible(false);
        utils::findChild(this, "listview")->setVisible(false);
        mj::helper::setGlobalZorderForNode(this, 0);
    }), nullptr));
    
    
    auto items = listview->getItems();
    
    for (auto i = 0; i < items.size(); ++i)
    {
        auto item = items.at(items.size() - i - 1);
        item->setVisible(true);
        item->runAction(Sequence::create(DelayTime::create(i * .025f), Spawn::create(FadeOut::create(.2f), EaseBackOut::create(ScaleTo::create(.2f, 0.5f)), nullptr), CallFunc::create([item] {
            
            //color
            int color = 0x418109;
            if(CONFIG_MANAGER->isAppVNStories()){
                color = 5067726;
            }
            
            item->setVisible(false);
            
            if (item->getName() != "header") {
                item->getChildByName<ui::Text *>("name")->setTextColor(Color4B::WHITE);
                item->setColor(mj::helper::colorFromHex(color));
            }
        }), NULL));
    }
    _delegate ? _delegate->onDeselectGradeBox(_selected_grade_id) : (void) nullptr;
}

bool MSGradeBox::forceSelectGrade(int grade_id)
{
    if (grade_id != _selected_grade_id)
    {
        if (auto listview = utils::findChild<ui::ListView *>(this, "listview"))
        {
            for (auto item : listview->getChildren())
            {
                if (item->getTag() == grade_id)
                {
                    this->onSelectedGrade(item);
                    return true;
                }
            }
        }
    }
    
    return false;
}

void MSGradeBox::onSelectedGrade(Ref * sender)
{
    if (_is_animating)
    {
        return;
    }
    
    ms::EventLogger::getInstance().logEvent("learn_grade", {});
    mj::helper::playButtonFX();
    
    auto grade_id = reinterpret_cast<ui::Button *>(sender)->getTag();
    _gradeId = grade_id;

    MJDEFAULT->setIntegerForKey("key_grade_phonic", _gradeId);
    
    ms::GradeManager::getInstance().setCurrentGradeId(grade_id);

    //ex: 1|1
    std::string customData = std::to_string(grade_id)+"|"+std::to_string(static_cast<int>(m_scope));
    auto e = EventCustom(key_event_grade_changed);
    e.setUserData(&customData);
    
    // TODO: CleverTap-pushEvent: "choose_level"
    try
    {
        std::string screen_name;
        switch (m_scope) {
            case ms::GradeManager::Scope::STORIES:      screen_name = "Stories"; break;
            case ms::GradeManager::Scope::LESSONS:      screen_name = "Lessons"; break;
            case ms::GradeManager::Scope::LESSONS_PHONIC:screen_name = "Phonics"; break;
            case ms::GradeManager::Scope::AUDIOBOOKS:   screen_name = "Audiobooks"; break;
            default: break;
        }
        if (_delegate && _delegate->isGradeBoxFromMaterialScreen()) {
            if (m_scope == ms::GradeManager::Scope::STORIES) {
                screen_name = "ws_story";
            } else if (m_scope == ms::GradeManager::Scope::LESSONS_PHONIC) {
                screen_name = "ws_phonics";
            } else {
                screen_name = "ws_problem_need_debug"; // rơi vào case này thì sai, debug nhé
            }
        }
        
        std::string grade_name;
        for (auto grade : _grades_info)
        {
            if (grade_id == grade.grade_id)
            {
                if (CONFIG_MANAGER->isVietnameseStories())
                {
                    if (grade.name.compare("Tất cả cấp độ") == 0 || grade.name.compare("All Grades") == 0)
                    {
                        grade_name = "id:" + std::to_string(grade_id) + " - " + "Tất cả cấp độ";
                    }
                    else
                    {
                       grade_name = "id:" + std::to_string(grade_id) + " - " + ms::GradeManager::getInstance().getNameByLanguage(grade.name, LANGUAGE_VN_CODE);
                    }
                }
                else
                {
                   grade_name = "id:" + std::to_string(grade_id) + " - " + ms::GradeManager::getInstance().getNameByLanguage(grade.name, LANGUAGE_EN_CODE);
                }
                
                break;
            }
        }
        
        CleverTapManager::GetInstance()->pushEvent("choose_level",
        {
            {"screen_name", cocos2d::Value(screen_name)},
            {"level", cocos2d::Value(grade_name)},
        });
    }
    catch (const std::exception&)
    {
        
    }

    ms::EventLogger::getInstance().logEvent("on_select_grade_dispatch",    {
        {"custom_data", cocos2d::Value(customData)},
    });
    
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    
    hideGrades();
    
}

void MSGradeBox::onOverlay(Ref * sender)
{
    if (_is_animating)
    {
        return;
    }
    
    hideGrades();
}

void MSGradeBox::onReceivedEventGradeChanged(int grade_id)
{
    ms::GradeManager::getInstance().setScope(m_scope);
    auto info = ms::GradeManager::getInstance().getGradeInfoById(grade_id);
    _selected_grade_id = info.grade_id;
    
    if(auto nameCurrent =utils::findChild<ui::Text *>(this, "name_current"))
    {
        nameCurrent->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
        nameCurrent->setString(ms::GradeManager::getInstance().getNameByLanguage(info.name, LANGUAGE_MANAGER->getDisplayLangId()));
    }
    
}

void MSGradeBox::setEnableInteraction(bool is_enable)
{
//    auto button = utils::findChild<ui::Button *>(this, "button");
//    if (button) {
//        button->setEnabled(is_enable);
//    }
}

std::string MSGradeBox::getGradeName(int gradeId)
{
    if (_grades_info.size() == 0) return "";

    for (int i = 0; i < _grades_info.size(); i++)
    {
        if (gradeId == _grades_info[i].grade_id)
        {
            return _grades_info[i].name;
        }
    }
}


