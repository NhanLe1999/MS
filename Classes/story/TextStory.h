#ifndef __DuaNgu_IOS__TextStory__
#define __DuaNgu_IOS__TextStory__
#define displayNVSize 0.5
#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "TextMCObj.h"
using namespace cocos2d;
using namespace std;
class TextStory : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(TextStory);
    void animShow();
    void animHide();
    TextMCObj _textMCObj;
    void nextPage();
    void backPage();
    void playTextMCEffect(std::string _str_find, float duration);
    void didLoadFromCSB();
    int fontSize;
    Color3B normal_color;
    Color3B highlight_color;
    void stopAllActionAndMC();
    bool hasNextBoxText();
    bool hasPrevBoxText();
    void startPlaying();
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    cocos2d::ui::ImageView *bg;
    void onHide(cocos2d::Ref * sender);
    bool isAnimRunning = false;
    float sizeTextStoryLayer = 0;
    Label *label;
    void highlightTextAction();
    void runCauAction();
    
    int iStart = 0;
    int iEnd = 1;
    ValueVector timeline;
    ValueMap mapText;
    int sync_index = 0, cau_index = 0;
    

    float duration = 0;
    void setFontNameAndFontSize(Label *hsLabel, cocos2d::ValueMap mapFont);
    std::vector<std::string> _fontNameArr;
    std::vector<Label *> _labelArr;
    int boxIndex = 0, textIndex = 0, soundMCIndex = 0;
    std::string trimContentText(std::string content);
    std::string replaceString(std::string subject, const std::string& search,
                              const std::string& replace);
    void displayEffectTextMC(Label *_label, std::string _str_find, float duration);
    std::vector<int> _listEffectID;
    std::vector<int> _listTagHSLabel;
    int count_delay_effect = 0;
    cocos2d::Rect getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find);
    void addTagHSLabelToArr(int tag);
    void removeHSLabelEffect();
    Rect convertRectBox(Rect _rectbox);
    void setColorWhenDoneHighlightCharacter();
    int getNumLineIndexOfString(float numLineH,float _height);
    bool isOneLine = false;
    void playGAFWhenTextMCNull();
    bool isStartMCRead = false;
    bool _is_done_highlight = false;
};

CREATE_READER_CLASS(TextStoryReader, TextStory);

#endif /* defined(__ALTP__TextStory__) */
