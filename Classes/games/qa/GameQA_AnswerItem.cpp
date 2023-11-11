
#include "GameQA_AnswerItem.h"
#include "audio/include/AudioEngine.h"
INITIALIZE_READER(MJGameQA_AnswerItem);

USING_NS_CC;

MJGameQA_AnswerItem* MJGameQA_AnswerItem::createFrame(mj::GameObject object, int type) {
    auto p = static_cast<MJGameQA_AnswerItem*>(cocos2d::CSLoader::createNode("csb/game/qa/GameQA_AnswerItem.csb"));
    if (p) {
        p->didLoadFromCSB(object, type);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameQA_AnswerItem::didLoadFromCSB(mj::GameObject object, int type) {
    _data = object;
    _type = type;

    answer_lb = this->getChildByName<Text*>("answer_lb");
    thumb = this->getChildByName<ImageView*>("thumb");
    num_lb = thumb->getChildByName<Text*>("num_lb");
    auto image_speaker = this->getChildByName<ImageView*>("Image_speaker");
    if (image_speaker) {
        btspeaker = image_speaker->getChildByName<Button*>("btspeaker");
    }
    else {
        btspeaker = this->getChildByName<Button*>("btspeaker");
    }
    

    answer_lb->setString(object.text);
    answer_lb->setTextColor(Color4B::BLACK);
    auto win_size = Director::getInstance()->getWinSize();
    if (answer_lb->getContentSize().width > win_size.width * 0.8) {
        answer_lb->setTextAreaSize(cocos2d::Size(win_size.width * 0.8, 85));
        answer_lb->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        answer_lb->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    }


    num_lb->setString(getStrABCDWithIndex(type));


    auto btchoose = thumb->getChildByName<Button*>("btchoose");
    btchoose->setContentSize(cocos2d::Size(btchoose->getContentSize().width + answer_lb->getContentSize().width, btchoose->getContentSize().height));

    thumb->setScale(0);
    answer_lb->setScaleX(0);
    btspeaker->setScale(0);
}

void MJGameQA_AnswerItem::setChooseAnswer() {
    answer_lb->setTextColor(Color4B::ORANGE);
    thumb->loadTexture("games/qa/qa_answer2.png");
    num_lb->setTextColor(Color4B::WHITE);
}

void MJGameQA_AnswerItem::setUnChooseAnswer() {
    thumb->stopAllActions();
    thumb->loadTexture("games/qa/qa_answer1.png");
    num_lb->setTextColor(Color4B(241, 87, 35, 255));

    answer_lb->setTextColor(Color4B::BLACK);
}

void MJGameQA_AnswerItem::setRedColorAnswer(bool enable)
{
    if (!enable) return;
    thumb->stopAllActions();
    if (cocos2d::FileUtils::getInstance()->isFileExist("games/qa/qa_answer2_red.png")) {
        thumb->loadTexture("games/qa/qa_answer2_red.png");
    }
    answer_lb->setTextColor(Color4B(255, 0, 0, 255));
}

void MJGameQA_AnswerItem::setBlueColorAnswer()
{
    if (cocos2d::FileUtils::getInstance()->isFileExist("games/qa/qa_answer2_green.png")) {
        thumb->loadTexture("games/qa/qa_answer2_green.png");
    }
}


void MJGameQA_AnswerItem::setHighlight() {
    answer_lb->setTextColor(Color4B::RED);
    thumb->runAction(Sequence::create(ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f), NULL));
}

void MJGameQA_AnswerItem::onShow() {
    thumb->setScale(0);
    answer_lb->setScaleX(0);
    btspeaker->setScale(0);
    btspeaker->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    thumb->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    answer_lb->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
}

void MJGameQA_AnswerItem::disableTouch()
{
    answer_lb->setTouchEnabled(false);
    thumb->setTouchEnabled(false);
    num_lb->setTouchEnabled(false);
    thumb->getChildByName<Button*>("btchoose")->setTouchEnabled(false);
    auto Image_speaker = this->getChildByName<ImageView*>("Image_speaker");
    Image_speaker->setTouchEnabled(false);
    Image_speaker->getChildByName<Button*>("btspeaker")->setTouchEnabled(false);
}

std::string MJGameQA_AnswerItem::getStrABCDWithIndex(int index) {
    switch (index) {
    case 0:
        return "A";
        break;
    case 1:
        return "B";
        break;
    case 2:
        return "C";
        break;
    case 3:
        return "D";
        break;
    default:
        return "E";
        break;
    }
}


ImageView* MJGameQA_AnswerItem::getImgWith(string path_img, int type) {//create ICON
    auto img = ImageView::create(path_img);
    if (type == 2) {
        img->setPosition(this->getContentSize() / 2);

    }
    else if (type == 3) {
        img->setPosition(Size(this->getContentSize().width / 2, 118));
    }
    return img;
}

std::string MJGameQA_AnswerItem::getObjectName() {
    return _data.text;
}

void MJGameQA_AnswerItem::setDelegate(MJGameQA_AnswerItemDelegate* delegate) {
    this->delegate = delegate;
}

void MJGameQA_AnswerItem::onChoose(Ref* sender) {//delegate when click item.
    if (delegate) {
        isChooseItem = true;
        delegate->onChooseItem(this);
    }
}

void MJGameQA_AnswerItem::onClickSpeaker(Ref* sender) {
    if (delegate) {
        delegate->onItemClickSpeaker(true);
    }
    cocos2d::experimental::AudioEngine::stopAll();
    auto saved_color = answer_lb->getTextColor();
    setHighlight();
    auto path_audio = _data.audio_path;
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(path_audio);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
        CCLOG("AUDIO PLAY DONE");
        answer_lb->setTextColor(saved_color);
        if (delegate) {
            delegate->onItemClickSpeaker(false);
        }
        });
    this->runAction(Sequence::create(DelayTime::create(5), CallFunc::create([=] {
        if (cocos2d::experimental::AudioEngine::getState(audio_id) == cocos2d::experimental::AudioEngine::AudioState::ERROR) {
            answer_lb->setTextColor(Color4B::BLACK);
        }
        }), nullptr));
}

ui::Widget::ccWidgetClickCallback MJGameQA_AnswerItem::onLocateClickCallback(const std::string& callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJGameQA_AnswerItem, onChoose),
        CLICK_MAP(MJGameQA_AnswerItem, onClickSpeaker),
    };

    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}


