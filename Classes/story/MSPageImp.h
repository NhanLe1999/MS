
//
//  Header.h
//  MJStory
//
//  Created by To Ngoc Linh on 8/31/18.
//

#ifndef Header_h
#define Header_h

#include "cocos2d.h"
#define KHUNG_SAO_DURATION (19.0f/20.0f) // "khung/sao.gaf" have 19 Frames with 20 FPS
class MSPageImp {
public:
    virtual void startPlaying() {}
    virtual void enableInteraction() {}
    virtual void disableInteraction() {}
    virtual bool hasNextBoxText() { return false; }
    virtual bool hasPrevBoxText() { return false; }

    virtual int getPageIndex() = 0;
    virtual float getStandardTime() = 0;
    virtual int getGAFClickedNum() = 0;
    virtual int getIMGClickedNum() = 0;
protected:
    bool _need_refreshed = false;
    bool _is_touch_enabled = false;

    //for tracking only
    float m_standardTime = 0.0f;//seconds
    int m_gafClickedNum = 0.0f;
    int m_imgClickedNum = 0.0f;
public:
    void setNeedRefreshed(bool v) { _need_refreshed = v; }
    bool isNeedRefreshed() { return _need_refreshed; }
};

#endif /* Header_h */
