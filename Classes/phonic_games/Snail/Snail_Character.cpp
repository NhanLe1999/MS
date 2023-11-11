//
//  MJ_Snail_Character.h
//

#include "Snail_Character.h"
#include "HsAudioEngine.h"
#include "PhonicConfig.h"
INITIALIZE_READER(Snail_Character);
#define CSB_NAME "csb/game/snail/Snail_Character.csb"

using namespace cocos2d;
Snail_Character * Snail_Character::createItem(ITEM_INFO item_info, int type, bool resize) {
    auto p = static_cast<Snail_Character *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info, type, resize);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void Snail_Character::didLoadFromCSB(ITEM_INFO item_info, int type, bool resize){
    this->item_info = item_info;
    if (type == TYPE_BEGIN) {
        cocos2d::utils::findChild(this, "begin")->setVisible(true);
        cocos2d::utils::findChild(this, "back")->setVisible(true);
    }else if (type == TYPE_MIDLE) {
        cocos2d::utils::findChild(this, "mid")->setVisible(true);
        cocos2d::utils::findChild(this, "back")->setVisible(true);
    }else {
        cocos2d::utils::findChild(this, "mid")->setVisible(true);
        cocos2d::utils::findChild(this, "end")->setVisible(true);
    }
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "text")->setString(item_info.character);
	mText = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "text");
	//// fit value text size
	//float width = this->getContentSize().width;
	//float label_width = mText->getContentSize().width;
	//float fSize = mText->getFontSize();
	//while (width < label_width) {
	//	fSize -= 2;
	//	mText->setFontSize(fSize);
	//	label_width = mText->getContentSize().width;
	//}
    if(resize){
        this->setContentSize(this->getContentSize()*0.9);
        for(auto c: this->getChildren()){
            c->setContentSize(Size(c->getContentSize().width*0.9, c->getContentSize().height*0.9));
        }
    }
}

void Snail_Character::onEnter(){
    cocos2d::ui::Layout::onEnter();
}
void Snail_Character::setDelegate(Snail_CharacterDelegate *delegate){
    this->delegate = delegate;
}

void Snail_Character::playCharacter(){
    auto highlight = cocos2d::utils::findChild(this, "highlight");
    highlight->setOpacity(0);
    highlight->setVisible(true);
    highlight->runAction(FadeIn::create(0.2f));
	auto text = mText;//cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "text");
    text->setTextColor(Color4B::RED);
    AUDIO_ENGINE->play(item_info.audio_path);
    text->runAction(Sequence::create(ScaleTo::create(0.2f, 1.3), EaseElasticOut::create(ScaleTo::create(0.5f, 1)), CallFunc::create([=]{
        text->setTextColor(Color4B::BLACK);
        highlight->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create([=]{
            if (delegate) {
                delegate->onCharacterPlayDone();
            }
        }), NULL));
    }), NULL));
}

void Snail_Character::playText(float duration){
//    auto highlight = cocos2d::utils::findChild(this, "highlight");
//    highlight->setOpacity(255);
//    highlight->setVisible(true);
	auto text = mText;//cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "text");
    text->setTextColor(Color4B::RED);
    this->runAction(Sequence::create(DelayTime::create(duration), CallFunc::create([=]{
        text->setTextColor(Color4B::BLACK);
//        highlight->setVisible(false);
    }), NULL));
}

