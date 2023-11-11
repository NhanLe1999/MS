
#include "GameLoadTheTrain_Item.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"

INITIALIZE_READER(GameLoadTheTrain_Item);

#define CSB_NAME "csb/game/load_the_train/GameLoadTheTrain_Item.csb"

#define  POS1 Vec2(91.5,40)
#define  POS1 Vec2(91.5,155)
#define  POS1 Vec2(91.5,270)

#define ITEM_CONTENSIZE cocos2d::Size(183,310)

GameLoadTheTrain_Item * GameLoadTheTrain_Item::createItem(ItemInfo item_info) {
    auto p = static_cast<GameLoadTheTrain_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void GameLoadTheTrain_Item::didLoadFromCSB(ItemInfo item_info) {
	this->setTouchEnabled(true);
	_item_info = item_info;
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(ITEM_CONTENSIZE);
	
	for (int i = 0; i < 3; i++){
		std::string name = StringUtils::format("item%d", i + 1);
		ImageView* img = cocos2d::utils::findChild<ImageView*>(this, name);
		poss.push_back(img->getPosition());
		imgs.push_back(img);
		img->setTag(i);
	}

	aGL = imgs.at(0)->getVirtualRenderer()->getGLProgram();
}

void GameLoadTheTrain_Item::loadData(std::vector<std::string> data_)
{
	_item_info.texts = data_;
	colors.clear();
	for (int i = 0; i < 3; i++){
		imgs.at(i)->setPosition(poss.at(i));
		imgs.at(i)->setVisible(true);
		imgs.at(i)->setColor(Color3B::WHITE);
		auto tex = imgs.at(i)->getChildByName<Text*>("text");
        
        if(CONFIG_MANAGER->isGameVMonkey())
        {
            tex->setFontName(PHONIC_MANAGER->getGameFontName());
            auto str = StringUtils::StringUTF8(data_.at(i));
            tex->setFontSize(str.length() >= 5?40:50);
        }
        
		tex->setString(data_.at(i));
		int num = 0;
		do{
			num = rand() % 6 + 1;
		} while (std::find(colors.begin(), colors.end(), num) != colors.end());
		
		std::string name = StringUtils::format("games/load_the_train/%d.png", num);
		imgs.at(i)->loadTexture(name);
		colors.push_back(num);
		imgs.at(i)->getVirtualRenderer()->setGLProgram(aGL);
		imgs.at(i)->getVirtualRenderer()->getGLProgram()->use();
	}
}

void GameLoadTheTrain_Item::setTop(ImageView* img, int z)
{
	for (int i = 0; i < 3; i++){
		if (img == imgs.at(i)){
			img->setLocalZOrder(z);
		}
	}
	this->setLocalZOrder(z);
}

void GameLoadTheTrain_Item::highLightItem(std::string trueText)
{
	for (int i = 0; i < 3; i++) {
		if (_item_info.texts.at(i).compare(trueText)==0){
			imgs.at(i)->runAction(Sequence::create(
				RotateTo::create(0.1f, 5)
				, RotateTo::create(0.2f, -5)
				, RotateTo::create(0.2f, 5)
				, RotateTo::create(0.1f, 0)
				, nullptr));
		}
	}
}

void GameLoadTheTrain_Item::setDone(ImageView* img)
{
	for (int i = 0; i < 3; i++){
		if (img == imgs.at(i)){
			img->setVisible(false);
		}
		else{
			auto tex = imgs.at(i)->getChildByName<Text*>("text");
			tex->setString("");
			//imgs.at(i)->setColor(Color3B::GRAY);
			
			auto grayScaleShader = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, ccPositionTexture_GrayScale_frag);
			grayScaleShader->retain();

			grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
			grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
			grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);

			grayScaleShader->link();
			CHECK_GL_ERROR_DEBUG();

			grayScaleShader->updateUniforms();
			CHECK_GL_ERROR_DEBUG();

			imgs.at(i)->getVirtualRenderer()->setGLProgram(grayScaleShader);
			imgs.at(i)->getVirtualRenderer()->getGLProgram()->use();
		}
	}
}

cocos2d::Vec2 GameLoadTheTrain_Item::getPosItem(ImageView* img)
{
	for (int i = 0; i < 3; i++){
		if (img == imgs.at(i)){
			return poss.at(i);
		}
	}
	return Vec2(0, 0);
}

int GameLoadTheTrain_Item::getColor(ImageView* img)
{
	for (int i = 0; i < 3; i++){
		if (img == imgs.at(i)){
			return colors.at(i);
		}
	}
	return 1;
}

void GameLoadTheTrain_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}
