//
//  MSEventPopup.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 9/13/18.
//

#include "MSEventPopup.h"

#define key_event_information "key.event.information"

MSEventPopup* MSEventPopup::createEventPopup(EventInfo info){
    auto p = MSEventPopup::create();
    if(p){
        p->setupIcon(info);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}
void MSEventPopup::setupIcon(EventInfo info){
    screen_size = Director::getInstance()->getWinSize();

    this->setContentSize(Size(150, 150));
//    this->setBackGroundColorType(BackGroundColorType::SOLID);
//    this->setBackGroundColor(Color3B::BLUE);
//    this->setBackGroundColorOpacity(100);
    this->setPosition(Vec2(screen_size.width*0.85, screen_size.height*0.1));
    this->setTouchEnabled(true);
    this->addClickEventListener(CC_CALLBACK_0(MSEventPopup::onIconClicked, this));
    this->setName("icon");
//    this->setOpacity(0);
    
    _info = info;

    if(!_info.is_image){
        //create GAF icon
        auto icon = GAFWrapper::createGAF(_info.icon_path);
//        auto icon = GAFWrapper::createGAF(FileUtils::getInstance()->getWritablePath()+ "event/gaf/airplane/airplane.gaf");
        icon->setOpacity(0);
        addChild(icon);
        icon->setPosition(this->getContentSize()/2);
        icon->play(true);
        icon->setName("icon_img");
       
    }else{
        //create image icon
        auto icon = ui::ImageView::create(_info.icon_path);
        icon->setOpacity(0);
        addChild(icon);
        icon->setPosition(this->getContentSize()/2);
         icon->setName("icon_img");
        
    }
  
    utils::findChild(this, "icon_img")->runAction(FadeIn::create(1));
    
}
bool MSEventPopup::show(){
    CCLOG("load data");
    //    auto str = MJDEFAULT->getStringForKey(key_event_information, "");
//    auto file = cocos2d::FileUtils::getInstance();
    auto str = cocos2d::FileUtils::getInstance()->getStringFromFile("event.json");
    rapidjson::Document doc;
    doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(str.c_str());
    
    if (doc.HasParseError()) {
        return false;
    }
    
    EventInfo info;
    auto data = doc["event_popup"].GetObject();
    
    std::string icon_path = data["icon_path"].GetString();
    std::size_t pos = icon_path.find(".");
    std::string path_tail =  icon_path.substr(pos);
    if(path_tail == ".zip"){
        info.is_image = false;
        //unzip
        FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "event/gaf/");
         auto writable_path = FileUtils::getInstance()->getWritablePath()+ "event/gaf/";
        mj::helper::unzip(icon_path, writable_path);
        info.icon_path = writable_path + icon_path.substr(0, pos) + ".gaf";
  
    }else{
        info.is_image = true;
        info.icon_path = icon_path;
    }
    
    info.content_path = data["content_path"].GetString();
    info.start_date = (int)(data["start_date"].GetDouble()/1000);
    info.end_date = (int)(data["end_date"].GetDouble()/1000);
    info.url = data["url"].GetString();
    
    auto rect = data["button"].GetArray();
    info.button_rect.origin.x = rect[0].GetInt();
    info.button_rect.origin.y = rect[1].GetInt();
    info.button_rect.size.width = rect[2].GetInt();
    info.button_rect.size.height = rect[3].GetInt();
    
    
    auto time_now = int(utils::getTimeInMilliseconds() / 1000);
    if(time_now < info.start_date || time_now > info.end_date){
        return false;
    }
    
    auto p = createEventPopup(info);
    p->setName("popup_icon");
    Director::getInstance()->getRunningScene()->addChild(p, std::numeric_limits<int>::max() - 1);
  
    return true;
    
}

void MSEventPopup::onEnter(){
    cocos2d::ui::Layout::onEnter();
    CCLOG("%s", this->getName().c_str());
    
}

void MSEventPopup::updateEventInfo(std::string str) {
    MJDEFAULT->setStringForKey(key_event_information, str);
}

void MSEventPopup::onExit(){
    ui::Layout::onExit();
}

void MSEventPopup::onExitTransitionDidStar(){
    ui::Layout::onExitTransitionDidStart();
}

void MSEventPopup::onIconClicked(){
    
    this->setTouchEnabled(false);
    this->setVisible(false);
    
    CCLOG("on cliked");
    trans_layout = Layout::create();
    trans_layout->setContentSize(screen_size);
    trans_layout->setBackGroundColor(Color3B::BLACK);
    trans_layout->setBackGroundColorType(BackGroundColorType::SOLID);
    trans_layout->setOpacity(200);
    Director::getInstance()->getRunningScene()->addChild(trans_layout, 9999);
    trans_layout->setPosition(Vec2(0,0));
    
    auto content_img = ui::ImageView::create(_info.content_path);
    content_img->setPosition(trans_layout->getContentSize()/2 );
    trans_layout->addChild(content_img, 99999);
    
    auto button = Layout::create();
    button->setContentSize(_info.button_rect.size);
    button->setPosition(Vec2(_info.button_rect.origin.x, _info.button_rect.origin.y));
    button->setBackGroundColor(Color3B::RED);
    button->setBackGroundColorType(BackGroundColorType::SOLID);
    button->setBackGroundColorOpacity(200);
    trans_layout->addChild(button);
    button->setTouchEnabled(true);
    button->addClickEventListener(CC_CALLBACK_0(MSEventPopup::onEventClicked, this));
    
    auto close_btn = Layout::create();
    close_btn->setContentSize(_info.button_rect.size);
    auto img_sz = content_img->getContentSize();
    close_btn->setPosition(Vec2(content_img->getPositionX() + img_sz.width/2, content_img->getPositionY() + img_sz.height/2));
    close_btn->setBackGroundColor(Color3B::GREEN);
    close_btn->setBackGroundColorType(BackGroundColorType::SOLID);
    close_btn->setBackGroundColorOpacity(200);
    trans_layout->addChild(close_btn);
    close_btn->setTouchEnabled(true);
    close_btn->addClickEventListener(CC_CALLBACK_0(MSEventPopup::onCloseClicked, this));
}

void MSEventPopup::onCloseClicked(){
    trans_layout->removeFromParent();
    this->setTouchEnabled(true);
    this->setVisible(true);
}

void MSEventPopup::onEventClicked(){
    CCLOG("directing to: %s", _info.url.c_str());
    Application::getInstance()->openURL(_info.url);
}

