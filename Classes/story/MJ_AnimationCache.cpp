//
//  MJ_AnimationCache.cpp
//  M
//
//  Created by To Ngoc Linh on 5/10/17.
//
//

#include "MJ_AnimationCache.h"

#include <limits>

#define ANIM_CACHE AnimationCache::getInstance()

namespace mj {
    AnimationCache::AnimationCache() {
        _isInSession = false;
        _sessionID = -1;
    }

    gaf::GAFAsset * AnimationCache::create(std::string animation_name, std::function<void ()> callback) {
        CCLOG("animation_cache = %s", animation_name.c_str());
        animation_name = cocos2d::FileUtils::getInstance()->fullPathForFilename(animation_name);

        auto it = ANIM_CACHE._cache.find(animation_name);
        if (it != ANIM_CACHE._cache.end()) {
            return it->second;
        }

        auto asset = gaf::GAFAsset::create(animation_name);
        if (asset) {
            ANIM_CACHE._animsInSession.push_back(animation_name);
            asset->retain();
            ANIM_CACHE._cache.emplace(animation_name, asset);
        }

        return asset;
    }

    void AnimationCache::startSession() {
        endSession();
        assert(ANIM_CACHE._sessionID == -1 && "Chưa kết thúc session cũ");
        ANIM_CACHE._isInSession = true;
        ANIM_CACHE._sessionID = cocos2d::random(0, INT_MAX);
        ANIM_CACHE._animsInSession.clear();
    }

    void AnimationCache::endSession() {
        for (auto name : ANIM_CACHE._animsInSession) {
            ANIM_CACHE.removeAnimation(name);
        }
        ANIM_CACHE._sessionID = -1;
        ANIM_CACHE._isInSession = false;
    }

    void AnimationCache::removeAnimation(std::string name) {
        name = cocos2d::FileUtils::getInstance()->fullPathForFilename(name);

        auto it = ANIM_CACHE._cache.find(name);
        if (it != ANIM_CACHE._cache.end()) {
            it->second->release();
            ANIM_CACHE._cache.erase(it);
        }
    }
}
