
#include "GameMakeAMatch_Item.h"
#include "HSAudioEngine.h"
INITIALIZE_READER(MJGameMakeAMatch_Item);

USING_NS_CC;

#define CSB_NAME "csb/game/makeamatch/GameMakeAMatch_Item.csb"

MJGameMakeAMatch_Item * MJGameMakeAMatch_Item::createFrame(mj::GameObject object, int type) {
    auto p = static_cast<MJGameMakeAMatch_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(object, type);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameMakeAMatch_Item::didLoadFromCSB(mj::GameObject object, int type) {
    _data = object;
    _type = type;
    box1 = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "box1"));
    box2 = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, "box2"));
    box1->setScaleX(0);
    
    auto typelayout = _type;
    if (typelayout == Merge_TextText_Type) {
        typelayout = Text_Type;
    }
    auto namelayout = StringUtils::toString(typelayout);
    layout_type = static_cast<ui::Layout *>(ui::Helper::seekWidgetByName(this, namelayout));
    layout_type->setVisible(true);
    switch (type) {
        case Text_Type: case Merge_TextText_Type:
        {
            auto text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(layout_type, "text"));
            text->setVisible(false);
            if (!label) {
                label = Text::create("", "fonts/KGWHATTHETEACHERWANTS.TTF", 40);
            }
            if(!layout_type->getChildByName("label"))
                layout_type->addChild(label);
            label->setName("label");
            label->setString(_data.text);
            label->setTextColor(Color4B(getColor()));
            label->setPosition(this->getContentSize()/2);
            auto width = label->getContentSize().width;
            if (width<100) {//chữ bé -> zoom to ra.
                label->setFontName("fonts/KGWHATTHETEACHERWANTS.TTF");
                label->setFontSize(35);
            }
            label->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            label->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
            
        }
            break;
        case Img_Type:
            layout_type->addChild(getImgWith(_data.image_path, type));
            break;
        case Merge_ImgText_Type:
        {
            layout_type->addChild(getImgWith(_data.image_path, type));
            text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(layout_type, "text"));
            text->setString(_data.text);
            text->setTextColor(Color4B(getColor()));
            
            auto width = text->getContentSize().width;
            if (width>150.0f) {//dài hơn box hiển thị.
                text->setScale(150.0f/width);
                text->setPositionY(text->getPositionY()+5);
            }
        }
            break;
        default:
            break;
    }
    
    
    if (type != Merge_ImgText_Type && type != Merge_TextText_Type) {//Kiểu item merge không cần flip.
        this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]{
            box1->setVisible(true);
            flipCard(box2, box1);//flip lại để ẩn content.
        }), NULL));
    }
}

#include "MSGradeManager.h"
ImageView* MJGameMakeAMatch_Item::getImgWith(string path_img, int type){//create ICON
    auto img = ImageView::create(path_img);
	if (type == Img_Type) {
		img->setPosition(this->getContentSize() / 2);
	}
	else if (type == Merge_ImgText_Type) {
		img->setPosition(Size(this->getContentSize().width / 2, 118));
	}

	// fix scale phonic lesson
	bool is_phonic_lesson = ms::GradeManager::getInstance().getScope() == ms::GradeManager::LESSONS_PHONIC;
	if (is_phonic_lesson) {
		if (type == Img_Type) {
			img->setScale(150.0f/250);
		}
		else if (type == Merge_ImgText_Type) {
			img->setScale(120.0f / 250);
		}
	}
	else {
		img->setScale(120.f / 225);
	}
    
    return img;
}


void MJGameMakeAMatch_Item::flipCard(Node *card1, Node *card2){
    AUDIO_ENGINE->playEffect("sounds/makeamatch/makeamatch_latbai.mp3");
    isRunAni = true;
    auto duration = 0.25f;
    card1->runAction(Sequence::create(ScaleTo::create(duration, 0, 1.0f), CallFunc::create([=]{
        card2->runAction(Sequence::create(ScaleTo::create(duration, 1.0f, 1.0f), CallFunc::create([=]{
            isRunAni = false;
        }), NULL));
    }),NULL));
}

Color3B MJGameMakeAMatch_Item::getColor(){
    return Color3B::BLACK;
}

void MJGameMakeAMatch_Item::setOnResults(){
    static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "box_bg1"))->setVisible(true);
    static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "box_bg2"))->setVisible(false);
}

std::string MJGameMakeAMatch_Item::getObjectName() {
    return _data.text;
}

void MJGameMakeAMatch_Item::setColorForLabel(Color3B color)
{
    if (label) {
        label->setTextColor(Color4B(color));
    }
    if (text) {
        text->setTextColor(Color4B(color));
    }
}

Text* MJGameMakeAMatch_Item::getText()
{
    if (label) {
        return label;
    }
    if (text) {
        return text;
    }
}

void MJGameMakeAMatch_Item::setFontSizeText(float fontSize) {
    if (label) {
        label->setFontSize(fontSize);
    }
    if (text) {
        text->setFontSize(fontSize);
    }
}

float MJGameMakeAMatch_Item::getFontSizeText() {
    if (label) {
        return label->getFontSize();
    }
    if (text) {
        return text->getFontSize();
    }
}

void MJGameMakeAMatch_Item::setDelegate(MJGameMakeAMatch_ItemDelegate * delegate) {
    this->delegate = delegate;
}

void MJGameMakeAMatch_Item::onFlip(Ref *sender){//Flip when click item.
    if (isRunAni) return;

    if (delegate) {
        delegate->onFlipCardWithName(this);//delegate
    }
}

ui::Widget::ccWidgetClickCallback MJGameMakeAMatch_Item::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJGameMakeAMatch_Item, onFlip),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}