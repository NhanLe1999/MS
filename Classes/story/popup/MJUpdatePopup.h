
#ifndef MJUpdatePopup_hpp
#define MJUpdatePopup_hpp

#include "cocostudiomacro2.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
class MJUpdatePopupDelegate {
public:
    virtual void onDoneDelegate(){};
};

class MJUpdatePopup : public cocos2d::ui::Layout, public MJUpdatePopupDelegate, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MJUpdatePopup);
    static MJUpdatePopup * createUpdatePopup(bool is_required,std::string yes ="",std::string no = "");
    void setDelegate(MJUpdatePopupDelegate *delegate);
private:
    void didLoadFromCSB(bool is_required,std::string yes,std::string no);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onEnter() override;
    MJUpdatePopupDelegate *_delegate = nullptr;
    void onOk(cocos2d::Ref *sender);
    void onCancel(cocos2d::Ref *sender);
    bool isRunning = false;
    cocos2d::ui::ScrollView *scrollview = nullptr;
};

CREATE_READER(MJUpdatePopup);

#endif
