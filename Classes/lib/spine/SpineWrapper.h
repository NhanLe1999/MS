//
//  SpineWrapper.hpp
//
//
//  Created by tunado
//
//

#ifndef SpineWrapper_hpp
#define SpineWrapper_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>
#include "NativeBridge.h"

using namespace spine;

class MKSkeletonAnimation :public SkeletonAnimation {

public:

    static MKSkeletonAnimation* createSkeletonAnimationWithPath(const std::string& path, bool cache = true, float scaleRate = 0.5f, std::string animationName = "action", bool isLoop = false);

    void playSequence(std::string animationName, bool looped = false, std::function<void(SkeletonAnimation* obj, const std::string& callback_name)> callback = nullptr);
    
    Vec2 getBonePosition(const std::string& boneName);

    void setBonePosition(const std::string& boneName, const Vec2& position);

    void setBonePositionX(const std::string& boneName, const float posX);

    cocos2d::Size getContentSize() { return this->getBoundingBox().size; };

    float getAnimationDuration(const std::string& animationName);
    
    static void clearCache();

protected:
    
    void init(const std::string& skeletonDataFile, const std::string& atlasFile,bool cache, float scale);
    
    static std::unordered_map<std::string, spine::SkeletonRenderer*> _spineCache;

private:

    CC_SYNTHESIZE(int, _indexAnimation, IndexAnimation);

};

class SpineWrapper : public cocos2d::ui::Layout {

public:

    static SpineWrapper * createWithJson(const std::string &json_path, const std::string &atlas_path, float scale_rate = 0.5 /*ti le scale mac dinh 0.5 vi spine design theo hdr */);
    
    static SpineWrapper * createWithPath(const std::string &path, float scale_rate = 0.5 /*ti le scale mac dinh 0.5 vi spine design theo hdr */);
    
    static SkeletonAnimation* createSkeletonAnimationWithPath(const std::string& path, float scaleRate = 0.5f,std::string animationName = "action",bool isLoop = false);

    static void setSkeletonAnimationPosition(SkeletonAnimation* animation, const std::string& boneName, const Vec2& position);
    static Vec2 getSkeletonAnimationPosition(SkeletonAnimation* animation, const std::string& boneName);

    static void playSequence(SkeletonAnimation* animation, std::string sequence, bool looped = false, std::function<void(SkeletonAnimation* obj, const std::string& callback_name)> callback = nullptr, bool is_delay = true,int index_animation=0);
   
    void playSequence(std::string sequence, bool looped = false, std::function<void(SkeletonAnimation * obj, const std::string & callback_name)> callback = nullptr, bool is_delay = true);
    
    void playSequence(std::vector<std::string> sequence, bool looped = false, std::function<void(SkeletonAnimation * obj)> callback = nullptr, bool is_delay = true);
    
    void playSequence(std::string sequence, float duration, bool looped = false, std::function<void(SkeletonAnimation * obj, const std::string & callback_name)> callback = nullptr, bool is_delay = true);
    
    void setEnableBound(bool show);
    
    float getTimeAction(std::string action_name);
    
    void setSkin(std::string skin);
  
    void pauseAnimation();

    void update(float time);

    void clearTrack();
    
    void updateAnimationPosition(Vec2 relativePosition);
    
    void setSkeletonAnimationPosition(const std::string& boneName, const Vec2& position);

private:

    bool init(const std::string &json_path, const std::string &atlas_path, float scale_rate);
    
    SkeletonAnimation* _animation = nullptr;
    
    int index_animation = 0;
    
    std::string _currentAnimationName = "";
    
    void onExit() override;
};

#endif /* SpineWrapper_hpp */
