#include "TimTuCungVanLoai_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"
#include "MJHelper.h"
#include "PhonicConfig.h"
INITIALIZE_READER(TimTuCungVanLoai_Item);

#define CSB_NAME "csb/game/TimTuCungVanLoai/TimTuCungVanLoai_Item.csb"
//#define img_path "games/TimTuCungVanLoai/"

TimTuCungVanLoai_Item * TimTuCungVanLoai_Item::createItem(Answer ans, std::string path, bool rotate) {

	auto p = static_cast<TimTuCungVanLoai_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(ans,path,rotate);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void TimTuCungVanLoai_Item::didLoadFromCSB(Answer ans, std::string path, bool rotate)
{
	auto image_answer = utils::findChild<ImageView*>(this, "answer");
	_ans = ans;
	_audio = _ans.audio;
	item_layout = utils::findChild<Layout*>(this, "it_layout");
	item_layout->setTouchEnabled(true);
	item_layout->addClickEventListener([=](cocos2d::Ref * sender) {
		if (_delegate) {
			_delegate->onClickedBox(this);
		}
	});
	//text = utils::findChild<Text*>(this, "text_answer");
	//text->setText(ans.text);
	//text->removeFromParent();
	//gaf
	gaf_fish = GAFWrapper::createGAF(path);
	this->addChild(gaf_fish);
	gaf_fish->setAnchorPoint(Vec2(0.5, 0.5));
	gaf_fish->setPosition(this->getContentSize()/2);
	//
    font_name = "fonts/danhvan.ttf";
    auto s = StringUtils::StringUTF8(ans.text);
    auto font_size = (s.length() <= 4) ? 45 : 35;
    if (!ans.co_dau){
        text = Text::create(ans.text,font_name,font_size);
        this->addChild(text);
        text->setPosition(gaf_fish->getPosition());
    }else{
        text = Text::create(ans.phonics.front(),font_name,font_size);
        this->addChild(text);
        text->setPosition(gaf_fish->getPosition());
        auto dau = Text::create(ans.phonics.back(),font_name,font_size);
        text->addChild(dau);
        auto pos = text->getLetter(ans.pos_dau)->getPosition();
        auto add_pos = Vec2(0,7);
        if (ans.phonics.back() == "."){
            add_pos = Vec2(0,-7);
        }
        if (ans.phonics.back() == "?"){
            add_pos = Vec2(0,13);
        }
        if (ans.phonics.back() == "´"){
            add_pos = Vec2(2,5);
        }
        dau->setPosition(pos+add_pos);
    }
}

void TimTuCungVanLoai_Item::onEnter() {
	cocos2d::ui::Layout::onEnter();

}

void TimTuCungVanLoai_Item::showItem() {
	this->setScale(0);
	this->runAction(EaseIn::create(ScaleTo::create(0.5, 1.f), 1.5));
	
	//this->playAudio();
}



void TimTuCungVanLoai_Item::setTouchEnable(bool enable) {
	item_layout->setTouchEnabled(enable);
}

void TimTuCungVanLoai_Item::setDelegate(TimTuCungVanLoai_ItemDelegate *delegate) {
	_delegate = delegate;
}



void TimTuCungVanLoai_Item::setAudio(std::string audio) {
	_audio = audio;
}


void TimTuCungVanLoai_Item::playAudio() {
	AUDIO_ENGINE->playEffect(_audio);
}

std::string TimTuCungVanLoai_Item::getAudio() {
	return _audio;
}
void TimTuCungVanLoai_Item::effectCorrect(std::string result, bool is_phonic) {
	this->playAudio();
	/*AUDIO_ENGINE->playSound("sounds/TimTuCungVanLoai/Correct.mp3", false, [=]() {
		
	});*/

    result = mj::helper::to_no_accents_v2(result);
    result.erase(0,result.find_first_not_of("\"")); // trim \" \"
    result.erase(result.find_last_not_of("\"") + 1);
	AUDIO_ENGINE->playSound("sounds/TimTuCungVanLoai/Correct2.mp3", false, nullptr, 0.4f);
	gaf_fish->playSequence("correct", true);

    auto ans_text = mj::helper::to_no_accents_v2(_ans.text);
	if (is_phonic) {
        int found = -1;
        while(1){
            found = ans_text.find(result,found+1);
            if(found != std::string::npos)
            {
                for (int i = found; i < found + result.size(); i++) {
                    auto letter =text->getLetter(i);
                    if(letter)
                    {
                       letter->runAction(Repeat::create(Sequence::create(
                           DelayTime::create(0.25f),CallFunc::create([letter]{
                               letter->setColor(Color3B::RED);
                       }), DelayTime::create(0.25f),CallFunc::create([letter]{
                               letter->setColor(Color3B::WHITE);
                       }), NULL),100000));
                    }
                }
            }else{
                break;
            }
        }
		
	}
}
void TimTuCungVanLoai_Item::effectWrong(std::string result, bool is_phonic) {
	AUDIO_ENGINE->playEffect("sounds/TimTuCungVanLoai/wrong.mp3");
    this->playAudio();
	gaf_fish->playSequence("wrong", false);
	if (!is_phonic) {
		std::size_t found = _ans.text.find(result);
		for (int i = found; i < found + result.size(); i++) {
			text->getLetter(i)->runAction(Sequence::create(CallFunc::create([=] {
				text->getLetter(i)->setColor(Color3B::BLACK);
			}), ScaleTo::create(0.3f, 1, 1.2f), ScaleTo::create(0.3f, 1, 1), CallFunc::create([=] {
				text->getLetter(i)->setColor(Color3B::WHITE);
			}), NULL));
		}
	}
}
