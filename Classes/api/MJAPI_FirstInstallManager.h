#ifndef MJAPIFirstInstallManager_hpp
#define MJAPIFirstInstallManager_hpp

#include "MJRequest.h"
#include "network/CCDownloader.h"
#pragma once
namespace mj {
	namespace api {
		class MJAPIFirstInstallManager : public RequestDelegate
		{
        public:
            static void call(std::function<void(bool)> callback = nullptr);

        private:
            void send();
            MJAPIFirstInstallManager();

            std::function<void(bool)> _callback;
            std::function<void(float)> _callback_progress;

            std::shared_ptr<cocos2d::network::Downloader> _downloader;

            Request* _request = nullptr;
            int onRequestRaw(int tag, std::string data) override;
            void onRequestSuccess(int tag, rapidjson::Value data) override;
            void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;

            void DecodeAppInfo(int tag, rapidjson::Document* document);
            void IntinialStoryInfo(int tag, rapidjson::Document* document);
            void IntinialAISpeakingInfo(int tag, rapidjson::Document* document);
            void IntinialAudioInfo(int tag, rapidjson::Document* document);
            void IntinialWorksheetInfo(int tag, rapidjson::Document* document);
            void IntinialGameListInfo(int tag, rapidjson::Document* document);
            void IntinialLessonListInfo(int tag, rapidjson::Document* document);
            void IntinialListActivitiyInfo(int tag, rapidjson::Document* document);
            void IntinialCategoriesInfo(int tag, rapidjson::Document* document);
            void IntinialProductListInfo(int tag, rapidjson::Document* document);
            void IntinialLangguageListInfo(int tag, rapidjson::Document* document);
            void IntinialAppProblemsInfo(int tag, rapidjson::Document* document);
            void GetListThumbZip(int tag, rapidjson::Document* document);


        };
	
	}
}

#endif /* MJAPIFirstInstallManager_hpp */
