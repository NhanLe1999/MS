#include "MJAPI_DecodeObjectJsonInfo.h"
#include "MJDefinitions.h"
#include "MJDefault.h"
#include "../external/json/writer.h"
#include "MJHelper.h"
#include "MJAPIGetLinkDownload.h"
#include "StoryConfigManager.h"
#include "MSServerManager.h"
#include "MSGradeManager.h"
#include "MJPay.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/istreamwrapper.h"
#include <fstream>
#include "manager/RatingManager.h"
#include "manager/GettingUserInfoManager.h"
#include "MSLessonManager.h"
#include "LTDate.h"
#include "manager/CleverTapManager.h"
#include "StoryDataModel.h"
#include "MJStoryInfoModel.h"
#include "StoryFreeManager.h"
#include "MJStoryInfo.h"
#include "StoryLanguageManager.h"
#include "MJAPI_Story_Zip.h"
#include "HSLib.h"
#include "MJPay.h"
#include "IAPManager.h"
#include "APPurchaseManager.h"
#include "ThumbQueue.h"
#include "RemoteImageView.h"
#include "F2PManager.h"
#define maxSizeInsertDB 100
bool mj::api::MJAPIDecodeObjectJsonInfo::shouldBeCalled() {
    return true; // FIXME: tạm thời disable tính năng này
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    return true;
#endif
    if (CONFIG_MANAGER->isProductEnvironment() || CONFIG_MANAGER->isDevEnvironment()) {
        return true;
    }
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    return true;
#endif
    auto last_time_called = MJDEFAULT->getIntegerForKey(key_api_appinfo_called, 0);
    auto current_time = int(utils::getTimeInMilliseconds() / 1000);
    auto seconds_between_call = DAYS_BETWEEN_CALL * 24 * 3600;
    if (current_time - last_time_called >= seconds_between_call) {
        return true;
    }
    else {
        auto date = LTDate(current_time);
        auto last_date = LTDate(last_time_called);
        if (date.weekday == 0 /* sunday */ || date.weekday == 5 /* friday */ || date.weekday == 6 /* sunday */) {
            if (last_date.day == date.day && last_date.month == date.month && last_date.year == date.year) {
                //called in same day
                return false;
            }
        }
    }
    return true;
}

int mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAppInfoData(rapidjson::Document* document)
{

    int v_audio = -1;
    int v_story = -1;
    int v_material = -1;
    auto map_response = document->GetObject();
    auto database_version = JSON_SAFE_CHECK(map_response, "database_version", Int) ? map_response["database_version"].GetInt() : 0;
    if (database_version > MJDEFAULT->getIntegerForKey("key_current_database_version", 0)) {
        MJDEFAULT->setIntegerForKey("key_current_database_version", database_version);
        mj::helper::resetDatabase();
        return 1;
    }
    if (JSON_SAFE_CHECK(map_response, "list_api_use", Object)) {
        auto o = map_response["list_api_use"].GetObject();
        if (JSON_SAFE_CHECK(o, "api_ver_audio_book", Int)) {
            v_audio = o["api_ver_audio_book"].GetInt();
            MJDEFAULT->setIntegerForKey(key_api_version_audio_v2_fromServer, v_audio);

        }
        if (JSON_SAFE_CHECK(o, "api_ver_story", Int)) {
            v_story = o["api_ver_story"].GetInt();
            MJDEFAULT->setIntegerForKey(key_api_version_story_v2_fromJsonServer, v_story);
        }
        if (JSON_SAFE_CHECK(o, "api_ver_worksheet", Int)) {
            v_material = o["api_ver_worksheet"].GetInt();
            MJDEFAULT->setIntegerForKey(key_api_version_material_fromServer, v_material);
        }
    }

    if (JSON_SAFE_CHECK(map_response, "common_resource", Object)) {
        auto o = map_response["common_resource"].GetObject();
        auto version_common = o["version_common"].GetInt();
        auto path_common = o["path_common"].GetString();
        //         auto path_common = "http://datav2.daybehoc.com/platform/uploads/common-resource/COMMON_RESOURCE_51.zip";
        MJDEFAULT->setIntegerForKey(key_common_res_version_request, version_common);
        MJDEFAULT->setStringForKey(key_common_res_path, path_common);

    }

    if (JSON_SAFE_CHECK(map_response, "is_submit", Bool)) {
        auto is_submit = map_response["is_submit"].GetBool();
        MJDEFAULT->setBoolForKey(key_is_submit, is_submit);
    }

    if (JSON_SAFE_CHECK(map_response, "show_feature_license", Bool))
    {
        MJDEFAULT->setBoolForKey("is_license_feature_enable", map_response["show_feature_license"].GetBool());
    }

    if (JSON_SAFE_CHECK(map_response, "is_skip", Bool)) {
        MJDEFAULT->setBoolForKey(key_skip, map_response["is_skip"].GetBool());
    }
    else {
        MJDEFAULT->setBoolForKey(key_skip, false);
    }
    if (JSON_SAFE_CHECK(map_response, "feature_phonics", Bool)) {
        bool is_feature_phonics = map_response["feature_phonics"].GetBool();
        MJDEFAULT->setBoolForKey(key_feature_phonics, is_feature_phonics);
    }

    MJDEFAULT->setBoolForKey(key_feature_worksheet, true);

    if (JSON_SAFE_CHECK(map_response, "feature_worksheet", Bool)) {
        bool is_feature_worksheet = map_response["feature_worksheet"].GetBool();
        MJDEFAULT->setBoolForKey(key_feature_worksheet, true);
    }

    if (JSON_SAFE_CHECK(map_response, "is_vn", Bool)) {
        auto isServerVN = map_response["is_vn"].GetBool();
        MJDEFAULT->setBoolForKey(key_serverVN, isServerVN);
    }

    if (JSON_SAFE_CHECK(map_response, "is_licence", Bool)) {
        MJDEFAULT->setBoolForKey(key_is_licence, map_response["is_licence"].GetBool());
    }

    if (JSON_SAFE_CHECK(map_response, "app_ver", Int)) {
        MJDEFAULT->setStringForKey(key_version_api_app, StringUtils::toString(map_response["app_ver"].GetInt()));
    }

    if (JSON_SAFE_CHECK(map_response, "country_code", String)) {
        MJDEFAULT->setStringForKey(key_country_code, map_response["country_code"].GetString());
    }

    if (CONFIG_MANAGER->isDevEnvironment() || CONFIG_MANAGER->isProductEnvironment()) {

    }
    else {
        if (JSON_SAFE_CHECK(map_response, "use_url", String)) {
            std::vector<std::string> hosts;
            hosts.push_back(map_response["use_url"].GetString());
            MJDEFAULT->setStringForKey("use_url", hosts[0]);
            mj::Request::setHost(hosts);
        }
    }

    if (JSON_SAFE_CHECK(map_response, "pay_use_url", String)) {
        mj::Request::_onepay_url = map_response["pay_use_url"].GetString();
        //MJDEFAULT->setStringForKey("pay_use_url", mj::Request::_onepay_url);
    }

    if (JSON_SAFE_CHECK(map_response, "coupon_code", String)) {
        MJDEFAULT->setStringForKey(keyCoupon, map_response["coupon_code"].GetString());
    }

    if (JSON_SAFE_CHECK(map_response, "group_server", Array)) {
        auto server_ids = map_response["group_server"].GetArray();
        std::vector<int> ids_arr;
        for (auto i = 0; i < server_ids.Size(); i++) {
            ids_arr.push_back(server_ids[i].GetInt());
        }
        //            mj::GetLinkDownloadAPI::_server_ids = ids_arr;
        ms::ServerManager::getInstance().saveServerIds(ids_arr);
    }

    if (map_response.HasMember("version_update") && map_response["version_update"].IsString()) {
        auto ver_update_app = map_response["version_update"].GetString();
        MJDEFAULT->setStringForKey(key_ver_update_app, ver_update_app);
    }

    if (map_response.HasMember("version_required") && map_response["version_required"].IsString()) {
        auto version_required = map_response["version_required"].GetString();
        MJDEFAULT->setStringForKey(key_version_required, version_required);
    }
    if (JSON_SAFE_CHECK(map_response, "game_list", Object)) {
        auto gameListResponse = map_response["game_list"].GetObject();
        mj::api::MJAPIDecodeObjectJsonInfo::onDecodeGameListData(-1, gameListResponse);
    }
    if (JSON_SAFE_CHECK(map_response, "product_list", Object))
    {
        auto object = map_response["product_list"].GetObject();
        if (JSON_SAFE_CHECK(map_response, "VN", Array)) {
            rapidjson::Value list = map_response["VN"].GetArray();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            list.Accept(writer);
            auto json_string = std::string(buffer.GetString());
            MJDEFAULT->setStringForKey(keyPriceInfo, json_string);
            mj::Pay::getInstance().refresh(json_string);

        }

    }
    else {
        mj::Pay::getInstance().refresh();
    }

    if (JSON_SAFE_CHECK(map_response, "redirect_uri", String))
    {
        MJDEFAULT->setStringForKey("redirect_uri", map_response["redirect_uri"].GetString());
    }    
    return 0;
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLocationAndDeviceInfo(int tag, rapidjson::Document* document)
{
    auto map_response = document->GetObject();
    if (JSON_SAFE_CHECK(map_response, "device_id", String)) {
        auto device_id = map_response["device_id"].GetString();
        MJ_PLATFORM_CONFIG.setDeviceID(std::string(device_id));
    }
    if (JSON_SAFE_CHECK(map_response, "ip", String)) {
        auto ip = map_response["ip"].GetString();
        MJDEFAULT->setStringForKey(key_is_licence, ip);
    }
    if (JSON_SAFE_CHECK(map_response, "is_licence", Bool)) {
        auto is_licence = map_response["is_licence"].GetBool();
        MJDEFAULT->setBoolForKey(key_is_licence, is_licence);
    }
    if (JSON_SAFE_CHECK(map_response, "country_code", String)) {
        auto country_code = map_response["country_code"].GetString();
        MJDEFAULT->setStringForKey(key_country_code, country_code);
    }

    if (JSON_SAFE_CHECK(map_response, "is_rate", Bool)) {
        auto is_rate = map_response["is_rate"].GetBool();
        MJDEFAULT->setBoolForKey(key_rate_app_won, is_rate);
    }

    mj::api::MJAPIDecodeObjectJsonInfo::onDecodeDomainInfoData(tag, map_response);
    if (JSON_SAFE_CHECK(map_response, "is_skip", Bool)) {
        auto is_skip = map_response["is_skip"].GetBool();
        MJDEFAULT->setBoolForKey(IS_SKIP_KEY, is_skip);
    }
    if (JSON_SAFE_CHECK(map_response, "is_submit", Bool)) {
        auto is_submit = map_response["is_submit"].GetBool();
        MJDEFAULT->setBoolForKey(key_is_submit, is_submit);
    }

    if (JSON_SAFE_CHECK(map_response, "location", Object)) {
        auto location = map_response["location"].GetObject();
        mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLocationData(tag, location);
    }

    if (JSON_SAFE_CHECK(map_response, "path_install", String)) {
        auto path_install = map_response["path_install"].GetString();
        MJDEFAULT->setStringForKey(PATH_FIRST_INSTALL_KEY, path_install);
    }
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeDomainInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    if (JSON_SAFE_CHECK(map_response, "download_url_media", String)) {
        auto download_url_media = map_response["download_url_media"].GetString();
        MJDEFAULT->setStringForKey(DOMAIN_MEDIA_KEY, download_url_media);
    }

    if (JSON_SAFE_CHECK(map_response, "download_url_story", String)) {
        auto download_url_story = map_response["download_url_story"].GetString();
        MJDEFAULT->setStringForKey(DOMAIN_STORY_USING_KEY, download_url_story);
    }
    if (JSON_SAFE_CHECK(map_response, "download_url_game", String)) {
        auto download_url_game = map_response["download_url_game"].GetString();
        MJDEFAULT->setStringForKey(DOMAIN_GAME_USING_KEY, download_url_game);
    }
    if (JSON_SAFE_CHECK(map_response, "download_url_audio", String)) {
        auto download_url_audio = map_response["download_url_audio"].GetString();
        MJDEFAULT->setStringForKey(DOMAIN_AUDIO_USING_KEY, download_url_audio);
    }

    if (JSON_SAFE_CHECK(map_response, "download_url_activities", String)) {
        auto download_url_activities = map_response["download_url_activities"].GetString();
        MJDEFAULT->setStringForKey(DOMAIN_ACTIVITIES_USING_KEY, download_url_activities);
    }
    if (JSON_SAFE_CHECK(map_response, "download_url_media", String)) {
        auto download_url_media = map_response["download_url_media"].GetString();
        MJDEFAULT->setStringForKey(DOMAIN_MEDIA_USING_KEY, download_url_media);
    }
    
    bool show_monkey_talking = true;
        
    MJDEFAULT->setBoolForKey(SHOW_MONKEY_TALKING_KEY, PLATFORM_SUPPORT_FOR_AISPEAKING);

    if (JSON_SAFE_CHECK(map_response, "show_monkey_talking", Bool)) {
        
        show_monkey_talking = map_response["show_monkey_talking"].GetBool();
        
        MJDEFAULT->setBoolForKey(SHOW_MONKEY_TALKING_KEY, PLATFORM_SUPPORT_FOR_AISPEAKING && show_monkey_talking);
    }
    
    MJDEFAULT->setBoolForKey(SHOW_MONKEY_TALKING_KEY, false); // disable ai talking
    
    if (StoryData::getInstance()->isHdr) {
        if (JSON_SAFE_CHECK(map_response, "download_url_category_hdr", String)) {
            auto download_url_category_hdr = map_response["download_url_category_hdr"].GetString();
            MJDEFAULT->setStringForKey(DOMAIN_CATEGORY_URL_USING_KEY, download_url_category_hdr);
        }
    }
    else
    {
        if (JSON_SAFE_CHECK(map_response, "download_url_category_hd", String)) {
            auto download_url_category_hd = map_response["download_url_category_hd"].GetString();
            MJDEFAULT->setStringForKey(DOMAIN_CATEGORY_URL_USING_KEY, download_url_category_hd);
        }
    }


    if (JSON_SAFE_CHECK(map_response, "list_download_url_story", Array))
    {
        auto listUrlStories = map_response["list_download_url_story"].GetArray();

        std::vector<std::string> listUrl = {};

        for (int i = 0; i < listUrlStories.Size(); i++)
        {
            if (listUrlStories[i].IsString())
            {
                listUrl.push_back(listUrlStories[i].GetString());
                CCLOG("%d", listUrlStories[i].GetString());
            }

        }

        if (!listUrl.empty())
        {
            StoryDataModel::getInstance()->insertTableListDomainStories(listUrl, StoryDataModel::TYPEURL::STORY);
        }
    }

    if (JSON_SAFE_CHECK(map_response, "list_download_url_activities", Array))
    {
        auto listUrlAct = map_response["list_download_url_activities"].GetArray();

        std::vector<std::string> listUrl = {};

        for (int i = 0; i < listUrlAct.Size(); i++)
        {
            if (listUrlAct[i].IsString())
            {
                listUrl.push_back(listUrlAct[i].GetString());
                CCLOG("%d", listUrlAct[i].GetString());
            }
        }

        if (!listUrl.empty())
        {
            StoryDataModel::getInstance()->insertTableListDomainStories(listUrl, StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME);
        }
    }

}

void mj::api::MJAPIDecodeObjectJsonInfo::onSetDomainByLocation()
{
    std::string country_code = MJDEFAULT->getStringForKey(COUNTRY_CODE_HERE_KEY, "VN");
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    if (JSON_SAFE_CHECK(map_response, "story", Array)) {
            std::vector<mj::model::StoryInfo> list_stories;
            std::vector<std::pair<int, int>> list_story_level;
            std::vector<std::pair<int, int>> list_story_topic;
            std::vector<std::pair<int, int>> list_story_char;
            std::vector<std::pair<int, int>> list_id_version;
            std::vector<std::tuple<int, std::string, std::string>> list_link;
            std::vector<std::pair<std::string, int>> list_link_size;

            auto stories = map_response["story"].GetArray();

            std::vector<std::string> listThumbUrl = {};

            for (auto i = 0; i < stories.Size(); i++) {
                auto story = &stories[i];


                auto so = story->GetObject();
                std::string link_hd = "";
                std::string link_hdr = "";
                if (JSON_SAFE_CHECK(so, "download_link", String)) {
                    link_hd = so["download_link"].GetString();
                }


                if (JSON_SAFE_CHECK(so, "download_link_hd_size", Number)) {
                    int size = so["download_link_hd_size"].GetFloat();
                    if (size != 0)
                    {
                        list_link_size.push_back(std::make_pair(link_hd, size));
                    }
                };

                if (JSON_SAFE_CHECK(so, "download_link", String)) {
                    link_hdr = so["download_link"].GetString();
                }

                if (JSON_SAFE_CHECK(so, "download_link_hdr_size", Number)) {
                    int size = so["download_link_hdr_size"].GetFloat();
                    if (size != 0)
                    {
                        list_link_size.push_back(std::make_pair(link_hdr, size));
                    }
                };

                if (JSON_SAFE_CHECK(so, "story_id", String)) {
                    std::string storyID = so["story_id"].GetString();
                    if (JSON_SAFE_CHECK(so, "version_story", String)) {
                        std::string versionNumber = so["version_story"].GetString();
                        list_id_version.push_back({ std::atoi(versionNumber.c_str()), std::atoi(storyID.c_str()) });
                    }

                }

                auto info = mj::model::StoryInfo(so);
                listThumbUrl.push_back(info.thumbnail_url);
                list_stories.push_back(info);

                list_link.push_back(std::make_tuple(info.unique_id, link_hd, link_hdr));


                for (auto l : info.levels) {
                    list_story_level.push_back(std::make_pair(info.unique_id, l));
                }

                for (auto t : info.categories) {
                    list_story_topic.push_back(std::make_pair(info.unique_id, t));
                }

                for (auto c : info.characters) {
                    list_story_char.push_back(std::make_pair(info.unique_id, c));
                }


                // insert to db if size of too much 

                if (list_id_version.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->updateStoriesListByVersion(list_id_version);
                    list_id_version.clear();
                }
                if (list_stories.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->insertStoryWithData(list_stories);
                    list_stories.clear();
                }
                if (list_story_level.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->insertStoryLevelRelation(list_story_level);
                    list_story_level.clear();
                }
                if (list_story_char.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->insertStoryCharaRelation(list_story_char);
                    list_story_char.clear();
                }
                if (list_story_topic.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->insertStoryTopicRelation(list_story_topic);
                    list_story_topic.clear();
                }
                if (list_link_size.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->updateFileSize(list_link_size);
                    list_link_size.clear();
                }
            }
            if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update)
            {
                vector<int> updateStoryIds;
                for (int i = 0; i < list_stories.size(); i++)
                {
                    updateStoryIds.push_back(list_stories.at(i).unique_id);
                }
                std::ostringstream vts;
                std::copy(updateStoryIds.begin(), updateStoryIds.end(),
                    std::ostream_iterator<int>(vts, ", "));

                MJDEFAULT->setStringForKey(key_update_story_ids, vts.str());
            }
            StoryDataModel::getInstance()->updateStoriesListByVersion(list_id_version);
            StoryDataModel::getInstance()->insertStoryWithData(list_stories);
            StoryDataModel::getInstance()->insertStoryLevelRelation(list_story_level);
            StoryDataModel::getInstance()->insertStoryCharaRelation(list_story_char);
            StoryDataModel::getInstance()->insertStoryTopicRelation(list_story_topic);
            StoryDataModel::getInstance()->setLinkDownloadStory(list_link);
            StoryDataModel::getInstance()->updateFileSize(list_link_size);
            if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update)
            {
                loadThumbUrl(listThumbUrl, RemoteImageView::Type::STORIES);
            }
    }

    if (JSON_SAFE_CHECK(map_response, "delete", Array)) {
        std::vector<int> ids;
        auto ids_to_delete = map_response["delete"].GetArray();
        for (auto i = 0; i < ids_to_delete.Size(); ++i) {
            if (ids_to_delete[i].IsInt())
                ids.push_back(ids_to_delete[i].GetInt());
        }
        StoryDataModel::getInstance()->deleteStories(ids);
    }

    if (JSON_SAFE_CHECK(map_response, "free_story", Bool)) {
        auto freeStoryIsExits = map_response["free_story"].GetBool();
        MJDEFAULT->setBoolForKey(key_is_free_story, freeStoryIsExits);
    }

    if (MJDEFAULT->getBoolForKey(key_is_free_story, false))
    {
        onDecodeFreBookAndFreeStory(1, map_response);
        //if (JSON_SAFE_CHECK(map_response, "free_story", Object)) {
        //    auto fso = map_response["free_story"].GetObject();
        //    std::unordered_map<int, std::vector<int> > fsm;
        //    for (auto j = fso.MemberBegin(); j != fso.MemberEnd(); ++j) {
        //        if (!j->value.IsArray()) {
        //            continue;
        //        }
        //        auto lang_id = ATOI(j->name.GetString());
        //        auto fsa = j->value.GetArray();
        //        std::vector<int> t;
        //        for (auto u = 0; u < fsa.Size(); ++u) {
        //            if (!fsa[u].IsInt())
        //                continue;
        //            t.push_back(fsa[u].GetInt());
        //        }
        //        fsm.emplace(lang_id, t);
        //    }
        //    FREE_STORY_MANAGER->setListFreeStoriesOfDay(fsm);
        //}
    }
   
    if (JSON_SAFE_CHECK(map_response, "version_story", Int)) {
        auto ver_api_level = map_response["version_story"].GetInt();
        if (ver_api_level > 0) {
            if(tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install && ver_api_level> MJDEFAULT->getIntegerForKey(key_api_version_story_fisrt_install, -1))
                MJDEFAULT->setIntegerForKey(key_api_version_story_fisrt_install,ver_api_level);
            MJDEFAULT->setStringForKey(key_api_version, StringUtils::toString(ver_api_level));
            MJDEFAULT->setIntegerForKey(key_api_version_story_local, ver_api_level);
            MJDEFAULT->setIntegerForKey(key_api_version_story_v2_fromJsonServer, ver_api_level);
        }
    }

    if (JSON_SAFE_CHECK(map_response, "popular_search", Array)) {

        auto booksArr = map_response["popular_search"].GetArray();

        std::vector< mj::model::PopularSearchStoryInfo> books;

        for (auto i = 0; i < booksArr.Size(); ++i) {

            auto obj = booksArr[i].GetObject();

            mj::model::PopularSearchStoryInfo book;

            book.id = JSON_SAFE_EXTRACT(obj, "id", Int, -1);
            book.keyword = JSON_SAFE_EXTRACT(obj, "keyword", String, "");
            book.thumbnailUrl = JSON_SAFE_EXTRACT(obj, "thumb", String, "");
            book.sort = JSON_SAFE_EXTRACT(obj, "sort", Int, -1);
            book.date_publish = JSON_SAFE_EXTRACT(obj, "date_publish", Int, -1);
            //book.type = JSON_SAFE_EXTRACT(obj, "type", Int, -1);
            books.push_back(book);
        }
        if(books.size()>0)
            StoryDataModel::getInstance()->setPopularSearchStories(books);
    }
    if (JSON_SAFE_CHECK(map_response, "all_level", Array)) {

        //update danh s�ch level
        auto lang_id = LANGUAGE_MANAGER->getCurrentLangId();
        auto list_levels = map_response["all_level"].GetArray();
        std::vector<int> lv;
        for (auto i = 0; i < list_levels.Size(); i++) {
            if (list_levels[i].IsInt()) {
                lv.push_back(list_levels[i].GetInt());
            }
        }

        MJDEFAULT->setStringForKey(StringUtils::format("%s%d", key_level_listlevellang, lang_id), mj::helper::vectorIntToString(lv));
    }

#pragma region detect homedata
    {
        //begin LevelInfo API "grade", "description", "level"
        std::vector<mj::model::LevelInfo> levels;
        std::vector<mj::model::GradeInfo> grades;
        std::unordered_map<int, std::pair<int, int> > grade_id_map;
        mj::api::HomeResponse response;
        auto data = map_response;

        if (JSON_SAFE_CHECK(data, "category", Array)) {
            auto categories = data["category"].GetArray();
            for (auto i = 0; i < categories.Size(); i++) {
                if (categories[i].IsObject()) {
                    response.categories.push_back(mj::model::CategoryInfo(categories[i].GetObject()));
                }
            }
            if(response.categories.size()>0)
                StoryDataModel::getInstance()->updateCategories(response.categories);
        }

        if (JSON_SAFE_CHECK(data, "character", Array)) {
            auto characters = data["character"].GetArray();
            for (auto i = 0; i < characters.Size(); ++i) {
                if (characters[i].IsObject()) {
                    response.characters.push_back(mj::model::CharacterInfo(characters[i].GetObject()));
                }
            }
            if(response.characters.size() >0)
                StoryDataModel::getInstance()->updateCharacters(response.characters);
        }

        if (JSON_SAFE_CHECK(data, "feature", Array)) {
            auto features = data["feature"].GetArray();
            for (auto i = 0; i < features.Size(); ++i) {
                if (features[i].IsObject()) {
                    std::pair<int, std::string> info;
                    auto feature = features[i].GetObject();
                    if (JSON_SAFE_CHECK(feature, "id", Int)) {
                        info.first = feature["id"].GetInt();
                    }

                    if (JSON_SAFE_CHECK(feature, "image_feature", String)) {
                        info.second = feature["image_feature"].GetString();
                    }

                    response.features.push_back(info);
                }
            }
            StoryDataModel::getInstance()->insertFeatureStories(response.features);
        }

        if (JSON_SAFE_CHECK(data, "grade", Object)) {
            auto grades_obj = data["grade"].GetObject();
            for (auto i = grades_obj.MemberBegin(); i != grades_obj.MemberEnd(); ++i) {
                if (!i->value.IsArray()) {
                    continue;
                }
                auto lang_id = ATOI(i->name.GetString());
                auto grade_lang_arr = i->value.GetArray();
                for (auto c = 0; c < grade_lang_arr.Size(); c++) {
                    if (!grade_lang_arr[c].IsObject()) {
                        continue;
                    }
                    auto grade_info_obj = grade_lang_arr[c].GetObject();

                    mj::model::GradeInfo grade;
                    if (JSON_SAFE_CHECK(grade_info_obj, "name", String)) {
                        grade.name = grade_info_obj["name"].GetString();
                    }
                    if (JSON_SAFE_CHECK(grade_info_obj, "des", String)) {
                        grade.description = grade_info_obj["des"].GetString();
                    }
                    if (JSON_SAFE_CHECK(grade_info_obj, "order", Int)) {
                        grade.grade_id = grade_info_obj["order"].GetInt();
                    }
                    grade.lang_id = lang_id;
                    grades.push_back(grade);

                    if (JSON_SAFE_CHECK(grade_info_obj, "id", Int)) {
                        grade_id_map.insert({ grade_info_obj["id"].GetInt(), std::make_pair(grade.grade_id, grade.lang_id) });
                    }
                }
            }
            StoryDataModel::getInstance()->insertGradeInfo(grades);
        }

        if (JSON_SAFE_CHECK(data, "level", Object)) {
            auto levels_obj = data["level"].GetObject();
            for (auto i = levels_obj.MemberBegin(); i != levels_obj.MemberEnd(); ++i) {
                if (!i->value.IsArray()) {
                    continue;
                }

                auto lang_id = ATOI(i->name.GetString());
                auto level_arr = i->value.GetArray();
                for (auto c = 0; c < level_arr.Size(); ++c) {
                    if (!level_arr[c].IsObject()) {
                        continue;
                    }

                    auto level_obj = level_arr[c].GetObject();
                    mj::model::LevelInfo level;
                    if (JSON_SAFE_CHECK(level_obj, "id", Int)) {
                        level.level_id = level_obj["id"].GetInt();
                    }
                    if (JSON_SAFE_CHECK(level_obj, "name", String)) {
                        level.name = level_obj["name"].GetString();
                    }
                    if (JSON_SAFE_CHECK(level_obj, "grade_id", Int)) {
                        auto temp_id = level_obj["grade_id"].GetInt();
                        if (grade_id_map.count(temp_id)) {
                            auto pair_id = grade_id_map[temp_id];
                            level.grade_id = pair_id.first;
                        }
                        else {
                            level.grade_id = 0;
                        }
                    }
                    level.lang_id = lang_id;
                    levels.push_back(level);
                }
            }
            StoryDataModel::getInstance()->insertLevelInfo(levels);
        }

        if (JSON_SAFE_CHECK(data, "description", Array)) {
            auto levels_description_arr = data["description"].GetArray();
            for (auto i = 0; i < levels_description_arr.Size(); ++i) {
                if (!levels_description_arr[i].IsObject()) {
                    continue;
                }

                auto level_description = levels_description_arr[i].GetObject();
                auto level_id = 0;
                auto lang_id = 0;
                auto description = std::string("");
                if (JSON_SAFE_CHECK(level_description, "order", Int)) {
                    level_id = level_description["order"].GetInt();
                }
                if (JSON_SAFE_CHECK(level_description, "lang_display_id", Int)) {
                    lang_id = level_description["lang_display_id"].GetInt();
                };
                if (JSON_SAFE_CHECK(level_description, "description", String)) {
                    description = level_description["description"].GetString();
                };

                for (auto& level : levels) {
                    if (level.lang_id == lang_id && level.level_id == level_id) {
                        level.description = description;
                    }
                }
            }
        }

        if (JSON_SAFE_CHECK(data, "list_language_display", Object)) {
            auto levels_obj = data["list_language_display"].GetObject();
        }

    }
#pragma endregion

    
    

    ms::GradeManager::getInstance().updateGrades();
}
void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeF2PTrialItemsData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    auto mapResponse = map_response;
    vector<mj::model::FreeItemInfo> freeItemInfosFinal;
    if (JSON_SAFE_CHECK(mapResponse, "last_version", Int)) {
        int f2PVersion = mapResponse["last_version"].GetInt();
        MJDEFAULT->setIntegerForKey(key_api_version_free_trial, f2PVersion);
    }
    
    if (JSON_SAFE_CHECK(mapResponse, "age", Int)) {
        int f2PAge = mapResponse["age"].GetInt();
        MJDEFAULT->setIntegerForKey(key_api_age_free_trial, f2PAge);
    }
    if (JSON_SAFE_CHECK(mapResponse, "option_1_item_stories", Array)) {
        std::string option_1_item_stories = "option_1_item_stories";
        vector<mj::model::FreeItemInfo> freeItemInfos = onDecodeF2PTrialItemsDataOptionItem(tag, mapResponse, option_1_item_stories.c_str(), FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY);
        freeItemInfosFinal.insert(freeItemInfosFinal.end(), freeItemInfos.begin(), freeItemInfos.end());

    }
    if (JSON_SAFE_CHECK(mapResponse, "option_1_item_others", Array)) {
        std::string option_1_item_others = "option_1_item_others";
        vector<mj::model::FreeItemInfo> freeItemInfos = onDecodeF2PTrialItemsDataOptionItem(tag, mapResponse, option_1_item_others.c_str(), FreeContentOption::MONO_ITEM_ONE_DAY_RADOM);
        freeItemInfosFinal.insert(freeItemInfosFinal.end(), freeItemInfos.begin(), freeItemInfos.end());

    }

    if (JSON_SAFE_CHECK(mapResponse, "option_2_items", Array)) {
        std::string option_2_items = "option_2_items";
        vector<mj::model::FreeItemInfo> freeItemInfos = onDecodeF2PTrialItemsDataOptionItem(tag, mapResponse, option_2_items.c_str(), FreeContentOption::MULTILE_ITEM_ONE_DAY);
        freeItemInfosFinal.insert(freeItemInfosFinal.end(), freeItemInfos.begin(), freeItemInfos.end());
    }

    if (JSON_SAFE_CHECK(mapResponse, "free_items", Array)) {
        std::string option_10item_free_unlock = "free_items";
        vector<mj::model::FreeItemInfo> freeItemInfos = onDecodeF2PTrialItemsDataOptionItem(tag, mapResponse, option_10item_free_unlock.c_str(), FreeContentOption::TEN_ITEM_FREE);
        freeItemInfosFinal.insert(freeItemInfosFinal.end(), freeItemInfos.begin(), freeItemInfos.end());

    }

    if (freeItemInfosFinal.size() > 0)
    {
        StoryDataModel::getInstance()->deleteFreeTrialInfosTable();
        StoryDataModel::getInstance()->insertFreeTrialInfo(freeItemInfosFinal);
    }
}
std::vector<mj::model::FreeItemInfo> mj::api::MJAPIDecodeObjectJsonInfo::onDecodeF2PTrialItemsDataOptionItem(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response, const char* subOptionItems, FreeContentOption option)
{
    std::vector<mj::model::FreeItemInfo> freeItemInfos;
    if (JSON_SAFE_CHECK(map_response, subOptionItems, Array)) {
        auto option_1_item_stories = map_response[subOptionItems].GetArray();
        for (auto i = 0; i < option_1_item_stories.Size(); ++i) {
            if (option_1_item_stories[i].IsObject()) {
                auto dataItem = option_1_item_stories[i].GetObject();

                if (JSON_SAFE_CHECK(dataItem, "story_id", Int))
                {
                    mj::model::FreeItemInfo freeItemInfoStory;
                    freeItemInfoStory._id = JSON_SAFE_CHECK(dataItem, "story_id", Int) ? dataItem["story_id"].GetInt() : freeItemInfoStory._id;
                    freeItemInfoStory._type = DocumentsType::STORY;
                    freeItemInfoStory._day = i + 1;
                    freeItemInfoStory._option = option;
                    freeItemInfos.push_back(freeItemInfoStory);
                }
                if (JSON_SAFE_CHECK(dataItem, "item_id", Int))
                {
                    mj::model::FreeItemInfo freeItemInfo;
                    freeItemInfo._id = JSON_SAFE_CHECK(dataItem, "item_id", Int) ? dataItem["item_id"].GetInt() : freeItemInfo._id;
                    std::string typeStr = JSON_SAFE_CHECK(dataItem, "type", String) ? dataItem["type"].GetString() : "";
                    if (typeStr == "story")
                        freeItemInfo._type = DocumentsType::STORY;
                    else if (typeStr == "phonics") {
                        freeItemInfo._type = DocumentsType::PHONICS;
                    }
                    else if (typeStr == "reading") {
                        freeItemInfo._type = DocumentsType::READING_COMPREHENTION;
                    }
                    freeItemInfo._day = i + 1;
                    freeItemInfo._option = option;
                    freeItemInfos.push_back(freeItemInfo);                }

            }
            
        }
    }
    return freeItemInfos;
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    auto mapResponse = map_response;
    if (JSON_SAFE_CHECK(mapResponse, "speakingTopicList", Object)) {
        auto speakingTopicListObject = mapResponse["speakingTopicList"].GetObject();
        onDecodeAISpeakingTopicData(tag,speakingTopicListObject);
    }
    if (JSON_SAFE_CHECK(mapResponse, "lessonList", Object)) {
        auto speakingLessonListObject = mapResponse["lessonList"].GetObject();
        onDecodeAISpeakingLessonListData(tag, speakingLessonListObject);
    }
    if (JSON_SAFE_CHECK(mapResponse, "itemList", Array)) {
        onDecodeAISpeakingActivitiesListData(tag, mapResponse);
    }
    if (JSON_SAFE_CHECK(mapResponse, "common_mk_talking", Object)) {
        auto speakingCommonListObject = mapResponse["common_mk_talking"].GetObject();
        onDecodeAISpeakingCommonInfoListData(tag, speakingCommonListObject);
    }
    
}
std::vector<mj::model::AISpeakingCategoryInfo> mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingTopicData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    
    auto speakingTopicListObject = map_response;
    std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingTopicInfosReturn;
    int speakingTopicListVersion = JSON_SAFE_CHECK(speakingTopicListObject, "version_categories", Int) ? speakingTopicListObject["version_categories"].GetInt() : 0;

    if (speakingTopicListVersion > 0)
    {
        if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install && speakingTopicListVersion > MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fisrt_install, -1))
        {
            MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_topic_fisrt_install, speakingTopicListVersion);
        }
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_topic_local, speakingTopicListVersion);
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, speakingTopicListVersion);
    }

    if (JSON_SAFE_CHECK(speakingTopicListObject, "remove_categories_list", Array)) {
        std::vector<int> topicIDListDelete;
        auto dataIDListDelete = speakingTopicListObject["remove_categories_list"].GetArray();
        for (auto i = 0; i < dataIDListDelete.Size(); ++i) {
            if (dataIDListDelete[i].IsInt())
                topicIDListDelete.push_back(dataIDListDelete[i].GetInt());
        }
        if (topicIDListDelete.size() > 0)
        {
            StoryDataModel::getInstance()->removeAiSpeakingTopicById(topicIDListDelete);
        }
    }

    if (JSON_SAFE_CHECK(speakingTopicListObject, "categories_list", Array)) {
        auto dataList = speakingTopicListObject["categories_list"].GetArray();
        std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingTopicInfos;
        for (auto i = 0; i < dataList.Size(); ++i) {
            if (dataList[i].IsObject()) {
                auto dataItem = dataList[i].GetObject();
                mj::model::AISpeakingCategoryInfo aISpeakingTopicInfo;
                aISpeakingTopicInfo.topic_id = JSON_SAFE_CHECK(dataItem, "id", Int) ? dataItem["id"].GetInt() : aISpeakingTopicInfo.topic_id;
                aISpeakingTopicInfo.topic_name = JSON_SAFE_CHECK(dataItem, "name", String) ? dataItem["name"].GetString() : aISpeakingTopicInfo.topic_name;
                aISpeakingTopicInfo.thumb_url = JSON_SAFE_CHECK(dataItem, "thumb", String) ? dataItem["thumb"].GetString() : aISpeakingTopicInfo.thumb_url;
                aISpeakingTopicInfo.free = JSON_SAFE_CHECK(dataItem, "free", Int) ? dataItem["free"].GetInt() : aISpeakingTopicInfo.free;
                aISpeakingTopicInfo.status = JSON_SAFE_CHECK(dataItem, "status", Int) ? dataItem["status"].GetInt() : aISpeakingTopicInfo.status;
                aISpeakingTopicInfo.cat_version = JSON_SAFE_CHECK(dataItem, "cat_version", Int) ? dataItem["cat_version"].GetInt() : aISpeakingTopicInfo.cat_version;
                aISpeakingTopicInfo.order_by = JSON_SAFE_CHECK(dataItem, "order_by", Int) ? dataItem["order_by"].GetInt() : aISpeakingTopicInfo.order_by;
                aISpeakingTopicInfo.icon_zip_url = JSON_SAFE_CHECK(dataItem, "path", String) ? dataItem["path"].GetString() : aISpeakingTopicInfo.icon_zip_url;
                if (JSON_SAFE_CHECK(dataItem, "audio", Object))
                { 
                    auto audioObject = dataItem["audio"].GetObject();
                    if (JSON_SAFE_CHECK(audioObject, "image", Object)) {
                        auto iconInfoItem = audioObject["image"].GetObject();
                        aISpeakingTopicInfo.iconInfo.icon_thumb = JSON_SAFE_CHECK(iconInfoItem, "thumb", String) ? iconInfoItem["thumb"].GetString() : aISpeakingTopicInfo.iconInfo.icon_thumb;
                        aISpeakingTopicInfo.iconInfo.check_icon = JSON_SAFE_CHECK(iconInfoItem, "check_icon", String) ? iconInfoItem["check_icon"].GetString() : aISpeakingTopicInfo.iconInfo.check_icon;
                        aISpeakingTopicInfo.iconInfo.disable_icon = JSON_SAFE_CHECK(iconInfoItem, "disable", String) ? iconInfoItem["disable"].GetString() : aISpeakingTopicInfo.iconInfo.disable_icon;
                        aISpeakingTopicInfo.iconInfo.progress_bg_icon = JSON_SAFE_CHECK(iconInfoItem, "progress_bg", String) ? iconInfoItem["progress_bg"].GetString() : aISpeakingTopicInfo.iconInfo.progress_bg_icon;
                        aISpeakingTopicInfo.iconInfo.selected_circle_icon = JSON_SAFE_CHECK(iconInfoItem, "selected_circle", String) ? iconInfoItem["selected_circle"].GetString() : aISpeakingTopicInfo.iconInfo.selected_circle_icon;
                        aISpeakingTopicInfo.iconInfo.design_icon = JSON_SAFE_CHECK(iconInfoItem, "design", String) ? iconInfoItem["design"].GetString() : aISpeakingTopicInfo.iconInfo.design_icon;
                    }
                }
             
                aISpeakingTopicInfos.push_back(aISpeakingTopicInfo);
                aISpeakingTopicInfosReturn.push_back(aISpeakingTopicInfo);
            }
            if (aISpeakingTopicInfos.size() >= maxSizeInsertDB) {
                StoryDataModel::getInstance()->insertAiSpeakingTopic(aISpeakingTopicInfos);
                aISpeakingTopicInfos.clear();
            }
            
        }
        StoryDataModel::getInstance()->insertAiSpeakingTopic(aISpeakingTopicInfos);
    }
    return aISpeakingTopicInfosReturn;
}
void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingLessonListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    auto speakingLessonListObject = map_response;

    int speakingLessonListVersion = JSON_SAFE_CHECK(speakingLessonListObject, "version", Int) ? speakingLessonListObject["version"].GetInt() : 0;

    if (speakingLessonListVersion > 0)
    {
        if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install && speakingLessonListVersion > MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fisrt_install, -1))
        {
            MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_lesson_list_fisrt_install, speakingLessonListVersion);
        }
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_lesson_list_local, speakingLessonListVersion);
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, speakingLessonListVersion);
    }

    if (JSON_SAFE_CHECK(speakingLessonListObject, "remove_list", Array)) {
        std::vector<int> itemsIDListDelete;
        auto dataIDListDelete = speakingLessonListObject["remove_list"].GetArray();
        for (auto i = 0; i < dataIDListDelete.Size(); ++i) {
            if (dataIDListDelete[i].IsInt())
                itemsIDListDelete.push_back(dataIDListDelete[i].GetInt());
        }
        if (itemsIDListDelete.size() > 0)
        {
            StoryDataModel::getInstance()->removeAiSpeakingLessonByIds(itemsIDListDelete);
        }

    }

    if (JSON_SAFE_CHECK(speakingLessonListObject, "list", Array)) {
        auto dataList = speakingLessonListObject["list"].GetArray();
        std::vector<mj::model::AISpeakingLessonInfo> aISpeakingLessonInfos;
        std::vector<int> lessonAISpeakingIds;
        for (auto i = 0; i < dataList.Size(); ++i) {
            if (dataList[i].IsObject()) {
                auto dataItem = dataList[i].GetObject();
                mj::model::AISpeakingLessonInfo aISpeakingLessonInfo;

                aISpeakingLessonInfo.lesson_id = JSON_SAFE_CHECK(dataItem, "id", Int) ? dataItem["id"].GetInt() : aISpeakingLessonInfo.lesson_id;
                aISpeakingLessonInfo.lesson_order = JSON_SAFE_CHECK(dataItem, "order_by", Int) ? dataItem["order_by"].GetInt() : aISpeakingLessonInfo.lesson_order;
                aISpeakingLessonInfo.lang_id = JSON_SAFE_CHECK(dataItem, "lang_id", Int) ? dataItem["lang_id"].GetInt() : 1;
                aISpeakingLessonInfo.name = JSON_SAFE_CHECK(dataItem, "title", String) ? dataItem["title"].GetString() : aISpeakingLessonInfo.name;
                aISpeakingLessonInfo.thumbnail_url = JSON_SAFE_CHECK(dataItem, "path_thumb", String) ? dataItem["path_thumb"].GetString() : aISpeakingLessonInfo.thumbnail_url;
                aISpeakingLessonInfo.category_id = JSON_SAFE_CHECK(dataItem, "game_category_id", Int) ? dataItem["game_category_id"].GetInt() : aISpeakingLessonInfo.category_id;
                aISpeakingLessonInfo.state = static_cast<mj::model::LessonInfo::State>(JSON_SAFE_CHECK(dataItem, "status", Int) ? dataItem["status"].GetInt() : aISpeakingLessonInfo.state);
                aISpeakingLessonInfo.time_published = JSON_SAFE_CHECK(dataItem, "time_publish", Int) ? dataItem["time_publish"].GetInt() : aISpeakingLessonInfo.time_published;
                aISpeakingLessonInfo._timeLearnedExpected = 0;
                aISpeakingLessonInfos.push_back(aISpeakingLessonInfo);
                lessonAISpeakingIds.push_back(aISpeakingLessonInfo.lesson_id);

                if (aISpeakingLessonInfos.size() >= maxSizeInsertDB) {
                    StoryDataModel::getInstance()->insertAiSpeakingLessons(aISpeakingLessonInfos);
                    aISpeakingLessonInfos.clear();
                }

            }
        }
        std::string lessonAISpeakingIdsStr = mj::model::ConvertArrayIntergerToString(lessonAISpeakingIds);
        MJDEFAULT->setStringForKey(key_update_lesson_ai_speking_ids, lessonAISpeakingIdsStr);
       
        StoryDataModel::getInstance()->insertAiSpeakingLessons(aISpeakingLessonInfos);

    }
}
void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingActivitiesListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    auto mapResponse = map_response;
    if (JSON_SAFE_CHECK(mapResponse, "itemList", Array)) {
        auto speakingActivitiesListObject = mapResponse["itemList"].GetArray();
        onDecodeStoryLessonInteractiveData(tag, speakingActivitiesListObject);
    }
}
std::vector<mj::model::TalkingCommonInfo> mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingCommonInfoListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    auto speakingCommonInfoListObject = map_response;

    int speakingCommonListVersion = JSON_SAFE_CHECK(speakingCommonInfoListObject, "version", Int) ? speakingCommonInfoListObject["version"].GetInt() : 0;
    std::vector<mj::model::TalkingCommonInfo>returnTalkingCommonInfos;
    if (speakingCommonListVersion > 0)
    {
        if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install && speakingCommonListVersion > MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_common_list_fisrt_install, -1))
        {
            MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_common_list_fisrt_install, speakingCommonListVersion);
        }
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_common_list_local, speakingCommonListVersion);
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_common_list_fromJsonServer, speakingCommonListVersion);
    }

    if (JSON_SAFE_CHECK(speakingCommonInfoListObject, "data", Array)) {
        auto dataList = speakingCommonInfoListObject["data"].GetArray();
        if (dataList.Size() > 0)
        {// need delete all data common info before update DB again
            StoryDataModel::getInstance()->deleteAiSpeakingCommonInfoTable();
        }
        std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos;
        for (auto i = 0; i < dataList.Size(); ++i) {
            if (dataList[i].IsObject()) {
                auto dataItem = dataList[i].GetObject();
                std::string actorName = JSON_SAFE_CHECK(dataItem, "actor_name", String) ? dataItem["actor_name"].GetString() : "";
                actorName = mj::helper::to_lower(actorName);
                if (JSON_SAFE_CHECK(dataItem, "version", Array)) {
                    auto versionInfoList = dataItem["version"].GetArray();
                    for (auto j = 0; j < versionInfoList.Size(); ++j) {
                        if (versionInfoList[j].IsObject()) {
                            mj::model::TalkingCommonInfo talkingCommonInfo;
                            auto versionInfoItem = versionInfoList[j].GetObject();
                            talkingCommonInfo.actor_name = actorName;
                            talkingCommonInfo.version_name = JSON_SAFE_CHECK(versionInfoItem, "version_name", String) ? versionInfoItem["version_name"].GetString() : "";
                            talkingCommonInfo.zipPath = JSON_SAFE_CHECK(versionInfoItem, "path", String) ? versionInfoItem["path"].GetString() : "";

                            if (JSON_SAFE_CHECK(versionInfoItem, "video_type", Array)) {
                            
                                readAISpeakingCommonInfoActorVideoURL(versionInfoItem["video_type"].GetArray(),"link",talkingCommonInfo);

                            }
                            if(talkingCommonInfo.zipPath.empty())
                                continue;
                            talkingCommonInfos.push_back(talkingCommonInfo);
                            returnTalkingCommonInfos.push_back(talkingCommonInfo);
                            if (talkingCommonInfos.size() >= maxSizeInsertDB) {
                                StoryDataModel::getInstance()->insertAiSpeakingCommonInfo(talkingCommonInfos);
                                talkingCommonInfos.clear();
                            }
                        }

                    }
                }

            }
        }

        StoryDataModel::getInstance()->insertAiSpeakingCommonInfo(talkingCommonInfos);
    }
    return returnTalkingCommonInfos;
}

void mj::api::MJAPIDecodeObjectJsonInfo::readAISpeakingCommonInfoActorVideoURL(rapidjson::GenericArray<false, rapidjson::Value> videoInfoList,std::string childKey,mj::model::TalkingCommonInfo &talkingCommonInfo, std::string dir){

    
    for (auto k = 0; k < videoInfoList.Size(); ++k) {
        if (videoInfoList[k].IsObject()) {
            
            auto videoInfoItem = videoInfoList[k].GetObject();
            
            std::string videoKey = JSON_SAFE_CHECK(videoInfoItem, "video_key", String) ? videoInfoItem["video_key"].GetString() : "";
            if (videoKey == "intro")
            {
                talkingCommonInfo.intro = JSON_SAFE_CHECK(videoInfoItem, childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                
                talkingCommonInfo.intro = dir!=""? dir + talkingCommonInfo.intro : talkingCommonInfo.intro;
            }
            else if (videoKey == "intro_mimic")
            {
                talkingCommonInfo.intro_mimic = JSON_SAFE_CHECK(videoInfoItem, childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                talkingCommonInfo.intro_mimic = dir!=""? dir + talkingCommonInfo.intro_mimic : talkingCommonInfo.intro_mimic;
                
            }
            else if (videoKey == "intro_talking")
            {
                talkingCommonInfo.intro_talking = JSON_SAFE_CHECK(videoInfoItem,childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                talkingCommonInfo.intro_talking = dir!=""? dir + talkingCommonInfo.intro_talking : talkingCommonInfo.intro_talking;
                
            }
            else if (videoKey == "your_turn")
            {
                talkingCommonInfo.your_turn = JSON_SAFE_CHECK(videoInfoItem, childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                talkingCommonInfo.your_turn = dir!=""? dir + talkingCommonInfo.your_turn : talkingCommonInfo.your_turn;
                
            }
            else if (videoKey == "cheer_up")
            {
                talkingCommonInfo.cheer_up = JSON_SAFE_CHECK(videoInfoItem, childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                talkingCommonInfo.cheer_up = dir!=""? dir + talkingCommonInfo.cheer_up : talkingCommonInfo.cheer_up;
                
            }
            else if (videoKey == "listening")
            {
                talkingCommonInfo.listening = JSON_SAFE_CHECK(videoInfoItem, childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                talkingCommonInfo.listening = dir!=""? dir + talkingCommonInfo.listening : talkingCommonInfo.listening;
                
            }
            else if (videoKey == "outro_mimic")
            {
                talkingCommonInfo.outro_mimic = JSON_SAFE_CHECK(videoInfoItem, childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                talkingCommonInfo.outro_mimic = dir!=""? dir + talkingCommonInfo.outro_mimic : talkingCommonInfo.outro_mimic;
                
            }
            else if (videoKey == "outro")
            {
                talkingCommonInfo.outro = JSON_SAFE_CHECK(videoInfoItem, childKey.c_str(), String) ? videoInfoItem[childKey.c_str()].GetString() : "";
                talkingCommonInfo.outro = dir!=""? dir + talkingCommonInfo.outro : talkingCommonInfo.outro;
            }
            
        }
    }
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAudioData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    auto mapResponse = map_response;

    if (JSON_SAFE_CHECK(mapResponse, "list_audio_book", Array)) {
        auto data = mapResponse["list_audio_book"].GetArray();
        std::vector<mj::model::AudiobookInfo> books;
        std::vector<std::pair<std::string, int>> list_size;
        std::vector<std::tuple<int, int, int>> chapter_list;
        std::vector<std::string> listUrlThumb = {};
        for (auto i = 0; i < data.Size(); ++i) {
            if (data[i].IsObject()) {
                auto block = data[i].GetObject();
                mj::model::AudiobookInfo info;
                info.book_id = JSON_SAFE_CHECK(block, "id", Int) ? block["id"].GetInt() : info.book_id;
                info.name = JSON_SAFE_CHECK(block, "title", String) ? block["title"].GetString() : info.name;
                info.duration = JSON_SAFE_CHECK(block, "duration", Int) ? block["duration"].GetInt() : info.duration;
                info.summary = JSON_SAFE_CHECK(block, "description", String) ? block["description"].GetString() : info.summary;
                info.thumbnail_url = JSON_SAFE_CHECK(block, "thumb_image", String) ? block["thumb_image"].GetString() : info.thumbnail_url;
                info.grade_id = JSON_SAFE_CHECK(block, "cateId", Int) ? block["cateId"].GetInt() : info.grade_id;
                info.lang_id = JSON_SAFE_CHECK(block, "lang_id", Int) ? block["lang_id"].GetInt() : info.lang_id;
                info.extra = JSON_SAFE_CHECK(block, "extra", String) ? block["extra"].GetString() : info.extra;
                info.sync_path =  JSON_SAFE_CHECK(block, "content", String) ? block["content"].GetString() : info.sync_path;;
                info.audio_url = JSON_SAFE_CHECK(block, "audio_file", String) ? block["audio_file"].GetString() : info.audio_url;
                info.date_publish = JSON_SAFE_CHECK(block, "date_publish", Int) ? block["date_publish"].GetInt() : info.date_publish;
                info.quality = JSON_SAFE_CHECK(block, "quality", Int) ? block["quality"].GetInt() : info.quality;
                info.score = JSON_SAFE_CHECK(block, "score", Int) ? block["score"].GetInt() : info.score;
                books.push_back(info);

                if (JSON_SAFE_CHECK(block, "audio_file_size", Number)) {
                    int size = block["audio_file_size"].GetFloat();
                    if (size != 0)
                    {
                        list_size.push_back(std::make_pair(info.audio_url, size));
                    }
                };

                if (JSON_SAFE_CHECK(block, "child", Array)) {
                    for (auto u = 0; u < block["child"].Size(); ++u) if (block["child"][u].IsObject()) {
                        auto child_block = block["child"][u].GetObject();
                        mj::model::AudiobookInfo child_info;

                        child_info.book_id = JSON_SAFE_CHECK(child_block, "id", Int) ? child_block["id"].GetInt() : child_info.book_id;
                        child_info.name = JSON_SAFE_CHECK(child_block, "title", String) ? child_block["title"].GetString() : child_info.name;
                        child_info.duration = JSON_SAFE_CHECK(child_block, "duration", Int) ? child_block["duration"].GetInt() : child_info.duration;
                        child_info.summary = JSON_SAFE_CHECK(child_block, "description", String) ? child_block["description"].GetString() : child_info.summary;
                        child_info.thumbnail_url = JSON_SAFE_CHECK(child_block, "thumb_image", String) ? child_block["thumb_image"].GetString() : child_info.thumbnail_url;
                        child_info.grade_id = JSON_SAFE_CHECK(child_block, "cateId", Int) ? child_block["cateId"].GetInt() : child_info.grade_id;
                        child_info.lang_id = JSON_SAFE_CHECK(child_block, "lang_id", Int) ? child_block["lang_id"].GetInt() : child_info.lang_id;
                        child_info.extra = JSON_SAFE_CHECK(child_block, "extra", String) ? child_block["extra"].GetString() : child_info.extra;
						
						#if NEW_AUDIO_BOOK
						   child_info.sync_path = JSON_SAFE_CHECK(child_block, "content", String) ? child_block["content"].GetString() : child_info.sync_path;
						#else
							child_info.sync_path = "";// JSON_SAFE_CHECK(child_block, "content", String) ? child_block["content"].GetString() : child_info.sync_path;
						#endif
						
                        child_info.audio_url = JSON_SAFE_CHECK(child_block, "audio_file", String) ? child_block["audio_file"].GetString() : child_info.audio_url;
                        //child_info.audio_url = mj::helper::isValidURL(child_info.audio_url) ? child_info.audio_url : "";
                        child_info.date_publish = JSON_SAFE_CHECK(child_block, "date_publish", Int) ? child_block["date_publish"].GetInt() : child_info.date_publish;
                        child_info.quality = JSON_SAFE_CHECK(child_block, "quality", Int) ? child_block["quality"].GetInt() : child_info.quality;
                        child_info.score = JSON_SAFE_CHECK(child_block, "score", Int) ? child_block["score"].GetInt() : child_info.score;
                        books.push_back(child_info);
                        listUrlThumb.push_back(child_info.thumbnail_url);

                        chapter_list.push_back(std::make_tuple(info.book_id, child_info.book_id, u));
                    }
                }

            }
            if (books.size() >= maxSizeInsertDB)
            {
                StoryDataModel::getInstance()->insertAudiobooks(books);
                books.clear();
            }
            if (list_size.size() >= maxSizeInsertDB)
            {
                StoryDataModel::getInstance()->updateFileSize(list_size);
                list_size.clear();
            }
            if (chapter_list.size() >= maxSizeInsertDB)
            {
                StoryDataModel::getInstance()->updateAudiobookChapter(chapter_list);
                chapter_list.clear();
            }

        }
        StoryDataModel::getInstance()->insertAudiobooks(books);

        StoryDataModel::getInstance()->updateFileSize(list_size);
        StoryDataModel::getInstance()->updateAudiobookChapter(chapter_list);

        if(tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update)
        {
            loadThumbUrl(listUrlThumb, RemoteImageView::Type::STORIES);
        }
    }
    
    if (JSON_SAFE_CHECK(mapResponse, "version_audio", Int)) {
        int audioBookVersion = mapResponse["version_audio"].GetInt();
        if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install && audioBookVersion > MJDEFAULT->getIntegerForKey(key_api_version_audio_fisrt_install, -1))
            MJDEFAULT->setIntegerForKey(key_api_version_audio_fisrt_install, audioBookVersion);
        MJDEFAULT->setIntegerForKey(key_api_version_audio_local, audioBookVersion);
        MJDEFAULT->setIntegerForKey(key_version_api_audiobook, audioBookVersion);
        MJDEFAULT->setIntegerForKey(key_api_version_audio_v2_fromServer, audioBookVersion);
    }

    if (JSON_SAFE_CHECK(mapResponse, "delete", Array)) {
        std::vector<int> idx;
        auto data = mapResponse["delete"].GetArray();
        for (auto i = 0; i < data.Size(); ++i) {
            if (data[i].IsInt()) {
                idx.push_back(data[i].GetInt());
            }
        }

        StoryDataModel::getInstance()->removeAudiobooks(idx);
    }

    if (JSON_SAFE_CHECK(map_response, "popular_search", Array))
    {
        auto booksArr = map_response["popular_search"].GetArray();

        std::vector< mj::model::PopularSearchAudiobookInfo> books;

        for (auto i = 0; i < booksArr.Size(); ++i) {

            auto obj = booksArr[i].GetObject();

            mj::model::PopularSearchAudiobookInfo book;

            book.id = JSON_SAFE_EXTRACT(obj, "id", Int, -1);
            book.keyword = JSON_SAFE_EXTRACT(obj, "keyword", String, "");
            book.thumbnailUrl = JSON_SAFE_EXTRACT(obj, "thumb", String, "");
            book.sort = JSON_SAFE_EXTRACT(obj, "sort", Int, -1);
            book.date_publish = JSON_SAFE_EXTRACT(obj, "date_publish", Int, -1);

            books.push_back(book);
        }

        StoryDataModel::getInstance()->setPopularSearchAudiobooks(books);
    }
   
#pragma region Series infoData
    {

        auto data = map_response;

        std::vector<mj::model::AudiobookSeriesInfo> series;

        if (JSON_SAFE_CHECK(data, "Series", Array)) {

            auto a = data["Series"].GetArray();

            for (auto i = 0; i < a.Size(); ++i) {

                auto obj = a[i].GetObject();

                mj::model::AudiobookSeriesInfo info;

                info.id = JSON_SAFE_EXTRACT(obj, "id", Int, -1);
                info.title = JSON_SAFE_EXTRACT(obj, "title", String, "");
                info.thumbnail_url = JSON_SAFE_EXTRACT(obj, "thumb", String, "");

                if (JSON_SAFE_CHECK(obj, "book", Array))
                {
                    rapidjson::StringBuffer buffer;

                    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);

                    obj["book"].Accept(jsonWriter);

                    info.jsonBooks = buffer.GetString();

                    //infoForLesson.convertBookJsonToBookArray();

                    CCLOG("info.jsonBooks:%s", info.jsonBooks.c_str());
                }

                info.hidden = JSON_SAFE_EXTRACT(obj, "hidden", Int, -1);

                series.push_back(info);
            }
        }

        if (!series.empty()) {

            StoryDataModel::getInstance()->removeAllAudiobookSeries();

            StoryDataModel::getInstance()->insertAudiobookSeries(series);
        }

    }
#pragma endregion
    onDecodeFreBookAndFreeStory(tag, mapResponse);
    /*if (JSON_SAFE_CHECK(mapResponse, "free_audio_book", Object)) {
        if (JSON_SAFE_CHECK(mapResponse, "today", String)) {
            auto today = LTDate(atoi(mapResponse["today"].GetString()));
            auto today_str = StringUtils::format("%d%02d%02d", today.year + 1900, today.month + 1, today.day);
            if (today_str != MJDEFAULT->getStringForKey(date_get_audiobookfree)) {
                MJDEFAULT->setStringForKey(date_get_audiobookfree, today_str);
                std::unordered_map<int, std::vector<int>> free_audiobooks;
                auto free_info = mapResponse["free_audio_book"].GetObject();
                for (auto j = free_info.MemberBegin(); j != free_info.MemberEnd(); j++) {
                    auto key = ATOI(j->name.GetString());
                    if (j->value.IsArray()) {
                        auto value = j->value.GetArray();
                        std::vector<int> ids;
                        for (auto u = 0; u < value.Size(); ++u) {
                            if (value[u].IsInt()) {
                                ids.push_back(value[u].GetInt());
                            }
                        }
                        free_audiobooks[key] = ids;

                        FREE_STORY_MANAGER->setListFreeAudioBook(free_audiobooks);
                    }
                    else {
                        continue;
                    }
                }
            }
        }
    }*/
    //}
 
}
void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeWorksheetData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    // MARK: - "workSheetStory"
    if (JSON_SAFE_CHECK(map_response, "version_worksheet", Int)) {
        int version_worksheet = map_response["version_worksheet"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_material_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_material_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_material_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_material_fromServer, version_worksheet);
    }

    if (JSON_SAFE_CHECK(map_response, "workSheetStory", Array)) {
        auto story_materials = map_response["workSheetStory"].GetArray();
        std::vector<mj::model::MaterialInfo> list_story_materials;

        for (int i = 0; i < story_materials.Size(); i++) {
            auto mat = &story_materials[i];
            auto matObj = mat->GetObject();

            auto matInfo = mj::model::MaterialInfo(matObj);
            list_story_materials.push_back(matInfo);
            if (list_story_materials.size() >= maxSizeInsertDB) {
                StoryDataModel::getInstance()->insertMaterialsWithVector(list_story_materials);
                list_story_materials.clear();
            }
        }
        StoryDataModel::getInstance()->insertMaterialsWithVector(list_story_materials);
    }

    // MARK: - "workSheetPhonic"
    if (JSON_SAFE_CHECK(map_response, "workSheetPhonic", Array)) {
        auto lesson_materials = map_response["workSheetPhonic"].GetArray();
        std::vector<mj::model::MaterialInfo> list_lesson_materials;

        for (int i = 0; i < lesson_materials.Size(); i++) {
            auto mat = &lesson_materials[i];
            auto matObj = mat->GetObject();

            auto matInfo = mj::model::MaterialInfo(matObj);
            list_lesson_materials.push_back(matInfo);
            if (list_lesson_materials.size() >= maxSizeInsertDB) {
                StoryDataModel::getInstance()->insertMaterialsWithVector(list_lesson_materials);
                list_lesson_materials.clear();
            }
        }
        StoryDataModel::getInstance()->insertMaterialsWithVector(list_lesson_materials);
    }

    // MARK: - "delete"
    if (JSON_SAFE_CHECK(map_response, "delete", Array)) {
        std::vector<int> ids;
        auto ids_to_delete = map_response["delete"].GetArray();
        for (auto i = 0; i < ids_to_delete.Size(); ++i) {
            if (ids_to_delete[i].IsInt())
                ids.push_back(ids_to_delete[i].GetInt());
        }
        StoryDataModel::getInstance()->deleteMaterials(ids);
    }

    // MARK: - "popular_search"
    if (JSON_SAFE_CHECK(map_response, "popular_search", Array)) {
        auto popular_materials = map_response["popular_search"].GetArray();
        std::vector<mj::model::MaterialPopularInfo> list_popular_materials;

        for (int i = 0; i < popular_materials.Size(); i++) {
            auto mat = &popular_materials[i];
            auto matObj = mat->GetObject();

            auto matInfo = mj::model::MaterialPopularInfo(matObj);
            list_popular_materials.push_back(matInfo);
            if (list_popular_materials.size() >= maxSizeInsertDB) {
                StoryDataModel::getInstance()->setPopularMaterials(list_popular_materials);
                list_popular_materials.clear();
            }
        }
        StoryDataModel::getInstance()->setPopularMaterials(list_popular_materials);
    }

    // MARK: - "max_worksheet_send"
    if (JSON_SAFE_CHECK(map_response, "max_worksheet_send", Object)) {
        auto obj_max_send = map_response["max_worksheet_send"].GetObject();

        if (JSON_SAFE_CHECK(obj_max_send, "user_purchase", Number)) {
            MJDEFAULT->setIntegerForKey(key_material_max_items_sent, obj_max_send["user_purchase"].GetInt());
        }

        if (JSON_SAFE_CHECK(obj_max_send, "user_free", Number)) {
            MJDEFAULT->setIntegerForKey(key_material_max_items_sent_free, obj_max_send["user_free"].GetInt());
        }
    }

}
void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeGameListData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    //all games
    auto gameListResponse = map_response;
    if (JSON_SAFE_CHECK(gameListResponse, "list", Array)) {
        auto list = gameListResponse["list"].GetArray();

        std::vector<mj::model::GameInfo> games;
        std::vector<std::pair<std::string, int>> list_size;
        for (auto i = 0; i < list.Size(); ++i) {
            if (list[i].IsObject()) {
                auto block = list[i].GetObject();

                mj::model::GameInfo info;

                info.game_id = JSON_SAFE_CHECK(block, "id", Int) ? block["id"].GetInt() : info.game_id;
                info.name = JSON_SAFE_CHECK(block, "name", String) ? block["name"].GetString() : info.name;
                info.resource_zip = JSON_SAFE_CHECK(block, "path_config", String) ? block["path_config"].GetString() : info.resource_zip;
                info.thumbnail_url = JSON_SAFE_CHECK(block, "path_images", String) ? block["path_images"].GetString() : info.thumbnail_url;
                info.zip_size = JSON_SAFE_CHECK(block, "zip_size", String) ? block["zip_size"].GetString() : info.zip_size;

                games.push_back(info);

                auto size = JSON_SAFE_CHECK(block, "zip_size", Number) ? block["zip_size"].GetFloat() : 0;
                if (size != 0)
                {
                    list_size.push_back(std::make_pair(info.resource_zip, size));
                }
            }
            if (games.size() >= maxSizeInsertDB) {
                StoryDataModel::getInstance()->insertGameInfo(games);
                games.clear();
            }
        }
        StoryDataModel::getInstance()->insertGameInfo(games);
        StoryDataModel::getInstance()->updateFileSize(list_size);
    }

    if (JSON_SAFE_CHECK(gameListResponse, "version_game_number", Int)) {
        int defaultGameVersion = gameListResponse["version_game_number"].GetInt();
        MJDEFAULT->setIntegerForKey(key_api_version_all_games_local,defaultGameVersion);
        MJDEFAULT->setIntegerForKey(key_api_version_all_games_fromServer, defaultGameVersion);   
    }
}
void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLessonListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    auto lessonListReponse = map_response;
        if (JSON_SAFE_CHECK(map_response, "list", Array)) {
            auto data = lessonListReponse["list"].GetArray();

            std::vector<mj::model::LessonInfo> lessons;
            std::vector<int> lessonIds;
            std::vector<std::string> listUrlThumb = {};
            for (auto i = 0; i < data.Size(); ++i) {
                if (data[i].IsObject()) {
                    auto block = data[i].GetObject();

                    mj::model::LessonInfo info;

                    info.lesson_id = JSON_SAFE_CHECK(block, "id", Int) ? block["id"].GetInt() : info.lesson_id;

                    info.lesson_order = JSON_SAFE_CHECK(block, "order_by", Int) ? block["order_by"].GetInt() : info.lesson_order;
                    info.lang_id = JSON_SAFE_CHECK(block, "lang_id", Int) ? block["lang_id"].GetInt() : 1;
                    info.name = JSON_SAFE_CHECK(block, "title", String) ? block["title"].GetString() : info.name;
                    info.thumbnail_url = JSON_SAFE_CHECK(block, "path_thumb", String) ? block["path_thumb"].GetString() : info.thumbnail_url;
                    info.category_id = JSON_SAFE_CHECK(block, "game_category_id", Int) ? block["game_category_id"].GetInt() : info.category_id;
                    info.time_published = JSON_SAFE_CHECK(block, "time_publish", Int) ? block["time_publish"].GetInt() : info.time_published;

                    lessons.push_back(info);
                    lessonIds.push_back(info.lesson_id);
                    listUrlThumb.push_back(info.thumbnail_url);
                }
                if (lessons.size() >= maxSizeInsertDB) {
                    StoryDataModel::getInstance()->insertLessons(lessons, tag);
                    lessons.clear();
                }
            }
            if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update && lessonIds.size()>0) {
                std::string vts = mj::model::ConvertArrayIntergerToString(lessonIds);
                MJDEFAULT->setStringForKey(key_update_lesson_ids, vts);
            }
            StoryDataModel::getInstance()->insertLessons(lessons, tag);
            if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update)
            {
                loadThumbUrl(listUrlThumb, RemoteImageView::Type::AUDIOBOOKS);
            }
        }

        if (JSON_SAFE_CHECK(lessonListReponse, "list_remove", Array)) {
            auto data = lessonListReponse["list_remove"].GetArray();
            std::vector<int> idx_to_remove;
            for (auto i = 0; i < data.Size(); ++i) {
                auto idx = data[i].GetInt();
                idx_to_remove.push_back(idx);
            }
            StoryDataModel::getInstance()->removeLessonsById(idx_to_remove, tag);
        }

        if (JSON_SAFE_CHECK(lessonListReponse, "version_lesson", Int)) {
            auto version_lesson = lessonListReponse["version_lesson"].GetInt();
            if (tag == mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::First_Install && version_lesson > MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fisrt_install, -1))
                MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_fisrt_install, version_lesson);
            MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_local,version_lesson);
            MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_fromServer, version_lesson);
        }
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryLessonInteractiveData(int tag, rapidjson::GenericArray<false, rapidjson::Value> data)
{
    std::vector<mj::model::ActInfo> activitiesForLesson, activitiesForStories;
    std::vector<std::pair<std::string, int>> list_size;
    //        auto lesson_id = JSON_SAFE_CHECK(data, "lesson_id", Int) ? data["lesson_id"].GetInt() : 0;
    for (auto i = 0; i < data.Size(); ++i) {
        if (data[i].IsObject()) {
            auto obj = data[i].GetObject();

            mj::model::ActInfo infoForLesson, inforForStoies;
            infoForLesson.lesson_id = JSON_SAFE_CHECK(obj, "lesson_id", Int) ? obj["lesson_id"].GetInt() : infoForLesson.lesson_id;
            infoForLesson.activity_id = JSON_SAFE_CHECK(obj, "activity_id", Int) ? obj["activity_id"].GetInt() : infoForLesson.activity_id;
            infoForLesson.quantity = JSON_SAFE_CHECK(obj, "quantity", Int) ? obj["quantity"].GetInt() : infoForLesson.quantity;
            infoForLesson.part = JSON_SAFE_CHECK(obj, "part", Int) ? obj["part"].GetInt() : infoForLesson.part;
            infoForLesson.order = JSON_SAFE_CHECK(obj, "order", Int) ? obj["order"].GetInt() : i;
            infoForLesson.name = JSON_SAFE_CHECK(obj, "name", String) ? obj["name"].GetString() : infoForLesson.name;
            infoForLesson.content = JSON_SAFE_CHECK(obj, "description", String) ? obj["description"].GetString() : infoForLesson.content;
            infoForLesson.resource_zip = JSON_SAFE_CHECK(obj, "resource_zip", String) ? obj["resource_zip"].GetString() : infoForLesson.resource_zip;
            infoForLesson.data_zip = JSON_SAFE_CHECK(obj, "url_download", String) ? obj["url_download"].GetString() : infoForLesson.data_zip;
            if (infoForLesson.resource_zip.empty())
            {
                infoForLesson.resource_zip = infoForLesson.data_zip;
            }
            if (JSON_SAFE_CHECK(obj, "game_id", Int))
            {
                infoForLesson.sub_id = JSON_SAFE_CHECK(obj, "game_id", Int) ? obj["game_id"].GetInt() : infoForLesson.sub_id;  // if is Activity list, sub_id will be game_id
            }
            else
            {
                infoForLesson.sub_id = JSON_SAFE_CHECK(obj, "story_id", Int) ? obj["story_id"].GetInt() : infoForLesson.sub_id; // if is storyItem list, sub_id will be stories_id
            }

            auto type = JSON_SAFE_CHECK(obj, "type", Int) ? obj["type"].GetInt() : 1;
            if (type == 1) {
                infoForLesson.type = mj::model::ActInfo::GAME;
            }
            else if (type == 3) {
                infoForLesson.type = mj::model::ActInfo::QUIZZES;
            }
            else if (type == 2) {
                infoForLesson.type = mj::model::ActInfo::STORY;
            }
            //                CCLOG("infoForLesson.sub_id = %d  | %s  | %s", infoForLesson.sub_id, infoForLesson.name.c_str(), infoForLesson.data_zip.c_str());
            // sau khi pushback item cho lessson table, can push cho ca story table
            inforForStoies = infoForLesson;
            inforForStoies.lesson_id = JSON_SAFE_CHECK(obj, "story_id", Int) ? obj["story_id"].GetInt() : infoForLesson.lesson_id;
            
            int storyID = JSON_SAFE_CHECK(obj, "story_id", Int) ? obj["story_id"].GetInt() : 0;
            
            if(infoForLesson.lesson_id == 0 && storyID > 0 ) {
                
                activitiesForStories.push_back(inforForStoies);
            }
            else
            {
                activitiesForLesson.push_back(infoForLesson);
            }
                
            auto size = JSON_SAFE_CHECK(obj, "file_size", Number) ? obj["file_size"].GetFloat() : 0;
            if (size != 0)
            {
                list_size.push_back(std::make_pair(infoForLesson.data_zip, size));
            }
        }
        // too much number of item, --> can't insert to sqlite db
        if (activitiesForLesson.size() >= maxSizeInsertDB)
        {
            StoryDataModel::getInstance()->insertActivities(activitiesForLesson);
            activitiesForLesson.clear();

        }
        if (activitiesForStories.size() >= maxSizeInsertDB)
        {
            StoryDataModel::getInstance()->insertStoriesActivities(activitiesForStories);
            activitiesForStories.clear();
        }
        if (list_size.size() >= maxSizeInsertDB)
        {
            StoryDataModel::getInstance()->updateFileSize(list_size);
            list_size.clear();
        }
    }

    //        StoryDataModel::getInstance()->removeActivitiesOfLesson(_lesson_id);
    StoryDataModel::getInstance()->insertActivities(activitiesForLesson);
    StoryDataModel::getInstance()->insertStoriesActivities(activitiesForStories);
    StoryDataModel::getInstance()->updateFileSize(list_size);

}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLocationData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    if (JSON_SAFE_CHECK(map_response, "ip", String)) {
        auto ip = map_response["ip"].GetString();
        MJDEFAULT->setStringForKey(IP_NAME_KEY, ip);
        MJDEFAULT->setStringForKey("key_ip_adress", ip);
    }
    if (JSON_SAFE_CHECK(map_response, "country_name", String)) {
        auto country_name = map_response["country_name"].GetString();
        MJDEFAULT->setStringForKey(COUNTRY_NAME_KEY, country_name);
    }
    if (JSON_SAFE_CHECK(map_response, "region_name", String)) {
        auto region_name = map_response["region_name"].GetString();
        MJDEFAULT->setStringForKey(REGION_NAME_KEY, region_name);
    }
    if (JSON_SAFE_CHECK(map_response, "city_name", String)) {
        auto city_name = map_response["city_name"].GetString();
        MJDEFAULT->setStringForKey(CITY_NAME_KEY, city_name);
    }
    if (JSON_SAFE_CHECK(map_response, "latitude", Float)) {
        auto latitude = map_response["latitude"].GetFloat();
        MJDEFAULT->setFloatForKey(LATITUDE_NAME_KEY, latitude);
    }
    if (JSON_SAFE_CHECK(map_response, "longitude", Float)) {
        auto longitude = map_response["longitude"].GetFloat();
        MJDEFAULT->setFloatForKey(LONGITUDE_NAME_KEY, longitude);
    }
    if (JSON_SAFE_CHECK(map_response, "countryCodeHere", String)) {
        auto countryCodeHere = map_response["countryCodeHere"].GetString();
        MJDEFAULT->setStringForKey(COUNTRY_CODE_HERE_KEY, countryCodeHere);
        MJDEFAULT->setStringForKey("countryCodeHere", countryCodeHere);
    }
    if (JSON_SAFE_CHECK(map_response, "is_vn", Bool)) {
        auto is_vn = map_response["is_vn"].GetBool();
        MJDEFAULT->setBoolForKey(IS_VN_KEY, is_vn);
    }  
    if (JSON_SAFE_CHECK(map_response, "push_error_firebase", Bool)) {
        auto push_error_firebase = map_response["push_error_firebase"].GetBool();
        MJDEFAULT->setBoolForKey(key_push_event_crash_to_fibase, push_error_firebase);
    }
    onSetDomainByLocation();
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeCategoriesInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
        auto categories_list = map_response;
        std::vector<int> langIds;
        if (JSON_SAFE_CHECK(categories_list, "remove_categories_list", Array)) {
            auto removeList = categories_list["remove_categories_list"].GetArray();
            for (auto i = 0; i < removeList.Size(); ++i)
            {
                if (removeList[i].IsInt())
                    langIds.push_back(removeList[i].GetInt());
                if (langIds.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->removeNewGradesByLangId(langIds);
                    langIds.clear();
                }
            }
           
            StoryDataModel::getInstance()->removeNewGradesByLangId(langIds);
            ms::GradeManager::getInstance().updateGrades();
        }

        if (JSON_SAFE_CHECK(categories_list, "categories_list", Array)) {
            auto list = categories_list["categories_list"].GetArray();

            std::vector<mj::model::GradeInfo> grades;
            for (auto i = 0; i < list.Size(); ++i) {
                if (list[i].IsObject()) {
                    auto block = list[i].GetObject();

                    mj::model::GradeInfo info;
                    info.grade_id = JSON_SAFE_CHECK(block, "id", Int) ? block["id"].GetInt() : info.grade_id;
                    info.name = JSON_SAFE_CHECK(block, "name", String) ? block["name"].GetString() : info.name;
                    info.lang_id = JSON_SAFE_CHECK(block, "language_id", Int) ? block["language_id"].GetInt() : info.lang_id;
                    info.parent_id = JSON_SAFE_CHECK(block, "parent_id", Int) ? block["parent_id"].GetInt() : info.parent_id;
                    info.order_by = JSON_SAFE_CHECK(block, "order_by", Int) ? block["order_by"].GetInt() : info.order_by;
                    grades.push_back(info);

                }
                if (grades.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->insertNewGrades(grades);
                    grades.clear();
                }

            }
            StoryDataModel::getInstance()->insertNewGrades(grades);
            ms::GradeManager::getInstance().updateGrades();
        }

        if (JSON_SAFE_CHECK(categories_list, "version_categories", Int)) {
            int versionCategoriesNumberDefault = categories_list["version_categories"].GetInt();
            MJDEFAULT->setIntegerForKey(key_api_version_categories_local,versionCategoriesNumberDefault);
            MJDEFAULT->setIntegerForKey(key_api_version_categories_fromServer, versionCategoriesNumberDefault);
        }
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeProductListInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    std::vector<mj::model::ProductListInfo> productListInfos;
    std::vector<char*> cities = { "VN", "US", "TH", "ID", "MY" };
    for (char* city : cities)
    {
        if (JSON_SAFE_CHECK(map_response, city, Array)) {
            rapidjson::Value list = map_response[city].GetArray();
            std::string contryCodeSelect = MJDEFAULT->getStringForKey(COUNTRY_CODE_HERE_KEY, "VN");
            std::string cititystr(city);
            if (cititystr == contryCodeSelect)
            {
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                list.Accept(writer);
                auto json_string = std::string(buffer.GetString());
                MJDEFAULT->setStringForKey(keyPriceInfo, json_string);
                mj::Pay::getInstance().refresh(json_string);
            }

            //mj::Pay::getInstance().parsePackagesFromData(map_response[city].GetArray());
            for (auto i = 0; i < list.Size(); ++i) {
                if (list[i].IsObject()) {
                    auto block = list[i].GetObject();

                    mj::model::ProductListInfo info;
                    info.city = city;
                    info.tien_ao = JSON_SAFE_CHECK(block, "tienao", Int) ? block["tienao"].GetInt() : info.tien_ao;
                    info.giam_40 = JSON_SAFE_CHECK(block, "giam40", Int) ? block["giam40"].GetInt() : info.giam_40;
                    info.giam_30 = JSON_SAFE_CHECK(block, "giam30", Int) ? block["giam30"].GetInt() : info.giam_30;
                    info.sale_off = JSON_SAFE_CHECK(block, "saleoff", Int) ? block["saleoff"].GetInt() : info.sale_off;
                    info.product_id = JSON_SAFE_CHECK(block, "product_id", String) ? block["product_id"].GetString() : info.product_id;
                    info.product_name = JSON_SAFE_CHECK(block, "product_name", String) ? block["product_name"].GetString() : info.product_name;
                    info.product_description = JSON_SAFE_CHECK(block, "product_description", String) ? block["product_description"].GetString() : info.product_description;
                    info.product_code = JSON_SAFE_CHECK(block, "product_code", String) ? block["product_code"].GetString() : info.product_code;
                    info.current_unit = JSON_SAFE_CHECK(block, "currency", String) ? block["currency"].GetString() : info.current_unit;

                    productListInfos.push_back(info);

                }
                if (productListInfos.size() >= maxSizeInsertDB)
                {
                    StoryDataModel::getInstance()->insertProductList(productListInfos);
                    productListInfos.clear();
                }
            }
        }
    }
    StoryDataModel::getInstance()->insertProductList(productListInfos);

}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAppProblemsListInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLangguageListInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeVersionAPIInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    if (JSON_SAFE_CHECK(map_response, "version_story", Int)) {
        int version_story = map_response["version_story"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer, MJDEFAULT->getIntegerForKey(key_api_version_story_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_story_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_story_v2_fromJsonServer, version_story);
    }

    if (JSON_SAFE_CHECK(map_response, "version_audio", Int)) {
        int version_audio = map_response["version_audio"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_audio_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_audio_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_audio_v2_fromServer, version_audio);
    }
    if (JSON_SAFE_CHECK(map_response, "version_worksheet", Int)) {
        int version_worksheet = map_response["version_worksheet"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_material_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_material_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_material_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_material_fromServer, version_worksheet);
    }
    if (JSON_SAFE_CHECK(map_response, "version_game", Int)) {
        int version_game = map_response["version_game"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_all_games_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_all_games_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_all_games_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_all_games_fromServer, version_game);
    }

    if (JSON_SAFE_CHECK(map_response, "version_lesson", Int)) {
        int version_lesson = map_response["version_lesson"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fromServer, MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_fromServer, version_lesson);
    }

    if (JSON_SAFE_CHECK(map_response, "version_categories", Int)) {
        int version_categories = map_response["version_categories"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_categories_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_categories_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_categories_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_categories_fromServer, version_categories);
    }
    //get version for ai_Speaking
    if (JSON_SAFE_CHECK(map_response, "version_categories_talking", Int)) {
        int version_ai_speaking_topic = map_response["version_categories_talking"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_topic_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, version_ai_speaking_topic);
    }
    if (JSON_SAFE_CHECK(map_response, "version_lesson_talking", Int)) {
        int version_ai_speaking_item = map_response["version_lesson_talking"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_lesson_list_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, version_ai_speaking_item);
    }
    if (JSON_SAFE_CHECK(map_response, "version_common_mk_talking", Int)) {
        int version_ai_speaking_item = map_response["version_common_mk_talking"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_common_list_fromJsonServer, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_common_list_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_common_list_local, old_version);
        MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_common_list_fromJsonServer, version_ai_speaking_item);
    }

    if (JSON_SAFE_CHECK(map_response, "version_trial_items", Int)) {
        int version_trial_items = map_response["version_trial_items"].GetInt();
        int old_version = MJDEFAULT->getIntegerForKey(key_api_version_version_trial_items_from_server, MJDEFAULT->getIntegerForKey(key_api_version_version_trial_items_local, -1));
        MJDEFAULT->setIntegerForKey(key_api_version_version_trial_items_from_server, version_trial_items);
    }
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeGeneralUpdateInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    if (map_response.HasMember("is_submit")) {
        MJDEFAULT->setBoolForKey(key_is_submit, map_response["is_submit"].GetBool());
    }

    if (map_response.HasMember("show_learn_to_spell")) {
        MJDEFAULT->setBoolForKey("show_hoc_van", map_response["show_learn_to_spell"].GetBool());
    }

    if (JSON_SAFE_CHECK(map_response, "show_feature_license", Bool))
    {
        MJDEFAULT->setBoolForKey("is_license_feature_enable", map_response["show_feature_license"].GetBool());
    }

    if (map_response.HasMember("current_time")) {
        int current_time = map_response["current_time"].GetInt();
        MJDEFAULT->setIntegerForKey(key_time_curent, current_time);
        
        if (!CONFIG_MANAGER->isAppActive()) {
            if (!FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_time_curent), map_response["current_time"].GetInt())) {
                FREE_STORY_MANAGER->map_story_recent.clear();
            }
        }


        if (CONFIG_MANAGER->isTestFreeTimeFake()) {
            FREE_STORY_MANAGER->freetime_fake_count++;
            if (MJDEFAULT->getIntegerForKey(key_app_firstdate, 0) != 0) {
                auto date_current = LTDate(map_response["current_time"].GetInt());
                date_current = date_current + FREE_STORY_MANAGER->freetime_fake_count * 24 * 3600;
                auto t = date_current.toTimeStruct();
                MJDEFAULT->setIntegerForKey(key_time_fake, (int)mktime(&t));
                MJDEFAULT->setIntegerForKey(key_app_firstdate, 1504604738);
                FREE_STORY_MANAGER->map_story_recent.clear();
            }
            else {
                MJDEFAULT->setIntegerForKey(key_time_fake, map_response["current_time"].GetInt());
            }
        }
    }

    if (map_response.HasMember("limit_seconds_offline")) {
        int time_offline_limit = map_response["limit_seconds_offline"].GetInt();
        MJDEFAULT->setIntegerForKey(key_time_offline_limit, time_offline_limit);
    }
    if (JSON_SAFE_CHECK(map_response, "new_version", String)) {
        std::string object = map_response["new_version"].GetString();
        MJDEFAULT->setStringForKey(key_new_version, object);

    }
    if (JSON_SAFE_CHECK(map_response, "pay_use_url", String)) {
        std::string object = map_response["pay_use_url"].GetString();
        MJDEFAULT->setStringForKey(key_pay_use_url, object);

    }
    if (JSON_SAFE_CHECK(map_response, "is_licence", Bool))
    {
        MJDEFAULT->setBoolForKey(key_is_licence, map_response["is_licence"].GetBool());
    }

    if (JSON_SAFE_CHECK(map_response, "country_code", String))
    {
        std::string country_code = map_response["country_code"].GetString();
        MJDEFAULT->setStringForKey(key_country_code, country_code);
    }

    if (JSON_SAFE_CHECK(map_response, "time_update", String))
    {
        auto time_update_app = map_response["time_update"].GetString();
    }

    if (JSON_SAFE_CHECK(map_response, "is_log_enable", Bool))
    {
        MJDEFAULT->setBoolForKey("is_log_enable", map_response["is_log_enable"].GetBool());
    }

    if (JSON_SAFE_CHECK(map_response, "version_update", String))
    {
        auto ver_update_app = map_response["version_update"].GetString();
        MJDEFAULT->setStringForKey(key_ver_update_app, ver_update_app);
    }

    // Quanpd: set key cho vesion mới nhất
    if (JSON_SAFE_CHECK(map_response, "new_version", String))
    {
        auto ver_update_app = map_response["new_version"].GetString();
        MJDEFAULT->setStringForKey(key_ver_newest_version, ver_update_app);
    }

    if (JSON_SAFE_CHECK(map_response, "version_required", String))
    {
        auto version_required = map_response["version_required"].GetString();
        MJDEFAULT->setStringForKey(key_version_required, version_required);
    }

    if (JSON_SAFE_CHECK(map_response, "update", Object))
    {
        const rapidjson::Value& obj_update = map_response["update"];

        if (JSON_SAFE_CHECK(obj_update, "id", Int))
        {
            auto update_id = obj_update["id"].GetInt();
            // reset time update skip when retrive new update
            if (MJDEFAULT->getIntegerForKey(key_last_update_id) != update_id)
            {
                MJDEFAULT->setIntegerForKey(key_last_update_id, update_id);
                MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
            }
        }

        auto status_update = JSON_SAFE_CHECK(obj_update, "status", String) ? std::string(obj_update["status"].GetString()) : "";

        if (status_update != "")
        {
            if (status_update == "require")
            {
                MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
            }

            MJDEFAULT->setBoolForKey(key_update_show, true);

            rapidjson::StringBuffer rj_buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(rj_buffer);
            obj_update.Accept(writer);
            auto json_string = std::string(rj_buffer.GetString());

            MJDEFAULT->setStringForKey(key_update_content, json_string);
        }

        MJDEFAULT->setStringForKey(key_ver_update_app, JSON_SAFE_CHECK(obj_update, "version_update", String) ? obj_update["version_update"].GetString() : "0");

        MJDEFAULT->setStringForKey(key_version_required, JSON_SAFE_CHECK(obj_update, "version_required", String) ? obj_update["version_required"].GetString() : "0");
    }
    else
    {
        MJDEFAULT->setStringForKey(key_ver_update_app, "0");
        MJDEFAULT->setStringForKey(key_version_required, "0");
        MJDEFAULT->setStringForKey(key_update_content, "");
    }

    auto time_popup = 0;
    auto id_popup = 0;

    if (JSON_SAFE_CHECK(map_response, "show_pay", Bool))
    {
        MJDEFAULT->setBoolForKey("key.default.show.payment", map_response["show_pay"].GetBool());
    }

    if (JSON_SAFE_CHECK(map_response, "pay_use_url", String))
    {
        MJDEFAULT->setStringForKey("pay_use_url", map_response["pay_use_url"].GetString());
    }

    if (JSON_SAFE_CHECK(map_response, "domain_download", Object))
    {
        auto block = map_response["domain_download"].GetObject();

        if (JSON_SAFE_CHECK(block, "story", String))
        {
            MJDEFAULT->setStringForKey(key_alternative_domain_story, block["story"].GetString());
        }

        if (JSON_SAFE_CHECK(block, "audio_book", String))
        {
            MJDEFAULT->setStringForKey(key_alternative_domain_audio, block["audio_book"].GetString());
        }

        if (JSON_SAFE_CHECK(block, "lesson", String))
        {
            MJDEFAULT->setStringForKey(key_alternative_domain_lesson, block["lesson"].GetString());
        }
    }

    if (JSON_SAFE_CHECK(map_response, "notify", Array))
    {
        std::vector<mj::model::NotificationInfo> notifications;
        auto noti_info = map_response["notify"].GetArray();
        for (auto i = 0; i < noti_info.Size(); i++)
        {
            if (noti_info[i].IsObject())
            {

                auto noti_o = noti_info[i].GetObject();

                if (JSON_SAFE_CHECK(noti_o, "popup", Int))
                {
                    if (noti_o["popup"].GetInt() == 1) {
                        if (JSON_SAFE_CHECK(noti_o, "created", String)) {
                            auto created = atoi(noti_o["created"].GetString());
                            if (created > time_popup) {
                                time_popup = created;
                                id_popup = atoi(noti_o["id"].GetString());
                            }
                        }
                    }
                }
                auto notification = mj::model::NotificationInfo();
                notification.notification_id = notification.time_created = 0;
                //notification.message = notification.url = notification.title = "";

                if (JSON_SAFE_CHECK(noti_o, "id", String)) {
                    notification.notification_id = ATOI(noti_o["id"].GetString());
                }

                if (JSON_SAFE_CHECK(noti_o, "id", Int)) {
                    notification.notification_id = noti_o["id"].GetInt();
                }

                if (JSON_SAFE_CHECK(noti_o, "title", Object))
                {
                    std::vector<std::string> languages = { "en","vi","th" };

                    for (auto lang : languages)
                    {
                        if (JSON_SAFE_CHECK(noti_o["title"], lang.c_str(), String))
                        {
                            notification.m_titles.insert({ lang.c_str(), noti_o["title"][lang.c_str()].GetString() });
                        }
                    }

                    //notification.title = noti_o["title"].GetString();
                }

                if (JSON_SAFE_CHECK(noti_o, "message", Object))
                {
                    std::vector<std::string> languages = { "en","vi","th" };

                    for (auto lang : languages)
                    {
                        if (JSON_SAFE_CHECK(noti_o["message"], lang.c_str(), String))
                        {
                            notification.m_messages.insert({ lang.c_str(), noti_o["message"][lang.c_str()].GetString() });
                        }
                    }

                    //notification.message = noti_o["message"].GetString();
                }

                if (JSON_SAFE_CHECK(noti_o, "url", String)) {
                    notification.url = noti_o["url"].GetString();
                }

                if (JSON_SAFE_CHECK(noti_o, "created", String)) {
                    notification.time_created = ATOI(noti_o["created"].GetString());
                }

                if (JSON_SAFE_CHECK(noti_o, "created", Int)) {
                    notification.time_created = noti_o["created"].GetInt();
                }

                notifications.push_back(notification);
            }
        }

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([notifications]
            {
                StoryDataModel::getInstance()->addMessageFromServer(notifications);

                auto unread_message = 0;

                for (auto notification : notifications)
                {
                    if (XHSLib->checkMessageUnRead(notification.notification_id)) {
                        ++unread_message;
                    }
                }

                if (unread_message > 0)
                {
                    EventCustom e("mjstory.shownoti");
                    int* edata = new int;
                    *edata = unread_message;
                    e.setUserData(edata);
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
                    delete edata;
                }
            });
    }

    if (id_popup != 0) {
        if (!MJDEFAULT->getBoolForKey(StringUtils::format("%s%d", key_popup_showed, id_popup), false)) {
            MJDEFAULT->setIntegerForKey(key_popup_id_to_show, id_popup);
        }
        else {
            MJDEFAULT->setIntegerForKey(key_popup_id_to_show, 0);
        }
    }


    Director::getInstance()->getScheduler()->schedule([](float) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_process_refresh_api_done);

        }, Application::getInstance(), 0, 0, 0.1, false, "delay_refresh_api_done");
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeUserInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{

    bool isShowDeletionUer = false;

    if (JSON_SAFE_CHECK(map_response, "show_deletion_user", Bool))
    {
        isShowDeletionUer = map_response["show_deletion_user"].GetBool();
    }

    MJDEFAULT->setBoolForKey("show_deletion_user", isShowDeletionUer);


    if (JSON_SAFE_CHECK(map_response, "userInfo", Object))
    {
        if (!JSON_SAFE_CHECK(map_response["userInfo"], "password", Bool) || (JSON_SAFE_CHECK(map_response["userInfo"], "password", Bool) && !map_response["userInfo"]["password"].GetBool()))
        {
            MJDEFAULT->setBoolForKey("need_to_fill_password", true);
        }
        else
        {
            MJDEFAULT->setBoolForKey("need_to_fill_password", false);
        }

        if (JSON_SAFE_CHECK(map_response["userInfo"], "show_rating", Bool))
        {
            RatingManager::GetInstance()->setRatingConditionFromServer(map_response["userInfo"]["show_rating"].GetBool());
        }

        auto id = StringUtils::toString(map_response["userInfo"]["users_id"].GetInt());
        MJDEFAULT->setIntegerForKey(key_user_id, map_response["userInfo"]["users_id"].GetInt());
        auto name = (map_response["userInfo"]["name"].IsString()) ? map_response["userInfo"]["name"].GetString() : "";
        auto mail = (map_response["userInfo"]["email"].IsString()) ? map_response["userInfo"]["email"].GetString() : "";
        auto phone = (map_response["userInfo"]["phone"].IsString()) ? map_response["userInfo"]["phone"].GetString() : "";
        auto address = (map_response["userInfo"]["address"].IsString()) ? map_response["userInfo"]["address"].GetString() : "";
        auto is_verify = (map_response["userInfo"].HasMember("is_verify") && map_response["userInfo"]["is_verify"].IsBool()) ? map_response["userInfo"]["is_verify"].GetBool() : false;
        auto is_user_tutoring = (map_response["userInfo"].HasMember("is_user_tutoring") && map_response["userInfo"]["is_user_tutoring"].IsBool()) ? map_response["userInfo"]["is_user_tutoring"].GetBool() : false;
        
        MJDEFAULT->setBoolForKey("key_check_user_is_tutoring", is_user_tutoring);
        
        if (JSON_SAFE_CHECK(map_response["userInfo"], "version", Int))
        {
            int user_info_version = map_response["userInfo"]["version"].GetInt();
            int user_info_version_from_server = MJDEFAULT->getIntegerForKey(key_api_version_app_info_from_server, 0);
            MJDEFAULT->setIntegerForKey(key_api_version_app_info_local, MJDEFAULT->getIntegerForKey(key_api_version_app_info_from_server, 0));
            MJDEFAULT->setIntegerForKey(key_api_version_app_info_from_server, user_info_version);

        }
        auto userName = JSON_SAFE_CHECK(map_response["userInfo"], "username_signin", String) ? map_response["userInfo"]["username_signin"].GetString() : "";

        // email học liệu
        if (JSON_SAFE_CHECK(map_response["userInfo"], "email_worksheet", String)) {
            auto email_worksheet = (map_response["userInfo"]["email_worksheet"].IsString()) ? map_response["userInfo"]["email_worksheet"].GetString() : "";
            MJDEFAULT->setStringForKey(StringUtils::format(key_email_material_x, map_response["userInfo"]["users_id"].GetInt()), email_worksheet);
        }

        if (is_verify)
        {
            MJDEFAULT->deleteValueForKey("is_user_skip");
        }

        ap::Database::getInstance().saveUserInfo(id, name, mail, phone, address, userName);

        auto max_profile = (map_response["userInfo"]["max_profile"].IsInt()) ? map_response["userInfo"]["max_profile"].GetInt() : 3;
        MJDEFAULT->setIntegerForKey(key_max_account_profile, max_profile);

        if(CONFIG_MANAGER->isTurnOnABTesting())
        {
            if(JSON_SAFE_CHECK(map_response["userInfo"], "classification", String))
            {
                std::string classification = map_response["userInfo"]["classification"].GetString();
                
                if(classification == "receipt_7_stories")
                {
                    //0
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY);
                }else if(classification == "receipt_7_items")
                {
                    //1
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_RADOM);
                }else if(classification == "receipt_2_items_day")
                {
                    //2
                    MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY);
                }
                
                
            }
        }

    }
}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodeFreBookAndFreeStory(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{
    if (JSON_SAFE_CHECK(map_response, "free_story", Object)) {
        auto fso = map_response["free_story"].GetObject();
        std::unordered_map<int, std::vector<int> > fsm;
        for (auto i = fso.MemberBegin(); i != fso.MemberEnd(); ++i) {
            if (!i->value.IsArray()) {
                continue;
            }
            auto lang_id = ATOI(i->name.GetString());
            auto fsa = i->value.GetArray();
            std::vector<int> t;
            for (auto u = 0; u < fsa.Size(); ++u) {
                if (!fsa[u].IsInt())
                    continue;
                t.push_back(fsa[u].GetInt());
            }
            fsm.emplace(lang_id, t);
        }
        FREE_STORY_MANAGER->setListFreeStoriesOfDay(fsm);
    }
    if (JSON_SAFE_CHECK(map_response, "free_audio_book", Object)) {
        std::unordered_map<int, std::vector<int>> free_audiobooks;
        auto free_info = map_response["free_audio_book"].GetObject();
        for (auto i = free_info.MemberBegin(); i != free_info.MemberEnd(); i++) {
            auto key = ATOI(i->name.GetString());
            if (i->value.IsArray()) {
                auto value = i->value.GetArray();
                std::vector<int> ids;
                for (auto u = 0; u < value.Size(); ++u) {
                    if (value[u].IsInt()) {
                        ids.push_back(value[u].GetInt());
                    }
                }
                free_audiobooks[key] = ids;
                
                FREE_STORY_MANAGER->setListFreeAudioBook(free_audiobooks);
            }
            else {
                continue;
            }
        }
    }

}

void mj::api::MJAPIDecodeObjectJsonInfo::onDecodePurchasedInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response)
{

    bool check_time = false;
    APPURCHASE_MANAGER->reset();
    if (JSON_SAFE_CHECK(map_response["purchased"], "time_expire", Int) &&
        JSON_SAFE_CHECK(map_response, "current_time", Int))
    {
        auto time_expire = map_response["purchased"]["time_expire"].GetInt();
        auto current_time = map_response["current_time"].GetInt();

        if (time_expire > current_time) check_time = true;
    }

    bool isActive = false;

    if (JSON_SAFE_CHECK(map_response["purchased"], "is_free", Bool))
    {
        bool isFreeUse = map_response["purchased"]["is_free"].GetBool();
        APPURCHASE_MANAGER->setFree(isFreeUse);
        MJDEFAULT->setBoolForKey("check_is_free", isFreeUse);
        if (check_time) isFreeUse = false;

        if (JSON_SAFE_CHECK(map_response["purchased"], "is_active", Bool))
        {
            bool isActive = map_response["purchased"]["is_active"].GetBool() & !isFreeUse;
            APPURCHASE_MANAGER->setActive(map_response["purchased"]["is_active"].GetBool());
        }
    }

    bool isSkip = JSON_SAFE_EXTRACT(map_response["purchased"], "is_user_skip", Bool, false);

    F2P_MANAGER->setSkipUser(isSkip);

    APPURCHASE_MANAGER->setSkipUser(isSkip);

    
    APPURCHASE_MANAGER->setFreeDays(JSON_SAFE_EXTRACT(map_response["purchased"], "free_days", Int, -1));//count down 7 free days trial.

    MJDEFAULT->setBoolForKey(key_isPurchased, JSON_SAFE_EXTRACT(map_response["purchased"], "is_purchased", Bool, false));
    APPURCHASE_MANAGER->setPurchased(JSON_SAFE_EXTRACT(map_response["purchased"], "is_purchased", Bool, false));

    if (JSON_SAFE_CHECK(map_response["purchased"], "purchased_packages", Object))
    {
        auto purchasedPackages = map_response["purchased"]["purchased_packages"].GetObject();
        if (JSON_SAFE_CHECK(purchasedPackages, "iap", Array))
        {
            auto iap = purchasedPackages["iap"].GetArray();
            for (int i = 0; i < iap.Size(); i++)
            {
                InAppPackageModel model;
                model._purchasedType = InAppPackageModel::PurchasedType::IAP;
                if (iap[i].IsString() && !iap[i].IsNull())
                {
                    model._productId = iap[i].GetString();
                }

                APPURCHASE_MANAGER->addPurchasedPackage(model);
            }
        }

        if (JSON_SAFE_CHECK(purchasedPackages, "cod", Array))
        {
            auto cod = purchasedPackages["cod"].GetArray();
            for (int i = 0; i < cod.Size(); i++)
            {
                InAppPackageModel model;
                model._purchasedType = InAppPackageModel::PurchasedType::COD;
                if (cod[i].IsString() && !cod[i].IsNull())
                {
                    model._productId = cod[i].GetString();
                }
                APPURCHASE_MANAGER->addPurchasedPackage(model);
            }
        }

    }

    if (!isActive) {
        if (map_response["purchased"].HasMember("time_expire")) {
            MJDEFAULT->setIntegerForKey(key_freetime_expire, map_response["purchased"]["time_expire"].GetInt());
        }

        if (MJDEFAULT->getIntegerForKey(key_app_firstdate, 0) == 0) {
            if (map_response.HasMember("day_readed")) {
                if (!map_response["day_readed"].IsNull() && map_response["day_readed"].IsString()) {
                    std::string day_readed = map_response["day_readed"].GetString();
                    if (day_readed != std::string("")) {
                        auto story_lang_id = std::atoi(day_readed.c_str());
                        if (story_lang_id > 0) {
                            auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
                            auto date_current = LTDate(time_current_day);
                            auto key = cocos2d::StringUtils::format("%s_%d_%d_%d", key_freetime_read, date_current.year, date_current.month, date_current.day);
                            MJDEFAULT->setIntegerForKey(key, story_lang_id);
                            FREE_STORY_MANAGER->setPercentActionFreePlayed(100);
                        }
                    }
                }
            }
            MJDEFAULT->setIntegerForKey(key_app_firstdate, MJDEFAULT->getIntegerForKey(key_time_curent));
        }

        mj::Pay::getInstance().forceOverrideLicense("", 0);
        mj::Pay::getInstance().updateActiveStatus(true, false);
    }
    else {
        mj::Pay::getInstance().updateActiveStatus(true, true);
    }

    if (JSON_SAFE_CHECK(map_response, "purchased", Object))
    {
        auto map_purchased = map_response["purchased"].GetObject();

        if (JSON_SAFE_CHECK(map_purchased, "time_activated", Int))
        {
            auto time = map_purchased["time_activated"].GetInt();
            APPURCHASE_MANAGER->setTimeActivated(time);
            MJDEFAULT->setIntegerForKey("key_time_activated", time);
        }

        if (JSON_SAFE_CHECK(map_purchased, "time_purchased", Int))
        {
            auto time = map_purchased["time_purchased"].GetInt();
            APPURCHASE_MANAGER->setTimePurchased(time);
            MJDEFAULT->setIntegerForKey("key_time_purchased", time);
        }
    }

    ms::EventLogger::getInstance().setUserProperties("purchased", isActive ? "yes" : "no");

    std::string license = "";
    if (map_response.HasMember("licence") && map_response["licence"].IsString()) {
        license = map_response["licence"].GetString();
    }
    auto time_expire = 0;
    if (map_response["purchased"].HasMember("time_expire") && map_response["purchased"]["time_expire"].IsInt()) {
        time_expire = map_response["purchased"]["time_expire"].GetInt();
        APPURCHASE_MANAGER->setTimeExpire(time_expire);
    }

    if (time_expire == 0 && isActive) {
        time_expire = -1;
        APPURCHASE_MANAGER->setTimeExpire(time_expire);
    }

    MJDEFAULT->setIntegerForKey("key_time_expire", time_expire);

    mj::Pay::getInstance().forceOverrideLicense(license, time_expire);

}

void mj::api::MJAPIDecodeObjectJsonInfo::loadThumbUrl(std::vector<std::string> listUrl, RemoteImageView::Type type)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

        auto typeUrl = (type == RemoteImageView::Type::STORIES) ? mj::UrlType::MSURL_STORY : mj::UrlType::MSURL_AUDIO;

        for (int i = 0; i < listUrl.size(); i++)
        {
            auto thumb = mj::helper::getUrlFromDomain(listUrl[i], typeUrl);
            ms::utils::ThumbQueue::getInstance().enqueue(thumb, nullptr, type, true);
        }
    });
}

