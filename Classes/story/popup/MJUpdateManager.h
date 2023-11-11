//
//  MJUpdateManager.hpp
//
//  Created by HoiSung on 03/2/18.
//

#ifndef MJUpdateManager_hpp
#define MJUpdateManager_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class MJUpdateManager : public cocos2d::ui::Layout {
    
public:
    MJUpdateManager() {};
    
    ~MJUpdateManager() {};
    
    static MJUpdateManager * createUpdate(std::string current_ver_app);
    bool onShowUpdatePopup();
    
private:
    std::string version_app;
    void didLoadData(std::string current_ver_app);
    void onEnter();
    
    bool checkVersionSplitLess(std::vector<std::string> ver1, std::vector<std::string> ver2);
    bool checkVersionLess(std::string ver1, std::string ver2);
    std::vector<std::string> splitVersion(std::string version_str);
    std::vector<std::string> explode(const std::string& str, const char& ch);
    void showUpdateAppPopup(bool is_required,std::string str_yes,std::string str_no);
    bool is_show = false;
};
#endif /* MJUpdateManager_hpp */
