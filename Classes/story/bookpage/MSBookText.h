
#ifndef MSBookText_hpp
#define MSBookText_hpp

#include "cocostudiomacro2.h"
#include "MSBookData.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
#define TYPE_BOX_0 "box_type_0"
#define TYPE_BOX_1 "box_type_1"
#define TYPE_BOX_2 "box_type_2"
class MSBookTextDelegate {
public:
    virtual void onDoneHighlightTextMC(){};
};

class MSBookTextInline;

class MSBookText : public cocos2d::ui::Layout, public MSBookTextDelegate, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MSBookText);
    static MSBookText * createBookText(std::vector<mj::BOOK_TEXT_INFO> list_text, std::vector<mj::BOOK_AUDIO_INFO> list_audio, std::string box_type);
    void startHighlightText();
    void setDelegate(MSBookTextDelegate *delegate);
    bool onNextBoxText();
    bool onBackBoxText();
    void stopAllActionAndSound();
    void playTextMCEffect(std::string _str_find, float duration);
    bool hasNextBoxText();
    bool hasPrevBoxText();
    void resetBox();
private:
    void didLoadFromCSB(std::vector<mj::BOOK_TEXT_INFO> list_text, std::vector<mj::BOOK_AUDIO_INFO> list_audio, std::string box_type);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    bool shouldBoxTextVisible(int idx);
    
    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    MSBookTextDelegate *_delegate = nullptr;
    
    mj::BOOK_TEXT_INFO _book_text_info;
    mj::BOOK_AUDIO_INFO _book_audio_info;
    std::vector<mj::BOOK_TEXT_INFO> _list_text_info;
    std::vector<mj::BOOK_AUDIO_INFO> _list_audio_info;
    std::vector<Text *> list_box_text;
    void highlightActionType2();
    void highlightActionType1();
    int highlight_index = 0, boxtext_index = 0, letter_index = 0, index_text_show = 0;
    std::vector<mj::BOOK_SYNC_TEXT> list_sync;
    std::vector<Color3B> list_normal_color, list_highlight_color;
    int getIndexLetterConvert();
    std::vector<int> list_size_boxtext;
    void showText();
    std::string _box_type;
    int audio_mc_id = -1;
    bool is_done_highlight = false;
    void onDoneHighlightTextBox();
    
    std::string trimContentText(std::string content);
    std::string replaceString(std::string subject, const std::string& search,
                              const std::string& replace);
    void displayEffectTextMC(Text *_label, std::string _str_find, float duration);
    std::vector<int> _listEffectID;
    std::vector<int> _listTagHSLabel;
    int count_delay_effect = 0;
    cocos2d::Rect getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find);
    void addTagHSLabelToArr(int tag);
    void removeHSLabelEffect();
    Rect convertRectBox(Rect _rectbox);
    int getNumLineIndexOfString(float numLineH,float _height);
    bool isOneLine = false;
    void addBoxText();
    void resetTextColor();

    MSBookTextInline * _inline_text = nullptr;
};

CREATE_READER(MSBookText);

#endif
