
#ifndef GameQA_Answer_hpp
#define GameQA_Answer_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
#include "GameQA_AnswerItem.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
class MJGameQA_AnswerDelegate {

public:
    virtual void onChooseAnswer(Ref* sender) {};
};


class MJGameQA_Answer : public cocos2d::ui::Layout, public MJGameQA_AnswerItemDelegate,
    public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGameQA_Answer);
    static MJGameQA_Answer* createFrame(mj::QA question, int type);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;

    std::string getObjectName();

    void setDelegate(MJGameQA_AnswerDelegate* delegate);
    mj::QA _data;
    bool isCheckAnswer = false;
    bool isChoose = false;
    int index_choose_answer = 0;
    void onPlayAudioQues(std::function<void()> finishCallback = nullptr);
    std::string getQASoundPath();
    bool GetReadingDone();
    void stopAllAudio();
    void onShow();
    void setRedColorWrongAnswer(bool enable);
    void setBlueColorCorrectAnswer();
    std::vector<MJGameQA_AnswerItem*> getList_AnswerItem();
    void setTouchSpeaker(bool req);
    bool isReadingDone();

private:
    void didLoadFromCSB(mj::QA question, int type);
    MJGameQA_AnswerDelegate* delegate = nullptr;
    ImageView* getImgWith(string path_img, int type);
    int _type = 0;
    void onChooseItem(Ref* sender) override;
    void onItemClickSpeaker(bool is_reading) override;
    std::vector<MJGameQA_AnswerItem*> list_item;
    void onPlayAudioAnswer(std::function<void()> finishCallback = nullptr);
    int index_play_audio = 0;
    Text* question_lb;
    bool isReadAudio = false, isReading = false , _readingDone = false;
    void onClickSpeaker(Ref* sender);
    int count_str = 0;
    void updateQuesLb(float time);
    std::vector<std::string> list_text;
    vector<string> explode(const string& str, const char& ch);

};

CREATE_READER(MJGameQA_Answer);

#endif /* GameQA_Answer_hpp */
