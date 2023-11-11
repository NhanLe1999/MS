//
//  MJHelper.h
//  M
//
//  Created by To Ngoc Linh on 5/5/17.
//
//

#ifndef MJHelper_hpp
#define MJHelper_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "platform/CCPlatformConfig.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MJHelper-Apple.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MJHelper-Android.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "MJHelper-Win32.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "MJHelper-Mac.h"
#endif

#define k_event_pay_popup_close "com.earlystart.stories.pay_subpopup.close"
#define key_device_retina "key.user.device.retina"


#define JSON_SAFE_CHECK(a, b, c) (a.HasMember(b) && (!a[b].IsNull()) && a[b].Is##c())

#define JSON_SAFE_EXTRACT(doc, key, type, default_value) (JSON_SAFE_CHECK(doc, key, type) ? doc[key].Get##type() : default_value)

#define MK_RETURN_IF(x, ...)  if(x){ CCLOG("error: return at %s", __FUNCTION__); return __VA_ARGS__;}

#define ATOF(x) cocos2d::utils::atof(x)
#define ATOI(x) static_cast<int>(ATOF(x))

#define URL_ORIGIN_URL     "origin_url"
#define URL_RAW_URL     "url"
#define URL_PROTOCOL    "protocol"
#define URL_DOMAIN      "domain"
#define URL_PORT        "port"
#define URL_ROOT_PART   "part"
#define URL_QUERY       "query"

#define mjhelper_name_storyinfocover            "mjhelper_name_storyinfocover"
#define mjhelper_name_audiobookplayer           "audiobook_player"
#define mjhelper_name_collectionaudiobookitem   "mjhelper_name_collectionaudiobookitem"
class MSCollectionStoryItem;
class MSCollectionAudiobookItem;

namespace mj {

    enum PayMethod {
        MJPAY_ONEPAY,
        MJPAY_COD,
        MJPAY_BANK_TRANSFER,
        MJPAY_STORE,
        MJPAY_BANK_INFO,
        MJPAY_NONE
    };
    enum UrlType {
        MSURL_STORY,
        MSURL_GAME,
        MSURL_AUDIO,
        MSURL_ACTIVITIES,
        MSURL_MEDIA,
        MSURL_AI_SPEAKING_CATEGORY_ICON,
        MSURL_NONE
    };

    enum FROMSTORY {
        DEFAULT = 0,
        FREE_TWO_PAID = 1
    };

    namespace helper {

        template<typename T>
        T get_random_item(std::vector<T> list) {
            if (list.size() == 0) {
                return T();
            }
            int index = cocos2d::RandomHelper::random_int(0, (int)list.size() - 1);
            return list[index];
        }

        void openDeviceAppSettings();
        
        bool isDigitString(const std::string& str);         // kiểm tra xem một chuỗi có phải toàn số hay không
        bool isNameValid(const std::string& name);          // kiểm tra xem một chuỗi tên có đúng không

        bool isEnglishCharacter(const char Character);
        bool isNumber(const char Character);
        bool isEmailAddressValid(const std::string& email); // kiểm tra xem một e-mail có hợp lệ không
        cocos2d::ui::Button* addSpeaker(cocos2d::ui::Layout* layout);
        bool str_isEqual(const std::string& str1, const std::string& str2);
        
        bool str_ispunct(const char& c);                                // check ký tự đặc biệt
        std::string str_standardize_keyword(const std::string& str);    // chuẩn hoá từ khoá
        
        inline std::string& str_rtrim(std::string& s, const char* t);   // trim bên phải chuỗi
        inline std::string& str_ltrim(std::string& s, const char* t);   // trim bên trái chuỗi
        inline std::string& str_rtrim(std::string& s);                  // trim bên phải chuỗi
        inline std::string& str_ltrim(std::string& s);                  // trim bên trái chuỗi
        std::string str_trim(const std::string& str);                   // trim 2 đầu
    
        std::map<std::string, std::string> url_parse(const std::string& raw_url);
        std::map<std::string, std::string> url_query_parse(const std::string& query);
    
        long long getTime();
        long long getTimeInMinutes();
        long long getTimeInHours();
        int getTimeInDays();
        int getCurrentYear();
        int getCurrentMonth();
        int getCurrentDay();
        void trimUIText(cocos2d::ui::Text * text, float width);
        
        void resetDatabase();
        bool isContainAlnum(const std::string & s);
        int parseDynamicLinkQR(const std::string & url);
        void startSpeechToText(std::function<void(std::string)> updateCallback);
        void stopSpeechToText(std::function<void(std::string)> updateCallback);
        std::string replaceDomain(std::string url, std::string domain);
        void purgeTextureData();
        void hideTap();
        void showTap(cocos2d::Point p);
        std::string compress_string(const std::string& str, int compressionlevel);
        std::string decompress_string(const std::string& encoded);
        cocos2d::Color3B colorFromHex(int hex);
        void grayscaleImageView(cocos2d::ui::ImageView * view, bool enable = true);
        void outlineImageView(cocos2d::ui::ImageView * view, bool enable = true);
        void grayscaleImageV2(cocos2d::ui::ImageView * view, bool enable = true);
        
        void playTransitionStoryCover(MSCollectionStoryItem * item, cocos2d::ui::Widget * content, double ratio = -1,int zorder = 0,const mj::FROMSTORY &from = mj::FROMSTORY::DEFAULT);
        void playTransitionAudiobookCover(MSCollectionAudiobookItem * item, cocos2d::ui::Widget * content, double ratio = -1);
        
        void playButtonFX();
        void playButtonFXClose();
        void playButtonFXOpen();
        
        std::pair<std::string, std::string> extractSeries(std::string title);
        
        std::string extractActivityName(std::string name);
        std::string extractAudiobookSeriesName(std::string name);
        std::string getActivitySound(std::string name);
        std::string getForcedAlignment(const std::string & audio_path);
        std::string convertForcedAlignment(const std::string & original_string, const std::string & content);
        
        unsigned char * unzip_json(std::string buffer);
        bool unzip(std::string zip, std::string directory);
        void unzipAsync(std::vector<std::pair<std::string, std::string>> zips, std::function<void(bool)> callback);
        void unzipAsync(std::string zip_file, std::string directory, std::function<void(bool)> callback);
        void setTextWidth(cocos2d::ui::Text * text, float width);
        std::string getFilenameFromURL(std::string url);
        std::string getFileNameWithoutExtentionFromURL(std::string url, std::string extention = ".zip");
        void getFolderSize(const std::string & folder_path, const std::string & callback_event);
        float setQueryForderSize(std::string folder_path);
        float getTotalSizeFileDownload();

        cocos2d::Sprite * captureNode(cocos2d::Node* startNode, float scale = 1);

        std::string prettyInteger(int n);
        std::string prettyPrice(float price, std::string currency);

        std::string epochTimeToText(long epoch_time);
        std::string epochTimeToText(long epoch_time,std::string i_formation);
    
        template<typename T>
        void shuffle(std::vector<T> &v) {
            static std::random_device mj_rd;
            static std::mt19937 mj_g(mj_rd());
            std::shuffle(v.begin(), v.end(), mj_g);
        }

        template<typename Out>
        void split(const std::string &s, char delim, Out result) {
            std::stringstream ss;
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                *(result++) = item;
            }
        }

        template<typename T>
        bool isContain(const std::vector<T> &array, const T & value) {
            return std::find(array.begin(), array.end(), value) != array.end();
        }

        bool isValidURL(std::string url);
        std::string getUrlFromDomain(std::string url, UrlType urlType,const int &index = -1, const std::vector<std::string> &listDomian = {},const std::string &typeError = "");
        std::string getUrlFromDomainNew(std::string url, UrlType urlType,const int& index, const std::vector<std::string> &listDomian, const std::string& typeError);
        std::vector<std::string> split(const std::string &s, char delim);
        std::vector<std::string> splitString(const std::string &s, char delim);

        int positionInString(std::string src, std::string tar, int index);
        std::string replace_string(std::string src, const std::string & tar, const std::string & rep);
        std::string replace_string(std::string src, const std::string & tar, std::vector<int> pos, const std::string & rep);
        void replace_string_all(std::string &str, const std::string& from, const std::string& to);

        std::string map_to_string(std::map<std::string, std::string>& m);
        std::map<std::string, std::string> string_to_map(std::string const& s);
//        std::wstring toLowerCase(const std::wstring &s);
//        std::wstring toUpperCase(const std::wstring &s);
//
        std::string ltrim(std::string& str, std::string character = " ");
    
        std::string to_upper(const std::string &s);
        std::string to_lower(const std::string &s);
        std::string toCapitalizedCase(const std::string &s);
        std::string to_no_accents(const std::string &s);
        std::string to_no_accents_v2(const std::string &s);
        
        bool is_alpha(cocos2d::StringUtils::StringUTF8::CharUTF8 c);
        bool is_digit(cocos2d::StringUtils::StringUTF8::CharUTF8 c);
//
//        std::string toNarrowString(const std::wstring &s);
//        std::wstring toWideString(const std::string &s);
//
        std::string vectorIntToString(std::vector<int> v);
        std::vector<int> stringToVectorInt(std::string);

        bool isValidEmail(std::string email);
        bool isValidLoginEmail(std::string email);
        bool isValidNumber(std::string number);
		bool isUTF8StringValidNumber(std::string number);
		bool isValidPhoneNumber(std::string number);
        bool isValidPhoneNumberString(const std::string& phone, const int minLength = 5, const int maxLength = 14);
		bool isUTF8StringValidPhoneNumber(std::string number);
		
        std::string urlencode(const std::string & url);
        
        cocos2d::Rect getTextBoundingBox(cocos2d::ui::Text * text);
        
        std::string getCSBName(std::string csb_name);
        
        bool isPointInPoly(cocos2d::Point p, std::vector<cocos2d::Point> v);
        bool isThaiLanguage(std::string checkString);
        std::string shortString(std::string i_input, int i_maxLength, std::string i_endString);
        bool isRetina();
        void setRetina(int retina_enabled);
        bool isWidescreen();

		void removeCharsFromString(std::string &str, std::string charsToRemove);

		std::string jsonToString(rapidjson::Document & jsonObject);

        bool isNameInValid(std::string name);

        void removeSpecialCharacter(cocos2d::ui::TextField* name);
        bool isWriablePathValid(std::string path);
        std::string createWindowValidPath(std::string path);

        std::vector<std::pair<std::string, cocos2d::Value>> getValueOfEventUpdateUserInfo(std::string click_type = "no", std::string source_update = "no", std::string update_type = "no", std::string email_verified = "none",
            std::string update_info_successfull = "no", std::string edit_user_info = "false", std::string log_out = "no", std::string edit_profile = "false", std::string detail_profile = "false", std::string upgrade_account = "false");
        void pushEventCrashNetWork(std::string link = "", std::string errorString = "", std::string type = "", int errorcode = -100, int countRetry = 0, std::string speed = "");
    
        void pushEvnetLostConnection(std::string link = "", std::string errorString = "", std::string type = "", std::string errorcode = "", std::string countRetry = 0, std::string speed = "", std::string remoteAddress = "", std::string httpStatusCode = "", std::string type_download = "old", std::vector<std::string> listMessageErrorIos = {});
    
        char* substring(char* source, size_t begin, size_t n, char* target);
        
        std::vector<std::pair<std::string, cocos2d::Value>> cutTextToVectorString(std::string text, std::string key);

        float getMaxFontSize(std::vector<std::string> listWords, cocos2d::Size box, float maxFontSize = (float)LONG_MAX, std::string fontName = "fonts/Nunito-ExtraBold.ttf");
        
        bool softCompareString(std::string str1, std::string str2);

        void setAnchorPointWithoutChangePosition(cocos2d::Node* node, cocos2d::Vec2 newAnchorPoint);

        void doAppearGradient(cocos2d::Node * n, float duration = 1.0f);
		
        bool isProfileNameValid(std::string name,int maxLength=15);
        std::string getStringTime(long long timestamp, std::string format);
        void encryptString(std::string &i_input);
        void decryptString(std::string &i_input);
        void showWaitView();
        void deleteWaitView();
        float getDiskSpace();
        void downloadFile(std::string url, std::function<void()> callback);

        void coppyFile(std::string pathFileCoppy = "", std::string pathFileToCopy = "");
        bool showPopupWarningFullDisk();

        void pasteText(cocos2d::ui::TextField* textField);
        void copyText(cocos2d::ui::TextField* textField);

        long long unixTimestamp(int year, int month, int day, int hour, int min, int sec);
    
        void inertiaScrollLeftOrRight(cocos2d::Node* parent, cocos2d::ui::ListView* listView, bool isLeft);
        void autoScaleRootLayout(float scaleFactor, cocos2d::Node* parent, std::string rootLayout);
        void setGlobalZorderForNode(cocos2d::Node* parent, int zorder);
        void ResetUserData();
        void SetFpsDefault();
        void SetFpsByNumberFrames(int framePerSecond);
        float lerp(float a, float b, float x, float y, float i);
        std::string getPackageTime(std::string producId);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        void changeIconAppForIos(std::string nameIcon);
        std::vector<int> getDateTimeIOS();
#endif
    void showATTrackingNotification();

    }
}

#endif /* MJHelper_hpp */
