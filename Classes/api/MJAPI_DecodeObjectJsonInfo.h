#ifndef MJAPIDecodeObjectJsonInfo_hpp
#define MJAPIDecodeObjectJsonInfo_hpp
#pragma once
#include "MJRequest.h"
#include "MJStoryInfoModel.h"
#include "ThumbQueue.h"
#include "F2PCommon.h"
#define DAYS_BETWEEN_CALL 2

namespace mj {
    namespace api {
        using namespace ms::f2p;
        using namespace ms::MsTypes;

        class MJAPIDecodeObjectJsonInfo
        {

        public:
            enum IsStateUpdateFistInstall
            {
                None,
                Default,
                First_Install,
                Update,
            };
            static int onDecodeAppInfoData(rapidjson::Document* document);
            static void onDecodeLocationAndDeviceInfo(int tag, rapidjson::Document* document);
            static void onDecodeDomainInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onSetDomainByLocation();

            static void onDecodeStoryData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeF2PTrialItemsData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static std::vector<mj::model::FreeItemInfo> onDecodeF2PTrialItemsDataOptionItem(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response, const char* subOptionItems, FreeContentOption option);

            static void onDecodeAISpeakingData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static std::vector<mj::model::AISpeakingCategoryInfo> onDecodeAISpeakingTopicData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeAISpeakingLessonListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeAISpeakingActivitiesListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static std::vector<mj::model::TalkingCommonInfo> onDecodeAISpeakingCommonInfoListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            

            static void readAISpeakingCommonInfoActorVideoURL(rapidjson::GenericArray<false, rapidjson::Value> map_response,std::string childKey,mj::model::TalkingCommonInfo &info, std::string dir = "");

            static void onDecodeAudioData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeWorksheetData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeGameListData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeLessonListData(IsStateUpdateFistInstall tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeStoryLessonInteractiveData(int tag, rapidjson::GenericArray<false, rapidjson::Value> data);
            static void onDecodeLocationData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeCategoriesInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeProductListInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeAppProblemsListInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeLangguageListInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeVersionAPIInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodePurchasedInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeGeneralUpdateInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeUserInfoData(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static void onDecodeFreBookAndFreeStory(int tag, rapidjson::GenericObject<false, rapidjson::Value> map_response);
            static bool shouldBeCalled();
            static void loadThumbUrl(std::vector<std::string> listUrl, RemoteImageView::Type type);

        };
    }
}
#endif /* MJAPIZip_hpp */
