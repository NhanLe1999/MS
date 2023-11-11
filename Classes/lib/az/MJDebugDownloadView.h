//
//  MJDebugDownloadView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/26/17.
//

#ifndef MJDebugDownloadView_hpp
#define MJDebugDownloadView_hpp

#include "cocostudiomacro2.h"

class MJDebugDownloadView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJDebugDownloadView);
    static MJDebugDownloadView * createView();
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name);
    
    void onDownload(cocos2d::Ref *);
    void onClose(cocos2d::Ref *);
};

CREATE_READER(MJDebugDownloadView);

#endif /* MJDebugDownloadView_hpp */
