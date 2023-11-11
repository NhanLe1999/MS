
#include "Robot_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"

INITIALIZE_READER(Robot_Item);

#define CSB_NAME "csb/game/Robot/Robot_Item.csb"
//#define img_path "games/robot/"

Robot_Item * Robot_Item::createItem(Robot_Item::Phonic phonic, std::string img_path) {
    auto p = static_cast<Robot_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(phonic, img_path);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void Robot_Item::didLoadFromCSB(Robot_Item::Phonic phonic, std::string img_path)
{
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
    Layout* item_layout = utils::findChild<Layout*>(this, "it_layout");
	this->_phonic = phonic;
    item_layout->setTouchEnabled(true);
    item_layout->addTouchEventListener(CC_CALLBACK_2(Robot_Item::onClickItem, this));
   
    //create random image
    ImageView* background = ImageView::create();
    background->loadTexture(img_path);
    item_layout->addChild(background);
    background->setPosition(item_layout->getContentSize()/2);
    background->setContentSize(Size(200,200));
    background->ignoreContentAdaptWithSize(false);
    
    //create text
    Text* text = Text::create();
    text->setString(phonic.text);
    text->setContentSize(Director::getInstance()->getWinSize() );
    text->setFontName(font_name);
	text->setFontSize(font_sz);
    text->setTextColor(Color4B::BLACK);
    text->enableOutline(Color4B::WHITE, 4);
    text->setName("text_");
    item_layout->addChild(text);
    text->setPosition(Vec2( item_layout->getContentSize().width*0.5, item_layout->getContentSize().height*0.5));
//    //TODO: fix text font size
//    while (text->getVirtualRendererSize().width > getContentSize().width * 0.9) {
//        auto font_size = text->getFontSize();
//        text->setFontSize(font_size - 2);
//        CCLOG("Scale down font: %f", font_size);
//    }
    
    Text* text_audio = Text::create();
    text_audio->setString(phonic.audio);
    text_audio->setFontName(font_name);
    text_audio->setFontSize(font_sz);
    text_audio->setTextColor(Color4B::BLACK);
    text_audio->setOpacity(0);
    text_audio->setName("text_audio");
    item_layout->addChild(text_audio);
    
}

void Robot_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void Robot_Item::onClickItem(cocos2d::Ref *sender, ui::Widget::TouchEventType type){
    if(_delegate){
        auto item = static_cast<Robot_Item*>(sender);
        _delegate->onClickedDelegate(item, type, _phonic.pair, _phonic.audio);
    }
}

void Robot_Item::setDelegate(Robot_ItemDelegate *delegate){
    _delegate = delegate;
}

void Robot_Item::setHint(bool is_hint){
    _hint = is_hint;
}

bool Robot_Item::getHint(){
    return _hint;
}

void Robot_Item::setAudio(std::string audio){
    _audio = audio;
}

std::string Robot_Item::getAudio(){
    return _audio;
}

void Robot_Item::playEffect(bool correct){
    
    if(correct){
        //right
        
    }else{
        //wrong
        
    }
}

void Robot_Item::setFontSize(int size){
    utils::findChild<Text*>(this, "text_")->setFontSize(size);
}
