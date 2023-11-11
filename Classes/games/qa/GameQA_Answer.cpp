
#include "GameQA_Answer.h"
#include "GAFWrapper.h"
#include "AudioEngine.h"

INITIALIZE_READER(MJGameQA_Answer);

USING_NS_CC;

MJGameQA_Answer* MJGameQA_Answer::createFrame(mj::QA question, int type) {
    auto p = static_cast<MJGameQA_Answer*>(cocos2d::CSLoader::createNode("csb/game/qa/GameQA_Answer.csb"));
    if (p) {
        p->didLoadFromCSB(question, type);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameQA_Answer::didLoadFromCSB(mj::QA question, int type) {
    _data = question;
    _type = type;
    auto win_size = Director::getInstance()->getWinSize();
    question_lb = this->getChildByName<Text*>("question_lb");
    //    question_lb->setString(question.question.text);
    question_lb->setString("");
    question_lb->setTextAreaSize(cocos2d::Size(win_size.width * 0.84, 80));
    question_lb->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    question_lb->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    //    question_lb->setFontSize(100);

    auto list_answer = question.answers;
    for (int i = 0; i < (int)question.answers.size(); i++) {
        auto item = MJGameQA_AnswerItem::createFrame(question.answers[i], i);
        item->setPosition(Vec2(75, 280 - i * 85));
        this->addChild(item);
        item->setDelegate(this);
        list_item.push_back(item);
    }
    list_text = explode(_data.question.text, ' ');
}

void MJGameQA_Answer::onShow() {
    question_lb->setString("");
    count_str = 0;
    this->schedule(schedule_selector(MJGameQA_Answer::updateQuesLb), 0.1f);
    for (int i = 0; i < (int)list_item.size(); i++) {
        list_item[i]->onShow();
    }
}

void MJGameQA_Answer::setRedColorWrongAnswer(bool enable)
{
    // enable - red; else green
    list_item[index_choose_answer]->setRedColorAnswer(enable);
}

void MJGameQA_Answer::setBlueColorCorrectAnswer()
{
    list_item[index_choose_answer]->setBlueColorAnswer();
}

std::vector<MJGameQA_AnswerItem*> MJGameQA_Answer::getList_AnswerItem()
{
    return list_item;
}

void MJGameQA_Answer::setTouchSpeaker(bool req)
{
    this->getChildByName<Button*>("btspeaker")->setTouchEnabled(req);
}

bool MJGameQA_Answer::isReadingDone()
{
    return isReadAudio;
}

void MJGameQA_Answer::updateQuesLb(float time) {

    if (count_str >= (int)list_text.size()) {
        this->unschedule(schedule_selector(MJGameQA_Answer::updateQuesLb));
        //onPlayAudioQues();
        return;
    }

    question_lb->setString(question_lb->getString() + " " + list_text[count_str]);
    //    question_lb->setFontSize(100 - count_str*(100-32)/(int)list_text.size());
    count_str++;
}

vector<string> MJGameQA_Answer::explode(const string& str, const char& ch) {
    string next;
    vector<string> result;
    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        }
        else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
        result.push_back(next);
    return result;
}

ImageView* MJGameQA_Answer::getImgWith(string path_img, int type) {//create ICON
    auto img = ImageView::create(path_img);
    if (type == 2) {
        img->setPosition(this->getContentSize() / 2);

    }
    else if (type == 3) {
        img->setPosition(Size(this->getContentSize().width / 2, 118));
    }
    return img;
}

void MJGameQA_Answer::onPlayAudioQues(std::function<void()> finishCallback) {
    if (isReadAudio) return;
    isReadAudio = true;
    isReading = true;
    this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([=] {
        question_lb->setTextColor(Color4B::BLACK);
        auto path_audio = _data.question.audio_path;
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(path_audio);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
            CCLOG("AUDIO PLAY DONE");
            if (!question_lb)
            {
                return;
            }
            question_lb->setTextColor(Color4B::WHITE);
            this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([=] {
                this->onPlayAudioAnswer(finishCallback);
                }), NULL));
            });
        }), NULL));
}

std::string MJGameQA_Answer::getQASoundPath()
{
    return _data.question.audio_path;
}

bool MJGameQA_Answer::GetReadingDone()
{
    return _readingDone;
}

void MJGameQA_Answer::onPlayAudioAnswer(std::function<void()> finishCallback) {
    list_item[index_play_audio]->setHighlight();
    auto path_audio = _data.answers[index_play_audio].audio_path;
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(path_audio);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
        CCLOG("AUDIO PLAY DONE %d", index_play_audio);
        list_item[index_play_audio]->setUnChooseAnswer();
        this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([=] {
            index_play_audio++;
            if (index_play_audio >= (int)_data.answers.size()) {
                isReading = false;
                _readingDone = true;
                if (finishCallback)
                    finishCallback();
                return;
            }
            this->onPlayAudioAnswer(finishCallback);
            }), NULL));
        });
}

void MJGameQA_Answer::onClickSpeaker(Ref* sender) {
    stopAllAudio();
    isReading = true;
    question_lb->setTextColor(Color4B::BLACK);
    auto path_audio = _data.question.audio_path;
    auto audio_id = cocos2d::experimental::AudioEngine::play2d(path_audio);
    cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path) {
        CCLOG("AUDIO PLAY DONE");
        isReading = false;
        question_lb->setTextColor(Color4B::WHITE);
        });
}

void MJGameQA_Answer::stopAllAudio() {
    cocos2d::experimental::AudioEngine::stopAll();
    this->unschedule(schedule_selector(MJGameQA_Answer::updateQuesLb));
    this->stopAllActions();
    if (!isReading) return;
    isReading = false;
    question_lb->setTextColor(Color4B::WHITE);
    for (int i = 0; i < (int)list_item.size(); i++) {
        list_item[i]->setUnChooseAnswer();
    }

    if (isChoose) {
        list_item[index_choose_answer]->setChooseAnswer();
    }
}


void MJGameQA_Answer::onItemClickSpeaker(bool is_reading) {
    isReading = is_reading;
}

std::string MJGameQA_Answer::getObjectName() {
    return "";
}

void MJGameQA_Answer::setDelegate(MJGameQA_AnswerDelegate* delegate) {
    this->delegate = delegate;
}

void MJGameQA_Answer::onChooseItem(cocos2d::Ref* sender) {//delegate when click item.
    stopAllAudio();
    isChoose = true;
    auto item = dynamic_cast<MJGameQA_AnswerItem*>(sender);
    if (index_choose_answer == (int)item->getTag()) {
        return;
    }
    cocos2d::experimental::AudioEngine::play2d("sounds/qa/qa_click.mp3");
    for (int i = 0; i < (int)list_item.size(); i++) {
        if (item == list_item[i]) {
            index_choose_answer = i;
            item->setChooseAnswer();
        }
        else list_item[i]->setUnChooseAnswer();
    }

    if (item && item->getObjectName() == _data.answer) {
        isCheckAnswer = true;
    }
    else {
        isCheckAnswer = false;       
    }
        

    if (delegate) {
        delegate->onChooseAnswer(this);
    }
}

ui::Widget::ccWidgetClickCallback MJGameQA_Answer::onLocateClickCallback(const std::string& callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJGameQA_Answer, onClickSpeaker),
    };

    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}


