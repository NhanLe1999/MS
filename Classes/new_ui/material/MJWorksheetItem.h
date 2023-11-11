#ifndef MJ_WORKSHEET_ITEM
#define MJ_WORKSHEET_ITEM

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

#include "RemoteImageView.h"

USING_NS_CC;

class MJWorksheetItemDelegate
{
public:
    virtual std::string getGradeNameByGradeId(int gradeId) {return "";};
    virtual int getSelectedWorksheetNumber() {return 0;};
    virtual void onWorksheetItemClicked(cocos2d::Ref * item) {};
};

class MJWorksheetItem : public cocos2d::ui::Layout , public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(MJWorksheetItem);
	static MJWorksheetItem* createItem(const mj::model::MaterialInfo& info, MJWorksheetItemDelegate * delegate = nullptr, bool isSent = false);
    
    mj::model::MaterialInfo getInfo() {return _info;};
    bool isSelected() {return _isSelected;}
    bool isSent() {return _isSent;};
    
    void setWorksheetName(const std::string& name);
    
    void forceClickItem() {this->onClickItem(nullptr);};
    
    void setSelected(bool selected);
    
    void setSent(bool sent);
    
    void setReaded(bool readed);
    
    void setOrderNumber(int numb);

private:
    MJWorksheetItemDelegate *       _delegate = nullptr;    // assign var
    ui::Layout *                    _maskThumb = nullptr;
    ui::ImageView *                 _icon_downloaded = nullptr;
    ui::ImageView *                 _icon_readed = nullptr;
    ui::Text *                      _textLevel = nullptr;
    RemoteImageView *               _thumbnail = nullptr;
    
    mj::model::MaterialInfo         _info;
    bool                            _isSelected = false;
    bool                            _isSent = false;
    bool                            _isReaded = false;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
    
	void didLoadFromCSB(const mj::model::MaterialInfo& info, MJWorksheetItemDelegate * delegate, bool isSent);
    
    void onClickItem(cocos2d::Ref * sender);
};

CREATE_READER(MJWorksheetItem);
#endif
