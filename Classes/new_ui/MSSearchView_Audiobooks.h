//
//  MSSearchView_Audiobooks_Audiobooks.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/8/18.
//

#ifndef MSSearchView_Audiobooks_Audiobooks_hpp
#define MSSearchView_Audiobooks_Audiobooks_hpp

#include "cocostudiomacro2.h"
#include "MSCollectionDataSource.h"
#include "MSCollectionDelegate.h"

#include "MJStoryInfoModel.h"

class MSComplexCollection;

class MSSearchView_Audiobooks : public cocos2d::ui::Layout, public ms::delegate::CollectionDataSource, public ms::delegate::Collection, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSSearchView_Audiobooks);
    static MSSearchView_Audiobooks * createView();
    
    void setContentSize(const cocos2d::Size & size) override;
    void onEnter() override;
    void hideVirtualKeyboard();
private:
    void didLoadFromCSB();
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBack(cocos2d::Ref * sender);
    
    cocos2d::ui::TextField * _textfield;
    
    MSComplexCollection * _collection = nullptr;
    
    int numberOfColumns() override;
    int numberOfCells() override;
    cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) override;
    cocos2d::Size modelSize() override;
    cocos2d::Size collectionSize() override;
    void offLoadCellAt(int index) override;
    void reloadData();
    
    std::vector<mj::model::AudiobookInfo> _audiobooks;
    std::vector<cocos2d::ui::Widget *> _ui_source;
    
    void onSelectCollectionItem(cocos2d::ui::Widget * item) override;
	void pushCleverTapEvent();

	mj::model::AudiobookInfo m_clickedAudiobookInfo;
};

CREATE_READER(MSSearchView_Audiobooks);
#endif /* MSSearchView_Audiobooks_Audiobooks_hpp */
