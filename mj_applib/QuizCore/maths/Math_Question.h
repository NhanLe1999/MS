//
//  Math_Question.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#ifndef Math_Question_hpp
#define Math_Question_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "CocoStudioMacro.h"

#include "Math_Libs.h"
#include "Math_Func.h"

class Math_Question : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(Math_Question);
    static Math_Question * createQuestion();
    void setEnable(std::function<void()> callback);
    void setDisable();
    bool isEnable();
    
    void pushData(math::question::Question data);

    bool goNext();
    bool goToQuestionAtIndex(int index);

    void onStart(float delay = 0);
    void onHide(float delay = 0);
    math::question::Question getData();
        
    std::vector<math::resource::Text> getQuestionTexts();
    std::string getQuestionTextValue();
    std::vector<std::string> getQuestionVoices(int voice_id);
    //std::vector<std::string> getOptionKeys();

    //Animation    
    void setAudioCallback(std::function<void(bool)> callback);
    
    void setKeywordValue(int value);
    void setOptionalTextsComp(std::string key, std::vector<math::resource::Image*> images);
    void setOptionalTexts(std::string key, std::vector<math::resource::Image*> images);
    void setOptionalTexts(std::string key, std::vector<math::resource::Text> texts);
    
    std::vector<std::string> getOptionalKeys();
    
    int getNumberQuestions ();
    
    void setQuestionWidth(float width);
    cocos2d::Size adjustQuestionSize();
    
    void doHighlight();
    void showQuestion();
    void hideQuestion();
    
    void enableTouchQuestion();

private:
    bool init() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onSpeak(cocos2d::Ref*);
    
    cocos2d::ui::Text*      _text = nullptr;
    cocos2d::ui::ImageView* _background = nullptr;
    cocos2d::ui::Button*    _button = nullptr;
    cocos2d::ui::Button*    _button_speaker = nullptr;

    bool _enable;
    bool _showing;
    float question_width = 850.0;

    std::vector<math::question::Question> data_set;
    int index_question = 0;
    
    std::vector<math::question::Highlight> highlight_set;
    std::vector<cocos2d::ui::ImageView*> highlight_icons;
 
    std::function<void()> _callback = nullptr;
    std::function<void(bool)> _audio_callback = nullptr;
};

CREATE_READER_CLASS(Math_QuestionReader, Math_Question);
#endif /* Math_Question_hpp */
