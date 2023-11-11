//
//  SpineWrapper.cpp
//
//
//  Created by tunado
//
//

#include    "SpineWrapper.h"
#include    "MJHelper.h"

std::unordered_map<std::string, spine::SkeletonRenderer*> MKSkeletonAnimation::_spineCache;

USING_NS_CC;

SpineWrapper * SpineWrapper::createWithJson(const std::string &json_path, const std::string &atlas_path, float scale_rate) {
    auto wrapper = new SpineWrapper();
    if (wrapper && wrapper->init(json_path, atlas_path, scale_rate)){
        wrapper->autorelease();
    } else {
        CC_SAFE_DELETE(wrapper);
        wrapper = nullptr;
    }

    return wrapper;
}

SpineWrapper * SpineWrapper::createWithPath(const std::string &path, float scale_rate) {

    auto wrapper = new SpineWrapper();

    std::string json_path = StringUtils::format("%s.json", path.c_str());
    
    std::string atlas_path = StringUtils::format("%s%s.atlas", path.c_str(), mj::helper::isRetina() ?"_hdr":"_hd");
    atlas_path = StringUtils::format("%s%s.atlas", path.c_str(), "_hd");

    if (wrapper && wrapper->init(json_path, atlas_path, scale_rate)) {
        wrapper->autorelease();
    } else {
        CC_SAFE_DELETE(wrapper);
        wrapper = nullptr;
    }

    return wrapper;
}

SkeletonAnimation* SpineWrapper::createSkeletonAnimationWithPath(const std::string& path, float scaleRate, std::string animationName , bool isLoop )
{
    std::string json_path = StringUtils::format("%s.json", path.c_str());

    std::string atlas_path = StringUtils::format("%s%s.atlas", path.c_str(), mj::helper::isRetina() ? "_hdr" : "_hd");
    
    atlas_path = StringUtils::format("%s%s.atlas", path.c_str(), "_hd");
   
    auto animation = SkeletonAnimation::createWithJsonFile(json_path, atlas_path, scaleRate);

    animation->update(0);

    return animation;
}

void SpineWrapper::setSkeletonAnimationPosition(SkeletonAnimation* animation, const std::string& boneName, const Vec2& position)
{
    MK_RETURN_IF(!animation);

    if (auto bone = animation->findBone(boneName)) {
        bone->setX(position.x);
        bone->setY(position.y);
    }
}

Vec2 SpineWrapper::getSkeletonAnimationPosition(SkeletonAnimation* animation, const std::string& boneName)
{
    if (animation) {

        if (auto bone = animation->findBone(boneName)) {
            return Vec2(bone->getX(), bone->getY());
        }
    }

    return Vec2::ZERO;
}

bool SpineWrapper::init(const std::string &json_path, const std::string &atlas_path, float scale_rate) {

    if (!ui::Layout::init()) {
        return false;
    }

    _animation = SkeletonAnimation::createWithJsonFile(json_path, atlas_path, scale_rate);
    _animation->update(0);
    auto rect = _animation->getBoundingBox();
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->setContentSize(rect.size);
    this->setBackGroundColor(Color3B::RED);
    this->setBackGroundColorOpacity(0);
    this->setBackGroundColorType(BackGroundColorType::SOLID);
    _animation->setPosition(Vec2(this->getContentSize().width/2,0));
    this->addChild(_animation);
   
    return true;
}

void SpineWrapper::setEnableBound(bool show) {
    this->setBackGroundColorOpacity(show ? 100:0);
}

void SpineWrapper::playSequence(SkeletonAnimation* animation, std::string sequence, bool looped, std::function<void(SkeletonAnimation* obj, const std::string& callback_name)> callback, bool is_delay, int index_animation) {

    MK_RETURN_IF(!animation);
    animation->addAnimation(index_animation, sequence, looped);

    std::string schedule_name = std::string("sequence_callback") + sequence + StringUtils::toString(index_animation);
    float anim_length = animation->findAnimation(sequence)->getDuration();
    index_animation++;
    if (callback) {
        animation->scheduleOnce([=](float dt) {
            callback(animation, sequence);
        }, anim_length, schedule_name);
    }
}

void SpineWrapper::playSequence(std::string sequence, bool looped, std::function<void(SkeletonAnimation * obj, const std::string & callback_name)> callback, bool is_delay) {
    
    MK_RETURN_IF(!_animation||_currentAnimationName==sequence);

    _currentAnimationName = sequence;
    
    _animation->addAnimation(index_animation, sequence, looped);
    
    std::string schedule_name = std::string("sequence_callback") + sequence + StringUtils::toString(index_animation);
    float anim_length = _animation->findAnimation(sequence)->getDuration();
    index_animation++;
    if (callback) {
        this->scheduleOnce([=](float dt) {
            callback(_animation, sequence);
        }, anim_length, schedule_name);
    }
}

void SpineWrapper::clearTrack() {
    _animation->clearTrack();
}

void SpineWrapper::playSequence(std::vector<std::string> sequence, bool looped, std::function<void(SkeletonAnimation * obj)> callback, bool is_delay) {
    
    MK_RETURN_IF(!_animation);

    float anim_length = 0;
    _animation->clearTracks();
    for (int i = 0; i < sequence.size(); i++) {
        if (i == sequence.size() - 1) {
            _animation->addAnimation(index_animation, sequence[i], looped);
        } else {
            _animation->addAnimation(index_animation, sequence[i], false);
        }
        
        CCLOG("spine length %f",_animation->findAnimation(sequence[i])->getDuration());
        anim_length += _animation->findAnimation(sequence[i])->getDuration();
    }
    
    std::string schedule_name = std::string("sequence_callback") + sequence.back() + StringUtils::toString(index_animation);
    index_animation++;
    if (callback) {
        this->scheduleOnce([=](float dt) {
            callback(_animation);
        }, anim_length, schedule_name);
    }
}

void SpineWrapper::playSequence(std::string sequence, float duration, bool looped, std::function<void(SkeletonAnimation * obj, const std::string & callback_name)> callback, bool is_delay ) {
    
    MK_RETURN_IF(!_animation||_currentAnimationName==sequence);

    _currentAnimationName = sequence;
    
    _animation->addAnimation(index_animation, sequence, looped);
    
    std::string schedule_name = std::string("sequence_callback") + sequence + StringUtils::toString(index_animation);
    
    if(auto animation =_animation->findAnimation(sequence)) {
        
        animation->setDuration(duration);
        
        float anim_length = animation->getDuration();
        
        index_animation++;
        
        if (callback)
        {
            this->scheduleOnce([=](float dt) {
                
                callback(_animation, sequence);
                
            }, anim_length, schedule_name);
        }
    }
}

float SpineWrapper::getTimeAction(std::string action_name) {

    MK_RETURN_IF(!_animation,-1.0f);

    float anim_length = _animation->findAnimation(action_name)->getDuration();
    return anim_length;
}

void SpineWrapper::onExit() {
    ui::Layout::onExit();
    //_animation->removeFromParentAndCleanup(true);
}

void SpineWrapper::pauseAnimation(){
    MK_RETURN_IF(!_animation);
    _animation->pause();
}

void SpineWrapper::update(float time) {
    MK_RETURN_IF(!_animation);
    _animation->update(time);
}

void SpineWrapper::setSkin(std::string skin) {
    MK_RETURN_IF(!_animation);
    _animation->setSkin(skin);
    _animation->setSlotsToSetupPose();
}

void SpineWrapper::updateAnimationPosition(Vec2 position) {
    MK_RETURN_IF(!_animation);
    _animation->setPosition(position);
}

void SpineWrapper::setSkeletonAnimationPosition(const std::string& boneName, const Vec2& position)
{
    MK_RETURN_IF(!_animation);

    if (auto bone = _animation->findBone(boneName)) {
        bone->setX(position.x);
        bone->setY(position.y);
    }
}

MKSkeletonAnimation* MKSkeletonAnimation::createSkeletonAnimationWithPath(const std::string& path, bool cache, float scaleRate, std::string animationName, bool isLoop)
{
    std::string json_path = StringUtils::format("%s.json", path.c_str());

    std::string atlas_path = StringUtils::format("%s%s.atlas", path.c_str(), mj::helper::isRetina() ? "_hdr" : "_hd");
    
    atlas_path = StringUtils::format("%s%s.atlas", path.c_str(), "_hd");

    MKSkeletonAnimation* animation = new MKSkeletonAnimation();

    if (animation)  {
        animation->init(json_path, atlas_path,cache, scaleRate);
        animation->update(0);
        return animation;
    }

    CC_SAFE_DELETE(animation);

    return nullptr;
}

void MKSkeletonAnimation::playSequence(std::string animationName, bool looped, std::function<void(SkeletonAnimation* obj, const std::string& callback_name)> callback)
{
    this->addAnimation(_indexAnimation, animationName, looped);

    std::string schedule_name = std::string("sequence_callback") + animationName + StringUtils::toString(_indexAnimation);
    float anim_length = this->findAnimation(animationName)->getDuration();
    _indexAnimation++;
    if (callback) {
        this->scheduleOnce([=](float dt) {
            callback(this, animationName);
        }, anim_length, schedule_name);
    }
}

Vec2 MKSkeletonAnimation::getBonePosition(const std::string& boneName)
{
    if (auto bone = this->findBone(boneName)) {
        return Vec2(bone->getX(), bone->getY());
    }

    return Vec2();
}

void MKSkeletonAnimation::setBonePosition(const std::string& boneName, const Vec2& position)
{
    if (auto bone = this->findBone(boneName)) {
        bone->setX(position.x);
        bone->setY(position.y);
    }
}

void MKSkeletonAnimation::setBonePositionX(const std::string& boneName, const float posX)
{
    if (auto bone = this->findBone(boneName)) {
        bone->setX(posX);
    }
}

float MKSkeletonAnimation::getAnimationDuration(const std::string& animationName)
{
    if (auto animation = this->findAnimation(animationName)) {

        return animation->getDuration();
    }

    return -1.0f;
}

void MKSkeletonAnimation::init(const std::string& skeletonDataFile, const std::string& atlasFile, bool cache, float scale)
{
    if (cache) {
        SkeletonRenderer* skeletonRenderer = NULL;
        auto it = _spineCache.find(skeletonDataFile);
        if (it != _spineCache.end()) {
            skeletonRenderer = it->second;
        } else {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
            if (NativeBridge::isRamWarning()) {
                this->initWithJsonFile(skeletonDataFile, atlasFile, scale);
            }
            else {
                skeletonRenderer = spine::SkeletonRenderer::createWithFile(skeletonDataFile, atlasFile, scale);
                skeletonRenderer->retain();
                _spineCache.insert(std::make_pair(skeletonDataFile, skeletonRenderer));
            }
#endif
        }
        this->initWithData(skeletonRenderer->getSkeleton()->getData());
    } else {
        this->initWithJsonFile(skeletonDataFile, atlasFile, scale);
    }
    
    this->autorelease();
}

void MKSkeletonAnimation::clearCache() {
//    if (1) {
//        for (const auto& kv : _spineCache) {
//            kv.second->autorelease();
//        }
//        _spineCache.clear();
//    }
}
