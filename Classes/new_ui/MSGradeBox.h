//
//  MSGradeBox.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#ifndef MSGradeBox_hpp
#define MSGradeBox_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"
#include "MSGradeManager.h"

#define UNDEFINED_GRADE_ID -113

namespace ms {
    namespace delegate {
        class GradeBox;
    }
}

class MSGradeBox : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSGradeBox);
    static MSGradeBox * createBox(bool is_new,int i_gradeID);
    void setEnableInteraction(bool is_enable);
    void loadGrades();
    void showGrades();
    void hideGrades();
    
    bool forceSelectGrade(int grade_id); // -1 là chọn All Grades, trả về true nếu đổi grade, false nếu không đổi
    
    int getSelectedGradeId() {return _selected_grade_id;}
    std::string getGradeName(int gradeId);
    int _gradeId = -1;
    void onReceivedEventGradeChanged(int grade_id);

private:
    void didLoadFromCSB(bool is_new,int i_gradeID=UNDEFINED_GRADE_ID);
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onSelect(cocos2d::Ref * sender);
    
    bool _is_showing_grades;
    bool _is_animating;
    
    int _saved_ids;
    
    void onSelectedGrade(cocos2d::Ref * sender);
    void onOverlay(cocos2d::Ref * sender);
    std::vector<mj::model::GradeInfo> _grades_info;
    int _selected_grade_id;
    cocos2d::Size _model_size;
    
    CC_SYNTHESIZE(ms::delegate::GradeBox *, _delegate, Delegate);
private:
    ms::GradeManager::Scope m_scope;
};

CREATE_READER(MSGradeBox);
#endif /* MSGradeBox_hpp */
