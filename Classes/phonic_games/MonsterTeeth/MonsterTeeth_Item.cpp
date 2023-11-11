//
//  MonsterTeeth_Item.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/10/18.
//

#include "MonsterTeeth_Item.h"
#include "MJHelper.h"
INITIALIZE_READER (MonsterTeeth_Item);

#define CSB_NAME "csb/game/MonsterTeeth/MonsterTeeth_Item.csb"
#define dirty_img_path "games/monsterteeth/teeth/vet_ban/"

MonsterTeeth_Item* MonsterTeeth_Item::createItem(std::string word, int index){
    auto p = static_cast<MonsterTeeth_Item*>(cocos2d::CSLoader::getInstance()->createNode(CSB_NAME));
    if(p){
        p->didLoadFromCSB(word, index);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MonsterTeeth_Item::didLoadFromCSB(std::string word, int index){
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
    auto item_layout = utils::findChild<Layout*>(this, "item");
    
    std::string img_path = dirty_img_path + cocos2d::StringUtils::toString(index) + ".png";
    ImageView* dirty_img = ImageView::create(img_path);
    item_layout->addChild(dirty_img);
    dirty_img->setPosition(item_layout->getPosition());
    dirty_img->setName("dirty");
    
    if(word.size() >= 3){
        dirty_img->setScale(1.3);
    }
    
	// random upper cas
	bool upper_case = cocos2d::random(0, 1);
	if (upper_case && word.size() == 1) {
		word = mj::helper::to_upper(word);
		//std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	}
	m_word = word;

    Text* text = Text::create();
    text->setString(word);
    text->setFontName(font_name);
    text->setFontSize(font_sz);
    text->enableOutline(Color4B::WHITE, 2);
    text->setTextColor(text_color[index-1]);
    item_layout->addChild(text);
    text->setPosition(item_layout->getPosition());
    text->setOpacity(0);
    text->setName("text");
}

void MonsterTeeth_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MonsterTeeth_Item::setClean(int clean){
    _clean = clean;
}

bool MonsterTeeth_Item::getClean(){
    return _clean;
}

std::string MonsterTeeth_Item::getWord()
{
	return m_word;
}

