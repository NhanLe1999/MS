//
//  MJHelper.cpp
//  M
//
//  Created by To Ngoc Linh on 5/5/17.
//
//

#include "MJHelper.h"
#include <clocale>
#include <regex>
#include <numeric>
#include <locale>
//#include <codecvt>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>
#include <regex>
#include "StoryInfoCover.h"
#include "MSCollectionStoryItem.h"
#include "MSCollectionAudiobookItem.h"
#include "MSAudiobookPlayer.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "AudioEngine.h"
#include "MJPlatformConfig.h"
#include <filesystem>
#include "NativeBridge.h"
#include "MSLessonPreview.h"
#include "StoryPageVC.h"
#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include <../external/unzip/unzip.h>
#endif
#include "id3v2lib.h"
#include "APProfileManager.h"
#include "MJDebugView.h"
#include "StoryFreeManager.h"

#define ZIP_BUFFER_SIZE    8192
#define ZIP_MAX_FILENAME   512
#define MIN_SIZE_DISK 100 //MB

USING_NS_CC;

namespace mj {
    namespace helper {
        void openDeviceAppSettings() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            openDeviceAppSettings_ios();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            openDeviceAppSettings_android();
#endif
        }
#pragma mark - String
        bool isDigitString(const std::string& str) {
            return !str.empty() && std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
        }
        
        bool isNameValid(const std::string& name) {
//            if (name.empty()) {
//                return false;
//            }
//            try {
//                for(auto c:name) {
//                    if(!isalpha(c)&&!isdigit(c)&&c!=' '&&c!='_'&&c!='-') {
//                        return false;
//                    }
//                }
//            } catch (const std::exception&) {
//
//            }
         
            return true;
        }
    
        bool isEnglishCharacter(const char Character) {
            return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
            //Checks if a Character is a Valid A-Z, a-z Character, based on the ascii value
        }
        
        bool isNumber(const char Character) {
            return ( Character >= '0' && Character <= '9');
            //Checks if a Character is a Valid 0-9 Number, based on the ascii value
        }
        
        bool isEmailAddressValid(const std::string& EmailAddress) {
            // If cannot read the Email Address...
            if(EmailAddress.empty())
                return 0;
            // If the First character is not A-Z, a-z or 0-9
            if(!(isEnglishCharacter(EmailAddress[0])||isNumber(EmailAddress[0])))
                return 0;
            // If contain a special character
            const regex pattern("[ |~|!|#|$|%|^|&|*|/|?|,|;|\\\\]");
            if (std::regex_search(EmailAddress, pattern))
                return 0;
            // START CHECKING
            int AtCounter = 0;
            int AtOffset = -1;
            int DotOffset = -1;
            unsigned int Length = (int)EmailAddress.length(); // Length = StringLength (strlen) of EmailAddress
            for(unsigned int i = 0; i < Length; i++) {
                if(EmailAddress[i] == '@') { // If one of the characters is @, store it's position in AtOffset
                    AtOffset = (int)i;
                    AtCounter++;
                } else if(EmailAddress[i] == '.') { // Same, but with the dot
                    DotOffset = (int)i;
                }
            }
            // If cannot find a Dot or a @
            if(AtCounter > 1 || AtOffset == -1 || DotOffset == -1)
                return 0;
            // If the @ is after the Dot
            if(AtOffset > DotOffset)
                return 0;
            //Chech there is some other letters after the Dot
            return !(DotOffset >= ((int)Length-1));
        }
        
        bool str_ispunct(const char& c) {
            return ispunct(static_cast<int>(c));
        }
        
        std::string str_standardize_keyword(const std::string& str) {
            /* CHUẨN HOÁ CHUỖI
             * - thay các ký tự đặc biệt "-", "_", ":"... bằng dấu cách
             * - trim chuỗi để loại bỏ các dấu cách ở đầu và cuối
             * - đổi hết các chữ hoa sang chữ thường
             */
            std::string sss = str;
            std::replace_if(sss.begin(), sss.end(), str_ispunct, ' ');
            sss = mj::helper::str_trim(sss);
            sss = mj::helper::to_lower(sss);
            return sss;
        }
    
        bool str_isEqual(const std::string& str1, const std::string& str2) {
            return str1.compare(str2) == 0 ? true : false;
        }
    
        inline std::string& str_rtrim(std::string& s, const char* t) {
            s.erase(s.find_last_not_of(t) + 1);
            return s;
        }
    
        inline std::string& str_ltrim(std::string& s, const char* t) {
            s.erase(0, s.find_first_not_of(t));
            return s;
        }
    
        inline std::string& str_rtrim(std::string& s) {
            const char* ws = " \t\n\r\f\v";
            return str_rtrim(s, ws);
        }
    
        inline std::string& str_ltrim(std::string& s) {
            const char* ws = " \t\n\r\f\v";
            return str_ltrim(s, ws);
        }
    
        std::string str_trim(const std::string& str) {
            auto s = str;
            return str_ltrim(str_rtrim(s));
        }
        
#pragma mark - URL parse
        std::map<std::string, std::string> url_parse(const std::string& raw_url) {
            if(raw_url.empty()) {
                return  std::map<std::string, std::string>();
            }
            std::map<std::string, std::string> RESULT;
            std::string path, domain, x, protocol, port, query;
            size_t pos1, pos2, pos3, pos4;
            int offset = 0;
            
            x = str_trim(raw_url);
            offset = offset==0 && x.compare(0, 8, "https://")==0 ? 8 : offset;
            offset = offset==0 && x.compare(0, 7, "http://" )==0 ? 7 : offset;
            pos1 = x.find_first_of('/', offset+1 );
            path = pos1==std::string::npos ? "" : x.substr(pos1);
            if (std::strlen(path.c_str()) > 1) path = path.substr(1);
            domain = std::string( x.begin()+offset, pos1 != std::string::npos ? x.begin()+pos1 : x.end() );
            path = (pos2 = path.find("#"))!=std::string::npos ? path.substr(0,pos2) : path;
            port = (pos3 = domain.find(":"))!=std::string::npos ? domain.substr(pos3+1) : "";
            domain = domain.substr(0, pos3!=std::string::npos ? pos3 : domain.length());
            protocol = offset > 0 ? x.substr(0,offset-3) : "";
            query = (pos4 = path.find("?"))!=std::string::npos ? path.substr(pos4+1) : "";
            path = pos4!=std::string::npos ? path.substr(0,pos4) : path;
            
            RESULT[URL_RAW_URL]     = raw_url;
            RESULT[URL_PROTOCOL]    = protocol;
            RESULT[URL_DOMAIN]      = domain;
            RESULT[URL_PORT]        = port;
            RESULT[URL_ROOT_PART]   = path;
            RESULT[URL_QUERY]       = query;
            
            return RESULT;
        }
        
        std::map<std::string, std::string> url_query_parse(const std::string& query) {
            std::map<std::string, std::string> QUERY_PARSE;
            std::vector<std::string> elems = split(query, '&');
            
            for (auto e : elems) {
                std::vector<std::string> zzz = split(e, '=');
                QUERY_PARSE[zzz.at(0)] = zzz.size() > 1 ? zzz.at(1) : "";
            }
            
            return QUERY_PARSE;
        }
    
#pragma mark - Zzz
        long long getTime() {
            auto timeSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            return timeSinceEpoch;
        }
        long long getTimeInMinutes() {
            return getTime() / 60;
        }
        long long getTimeInHours() {
            return getTime() / 3600;
        }
        int getTimeInDays() {
            return getTime() / (3600*24);
        }

        int getCurrentYear() {
            time_t _current_year = time(NULL);
            return 1900 + (localtime(&_current_year))->tm_year;
        }
        int getCurrentMonth() {
            time_t _current_month = time(NULL);
            return 1 + (localtime(&_current_month))->tm_mon;
        }
        int getCurrentDay() {
            time_t _current_day = time(NULL);
            return (localtime(&_current_day))->tm_mday;
        }
        void resetDatabase() {
            //set all api to zero
            MJDEFAULT->setBoolForKey(key_first_api_called, true);
            MJDEFAULT->setIntegerForKey(key_api_version_all_games_local, 0);
            MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_local, 0);
            MJDEFAULT->setIntegerForKey(key_api_version_audio_local, 0);
            MJDEFAULT->setIntegerForKey(key_api_version_story_local, 0);
            MJDEFAULT->setIntegerForKey(key_api_version, 0);
            MJDEFAULT->setIntegerForKey(key_version_api_audiobook, 0);
        }
        int parseDynamicLinkQR(const std::string & url) {
            if (url.find("monkeystories.vn") == std::string::npos) {
                return - 1;
            }
            std::vector<std::string> sp = split(url, '?');
            if (sp.size() > 1) {
                auto endpoint = sp[0].substr(sp[0].find_last_of('/') + 1, 100);
                if (endpoint != "qrbook") {
                    return -1;
                }
                auto param = sp[1];
                std::vector<std::string> p = split(param, '&');
                for (auto e : p) {
                    std::vector<std::string> i = split(e, '=');
                    if (i[0] == "id") {
                        return std::atoi(i[1].c_str());
                    }
                }
            }
            
            return -1;
        }
    
        std::string getPackageTime(std::string producId) {
            if (producId.find("6month") != std::string::npos) {
                return "6 months";
            }
            if (producId.find("1year") != std::string::npos) {
                return "1 year";
            }
            if (producId.find("1month") != std::string::npos) {
                return "1 month";
            }
            if (producId.find("3month") != std::string::npos) {
                return "3 months";
            }
            if (producId.find("2year") != std::string::npos) {
                return "2 years";
            }
            return "lifetime";
        }
        
        class comma_numpunct : public std::numpunct<char> {
        protected:
            virtual char do_thousands_sep() const {
                return '.';
            }

            virtual char do_decimal_point() const {
                return ',';
            }

            virtual std::string do_grouping() const {
                return "\03";
            }
        };
        
        void setTextWidth(ui::Text * text, float width) {
            text->ignoreContentAdaptWithSize(false);
            text->setTextAreaSize(Size(width, 0));
            text->setContentSize(text->getVirtualRendererSize());
        }

        Sprite * captureNode(Node* startNode, float scale)
        { // The best snapshot API, support Scene and any Node
            auto& size = startNode->getContentSize();

            Director::getInstance()->setNextDeltaTimeZero(true);

            RenderTexture* finalRtx = nullptr;

            auto rtx = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
            // rtx->setKeepMatrix(true);
            Point savedPos = startNode->getPosition();
            Point anchor;
            if (!startNode->isIgnoreAnchorPointForPosition()) {
                anchor = startNode->getAnchorPoint();
            }
            startNode->setPosition(Point(size.width * anchor.x, size.height * anchor.y));
            rtx->begin();
            startNode->visit();
            rtx->end();
            startNode->setPosition(savedPos);

            if (std::abs(scale - 1.0f) < 1e-6f/* no scale */) {
                finalRtx = rtx;
            }
            else {
                /* scale */
                auto finalRect = Rect(0, 0, size.width, size.height);
                Sprite *sprite = Sprite::createWithTexture(rtx->getSprite()->getTexture(), finalRect);
                sprite->setAnchorPoint(Point(0, 0));
                sprite->setFlippedY(true);

                finalRtx = RenderTexture::create(size.width * scale, size.height * scale, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);

                sprite->setScale(scale); // or use finalRtx->setKeepMatrix(true);
                finalRtx->begin();
                sprite->visit();
                finalRtx->end();
            }

            Director::getInstance()->getRenderer()->render();

            return Sprite::createWithTexture(finalRtx->getSprite()->getTexture());
        }
        
        std::string getFilenameFromURL(std::string url) {
            
            if(url.empty()) {
                return url;
            }
            
            auto pos = url.find_last_of('/');
            auto temp = url.substr(pos + 1);
            pos = temp.find_first_of('?');
            if (pos != std::string::npos) {
                temp = temp.substr(0, pos);
            }
            return temp;
        }
        
        std::string getFileNameWithoutExtentionFromURL(std::string url, std::string extention ) {
            std::string fullFileName = getFilenameFromURL(url);
            std::string fileNameWithoutExtention = "";
            // Check if the last three characters match the ext.
            const std::string ext(extention);
            if ( fullFileName != ext &&
                fullFileName.size() > ext.size() &&
                fullFileName.substr(fullFileName.size() - ext.size()) == extention )
            {
               // if so then strip them off
                fileNameWithoutExtention = fullFileName.substr(0, fullFileName.size() - ext.size());
            }
            return fileNameWithoutExtention;
        }
        std::string getBasenameFromURL(const std::string& path) {
            size_t found = path.find_last_of("/\\");
            
            if (std::string::npos != found) {
                return path.substr(0, found);
            } else {
                return path;
            }
        }

        std::string prettyInteger(int n){
            std::stringstream ss;
            std::locale comma_locale(std::locale(), new comma_numpunct());
            ss.imbue(comma_locale);
            ss << std::fixed << n;
            return ss.str();
        }

        std::string epochTimeToText(long epoch_time) {

            // TODO: custom date for vietnamese

            time_t time_date = epoch_time;
            struct tm date = *localtime(&time_date);
            std::stringstream ss;
            char buffer[1000];
            std::strftime(buffer, sizeof buffer, "%a, %d %b %Y", &date);

            return std::string(buffer);
        }
    
        std::string epochTimeToText(long epoch_time,std::string i_formation) {
            
            if(epoch_time <= 0)
            {
                return std::string("0");
            }
            
            time_t time_date = epoch_time;
            struct tm date = *localtime(&time_date);
            std::stringstream ss;
            char buffer[1000];
            std::strftime(buffer, sizeof buffer, i_formation.c_str(), &date);
            
            return std::string(buffer);
        }

        bool isValidURL(std::string url)  {
            return !url.empty() && ((url.find("http") != std::string::npos) || (url.find("https") != std::string::npos));
        }
    
        std::string getLinkOfDomain(std::string defaultDomain, std::string typeError, std::vector<std::string> listUrl, int index)
        {

            if (typeError == key_error_url_stories && index != -1)
            {
                return (!listUrl.empty() && index < listUrl.size()) ? listUrl[index] : defaultDomain;
            }
            else if(typeError == key_error_url_game_audio_book && index != -1)
            {
                return (!listUrl.empty() && index < listUrl.size()) ? listUrl[index] : defaultDomain;
            }
            else {
                return defaultDomain;
            }
        }

        std::string getUrlFromDomainNew(std::string url, UrlType urlType,const int& index, const std::vector<std::string> &listUrl, const std::string &typeError)
        {
            return "";
        }

        std::string getUrlFromDomain(std::string url, UrlType urlType,const int& index, const std::vector<std::string>& listUrl,const std::string& typeError)
        {
            if(listUrl.size() == 2)
            {
                std::string a1 = listUrl[0];
                std::string a2 = listUrl[1];

                int kk = 0;
            }
            if (mj::helper::isValidURL(url))
                return url;

            string domain;

            switch (urlType)
            {
            case mj::MSURL_STORY:            
            {
                domain = getLinkOfDomain(MJDEFAULT->getStringForKey(DOMAIN_STORY_USING_KEY), typeError, listUrl, index);
                break;
            }
            case mj::MSURL_GAME:
            {
                domain = getLinkOfDomain(MJDEFAULT->getStringForKey(DOMAIN_GAME_USING_KEY), typeError, listUrl, index);
                
                break;
            }
            case mj::MSURL_AUDIO:
            {
                domain = getLinkOfDomain(MJDEFAULT->getStringForKey(DOMAIN_AUDIO_USING_KEY), typeError, listUrl, index);
                break;
            }
            case mj::MSURL_ACTIVITIES:
            {
                domain = getLinkOfDomain(MJDEFAULT->getStringForKey(DOMAIN_ACTIVITIES_USING_KEY), typeError, listUrl, index);
                break;
            }
            case mj::MSURL_MEDIA:
                domain = MJDEFAULT->getStringForKey(DOMAIN_MEDIA_USING_KEY);
                break;
            case mj::MSURL_AI_SPEAKING_CATEGORY_ICON:
                domain = MJDEFAULT->getStringForKey(DOMAIN_CATEGORY_URL_USING_KEY);
            case mj::MSURL_NONE:
                break;
            default:
                break;
            }
            if (domain.empty())
                return domain;
            else
            {

                std::string fullUrl;
                
                if (!url .empty() && url.front() == '/')
                {
                    url.erase(url.begin() + 0);
                }
                if (!domain.empty() && domain.back() == '/')
                {
                    fullUrl = domain + url;
                }
                else
                {
                    fullUrl = domain + '/'+ url;
                }
                return mj::helper::isValidURL(fullUrl) ? fullUrl: "";
            }

            return std::string();
        }
        
        std::vector<std::string> split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            split(s, delim, std::back_inserter(elems));
            return elems;
        }
        
        std::vector<std::string> splitString(const std::string &s, char delim) {
            std::vector<std::string> elems;
            split(s, delim, std::back_inserter(elems));
            return elems;
        }
        
        int positionInString(std::string src, std::string tar, int index) {
            auto counter = 0;
            auto idx = -1;
            while (counter < index) {
                idx = (int)src.find(tar, idx + 1);
                if (idx == std::string::npos) {
                    return (int)std::string::npos;
                }
                counter++;
            }
            
            return idx;
        }
        
        void replace_string_all(std::string &str, const std::string& from, const std::string& to) {
            size_t start_pos = 0;
            while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
        }
        
        std::string replace_string(std::string src, const std::string & tar, std::vector<int> pos, const std::string & rep) {
            if (pos.empty()) {
                return src;
            }
            
            std::sort(pos.begin(), pos.end());
            
            if (tar.empty()) {
                return src;
            }
            
            if (src.empty()) {
                return src;
            }
            
            auto counter = 0;
            auto idx = -1;
            while (counter < pos[0]) {
                idx = (int)src.find(tar, idx + 1);
                if (idx == std::string::npos) {
                    return src;
                }
                counter++;
            }
            
            src.replace(idx, tar.length(), rep);
            
            std::vector<int> new_pos;
            for (auto i = 1; i < pos.size(); ++i) {
                new_pos.push_back(pos[i] - 1);
            }
            
            return replace_string(src, tar, new_pos, rep);
        }

        std::string replace_string(std::string src, const std::string & tar, const std::string & rep) {
            if (tar.empty()) {
                return src;
            }

            if (src.empty()) {
                return src;
            }

            size_t idx = src.find(tar);
            if (idx == std::string::npos) {
                return src;
            }

            src.replace(idx, tar.length(), rep);

            return src;
        }
        
        string map_to_string(map<string, string>& m) {
            string output = "";
            string result = "";
            
            for (auto it = m.cbegin(); it != m.cend(); it++) {
                output += (it->first) + ":" + (it->second)+"\n";
            }
            
//            result = output.substr(0, output.size() - 2);
            result = output;
            return result;
        }
        
        map<string, string> string_to_map(string const& s)
        {
            std::map<std::string, std::string> m;
            
            std::string key, val;
            std::istringstream iss(s);
            
            while (std::getline(std::getline(iss, key, ':') >> std::ws, val))
                m[key] = val;
            
            return m;
        }

        float setQueryForderSize(std::string folder_path)
        {
            std::vector<std::string> list_files;
            FileUtils::getInstance()->listFilesRecursively(folder_path, &list_files);
            auto temp_size = std::accumulate(list_files.begin(), list_files.end(), 0l, [](long folder_size, std::string file_path) {
                return folder_size + FileUtils::getInstance()->getFileSize(file_path);
                });
            float forderSize = temp_size / 1048576.f;
            CCLOG("Temp folder size: %.2f", forderSize);
            
            return forderSize;
        }

        float getTotalSizeFileDownload()
        {
            std::string pathCacched = FileUtils::getInstance()->getWritablePath() + "cached";
            std::string pathAudiobooks = FileUtils::getInstance()->getWritablePath() + "audiobooks";
            std::string pathStory = FileUtils::getInstance()->getWritablePath() + "mjstory";

            float sizeCached = mj::helper::setQueryForderSize(pathCacched);
            float sizeAudiobooks = mj::helper::setQueryForderSize(pathAudiobooks);
            float sizestory = mj::helper::setQueryForderSize(pathStory);

            return sizeCached + sizeAudiobooks + sizestory;
        }

        void getFolderSize(const std::string & folder_path, const std::string & callback_event) {
            long * total_size = new long;
            * total_size = 0;
            cocos2d::AsyncTaskPool::getInstance()->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_IO, [=](void * param) {
                auto r = reinterpret_cast<long *>(param);
                auto event = cocos2d::EventCustom(callback_event);
                event.setUserData(r);

                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                delete r;
            }, total_size, [=]() {
                std::vector<std::string> list_files;
                FileUtils::getInstance()->listFilesRecursively(folder_path, &list_files);
                auto temp_size = std::accumulate(list_files.begin(), list_files.end(), 0l, [](long folder_size, std::string file_path) {
                    return folder_size + FileUtils::getInstance()->getFileSize(file_path);
                });
                CCLOG("Temp folder size: %.2f", temp_size / 1000000.f);
                *total_size = temp_size;
            });
        }
        
        StringUtils::StringUTF8 u_lower_characters = StringUtils::StringUTF8("abcdefghijklmnopqrstuvwxyzáàảãạăắằẳẵặâấầẩẫậđéèẻẽẹêếềểễệíìỉĩịóòỏõọôốồổỗộơớờởỡợúùủũụưứừửữựýỳỷỹỵæçëîïœûüÿñ");
        StringUtils::StringUTF8 u_upper_characters = StringUtils::StringUTF8("ABCDEFGHIJKLMNOPQRSTUVWXYZÁÀẢÃẠĂẮẰẲẴẶÂẤẦẨẪẬĐÉÈẺẼẸÊẾỀỂỄỆÍÌỈĨỊÓÒỎÕỌÔỐỒỔỖỘƠỚỜỞỠỢÚÙỦŨỤƯỨỪỬỮỰÝỲỶỸỴÆÇËÎÏŒÛÜŸÑ");
        StringUtils::StringUTF8 u_lower_characters_no_accents = StringUtils::StringUTF8("abcdefghijklmnopqrstuvwxyzaaaaaaaaaaaaaaaaadeeeeeeeeeeeiiiiiooooooooooooooooouuuuuuuuuuuyyyyyæçëîïœûüÿñ");
        StringUtils::StringUTF8 u_upper_characters_no_accents = StringUtils::StringUTF8("ABCDEFGHIJKLMNOPQRSTUVWXYZAAAAAAAAAAAAAAAAADEEEEEEEEEEEIIIIIOOOOOOOOOOOOOOOOOUUUUUUUUUUUYYYYYÆÇËÎÏŒÛÜŸÑ");
        
        std::string to_upper(const std::string & s) {
            auto utf8 = StringUtils::StringUTF8(s);
            for (auto &c : utf8.getString()) {
                for (auto i = 0; i < u_lower_characters.length(); ++i) {
                    if (c._char == u_lower_characters.getString().at(i)._char) {
                        c._char = u_upper_characters.getString().at(i)._char;
                    }
                }
            }
            
            return utf8.getAsCharSequence();
        }
        
        std::string to_lower(const std::string & s) {
            auto utf8 = StringUtils::StringUTF8(s);
            for (auto &c : utf8.getString()) {
                for (auto i = 0; i < u_upper_characters.length(); ++i) {
                    if (c._char == u_upper_characters.getString().at(i)._char) {
                        c._char = u_lower_characters.getString().at(i)._char;
                    }
                }
            }
            
            return utf8.getAsCharSequence();
        }
    
        std::string toCapitalizedCase(const std::string & s) {
            
            MK_RETURN_IF(s.empty(),s);
            
            auto lowerCase = to_lower(s);
            
            if(!lowerCase.empty()) {
                
                std::string s = std::string(1,lowerCase.front());
                
                auto firstCharacter = to_upper(s);
                
                lowerCase = firstCharacter + lowerCase.substr(1);
                
            }
            
            return lowerCase;
        }
    
        
        std::string to_no_accents_v2(const std::string & s) {
            auto utf8 = StringUtils::StringUTF8(s);
            for (auto &c : utf8.getString()) {
                for (auto i = 0; i < u_upper_characters.length(); ++i) {
                    if (c._char == u_upper_characters.getString().at(i)._char) {
                        c._char = u_upper_characters_no_accents.getString().at(i)._char;
                    }
                    if (c._char == u_lower_characters.getString().at(i)._char) {
                        c._char = u_lower_characters_no_accents.getString().at(i)._char;
                    }
                }
            }
            
            return utf8.getAsCharSequence();
        }
        
        std::string to_no_accents(const std::string & s) {
			return "";
        }

        std::string vectorIntToString(std::vector<int> v) {
            if (v.empty()) {
                return "";
            }
            auto s = std::accumulate(v.begin(), v.end(), std::string("["), [](std::string s, int i) {
                return s + StringUtils::format("%d,", i);
            });
            s.resize(s.length() - 1);
            s += "]";
            return s;
        }

        std::vector<int> stringToVectorInt(std::string s) {
            if (s.empty()) {
                return {};
            }
            s = s.substr(1, s.length() - 2);
            auto temp = split(s, ',');
            std::vector<int> r;
            for (auto i : temp) {
                r.push_back(ATOI(i.c_str()));
            }
            return r;
        }

        bool isValidEmail(std::string s) {
            std::string str = s;
            if (str.size() > 150)
                return false;

            const auto at = std::find(str.cbegin(), str.cend(), '@');
            const auto dot = std::find(at, str.cend(), '.');

            if ((at == str.cend()) || (dot == str.cend()))
                return false;

            if (std::distance(str.cbegin(), at) < 1) //~@ - is minimal local part
                return false;

            if (std::distance(at, str.cend()) < 5)  //@i.ua - is minimal domain
                return false;

            return true;


            // phan check valid email duoi day da loi thoi, can cap nhat thuat toan check moi
            //const std::regex pattern("([a-z]+)([_.a-z0-9]*)([a-z0-9]+)(@)([a-z]+)([.a-z]+)([a-z]+)");// ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

             //try to match the string with the regular expression
            //return std::regex_match(s, pattern);
        }
    bool isValidLoginEmail(std::string s) {
        const std::regex pattern
                ("(.*)@(\\w+)(\\.(\\w+))+");

        // try to match the string with the regular expression
        return std::regex_match(s, pattern);
    }
        
        bool isValidNumber(std::string s) {
            for (auto c : s) {
                if (!isdigit(c)) {
                    return false;
                }
            }
            
            return true;
        }

		bool isUTF8StringValidNumber(std::string s)
		{
			for (auto c : s) {
				if (!iswdigit(c)) {
					return false;
				}
			}

			return true;
		}

        bool isValidPhoneNumber(std::string s) {
            for (auto c : s) {
                if (!isdigit(c)) {
                    return false;
                }
            }
            
            if (s.length() < 9) {
                return false;
            }

            return true;
        }

        bool isValidPhoneNumberString(const std::string& phone, const int minLength, const int maxLength) {
            MK_RETURN_IF(phone.empty(), false);

            if (phone.front() == '+') {

                auto newPhone = phone.substr(1, phone.size() - 2);

                return (isValidNumber(newPhone) && newPhone.length() >= 6 && newPhone.length() <= 18);
            }

            return (isValidNumber(phone) && phone.length() >= minLength && phone.length() <= maxLength);
        }

		bool isUTF8StringValidPhoneNumber(std::string s)
		{
			for (int i=0 ;i<s.size();i++)
			{
				auto c = s.at(i);

				if (i == 0 && c == '+')
				{
					continue;
				}

				if (!iswdigit(c)) 
				{
					return false;
				}
			}

			if (s.length() < 9) 
			{
				return false;
			}

			return true;
		}
        
        unsigned char * unzip_json(std::string bf) {
            unsigned char * buffer = nullptr;
            unzFile file = nullptr;
            
            std::string filename;
            
            do
            {
                file = unzOpenBuffer(bf.data(), bf.size());
                CC_BREAK_IF(!file);
                
                // FIXME: Other platforms should use upstream minizip like mingw-w64
                
                int ret = unzGoToFirstFile(file);
                
                CC_BREAK_IF(UNZ_OK != ret);
                
                char filePathA[260];
                unz_file_info fileInfo;
                ret = unzGetCurrentFileInfo(file, &fileInfo, filePathA, sizeof(filePathA), nullptr, 0, nullptr, 0);
                CC_BREAK_IF(UNZ_OK != ret);
                
                ret = unzOpenCurrentFile(file);
                CC_BREAK_IF(UNZ_OK != ret);
                
                buffer = (unsigned char*)malloc(fileInfo.uncompressed_size);
                int CC_UNUSED readedSize = unzReadCurrentFile(file, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
                CCASSERT(readedSize == 0 || readedSize == (int)fileInfo.uncompressed_size, "the file size is wrong");
                
//                *size = fileInfo.uncompressed_size;
                unzCloseCurrentFile(file);
            } while (0);
            
            if (file)
            {
                unzClose(file);
            }
            
            return buffer;
        }
        
        bool unzip(std::string zip, std::string directory) {
            
            std::string str = zip;
            std::string substr = getFilenameFromURL(zip);
            if (substr != "") {
                if (MSLessonPreview::getInstance()) {
                    //MSLessonPreview::getInstance()->_gameZip = substr;
                }
                if (StoryPageVC::getInstance()) {
                    //StoryPageVC::getInstance()->_gameZip = substr;
                }
            }

            // Find root path for zip file
            std::string pathError = "Unzip File: " + zip + " error";
            MJDEFAULT->setStringForKey(key_reason_error, pathError);
            MJDEFAULT->setStringForKey(key_type_error, "unzip_failed");
            size_t pos = zip.find_last_of("/\\");
            if (pos == std::string::npos) {
                CCLOG("AssetsManagerEx : no root path specified for zip file %s\n", zip.c_str());
                return false;
            }
            const std::string rootPath = directory;
            
            // Open the zip file
            unzFile zipfile = cocos2d::unzOpen(cocos2d::FileUtils::getInstance()->getSuitableFOpen(zip).c_str());
            if (! zipfile) {
                CCLOG("AssetsManagerEx : can not open downloaded zip file %s\n", zip.c_str());
                return false;
            }
            
            // Get info about the zip file
            cocos2d::unz_global_info global_info;
            if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK) {
                CCLOG("AssetsManagerEx : can not read file global info of %s\n", zip.c_str());
                cocos2d::unzClose(zipfile);
                return false;
            }
            
            // Buffer to hold data read from the zip file
            char readBuffer[ZIP_BUFFER_SIZE];
            // Loop to extract all files.
            uLong i;
            for (i = 0; i < global_info.number_entry; ++i) {
                // Get info about current file.
                cocos2d::unz_file_info fileInfo;
                char fileName[ZIP_MAX_FILENAME];
                if (unzGetCurrentFileInfo(zipfile,
                                          &fileInfo,
                                          fileName,
                                          ZIP_MAX_FILENAME,
                                          NULL,
                                          0,
                                          NULL,
                                          0) != UNZ_OK) {
                    CCLOG("AssetsManagerEx : can not read compressed file info\n");
                    cocos2d::unzClose(zipfile);
                    return false;
                }
                const std::string fullPath = rootPath + fileName;
                
                // Check if this entry is a directory or a file.
                const size_t filenameLength = strlen(fileName);
                if (fileName[filenameLength-1] == '/') {
                    //There are not directory entry in some case.
                    //So we need to create directory when decompressing file entry
                    if ( !cocos2d::FileUtils::getInstance()->createDirectory(getBasenameFromURL(fullPath))) {
                        // Failed to create directory
                        CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
                        cocos2d::unzClose(zipfile);
                        return false;
                    }
                } else {
                    // Create all directories in advance to avoid issue
                    std::string dir = getBasenameFromURL(fullPath);
                    if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(dir)) {
                        if (!cocos2d::FileUtils::getInstance()->createDirectory(dir)) {
                            // Failed to create directory
                            CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
                            cocos2d::unzClose(zipfile);
                            return false;
                        }
                    }
                    // Entry is a file, so extract it.
                    // Open current file.
                    if (cocos2d::unzOpenCurrentFile(zipfile) != UNZ_OK) {
                        CCLOG("AssetsManagerEx : can not extract file %s\n", fileName);
                        cocos2d::unzClose(zipfile);
                        return false;
                    }
                    
                    // Create a file to store current file.
                    FILE *out = fopen(cocos2d::FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
                    if (!out) {
                        CCLOG("AssetsManagerEx : can not create decompress destination file %s (errno: %d)\n", fullPath.c_str(), errno);
                        cocos2d::unzCloseCurrentFile(zipfile);
                        cocos2d::unzClose(zipfile);
                        return false;
                    }
                    
                    // Write current file content to destinate file.
                    int error = UNZ_OK;
                    do {
                        error = cocos2d::unzReadCurrentFile(zipfile, readBuffer, ZIP_BUFFER_SIZE);
                        if (error < 0) {
                            CCLOG("AssetsManagerEx : can not read zip file %s, error code is %d\n", fileName, error);
                            fclose(out);
                            cocos2d::unzCloseCurrentFile(zipfile);
                            cocos2d::unzClose(zipfile);
                            return false;
                        }
                        
                        if (error > 0) {
                            fwrite(readBuffer, error, 1, out);
                        }
                    } while(error > 0);
                    
                    fclose(out);
                }
                
                cocos2d::unzCloseCurrentFile(zipfile);
                
                // Goto next entry listed in the zip file.
                if ((i+1) < global_info.number_entry) {
                    if (cocos2d::unzGoToNextFile(zipfile) != UNZ_OK) {
                        CCLOG("AssetsManagerEx : can not read next file for decompressing\n");
                        cocos2d::unzClose(zipfile);
                        return false;
                    }
                }
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//                NativeBridge::addSkipBackupAttributeToItemAtPath(fullPath);
//#endif
            }
            MJDEFAULT->deleteValueForKey(key_reason_error);
            MJDEFAULT->deleteValueForKey(key_type_error);
            cocos2d::unzClose(zipfile);
            return true;
        }
        
        void unzipAsync(std::string zip_file, std::string directory, std::function<void(bool)> callback) {
            bool * unzipResult = new bool;
            * unzipResult = false;
            
            cocos2d::AsyncTaskPool::getInstance()->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_UNZIP, [=](void * param) {
                auto uzResult = reinterpret_cast<bool *>(param);
                if (uzResult) {
                    //*uzResult = false;
                    if (callback) {
                        callback(*uzResult);
                    }
                } else {
                    if (callback) {
                        callback(false);
                    }
                }
            }, unzipResult, [=]() {
                //                auto zipPath = cocos2d::FileUtils::getInstance()->getWritablePath()+ directory + filename;
                * unzipResult = mj::helper::unzip(zip_file, directory);
            });
        }
        
        void unzipAsync(std::vector<std::pair<std::string, std::string>> zips, std::function<void(bool)> callback) {
            if (zips.empty()) {
                callback(true);
                return;
            }
            
            unzipAsync(zips[0].first, zips[0].second, [=](bool success) {
                if (success) {
                    auto new_zips = std::vector<std::pair<std::string, std::string>>(zips.begin() + 1, zips.end());
                    unzipAsync(new_zips, callback);
                } else {
                    callback(false);
                }
            });
        }
        
        std::string getForcedAlignment(const std::string & audio_file) {
            auto full_path = FileUtils::getInstance()->fullPathForFilename(audio_file);
            auto tag = load_tag(full_path.c_str());

            if (tag) {
                auto comm_frame = tag_get_comment(tag);
                if (!comm_frame) {
                    free(tag);
                    return "";
                }
                auto comm_content = parse_comment_frame_content(comm_frame);
                if (!comm_content) {
                    free(tag);
                    return "";
                }
                auto comment = std::string(comm_content->text->data);
                free_tag(tag);
                return comment;
            } else {
                return "";
            }
        }
        
        std::string prettyPrice(float price, std::string currency) {
            if (currency.compare("THB") == 0){
                return prettyInteger(price);
            }else{
                if (price > 1000) {
                    int iprice = round(price / 100) * 100;
                    return prettyInteger(iprice);
                } else {
                    return StringUtils::format("%0.2f", price);
                }
            }
        }
        
        std::string convertForcedAlignment(const std::string & original_string, const std::string & content) {
            rapidjson::Document d;
            d.Parse<rapidjson::kParseStopWhenDoneFlag | rapidjson::kParseValidateEncodingFlag>(original_string.c_str());
            
            rapidjson::Document out;
            rapidjson::Value o(rapidjson::kArrayType);
            rapidjson::Document::AllocatorType & allocator = out.GetAllocator();
            
//            CCLOG("%s", original_string.c_str());
            
            if (d.HasParseError()) {
				CCLOG("parse data not valid");
                return "";
            }
            
            std::string::size_type last_id = 0;
            
            if (JSON_SAFE_CHECK(d, "fragments", Array)) {
                auto fragments = d["fragments"].GetArray();
                for (auto i = 0; i < fragments.Size(); ++i) {
                    if (!fragments[i].IsObject()) {
						CCLOG("parse data not valid");
                        return "";
                    }
                    
                    rapidjson::Value f(rapidjson::kObjectType);
                    
                    auto fragment = fragments[i].GetObject();
                    
                    auto s = ATOF(fragment["begin"].GetString()) * 1000;
                    auto e = ATOF(fragment["end"].GetString()) * 1000;
        
                    f.AddMember("s", rapidjson::Value(s), allocator);
                    f.AddMember("e", rapidjson::Value(e), allocator);
                    
                    if (JSON_SAFE_CHECK(fragment, "lines", Array)) {
                        auto lines = fragment["lines"].GetArray();
                        if (lines[0].IsString()) {
                            auto w = std::string(lines[0].GetString());
                            f.AddMember("w", lines[0], allocator);
                            
                            auto p = content.find(w, last_id);
                            if (p == std::string::npos) {
                                CCLOG("Word not found: %s", w.c_str());
#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0
//                                cocos2d::MessageBox("Word not found", w.c_str());
#endif
                                continue;
                            }
                            last_id = p + 1;
                            
                            auto prefix = content.substr(0, p);
                            auto prefix_count = (int)StringUtils::getCharacterCountInUTF8String(prefix);
                            auto actual_count = (int)StringUtils::getCharacterCountInUTF8String(w);
                            
                            f.AddMember("ts", rapidjson::Value(prefix_count), allocator);
                            f.AddMember("te", rapidjson::Value(prefix_count + actual_count - 1), allocator);
                        }
                    }
                    
                    o.PushBack(f, allocator);
                }
            }
            
            rapidjson::StringBuffer rj_buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
            o.Accept(writer);
            //CCLOG("%s: converted \n %s", __PRETTY_FUNCTION__, rj_buffer.GetString());
//            CCLOG("converted \n %s", rj_buffer.GetString());
            return rj_buffer.GetString();
        }
        
        const std::unordered_map<std::string, std::string> ACTIVITY_ABBR = {
            {"LV", "Learn Vocabulary"},
            {"BS", "Build Sentences"},
            {"LS", "Learn Sight Words"},
            {"RS", "Read the Story"},
            {"PRO", "Pronunciation"},
            {"RC", "Reading Comprehension"},
            {"Classify", "Classify"},
            {"LA", "Learn the Letters"},
            {"LN", "Learn the Numbers"},
            {"LSH", "Learn the Shapes"},
            {"LC", "Learn the Colors"},
            {"LL", "Learn the Letters"},
            {"PRE", "Pre-reading"},
           
            {"MPI","Introduction"},
            {"MPD","Sound Discrimination"},
            {"MPL","Letter Formation"},
            {"MPSI","Sound Isolation"},
            {"MPSB","Blend and Segment"},
            {"MPBW","Build the Word"},
            {"MPLS","Learn Sight Words"},
            {"MPCS","Count the Sound"},
            {"MPF","Word Family"},
            {"MPBS","Build Sentences"},
            {"MPR","Word Recognition"},
            {"MPV","Learn Vocabulary"},
            {"MPRS","Read the Story"},
            {"MPLU","Lower case-Upper case"},
            {"MPSY", "Count Syllables"}
        };
        
        const std::unordered_map<std::string, std::string> ACTIVITY_SOUND = {
            {"Learn Vocabulary", "learn_vocabulary.mp3"},
            {"LV", "learn_vocabulary.mp3"},
            {"BS", "build_sentence.mp3"},
            {"LS", "learn_sight_words.mp3"},
            {"RS", "read_a_story.mp3"},
            {"Read a story", "read_a_story.mp3"},
            {"PRO", "pronunciation.mp3"},
            {"RC", "reading_comprehension.mp3"},
            {"Classify", "classify.mp3"},
            {"LA", "learn_alphabet.mp3"},
            {"LN", "learn_numbers.mp3"},
            {"LSH", "learn_shapes.mp3"},
            {"LC", "learn_colors.mp3"},
            {"PRE", "pre_reading.mp3"},
            
            {"MPI", "Introduction.mp3"},
            {"MPD", "Sound Discrimination.mp3"},
            {"MPL", "Letter Formation.mp3"},
            {"MPSI", "Sound Isolation.mp3"},
            {"MPSB", "Blend and Segment.mp3"},
            {"MPBW", "Build the Word.mp3"},
            {"MPLS", "Learn Sight Words.mp3"},
            {"MPCS", "Count the Sound.mp3"},
            {"MPF", "Word Family.mp3"},
            {"MPBS", "Build Sentences.mp3"},
            {"MPR", "Word Recognition.mp3"},
            {"MPV", "Learn Vocabulary.mp3"},
            {"MPRS", "Read the Story.mp3"},
            {"MPLU", "Lower case - Upper case.mp3"},
            {"MPSY", "Count syllables.mp3"}
        };
        
        /*
         Learn vocabulary: LV    Learn vocabulary
         Build sentences: BS
         Learn sight words: LS
         Read a story: RS
         Pronunciation: PRO
         Reading comprehension: RC
         Classify: Classify
         Learn alphabets: LA
         Learn numbers: LN
         Learn shapes: LSH
         Learn colors: LC
         */
        
        std::string extractActivityName(std::string name) {
            auto temp = split(name, ':');
            if (temp.empty()) {
                return "";
            }
            
            auto arr = split(temp[0], '_');
            if (arr.empty()) {
                return temp[0];
            }
            
            auto abbr = arr[0];
            if (ACTIVITY_ABBR.find(abbr) != ACTIVITY_ABBR.end()) {
                return ACTIVITY_ABBR.at(abbr);
            }
            return abbr;
        }
        
        std::string getActivitySound(std::string name) {
            auto temp = split(name, ':');
            if (temp.empty()) {
                return "";
            }
            
            auto arr = split(temp[0], '_');
            if (arr.empty()) {
                return temp[0];
            }
            
            auto abbr = arr[0];
            abbr.erase(std::find_if(abbr.rbegin(), abbr.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), abbr.end());
            
            if (ACTIVITY_SOUND.find(abbr) != ACTIVITY_SOUND.end()) {
                return std::string("sounds/activities_name/") + ACTIVITY_SOUND.at(abbr);
            }
            return "";
        }
        
        void playButtonFX() {
            cocos2d::experimental::AudioEngine::play2d("sounds/ui/button_fx/click_soft.mp3");
        }
        
        void playButtonFXClose() {
            cocos2d::experimental::AudioEngine::play2d("sounds/ui/button_fx/2.mp3");
        }
        
        void playButtonFXOpen() {
            cocos2d::experimental::AudioEngine::play2d("sounds/ui/button_fx/9.mp3");
        }
        
        std::pair<std::string, std::string> extractSeries(std::string t) {
            std::size_t found1 = t.find("series");
            std::size_t found2 = t.find("Series");
            if (found1==std::string::npos && found2==std::string::npos) {
                return std::make_pair(t, "");
            }
            auto utf8 = StringUtils::StringUTF8(t);
            auto pos = 0;
            for (pos = 0; pos < utf8.getString().size(); ++pos) {
                if (utf8.getString()[pos]._char == "-" || utf8.getString()[pos]._char == "–") {
                    break;
                }
            }
            
            if (pos == utf8.getString().size()) {
                return std::make_pair(t, "");
            }
            
            auto last = utf8;
            
            utf8.getString().erase(utf8.getString().begin() + pos, utf8.getString().end());
            last.getString().erase(last.getString().begin(), last.getString().begin() + pos + 1);
            
            auto title = utf8.getAsCharSequence();
            auto series = last.getAsCharSequence();
            
            while ((*title.rbegin()) == ' ') {
                title.erase(title.begin() + title.length() -1);
            }
            
            while ((*series.begin()) == ' ') {
                series.erase(series.begin());
            }
            
            return std::make_pair(title, series);
        }
        
        void playTransitionStoryCover(MSCollectionStoryItem * item, cocos2d::ui::Widget * content, double ratio,int zorder, const mj::FROMSTORY &from) {
            playButtonFXOpen();
            auto overlay = ui::Layout::create();
            overlay->setTouchEnabled(true);
            overlay->setContentSize(Director::getInstance()->getVisibleSize());
            Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX);
            overlay->scheduleOnce([overlay](float dt) {
                overlay->removeFromParent();
            }, .5f, "dismiss_touch_block");

            auto senceSize = Director::getInstance()->getRunningScene()->getContentSize();
            auto storyinfocoverSence = StoryInfoCover::createScene(item, false, 1, from, senceSize);
            DIRECTOR->pushScene(storyinfocoverSence);
            //auto storyinfocover = StoryInfoCover::createView(item, false, 1, from);
            //if(zorder==0) {
            //    Director::getInstance()->getRunningScene()->addChild(storyinfocover);
            //} else {
            //    Director::getInstance()->getRunningScene()->addChild(storyinfocover,zorder);
            //}
            //storyinfocover->InitDisplay();
        }
        
        void playTransitionAudiobookCover(MSCollectionAudiobookItem * item, cocos2d::ui::Widget * content, double ratio) {
            playButtonFXOpen();
            auto overlay = ui::Layout::create();
            overlay->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            overlay->setBackGroundColor(Color3B::BLACK);
            overlay->setOpacity(0);
            overlay->runAction(FadeTo::create(0.5, 220));
            overlay->setTouchEnabled(true);
            overlay->setContentSize(Director::getInstance()->getVisibleSize());
            Director::getInstance()->getRunningScene()->addChild(overlay, 1001000);
            overlay->scheduleOnce([overlay](float dt) {
                overlay->removeFromParent();
            }, 1.6, "dismiss_touch_block");
            
            auto player = MSAudiobookPlayer::createView(item->getInfo());
            Director::getInstance()->getRunningScene()->addChild(player, 1001001);
            player->setName(mjhelper_name_audiobookplayer);
            player->setVisible(false);
            
            auto bb = item->getBoundingBox();
            if (item->getParent()) {
                bb.origin = item->getParent()->convertToWorldSpace(bb.origin);
            } else {
                bb.origin = player->convertToNodeSpace(bb.origin);
            }
            
            auto new_item = MSCollectionAudiobookItem::createThumb(item->getInfo());
            new_item->setName(mjhelper_name_collectionaudiobookitem);
            new_item->setAnchorPoint(Point::ANCHOR_MIDDLE);
            new_item->setPosition(Point(bb.getMidX(), bb.getMidY()));
            Director::getInstance()->getRunningScene()->addChild(new_item, 1001002);
            
            bb = player->getThumbBoundingBox();
            new_item->runAction(EaseExponentialOut::create(MoveTo::create(0.5, Point(bb.getMidX(), bb.getMidY()) + Point(0, 10))));
            new_item->runAction(EaseBackOut::create(ScaleTo::create(1, 1.25)));
            new_item->disableNewRead();
            
            player->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([player, new_item] {
                new_item->removeFromParentAndCleanup(false);
                auto container = utils::findChild(player, "thumb_container");
                auto p = new_item->getPosition();
                p = container->convertToNodeSpace(p);
                new_item->setPosition(p);
                container->addChild(new_item);
                
                player->setVisible(true);
                player->playTransitionEnter();
            }), NULL));
        }
        
        std::string extractAudiobookSeriesName(std::string name) {
            auto first_step = mj::helper::split(name, '|');
            std::unordered_map<std::string, std::string> m;
            if (first_step.empty()) {
                return name;
            }
            
            for (auto n : first_step) {
                auto second_step = mj::helper::split(n, ':');
                if (second_step.empty()) {
                    continue;
                }
                if (second_step.size() == 1) {
                    continue;
                }
                m.emplace(second_step[0], second_step[1]);
            }
            
            for (auto p : m) {
                LANGUAGE_MANAGER->addTextByKey(ATOI(p.first.c_str()), name, p.second);
            }
            
            auto currentLang = StringUtils::format("%d",LANGUAGE_MANAGER->getDisplayLangId());
            if (m.find(currentLang) != m.end()) {
                return m.at(currentLang);
            }

            if (m.find("1") != m.end()) {
                return m.at("1");
            }
            
            return name;
        }
        
        void grayscaleImageView(cocos2d::ui::ImageView * view, bool enable /*= true*/) {
//            if (enable) {
//                auto fileUtiles = FileUtils::getInstance();
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//                auto fragmentFullPath = fileUtiles->fullPathForFilename("shaders/example_Blur.fsh");
//#else
//                auto fragmentFullPath = fileUtiles->fullPathForFilename("shaders/example_Blur_winrt.fsh");
//#endif
//                auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
//                auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//#endif
//
//                auto glprogramstate = (glprogram == nullptr ? nullptr : GLProgramState::getOrCreateWithGLProgram(glprogram));
//
//                Size size = reinterpret_cast<Sprite *>(view->getVirtualRenderer())->getTexture()->getContentSizeInPixels();
//                glprogramstate->setUniformVec2("resolution", size);
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//                glprogramstate->setUniformFloat("blurRadius", 30);
//                glprogramstate->setUniformFloat("sampleNum", 5);
//#endif
//                view->getVirtualRenderer()->setGLProgramState(glprogramstate);
//            } else {
//                auto shader = cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
//                view->getVirtualRenderer()->setGLProgram(shader);
//                view->getVirtualRenderer()->getGLProgram()->use();
//            }
        }
        
        void outlineImageView(cocos2d::ui::ImageView * view, bool enable /*= true*/) {
                        if (enable) {
                            auto fileUtiles = FileUtils::getInstance();
//            #if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//                            auto fragmentFullPath = fileUtiles->fullPathForFilename("shaders/outline.fsh");
//            #else
//                            auto fragmentFullPath = fileUtiles->fullPathForFilename("shaders/outline.fsh");
//            #endif
//                            auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
                            auto glprogram = GLProgram::createWithFilenames("shaders/outline.vsh", "shaders/outline.fsh");

                            auto glprogramstate = (glprogram == nullptr ? nullptr : GLProgramState::getOrCreateWithGLProgram(glprogram));
            
//                            Size size = reinterpret_cast<Sprite *>(view->getVirtualRenderer())->getTexture()->getContentSizeInPixels();
//                            glprogramstate->setUniformVec2("cc_ViewSizeInPixels", size);

                            auto winsize = Director::getInstance()->getWinSizeInPixels();
                            glprogramstate->setUniformVec2("cc_ViewSizeInPixels", winsize);
                            
                            view->getVirtualRenderer()->setGLProgram(glprogram);
                            view->getVirtualRenderer()->setGLProgramState(glprogramstate);
                        } else {
                            auto shader = cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
                            view->getVirtualRenderer()->setGLProgram(shader);
                            view->getVirtualRenderer()->getGLProgram()->use();
                        }
        }
        
        void grayscaleImageV2(cocos2d::ui::ImageView * image, bool enabled /*= true*/) {
            if (enabled) {
                auto grayScaleShader = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, ccPositionTexture_GrayScale_frag);
                grayScaleShader->retain();
                
                grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
                grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
                grayScaleShader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
                
                grayScaleShader->link();
                CHECK_GL_ERROR_DEBUG();
                
                grayScaleShader->updateUniforms();
                CHECK_GL_ERROR_DEBUG();
                
                image->getVirtualRenderer()->setGLProgram(grayScaleShader);
                image->getVirtualRenderer()->getGLProgram()->use();
            } else {
                auto shader = cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
                image->getVirtualRenderer()->setGLProgram(shader);
                image->getVirtualRenderer()->getGLProgram()->use();
            }
        }
        
        cocos2d::Color3B colorFromHex(int hex) {
            auto r = (hex >> 16) & 0xff;
            auto g = (hex >> 8) & 0xff;
            auto b = hex & 0xff;
            return cocos2d::Color3B(r, g, b);
        }
        
        auto ansi_char = cocos2d::StringUtils::StringUTF8("qwertyuiopasdfghjklzxcvbnm");
        auto ansi_char_upper = cocos2d::StringUtils::StringUTF8("QWERTYUIOPASDFGHJKLZXCVBNM");
        bool is_alpha(cocos2d::StringUtils::StringUTF8::CharUTF8 c) {
            for (auto i = 0; i < 26; ++i) {
                auto ansi_c = ansi_char.getString().at(i);
                if (ansi_c._char == c._char) {
                    return true;
                }
                
                ansi_c = ansi_char_upper.getString().at(i);
                if (ansi_c._char == c._char) {
                    return true;
                }
            }
            return false;
        }
        
        void hideTap() {
            Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
        }
        
        void showTap(cocos2d::Point p) {
            auto overlay = ui::Layout::create();
            overlay->setContentSize(Director::getInstance()->getVisibleSize());
            overlay->setName("guide_overlay");
            Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);
        
            overlay->schedule([=](float) {
                SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tap/tap.plist");
                Vector<SpriteFrame *> tap_frames;
                for (auto i = 0; i < 19; ++i) {
                    auto name = StringUtils::format("tap_%04d.png", i);
                    tap_frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
                }
                
                auto anim_cache = Animation::createWithSpriteFrames(tap_frames, 0.05f);
                auto tap_anim = Animate::create(anim_cache);
                
                auto sprite = Sprite::createWithSpriteFrame(tap_frames.front());
                sprite->setPosition(p);
                
                auto hand = ui::ImageView::create("mjstory/new_ui/hand.png");
                hand->setPosition(p);
                hand->setAnchorPoint(Point(0.35, 1.));
                
                overlay->addChild(sprite, INT_MAX - 1);
                overlay->addChild(hand, INT_MAX - 1);
                
                hand->setOpacity(0);

                hand->runAction(Sequence::create(FadeIn::create(0.5), DelayTime::create(1.5), FadeOut::create(0.25), RemoveSelf::create(), NULL));
                sprite->runAction(Sequence::create(DelayTime::create(0.5), Repeat::create(tap_anim, 2), RemoveSelf::create(), NULL));
            }, 5, 100, 8, "animated_guide");
        }
        cocos2d::ui::Button* addSpeaker(cocos2d::ui::Layout *layout) {
            cocos2d::ui::Button* speaker = cocos2d::ui::Button::create("mjstory/new_ui/speaker.png");
            speaker->setScale(1.2f);
            speaker->setPosition(Vec2(70, Director::getInstance()->getWinSize().height - 60));
            layout->addChild(speaker, 1000);

            return speaker;
        }
        void purgeTextureData() {
            SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
        }

        std::string compress_string(const std::string& str, int compressionlevel) {
            z_stream zs;                        // z_stream is zlib's control structure
            memset(&zs, 0, sizeof(zs));

            if (deflateInit(&zs, compressionlevel) != Z_OK)
                throw(std::runtime_error("deflateInit failed while compressing."));

            zs.next_in = (Bytef*)str.data();
            zs.avail_in = (int)str.size();           // set the z_stream's input

            int ret;
            char outbuffer[32768];
            std::string outstring;

            // retrieve the compressed bytes blockwise
            do {
                zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
                zs.avail_out = sizeof(outbuffer);

                ret = deflate(&zs, Z_FINISH);

                if (outstring.size() < zs.total_out) {
                    // append the block to the output string
                    outstring.append(outbuffer,
                                     zs.total_out - outstring.size());
                }
            } while (ret == Z_OK);

            deflateEnd(&zs);

            if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
                std::ostringstream oss;
                oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
                throw(std::runtime_error(oss.str()));
            }

            return base64::encode(outstring);
        }

        std::string decompress_string(const std::string& encoded) {
            auto str = base64::decode(encoded);
            z_stream zs;                        // z_stream is zlib's control structure
            memset(&zs, 0, sizeof(zs));

            if (inflateInit(&zs) != Z_OK)
                throw(std::runtime_error("inflateInit failed while decompressing."));

            zs.next_in = (Bytef*)str.data();
            zs.avail_in = (int)str.size();

            int ret;
            char outbuffer[32768];
            std::string outstring;

            // get the decompressed bytes blockwise using repeated calls to inflate
            do {
                zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
                zs.avail_out = sizeof(outbuffer);

                ret = inflate(&zs, 0);

                if (outstring.size() < zs.total_out) {
                    outstring.append(outbuffer,
                                     zs.total_out - outstring.size());
                }

            } while (ret == Z_OK);

            inflateEnd(&zs);

            if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
                std::ostringstream oss;
                oss << "Exception during zlib decompression: (" << ret << ") "
                    << zs.msg;
                throw(std::runtime_error(oss.str()));
            }

            return outstring;
        }

        std::string replaceDomain(std::string url, std::string domain) {
            auto component = split(url, '/');
            auto saved = 0;
            for (auto i = 0; i < (int)component.size(); ++i) {
                if (component[i].empty()) {
                    continue;
                }
                if (component[i].find(".") != std::string::npos) {
                    //contain dot
                    saved = i;
                    break;
                }
            }
            
            url = domain;
            if (url[url.length() - 1] == '/') {
                url.resize(url.length() - 1);
            }
            for (auto i = saved + 1; i < (int)component.size(); ++i) {
                url += "/" + component[i];
            }
            return url;
        }
        
        std::string char2hex(char dec) {
            char dig1 = (dec & 0xF0) >> 4;
            char dig2 = (dec & 0x0F);
            if (0 <= dig1 && dig1 <= 9) dig1 += 48;    //0,48 in ascii
            if (10 <= dig1 && dig1 <= 15) dig1 += 65 - 10; //A,65 in ascii
            if (0 <= dig2 && dig2 <= 9) dig2 += 48;
            if (10 <= dig2 && dig2 <= 15) dig2 += 65 - 10;
            
            std::string r;
            r.append(&dig1, 1);
            r.append(&dig2, 1);
            return r;
        }
        
        std::string urlencode(const std::string &c) {
            std::string escaped;
            int max = (int) c.length();
            for (auto i = 0; i < max; i++) {
                if (('0' <= c[i] && c[i] <= '9') ||//0-9
                    ('A' <= c[i] && c[i] <= 'Z') ||//ABC...XYZ
                    ('a' <= c[i] && c[i] <= 'z') || //abc...xyz
                    (c[i] == '~' || c[i] == '-' || c[i] == '_' || c[i] == '.')) {
                    escaped.append(&c[i], 1);
                } else {
                    escaped.append("%");
                    escaped.append(char2hex(c[i]));//converts char 255 to string "FF"
                }
            }
            return escaped;
        }
        
        Rect getTextBoundingBox(ui::Text * t) {
            auto rect = Rect(0, 0, 0, 0);
            for (auto i = 0; i < StringUtils::getCharacterCountInUTF8String(t->getString()); ++i) {
                auto letter = t->getLetter(i);
                if (letter) {
                    if (rect.size.width == 0 && rect.size.height == 0) {
                        rect = letter->getBoundingBox();
                    } else {
                        rect = rect.unionWithRect(letter->getBoundingBox());
                    }
                }
            }
            return rect;
        }
        
        std::string getCSBName(std::string csb_name){
            if(CONFIG_MANAGER->isVietnameseStories()){
                auto pos = csb_name.find_first_of('.');
                auto temp = csb_name.substr(0, pos);
                auto tail = csb_name.substr(pos, pos+4);
                auto vn_csb_name = temp + "_VN"  + tail;
                if (cocos2d::FileUtils::getInstance()->isFileExist(vn_csb_name)) {
                    return vn_csb_name;
                }
            }
            return csb_name;
        }
        
        inline double isLeft( Point P0, Point P1, Point P2 )
        {
            return ((P1.x - P0.x) * (P2.y - P0.y)
                    - (P2.x -  P0.x) * (P1.y - P0.y));
        }

        bool isPointInPoly(cocos2d::Point p, std::vector<cocos2d::Point> v) {
            auto n = int(v.size());
            v.push_back(v.front());
            int    wn = 0;    // the  winding number counter
            // loop through all edges of the polygon
            for (int i=0; i<n; i++) {   // edge from V[i] to  V[i+1]
                if (v[i].y <= p.y) {          // start y <= P.y
                    if (v[i+1].y  > p.y)      // an upward crossing
                        if (isLeft(v[i], v[i+1], p) > 0)  // P left of  edge
                            ++wn;            // have  a valid up intersect
                }
                else {                        // start y > P.y (no test needed)
                    if (v[i+1].y  <= p.y)     // a downward crossing
                        if (isLeft(v[i], v[i+1], p) < 0)  // P right of  edge
                            --wn;            // have  a valid down intersect
                }
            }
            return wn;
        }
        bool isThaiLanguage(std::string name) {
            cocos2d::StringUtils::StringUTF8 txt = cocos2d::StringUtils::StringUTF8(name);
            cocos2d::StringUtils::StringUTF8 s = cocos2d::StringUtils::StringUTF8("ขฃคฅฆงจฉชซฌญฎฏฐฑฒณดตถทธนบปผฝพฟภมยรลวศษสหฬอฮ");
            for (auto character : txt.getString()) {
                for (auto is_thai : s.getString())
                    if (character._char == is_thai._char)
                        return true;
            }
            return false;
        };
        std::string shortString(std::string i_input, int i_maxLength, std::string i_endString)
        {
            MK_RETURN_IF(i_input.empty(), "");
            
            cocos2d::StringUtils::StringUTF8 txt = cocos2d::StringUtils::StringUTF8(i_input);

            if (txt.length() < i_maxLength) {
                return i_input;
            }

            auto new_char = txt.getString().at(txt.length() - 1)._char;

            while (txt.length() > i_maxLength)
            {
                txt.deleteChar(txt.length() - 1);

                if (txt.getString().empty())
                {
                    break;
                }
            }

            return txt.getAsCharSequence() + i_endString;
        }
        bool isRetina() {
            auto retina = MJDEFAULT->getIntegerForKey(key_device_retina, -1);
            if (retina == -1) {
                //not set
                auto is_retina = (Director::getInstance()->getOpenGLView()->getFrameSize().height > 768) && (MJ_PLATFORM_CONFIG.getDeviceYearClass() > 2013);
                retina = is_retina ? 1 : 0;
                setRetina(is_retina ? 1 : 0);
            }
            return (retina == 1);
        }
        
        void setRetina(int retina_enabled) {
            MJDEFAULT->setIntegerForKey(key_device_retina, retina_enabled);
        }
        
        bool isWidescreen() {
            auto screen_size = Director::getInstance()->getVisibleSize();
            return screen_size.width / screen_size.height > 1.5;
        }

		void removeCharsFromString(std::string & str, std::string charsToRemove)
		{
			if (str.empty()|| charsToRemove.empty())
			{
				CCLOG("cannot excute  removeCharsFromString function because the string inputs are empty.");
				return;
			}

			for (auto cha : charsToRemove)
			{
				str.erase(remove(str.begin(), str.end(), cha), str.end());
			}
		}
        
        bool isContainAlnum(const std::string & s) {
            auto utf8 = StringUtils::StringUTF8(s);
            for (auto &c : utf8.getString()) {
                for (auto i = 0; i < u_lower_characters.length(); ++i) {
                    if (c._char == u_lower_characters.getString().at(i)._char) {
                        return true;
                    }
                    if (c._char == u_upper_characters.getString().at(i)._char) {
                        return true;
                    }
                }
                
                for (auto i = 0; i < 10; ++i) {
                    if (c._char == StringUtils::toString(i)) {
                        return true;
                    }
                }
            }
            
            return false;
        }
    
        std::string ltrim(std::string& str, string character)
        {
            str.erase(0, str.find_first_not_of(character));       //prefixing spaces
//            str.erase(str.find_last_not_of(character)+1);         //surfixing spaces
            return str;
        }

		std::string jsonToString(rapidjson::Document & jsonObject)
		{
			rapidjson::StringBuffer buffer;

			rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);

			jsonObject.Accept(jsonWriter);

			return buffer.GetString();
		}

        static std::regex special_char_expression{ "[\"!@#{}\\\\;,\\[\\]\\{\\}()\\'%^&*?/$=/]+" };

        bool isNameInValid(std::string name) {
            if (name.empty()) return true;
            return std::regex_search(name, special_char_expression);
        }

        void removeSpecialCharacter(TextField* name) {
            if (!name) return;
            std::string txt = name->getString();
            if (txt.empty()) return;
            txt = std::regex_replace(txt,special_char_expression,"");
            name->setString(txt);
        }

        bool isWriablePathValid(std::string path) {
            if (path.empty()) return false;
            std::regex reg{ "([ -:_.a-zA-Z0-9]*)" };
            std::vector< std::string> elements = mj::helper::split(path, '/');
            for (auto el : elements) {
                if (!std::regex_match(el, reg)) return false;
            }
            return true;
        }
        
        std::string createWindowValidPath(std::string path) {
            std::vector< std::string> elements = mj::helper::split(path, '/');
            std::regex reg{ "([ -:_.a-zA-Z0-9]*)" };
            std::string newPath = elements[0] + '/';
            return newPath;
        }

        std::vector<std::pair<std::string, cocos2d::Value>> getValueOfEventUpdateUserInfo(std::string click_type, std::string source_update, std::string update_type, std::string email_verified, std::string update_info_successfull, std::string edit_user_info, std::string log_out, std::string edit_profile, std::string detail_profile, std::string upgrade_account) {
            if (click_type == "") click_type = "no";
            if (source_update == "") source_update = "no";
            if (update_type == "") update_type = "no";
            if (email_verified == "") email_verified = "none";
            if (update_info_successfull == "") update_info_successfull = "no";
            if (edit_user_info == "") edit_user_info = "no";
            if (log_out == "") log_out = "no";
            if (edit_profile == "") edit_profile = "false";
            if (detail_profile == "") detail_profile = "false";
            if (upgrade_account == "") upgrade_account = "false";

            std::vector<std::pair<std::string, cocos2d::Value>> value = {
                {"click_type", cocos2d::Value(click_type) },
                {"email_verified", cocos2d::Value(email_verified) },
                {"update_info_successfull", cocos2d::Value(update_info_successfull) },
                {"source_update", cocos2d::Value(source_update) },
                {"edit_user_info", cocos2d::Value(edit_user_info) },
                {"log_out", cocos2d::Value(log_out) },
                {"edit_profile", cocos2d::Value(edit_profile) },
                {"upgrade_account", cocos2d::Value(upgrade_account) },
                {"detail_profile", cocos2d::Value(detail_profile) }
            };
            return value;
        }
    
        void pushEvnetLostConnection(std::string link, std::string errorType, std::string type, std::string errorcode, std::string countRetry, std::string ipAndspeed, std::string remoteAddress, std::string httpStatusCode, std::string type_download, std::vector<std::string> listMessageErrorIos)
        {
            
            std::string _url = link;

            if (type == "")
            {
                type = "None";
            }
            auto screen = MJDEFAULT->getStringForKey(key_live_in_screen, "None");

            std::string ip = MJDEFAULT->getStringForKey(IP_NAME_KEY);
            std::string speed = std::to_string(MJDEFAULT->getIntegerForKey(key_speed_internet));

            bool isShowpop =  MJDEFAULT->getBoolForKey(key_show_pop_lost_connection, false);

            if(isShowpop)
            {
                MJDEFAULT->deleteValueForKey(key_show_pop_lost_connection);
            }

            std::string showPopup = isShowpop ? "Yes" : "No";

            ipAndspeed = ip + "/" + speed;

            // MJDEFAULT->deleteValueForKey(key_live_in_screen);
             //module Man hinh bi loi
             //type nguyen nhan bi crash
             //domain linkdowload
             //errorType loi dan den crash
            std::vector<std::pair<std::string, cocos2d::Value>> value = {};
            value.push_back({ "module", cocos2d::Value(screen) });
            value.push_back({ "error_code", cocos2d::Value(errorcode) });
            value.push_back({ "IP_Speed", cocos2d::Value(ipAndspeed) });
            value.push_back({ "retry_count", cocos2d::Value(countRetry) });
            value.push_back({ "show_popup", cocos2d::Value(showPopup) });
            value.push_back({ "type_download", cocos2d::Value(type_download) });
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            value.push_back({ "remote_address", cocos2d::Value(remoteAddress) });
            value.push_back({ "http_status_code", cocos2d::Value(httpStatusCode) });
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        value.push_back({ "error_message", cocos2d::Value(base64::encode(errorType)) });
#else
            value.push_back({ "error_message", cocos2d::Value(errorType) });
#endif
            value.push_back({ "url", cocos2d::Value(base64::encode(link)) });
            value.push_back({ "type", cocos2d::Value(type) });

            try
            {
                if (!value.empty())
                {
                    CleverTapManager::GetInstance()->pushEvent("lost_connections", value);
                    if (MJDEFAULT->getBoolForKey(key_push_event_crash_to_fibase, false))
                    {
                        mm::RealtimeDatabase::getInstance().updateInformationEventNetworkError(3485680, link, screen, errorType, errorcode, httpStatusCode, remoteAddress);
                    }
                }
            }
            catch (const std::exception&)
            {

            }
        }

        void pushEventCrashNetWork(std::string link, std::string errorType, std::string type, int errorcode, int countRetry, std::string ipAndspeed)
        {
            CCLOG("pushEventCrashNetWork: error url: %s",link.c_str());
            
            if (!NativeBridge::networkAvaiable() || errorType == "")
            {
                return;
            }

            std::vector<int> listErrorCode = { 200 , 306, 308, 310, 305, 303, 302, 307};

            auto it = std::find_if(listErrorCode.begin(), listErrorCode.end(), [=](int a) {
                return errorcode == a;
                });

            if (it != listErrorCode.end())
            {
                return;
            }


#if !(defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0))
           
            if (!MJDEFAULT->getBoolForKey("debugkey_product_environment", false))
            {
                if (errorcode < 316 || errorcode > 319)
                {
                    if(link == "")
                    {
                        link = "None";
                    }
                    
                    if(errorType == "")
                    {
                        errorType = "None";
                    }
                    
                    if(type == "")
                    {
                        type = "None";
                    }
                    
                    if(ipAndspeed == "")
                    {
                        ipAndspeed = "None";
                    }
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                        mj::helper::pushEventLostConnecttionToClevertab(link, errorType, type, std::to_string(errorcode), std::to_string(countRetry), ipAndspeed);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity",
                                                                 "pushEventLostConnection", link, errorType,
                                                                 type, std::to_string(errorcode), std::to_string(countRetry),
                                                                 ipAndspeed);
#else
                        pushEvnetLostConnection(link, errorType, type, std::to_string(errorcode), std::to_string(countRetry), ipAndspeed);
#endif
                    });
                }
            }
#endif
        }


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        void changeIconAppForIos(std::string nameIcon)
        {
            //changeIconAPP(nameIcon);
        }
    
        std::vector<int> getDateTimeIOS()
        {
           /* std::string stringTimeDate = getTimeDate();
            
            std::vector<std::string> valueStringTimeDate = mj::helper::split(stringTimeDate, '-');
            
            std::vector<int> valueIntTimeDate = {};
            
            valueIntTimeDate.push_back(std::stoi(valueStringTimeDate[0]));
            valueIntTimeDate.push_back(std::stoi(valueStringTimeDate[1]));
            valueIntTimeDate.push_back(std::stoi(valueStringTimeDate[2]));
            
            return valueIntTimeDate;*/
            
        }
#endif
        /*Tạo hàm cắt chuỗi trong C*/
        char* substring(char* source, size_t begin, size_t n, char* target) {
            try
            {
                int target_size = sizeof(target);

                /*Kiểm tra đầu vào và phát hiện lỗi nếu có*/
                if( source != NULL && target != NULL)
                {
                    /*Kiểm tra vị trí cắt có nằm trong chuỗi không*/
                    if (begin < strlen(source)) {
                        /*Cắt n ký tự từ vị trí begin trong chuỗi source*/
                        strncpy(target, source + begin, n);
                        /*Thêm ký tự kết thúc chuỗi vào kết quả*/
                        target[n] = '\0';
                    }
                        /* Bỏ qua hàm nếu như vị trí bắt đầu cắt nằm ngoài chuỗi ban đầu*/
                    else {
                        target[0] = '\0';
                    }
                } else
                {
                    target[0] = '\0';
                }

                return target;
            }
            catch (const char * e) {

            }
        }

        std::vector<std::pair<std::string, cocos2d::Value>> cutTextToVectorString(std::string text, std::string key)
        {
            std::vector<std::pair<std::string, cocos2d::Value>> value = {};
            int sizeArr = text.size() / 100;
            int size = text.size();
            char* source = const_cast<char*>(text.c_str());
            if (text.size() > 100)
            {
                for (int i = 0; i <= sizeArr; i++)
                {
                    char target[105];
                    int numberBegen = 100 * i;
                    int numberEnd = 100;

                    if (i == sizeArr)
                    {
                        numberEnd = size - 100 * i;
                    }
                    substring(source, numberBegen, numberEnd, target);
                    std::string textUrl(target);
                    std::string url = key + std::to_string(i);
                    value.push_back({ url, cocos2d::Value(textUrl.c_str()) });
                }
            }
            else
            {
                value.push_back({ key, cocos2d::Value(text.c_str()) });
            }
            return value;
        }

        float getMaxFontSize(std::vector<std::string> listWords, cocos2d::Size box, float maxFontSize, std::string fontName) {
            auto tempText = cocos2d::ui::Text::create(listWords[0], fontName, 1);
            std::vector<float> maxFontOfEachWord;
            for (auto word : listWords) {
                if (word.empty()) {
                    continue;
                }
                tempText->setString(word);
                tempText->setFontSize(1);
                while (tempText->getBoundingBox().size.width < box.width && tempText->getBoundingBox().size.height < box.height && tempText->getFontSize() <= maxFontSize)
                {
                    tempText->setFontSize(tempText->getFontSize() + 1);
                }
                maxFontOfEachWord.push_back(tempText->getFontSize());
            }
            return *std::min_element(maxFontOfEachWord.begin(), maxFontOfEachWord.end());
        }

        bool softCompareString(std::string str1, std::string str2) {
            transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
            transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
            str1.erase(remove_if(str1.begin(), str1.end(), [](char c) { return !isalpha(c); }), str1.end());
            str2.erase(remove_if(str2.begin(), str2.end(), [](char c) { return !isalpha(c); }), str2.end());
            return !str1.compare(str2);
        }

        void setAnchorPointWithoutChangePosition(cocos2d::Node* node, cocos2d::Vec2 newAnchorPoint) {
            auto anchorChange = newAnchorPoint - node->getAnchorPoint();
            node->setAnchorPoint(newAnchorPoint);
            node->setPosition(node->getPosition() + Vec2(node->getBoundingBox().size.width * anchorChange.x, node->getBoundingBox().size.height * anchorChange.y));
        }

        void doAppearGradient(Node * n, float duration /*= 1.0f*/) {
    #if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
            auto overlay = n->getChildByName<LayerRadialGradient *>("grad_overlay");
            if (!overlay) {
                return;
            }

            overlay->setVisible(true);
            overlay->runAction(ActionFloat::create(duration, 0, 0xee, [overlay](float v) {
                overlay->setEndColor(Color4B(0, 0, 0, (int)v));
                overlay->setStartColor(Color4B(0, 0, 0, (int)v / 2));
            }));
    #else
            auto overlay = n->getChildByName<LayerColor *>("grad_overlay");
            if (!overlay) {
                return;
            }

            overlay->setVisible(true);
            overlay->runAction(FadeTo::create(duration, 0xaa));
    #endif
        }
		
	     bool isProfileNameValid(std::string name, int maxLength) {

            cocos2d::StringUtils::StringUTF8 txt = cocos2d::StringUtils::StringUTF8(name);

            if (txt.length() <= 0 || txt.length() > maxLength) {
                return false;
            }

            return !(std::regex_search(name, special_char_expression));
        }

        std::string getStringTime(long long timestamp, std::string format) {
            time_t rawtime = (time_t)timestamp;
            struct tm* dt;
            char buffer[30];
            dt = localtime(&rawtime);
            strftime(buffer, sizeof(buffer), format.c_str(), dt);
            return (std::string)buffer;
        }

        void encryptString(std::string &i_input) {

            std::string key = "B0-10-41-3E-5A-91";
            std::vector<unsigned char> key_data(key.begin(), key.end());
            std::vector<unsigned char> value_data(i_input.begin(), i_input.end());
            std::vector<unsigned char> encrypted_data;
            Aes256::encrypt(key_data, value_data, encrypted_data);
            std::string encrypted_string(encrypted_data.begin(), encrypted_data.end());
            i_input = base64::encode(encrypted_string);
        }

        void decryptString(std::string &i_input) {
            std::string key = "B0-10-41-3E-5A-91";
            i_input = base64::decode(i_input);
            std::vector<unsigned char> key_data(key.begin(), key.end());
            std::vector<unsigned char> encrypted_data(i_input.begin(), i_input.end());
            std::vector<unsigned char> decrypted_data;
            Aes256::decrypt(key_data, encrypted_data, decrypted_data);
            std::string decrypted_string(decrypted_data.begin(), decrypted_data.end());
            i_input = std::string(decrypted_data.begin(), decrypted_data.end());
        }
        void showWaitView()
        {
            if (auto waitingView = utils::findChild(Director::getInstance()->getRunningScene(), "waiting_view"))
            {
                waitingView->setZOrder(INT_MAX);
                waitingView->setVisible(true);
                return;
            }

            auto waitView = APWaitView::createView(std::string(""), false, true);
            waitView->setName("waiting_view");
            Director::getInstance()->getRunningScene()->addChild(waitView, INT_MAX - 1);
            
            Director::getInstance()->getRunningScene()->schedule([=](float) {
                int countTime = MJDEFAULT->getIntegerForKey("countTimeCheckWaitingView", 0);
                countTime++;
                MJDEFAULT->setIntegerForKey("countTimeCheckWaitingView", countTime);
                if (countTime >= 240) {
                    Director::getInstance()->getRunningScene()->unschedule("checkWaitingView");
                    deleteWaitView();
                }
                }, 1, CC_REPEAT_FOREVER, 0, "checkWaitingView");
        }

        void deleteWaitView()
        {

            if (auto waitingView = utils::findChild(Director::getInstance()->getRunningScene(), "waiting_view"))
            {
                waitingView->removeFromParent();
                if (Director::getInstance()->getRunningScene()->isScheduled("checkWaitingView"))
                {
                    MJDEFAULT->setIntegerForKey("countTimeCheckWaitingView", 0);
                    Director::getInstance()->getRunningScene()->unschedule("checkWaitingView");
                }
            }
        }

        float getDiskSpace()
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
            BOOL  fResult;
            unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
            fResult = GetDiskFreeSpaceEx(L"C:", (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);
            if (fResult)
            {
                float freeSpace = i64FreeBytes / (1024 * 1024 * 1024);
                return freeSpace;
            }
            return 0;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            return cocos2d::JniHelper::callStaticFloatMethod("org.cocos2dx.cpp.AppActivity", "getFreeInternalMemory");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            return mj::helper::getDiskSpaceIOS();
#endif
        }

        void coppyFile(std::string pathFileCoppy , std::string pathFileToCopy)
        {
            string dbPath = FileUtils::getInstance()->getWritablePath() + pathFileCoppy;
            //string dbPath = "F:/Project/Ms_build_win/Design";

            auto data = FileUtils::getInstance()->getDataFromFile(pathFileCoppy);

            FileUtils::getInstance()->writeDataToFile(data, dbPath);
        }

        void downloadFile(std::string url, std::function<void()> callback)
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                
                });
        }
    
        bool showPopupWarningFullDisk()
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
            return false;
#endif
            if(getDiskSpace() * 1024.0f < MIN_SIZE_DISK)
            {
                StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("fullmemory.warning"), LANGUAGE_MANAGER->getDisplayTextByKey("deactive.yes"), "Close");
                return true;
            }
            return false;
        }

        void autoScaleRootLayout(float scaleFactor, cocos2d::Node* parent, std::string rootLayout)
        {
            auto myVisibleSize = Director::getInstance()->getVisibleSize();

            if (auto myRootLayout = cocos2d::utils::findChild(parent, rootLayout)) {
                const float k_scaleFactor = ((myVisibleSize.width / myVisibleSize.height) / (4.0f / 3.0f));

                myRootLayout->setScale(std::min(k_scaleFactor, scaleFactor));
            }
        }
        void setGlobalZorderForNode(cocos2d::Node* parent, int zorder)
        {
            if (!parent)
            {
                return;
            }

            parent->setGlobalZOrder(zorder);
            auto children = parent->getChildren();

            if (!children.empty())
            {
                for (auto child : children)
                {
                    setGlobalZorderForNode(child, zorder);
                }
            }
            else {
                return;
            }
        }
        void ResetUserData()
        {
            auto user_info = ap::Database::getInstance().getUserInfo();
            // delete listener for user id
            mm::RealtimeDatabase::getInstance().removeListener(user_info._id);

            PROFILE_MANAGER.clearDataModel();
            ms::LessonManager::getInstance().clearDataLessonFree();
            //clear all information
            CCLOG("reset...");
            auto user_defaults_xml = cocos2d::UserDefault::getInstance()->getXMLFilePath();

            if (FileUtils::getInstance()->removeFile(user_defaults_xml)) {
                CCLOG("Removed User Defaults XML %s", user_defaults_xml.c_str());
            }
            else {
                CCLOG("Cannot remove %s", user_defaults_xml.c_str());
            }

            std::map<std::string, bool > debug_mode = {};
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            debug_mode[debugkey_full_features] = MJDEFAULT->getBoolForKey(debugkey_full_features);
            debug_mode[debugkey_country_gb] = MJDEFAULT->getBoolForKey(debugkey_country_gb);
            debug_mode[debugkey_license_disable] = MJDEFAULT->getBoolForKey(debugkey_license_disable);
            debug_mode[debugkey_test_config] = MJDEFAULT->getBoolForKey(debugkey_test_config);
            debug_mode[debugkey_quick_play_game] = MJDEFAULT->getBoolForKey(debugkey_quick_play_game);
            debug_mode[debugkey_test_7_days] = MJDEFAULT->getBoolForKey(debugkey_test_7_days);
            debug_mode[debugkey_test_free] = MJDEFAULT->getBoolForKey(debugkey_test_free);
            debug_mode[debugkey_dev_environment] = MJDEFAULT->getBoolForKey(debugkey_dev_environment);
            debug_mode[debugkey_product_environment] = MJDEFAULT->getBoolForKey(debugkey_product_environment);
            debug_mode[debugkey_fake_device_id] = MJDEFAULT->getBoolForKey(debugkey_fake_device_id);
            debug_mode[debugkey_all_lessons] = MJDEFAULT->getBoolForKey(debugkey_all_lessons);
            debug_mode[debugkey_fake_ipx] = MJDEFAULT->getBoolForKey(debugkey_fake_ipx);
            debug_mode[debugkey_is_test_license] = MJDEFAULT->getBoolForKey(debugkey_is_test_license);
            debug_mode[debugkey_test_act_vmonkey] = MJDEFAULT->getBoolForKey(debugkey_test_act_vmonkey);
#endif

            auto v_cmonres_cur = MJDEFAULT->getIntegerForKey(key_common_res_version_current, -1);
            auto v_cmonres_req = MJDEFAULT->getIntegerForKey(key_common_res_version_request, 0);

            auto current_lang_Id = MJDEFAULT->getIntegerForKey(key_current_lang);
            auto key_common_res = MJDEFAULT->getIntegerForKey(key_common_res_version_current);
            auto key_converted = MJDEFAULT->getBoolForKey(key_data_converted);
            auto lang_display = MJDEFAULT->getIntegerForKey(key_lang_display, LANGUAGE_EN_CODE);
            auto is_account_unverified = MJDEFAULT->getBoolForKey("account_unverified", false);
            auto is_skip_login = MJDEFAULT->getBoolForKey("is_user_skip", false);
            auto time_popup_upgrade = MJDEFAULT->getIntegerForKey("time_skip_popup_upgrade_account", 0);

            auto isShowLanguageScene = MJDEFAULT->getBoolForKey("is_language_selection_show", false);

            auto last_time_enter_info = MJDEFAULT->getStringForKey(key_last_time_enter_info + std::to_string(user_info._id));

            auto keyLastUpdateId = MJDEFAULT->getIntegerForKey("key_last_update_id", -1);
            auto keyUpdateShow = MJDEFAULT->getBoolForKey(key_update_show, false);
            auto keyTimeUpdateSkip = MJDEFAULT->getIntegerForKey(key_time_update_skip, 0);
            auto loginType = MJDEFAULT->getStringForKey("ap_login_type", "none");
            auto keyListLang = MJDEFAULT->getStringForKey(key_list_lang, "[1]");


#pragma region MyTruongBX update
            /// <summary>
    ///-------------------------------------------------
    /// </summary>
            auto key_device_identifier_ = MJDEFAULT->getStringForKey(key_device_identifier, "");
            auto first_install_is_finished = MJDEFAULT->getBoolForKey(key_check_is_first_install_is_finished);
            auto checkIsLoadUpdate = MJDEFAULT->getBoolForKey(key_checkIsLoadUpdate);
            auto first_api_called = MJDEFAULT->getBoolForKey(key_first_api_called);

            auto country_code = MJDEFAULT->getStringForKey(key_country_code);
            auto is_rate = MJDEFAULT->getBoolForKey(key_rate_app_won);
            auto path_install = MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY);
            auto download_url_story = MJDEFAULT->getStringForKey(DOMAIN_STORY_USING_KEY);
            auto download_url_game = MJDEFAULT->getStringForKey(DOMAIN_GAME_USING_KEY);
            auto download_url_audio = MJDEFAULT->getStringForKey(DOMAIN_AUDIO_USING_KEY);
            auto download_url_activities = MJDEFAULT->getStringForKey(DOMAIN_ACTIVITIES_USING_KEY);
            auto download_url_media = MJDEFAULT->getStringForKey(DOMAIN_MEDIA_USING_KEY);
            std::string country_code_here = MJDEFAULT->getStringForKey(COUNTRY_CODE_HERE_KEY, "VN");

            auto audioBookVersion_fisrtInstall = MJDEFAULT->getIntegerForKey(key_api_version_audio_fisrt_install);
            auto audioBookVersion_local = MJDEFAULT->getIntegerForKey(key_api_version_audio_local);
            auto audioBookVersion_server = MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer);

            auto version_story_fisrt_install = MJDEFAULT->getIntegerForKey(key_api_version_story_fisrt_install);
            auto version_story_local = MJDEFAULT->getIntegerForKey(key_api_version_story_local);
            auto story_v2_fromJsonServer = MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer);

            auto version_material_local = MJDEFAULT->getIntegerForKey(key_api_version_material_local);
            auto version_material_fromServer = MJDEFAULT->getIntegerForKey(key_api_version_material_fromServer);

            auto version_all_games_local = MJDEFAULT->getIntegerForKey(key_api_version_all_games_local);
            auto version_all_games_fromServer = MJDEFAULT->getIntegerForKey(key_api_version_all_games_fromServer);

            auto lessons_version_fisrt_install = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fisrt_install);
            auto lessons_version_local = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_local);
            auto lessons_version_fromServer = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fromServer);


            auto version_categories_local = MJDEFAULT->getIntegerForKey(key_api_version_categories_local);
            auto version_categories_fromServer = MJDEFAULT->getIntegerForKey(key_api_version_categories_fromServer);

            auto priceInfo = MJDEFAULT->getStringForKey(keyPriceInfo);

            auto THUMB_STORY_URL_KEY_ = MJDEFAULT->getStringForKey(THUMB_STORY_URL_KEY);

            auto THUMB_AUDIO_URL_KEY_ = MJDEFAULT->getStringForKey(THUMB_AUDIO_URL_KEY);

            auto THUMB_LESSON_URL_KEY_ = MJDEFAULT->getStringForKey(THUMB_LESSON_URL_KEY);
            auto THUMB_OTHER_URL_KEY_ = MJDEFAULT->getStringForKey(THUMB_OTHER_URL_KEY);


            auto push_error_firebase = MJDEFAULT->getBoolForKey(key_push_event_crash_to_fibase);

            auto ip = MJDEFAULT->getStringForKey(IP_NAME_KEY);
            auto country_name = MJDEFAULT->getStringForKey(COUNTRY_NAME_KEY);
            auto region_name = MJDEFAULT->getStringForKey(REGION_NAME_KEY);
            auto city_name = MJDEFAULT->getStringForKey(CITY_NAME_KEY);
            auto latitude = MJDEFAULT->getFloatForKey(LATITUDE_NAME_KEY);
            auto longitude = MJDEFAULT->getFloatForKey(LONGITUDE_NAME_KEY);
            auto countryCodeHere = MJDEFAULT->getStringForKey(COUNTRY_CODE_HERE_KEY);
            auto is_vn = MJDEFAULT->getBoolForKey(IS_VN_KEY);
            auto is_submit = MJDEFAULT->getBoolForKey(key_is_submit, false);

            auto key_user_id_ = MJDEFAULT->getIntegerForKey(key_user_id);
            ap::UserInfo  userInfo = ap::Database::getInstance().getUserInfo();

            auto keyf2p_account_free_info_for_merge = MJDEFAULT->getStringForKey(key_f2p_account_free_info_for_merge);
            auto keyf2p_account_paid_info_for_merge = MJDEFAULT->getStringForKey(key_f2p_account_paid_info_for_merge);
            auto f2pis_megre_skip_account = MJDEFAULT->getBoolForKey("f2p_is_megre_skip_account", false);
            auto f2p_is_skip_user = MJDEFAULT->getBoolForKey("f2p_is_skip_user", false);
            auto setKeyGotoF2PGetStarted = MJDEFAULT->getBoolForKey("setKeyGotoF2PGetStarted");
            auto getKeyGotoF2PSplash =  MJDEFAULT->getBoolForKey("KeyGotoF2PSplash", false);
            auto f2p_is_megred_account =  MJDEFAULT->getBoolForKey("f2p_is_megred_account", false);

            //-------------------------------------------------------------------------------
#pragma endregion

#pragma region MyRegion
            MJDEFAULT->deleteValueForKey("require_load_update");
            mj::PlatformConfig::getInstance().clearUserDefaults();

            auto writable_path = FileUtils::getInstance()->getWritablePath();

            FileUtils::getInstance()->removeDirectory(writable_path + "profile/");

            StoryDataModel::getInstance()->resetLessonLearned();

            MJDEFAULT->setIntegerForKey(key_common_res_version_current, v_cmonres_cur);
            MJDEFAULT->setIntegerForKey(key_common_res_version_request, v_cmonres_req);

            MJDEFAULT->setBoolForKey("is_language_selection_show", isShowLanguageScene);

            MJDEFAULT->setStringForKey("ap_login_type", loginType);
            MJDEFAULT->setBoolForKey("is_user_skip", is_skip_login);
            ap::Database::getInstance().dropDatabaseProfiles();

            MJDEFAULT->setIntegerForKey("key_last_update_id", keyLastUpdateId);
            MJDEFAULT->setBoolForKey(key_update_show, keyUpdateShow);
            MJDEFAULT->setIntegerForKey(key_time_update_skip, keyTimeUpdateSkip);
            MJDEFAULT->setBoolForKey(key_logged_in, false);

            //máy đã từng sign out -> đã có tài khoản đc convert
            MJDEFAULT->setBoolForKey(key_first_logged_out, true);
            MJDEFAULT->setIntegerForKey(key_common_res_version_current, key_common_res);
            MJDEFAULT->setIntegerForKey(key_current_lang, current_lang_Id);
            MJDEFAULT->setBoolForKey(key_data_converted, key_converted);
            MJDEFAULT->setIntegerForKey(key_lang_display, lang_display);
            MJDEFAULT->setIntegerForKey("time_skip_popup_upgrade_account", time_popup_upgrade);
            MJDEFAULT->setStringForKey(key_list_lang, keyListLang);
            MJDEFAULT->setStringForKey(key_last_time_enter_info + std::to_string(user_info._id), last_time_enter_info);

            LANGUAGE_MANAGER->updateLanguageWithID(lang_display);
#pragma endregion

#pragma region TruongBXUpdate

            //-----------------------------------------------
            MJDEFAULT->setBoolForKey(key_check_is_first_install_is_finished, first_install_is_finished);
            MJDEFAULT->setBoolForKey(key_checkIsLoadUpdate, checkIsLoadUpdate);
            MJDEFAULT->setBoolForKey(key_first_api_called, first_api_called);

            MJDEFAULT->setStringForKey(key_country_code, country_code);
            MJDEFAULT->setBoolForKey(key_rate_app_won, is_rate);
            MJDEFAULT->setStringForKey(PATH_FIRST_INSTALL_KEY, path_install);
            MJDEFAULT->setStringForKey(DOMAIN_MEDIA_KEY, download_url_media);
            MJDEFAULT->setStringForKey(DOMAIN_STORY_USING_KEY, download_url_story);
            MJDEFAULT->setStringForKey(DOMAIN_GAME_USING_KEY, download_url_game);
            MJDEFAULT->setStringForKey(DOMAIN_AUDIO_USING_KEY, download_url_audio);
            MJDEFAULT->setStringForKey(DOMAIN_ACTIVITIES_USING_KEY, download_url_activities);
            MJDEFAULT->setStringForKey(DOMAIN_MEDIA_USING_KEY, download_url_media);
            MJDEFAULT->setStringForKey(COUNTRY_CODE_HERE_KEY, country_code_here);

            MJDEFAULT->setIntegerForKey(key_api_version_audio_fisrt_install, audioBookVersion_fisrtInstall);
            MJDEFAULT->setIntegerForKey(key_api_version_audio_local, audioBookVersion_local);
            MJDEFAULT->setIntegerForKey(key_api_version_audio_v2_fromServer, audioBookVersion_server);

            MJDEFAULT->setIntegerForKey(key_api_version_story_fisrt_install, version_story_fisrt_install);
            MJDEFAULT->setIntegerForKey(key_api_version_story_local, version_story_local);
            MJDEFAULT->setIntegerForKey(key_api_version_story_v2_fromJsonServer, story_v2_fromJsonServer);

            MJDEFAULT->setIntegerForKey(key_api_version_material_local, version_material_local);
            MJDEFAULT->setIntegerForKey(key_api_version_material_fromServer, version_material_fromServer);

            MJDEFAULT->setIntegerForKey(key_api_version_all_games_local, version_all_games_local);
            MJDEFAULT->setIntegerForKey(key_api_version_all_games_fromServer, version_all_games_fromServer);

            MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_fisrt_install, lessons_version_fisrt_install);
            MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_local, lessons_version_local);
            MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_fromServer, lessons_version_fromServer);


            MJDEFAULT->setIntegerForKey(key_api_version_categories_local, version_categories_local);
            MJDEFAULT->setIntegerForKey(key_api_version_categories_fromServer, version_categories_fromServer);

            MJDEFAULT->setStringForKey(keyPriceInfo, priceInfo);

            MJDEFAULT->setStringForKey(THUMB_STORY_URL_KEY, THUMB_STORY_URL_KEY_);

            MJDEFAULT->setStringForKey(THUMB_AUDIO_URL_KEY, THUMB_AUDIO_URL_KEY_);

            MJDEFAULT->setStringForKey(THUMB_LESSON_URL_KEY, THUMB_LESSON_URL_KEY_);

            MJDEFAULT->setStringForKey(THUMB_OTHER_URL_KEY, THUMB_OTHER_URL_KEY_);

            MJDEFAULT->setBoolForKey(key_push_event_crash_to_fibase, push_error_firebase);
            MJDEFAULT->setBoolForKey(key_is_submit, is_submit);
            MJDEFAULT->setStringForKey(IP_NAME_KEY, ip);
            MJDEFAULT->setStringForKey(COUNTRY_NAME_KEY, country_name);
            MJDEFAULT->setStringForKey(REGION_NAME_KEY, region_name);
            MJDEFAULT->setStringForKey(CITY_NAME_KEY, city_name);
            MJDEFAULT->setFloatForKey(LATITUDE_NAME_KEY, latitude);
            MJDEFAULT->setFloatForKey(LONGITUDE_NAME_KEY, longitude);
            MJDEFAULT->setStringForKey(COUNTRY_CODE_HERE_KEY, countryCodeHere);
            MJDEFAULT->setBoolForKey(IS_VN_KEY, is_vn);
            MJDEFAULT->setStringForKey(key_device_identifier, key_device_identifier_);
            ap::Database::getInstance().saveUserInfo(std::to_string(userInfo._id), userInfo._name, userInfo._mail, userInfo._phone, userInfo._address, userInfo._userName);
            MJDEFAULT->getIntegerForKey(key_user_id, key_user_id_);
            MJDEFAULT->setIntegerForKey(key_api_version_app_info_local, 0);
            MJDEFAULT->setIntegerForKey(key_api_version_app_info_from_server, 0);
            
            MJDEFAULT->setBoolForKey("f2p_is_megre_skip_account", f2pis_megre_skip_account);
            MJDEFAULT->setStringForKey(key_f2p_account_free_info_for_merge, keyf2p_account_free_info_for_merge);
            MJDEFAULT->setStringForKey(key_f2p_account_paid_info_for_merge, keyf2p_account_paid_info_for_merge);
            MJDEFAULT->setBoolForKey("f2p_is_skip_user", f2p_is_skip_user);
            MJDEFAULT->setBoolForKey("setKeyGotoF2PGetStarted", setKeyGotoF2PGetStarted);
            MJDEFAULT->setBoolForKey("KeyGotoF2PSplash", getKeyGotoF2PSplash);
            MJDEFAULT->setBoolForKey("f2p_is_megred_account", f2p_is_megred_account);

            //-----------------------------------------------
#pragma endregion

            for (auto m : debug_mode)
            {
                MJDEFAULT->setBoolForKey(m.first, m.second);
            }
        }

        void SetFpsDefault()
        {
            Director::getInstance()->setAnimationInterval(1.0 / 40);
        }

        void SetFpsByNumberFrames(int framePerSecond)
        {
            // hien chi can toi uu cho windows
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
            Director::getInstance()->setAnimationInterval(1.0 / framePerSecond);
#endif
        }

        float lerp(float a, float b, float x, float y, float i)
        {
            if (a == b) {
                CCLOG("something wrong!");
                return -1;
            }

            return (((i - a) / (b - a)) * (y - x)) + x;
        }
		
        void pasteText(ui::TextField* textField)
        {

            if (textField == nullptr)
            {
                return;
            }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            CCLOG("%s", textField->getName());
            if (!OpenClipboard(nullptr))
            {
                CCLOG("Clip board not found");
                return;
            }
            HANDLE m_handleData = GetClipboardData(CF_TEXT);
            if (m_handleData == nullptr)
            {
                CCLOG("Clip board text not found");
                return;
            }
            char* m_pText = static_cast<char*>(GlobalLock(m_handleData));
            if (m_pText == nullptr)
            {
                CCLOG("Text null");
                return;
            }
            std::string m_txt(m_pText);
            GlobalUnlock(m_handleData);
            CloseClipboard();

            textField->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
            textField->setBright(true);
            textField->setHighlighted(true);
            textField->setString(textField->getString() + m_txt);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
            FILE* clipB = popen("pbpaste", "r");
            if (!clipB)
            {
                return;
            }
            char buffer[128];
            std::string result = "";
            while (!feof(clipB))
            {
                if (fgets(buffer, 128, clipB) != NULL)
                {
                    result += buffer;
                }
            }
            if (result.at(result.size() - 1) == '\n')
            {
                result.at(result.size() - 1) = '\0';
            }
            pclose(clipB);
            textField->setString(textField->getString() + result);

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

        }


        void copyText(ui::TextField* textField)
        {
            if (textField == nullptr)
            {
                return;
            }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            auto m_glb = GlobalAlloc(GMEM_FIXED, 32);
            memcpy(m_glb, (textField->getString()).c_str(), 32);
            CCLOG("%s", textField->getName());
            if (!OpenClipboard(nullptr))
            {
                CCLOG("Clip board not found");
                return;
            }
            EmptyClipboard();
            SetClipboardData(CF_TEXT, m_glb);
            CloseClipboard();
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
            const char* str = (textField->getString()).c_str();
            const char proto_cmd[] = "echo '%s' | pbcopy";
            int a = strlen(proto_cmd);
            int b = strlen(str);
            char* cmd = (char*)malloc((a + b - 1) * sizeof(char*));
            sprintf(cmd, proto_cmd, str);
            system(cmd);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)


        }

        long long unixTimestamp(int year, int month, int day, int hour, int min, int sec)
        {
            struct tm tm;
            time_t rawtime;
            time(&rawtime);
            tm = *localtime(&rawtime);
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            tm.tm_hour = hour;
            tm.tm_min = min;
            tm.tm_sec = sec;
            long long time_stamp = mktime(&tm);
            CCLOG("timestamp: %lld", time_stamp);
            return time_stamp;
        }
        //hàm tạo quán tính cho listView khi jumToLeft or jumToRight(k hiểu tại sao hàm có sẵn k dùng được)
        void inertiaScrollLeftOrRight(cocos2d::Node* parent, cocos2d::ui::ListView* listView, bool isLeft)
        {
            if (!listView || !parent)
                return;
            parent->runAction(Sequence::create(DelayTime::create(0.00001), CallFunc::create([=] {
                listView->jumpToPercentHorizontal(isLeft ? listView->getScrolledPercentHorizontal() + 1 : listView->getScrolledPercentHorizontal() - 1);
                }), DelayTime::create(0.00001), CallFunc::create([=] {
                    int per = listView->getScrolledPercentHorizontal();
                    if ((listView->getScrolledPercentHorizontal() >= 0 && isLeft) || (listView->getScrolledPercentHorizontal() <= 100 && !isLeft))
                    {
                        isLeft ? listView->jumpToLeft() : listView->jumpToRight();
                    }
                    else
                    {
                        inertiaScrollLeftOrRight(parent, listView, isLeft);
                    }
                    }), NULL));
        }
    }
        
}

