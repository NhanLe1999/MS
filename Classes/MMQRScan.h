//
//  MMQRScan.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/18/18.
//

#ifndef MMQRScan_hpp
#define MMQRScan_hpp

#include "cocostudiomacro2.h"

//#define key_qrcode_received "key.qrcode.scan.received"

#define mmqrscan_name               "qr_scan_screen"

#define key_app_should_show_qr      "key.app.should.show.qr"
#define key_app_should_onboard_qr   "key.app.should.onboard.qr"

#define event_deeplink_handled      "event.deeplink.handled"
#define event_scan_closed           "event.scan.closed"
#define event_enable_close_button   "event.enable.close.button"

class MMQRScan : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MMQRScan);
    static MMQRScan * createView();
    static bool showAlert();
    static bool Is_parsing_deeplink;
    
    void onClose(cocos2d::Ref * sender);
    
    void handleQRLink(const std::string& str);
    
    static void handleQRDeeplinkLesson(const std::string& query,const bool isPhonic);

private:
    void didLoadFromCSB();
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExit() override;
    
    void onLinkClicked(cocos2d::Ref * sender);

    void onBack(cocos2d::Ref * sender);
    void onRead(cocos2d::Ref * sender);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    //Camera
    enum ScanState {
        SS_INIT = 0,
        SS_START,
        SS_STOP,
        SS_PARSEDEEPLINK,
        SS_LOADING,
        SS_LOADINGERROR
    };
    
    std::string _qr_string;
    std::string _deep_link;
    ScanState _scan_state;
    
    void update(float dt) override;
    
    void startCamera(cocos2d::Rect rect);
    void stopCamera();
    void hideCamera();
    void showCamera();

    std::string getReadedDeepLink();
    void cleanReadedDeepLink();
    
    std::string getQRLink();
    void cleanQRLink();
    
    void handleQRDeeplinkStory(const std::string& query);

    void parseDeepLink(const std::string & qrLink);
    std::string getDeepLink();
    void cleanDeepLink();
    
    //for android button overlay
    bool isScanCloseClicked();
    bool isScanLinkClicked();
    void onDeeplinkHandled(std::string i_link);
    
    // Clever Tap event(s)
    void pushCleverTapEvent_scanQR(const std::string& str_is_success); // có 3 giá trị: "yes", "no", "none"
};

CREATE_READER(MMQRScan);

#endif /* MMQRScan_hpp */
