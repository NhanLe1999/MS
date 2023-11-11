#include "MJAPI_Audiobook_Detail.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryDataModel.h"
#include "MJStoryInfoModel.h"
#include "StoryFreeManager.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"
#include <fstream>
#include "MsNewRequest.h"


namespace mj {
	namespace api {
		void MJAPI_Audiobook_Detail::call(std::string bookId, std::function<void(bool, std::string)> callback) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			ms::newApi::Request::getInstance().setNotRequestApiLogOut(false);
#endif
			auto refresh_call = new MJAPI_Audiobook_Detail(bookId);
			refresh_call->_callback = callback;
			refresh_call->send();
		}

		MJAPI_Audiobook_Detail::MJAPI_Audiobook_Detail(std::string bookId) {
			_request = new mj::Request(mj::Request::ActionType::AUDIOBOOK_DETAIL, {
				{ "id",bookId},
				});
			_request->setDelegate(this);
		}

		void MJAPI_Audiobook_Detail::send() {
			if (_request) {
				_request->send();
			}
		}

		int MJAPI_Audiobook_Detail::onRequestRaw(int tag, std::string data) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			ms::newApi::Request::getInstance().setNotRequestApiLogOut(true);
#endif
			CCLOG("MJAPI_Audiobook_Detail::onRequestRaw ");

			auto unzip_json = mj::helper::unzip_json(data);
			if (!unzip_json) {
				return -1;
			}

			rapidjson::Document doc;
			doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char *)unzip_json);

			if (doc.HasParseError()) {
				doc.GetParseError();
				return -1;
			}

			if (!doc.IsString()) {
				return -1;
			}


			if (_callback) {
				_callback(true, doc.GetString());
			}
		
			return 1;
		}

		void MJAPI_Audiobook_Detail::onRequestSuccess(int tag, rapidjson::Value data) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			ms::newApi::Request::getInstance().setNotRequestApiLogOut(true);
#endif
			delete this;
		}

		void MJAPI_Audiobook_Detail::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			ms::newApi::Request::getInstance().setNotRequestApiLogOut(true);
#endif
			if (_callback) {
				_callback(false, message);
			}
			delete this;
		}
	}
}
