
#ifndef AppInbox_hpp
#define AppInbox_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "platform/CCPlatformConfig.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"

#include "StoryParentNotification_Landscape.h"

#define JSON_SAFE_CHECK(a, b, c) (a.HasMember(b) && (!a[b].IsNull()) && a[b].Is##c())

#define JSON_SAFE_EXTRACT(doc, key, type, default_value) (JSON_SAFE_CHECK(doc, key, type) ? doc[key].Get##type() : default_value)

class MSCollectionStoryItem;
class MSCollectionAudiobookItem;

namespace mj {

    namespace appinbox {
        StoryNotificationInfo setValueObject(std::string value);

        std::vector<StoryNotificationInfo> getMessageAppinbox();
    }
}

#endif /* AppInbox_hpp */
