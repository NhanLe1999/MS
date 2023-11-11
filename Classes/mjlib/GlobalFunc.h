//
//  GlobalFunc.h
//  MonkeyJunior
//
//  Created by developer on 3/10/16.
//
//

#ifndef GlobalFunc_h
#define GlobalFunc_h


#include "cocos2d.h"
#include "IAPDefinitions.h"
//#include "HandleDeepLink.h"

using namespace cocos2d;
using namespace std;

USING_NS_CC;

class GlobalFunc
{
public:

    static int _currentIndexListenerKeyboard;
    static GlobalFunc* getInstance();
    static bool testUSDevice();
    static bool testPhonic();
    static bool checkDevice();
    static ValueVector chunkValueVector(ValueVector array, int offset);
    static string urlEncode(string &s);
    static string joinIntVector(std::vector<int> vv, std::string delimiter);
    static string joinStringVector(std::vector<string> vv, std::string delimiter);
    static std::string trim(std::string& str, string character);
    static void ReplaceString(std::string &subject, const std::string& search,
                                            const std::string& replace);
    static bool icompare(string a, string b);
    static string utf8characterAtIndex(string str, int index);
    static string string_towupper(string str);
    static string string_towlower(string str);
    static bool vectorExits(vector<string> vector, string str);
    
    static void insertDatabase();
    static string buildImagePath(string img, string ext);
    static string fileInResourse(string fileName);
    static vector<string> explode(const string& str, const char& ch);
    static void changeUserID(int uid);
    static int userID();
    static string getCurrentLessonId();
    static int getTotalLetter(string str);
    static void shuffleArray(ValueVector &vector);
    static void reverseArray(ValueVector &vector);
    static string dauVN(int kieudau, bool sound = false);
    //bai hoc
    static bool configLessonAuto();
    static bool configLessonChildlock();
    //ngu am
    static bool configPhonicVisible();
    static bool configPhonicFast();
    static bool configPhonicHighlight();
    //trao the
    static float configFastCardDelayTime();
    static bool configFastCardAudio();
    static int configFastCardType();
    //cap do kho
    static bool configSentenceImageVisible();
    //the chu
    static bool configPointerDisplay();
    
    
    static int getSoBaiDuocHoc();
    static void setSoBaiDuocHoc(int soBaihoc);
    
    static int getThoigianGiuaHaiLanHoc();
    static void setThoigianGiuaHaiLanHoc(int soGiay);
    
    static std::string getStringOfDateHanChe(std::string keyStr);
    static void setStringOfDateHanChe(std::string keyStr, std::string dateStr);
    
    static int getNumLessonHanche();
    static void setNumLessonHanche(int value);
    
    static void configSetLessonAuto(bool vl);
    static void configSetLessonChildlock(bool vl);
    //ngu am
    static void configSetPhonicVisible(bool vl);
    static void configSetPhonicFast(bool vl);
    static void configSetPhonicHighlight(bool vl);
    //trao the
    static void configSetFastCardDelayTime(float vl);
    
    static void configSetFastCardType(int vl);
    
    static void configSetFastCardAudio(bool vl);
    //cap do kho
    static void configSetSentenceImageVisible(bool vl);
    //nhac hoc
    static void configSetPointerDisplay(bool vl);
    
    //MARK: lien quan den ngon ngu
    static vector<int> getComingSoonLanguage();
    static ValueMap getMapLanguageName();
    static ValueMap getMapLanguageCode();
    static int getLangidByCode(string code);
    static string getLanguageName(int lang_id);
    static string getLanguageCode(int lang_id = 0);
    static bool languageIsNew(int lang_id);
    static int detectLanguageBuyPackage(string package);
    static int getCurrentLanguage();
    static void setCurrentLanguage(int lang_id);
    static string getLanguageFont();
    static void phonicColor(Color3B &hl, Color3B &nm, int index = -1);
        
    // load spritesheet
    static void loadSpriteSheets(string path);
    static vector<int> getLanguageAvaiable();
    
    //MARK: lien quan den level
    /**
     ham nay tra ve level type cua ngon ngu
     1 => Easy
     2 => Medium
     3 => Advanced
     */
    static string getLevelType(int level_type, bool upper = false, bool hard = false);
    static bool networkAvaiable();
    static int getDeviceID();
    static vector<string> getStickerNeedPerchase(bool new_pk = true);
    static ValueVector getStickerPerchased();
    static void stickerPerchased(string package_name);
    
    static void setCouponCode(string coupon);
    static string getCouponCode();
    
    static void replacePackage(string &package);
    static void createLicenceMessage(string licence, string product_name,int total_device = 0);
    static void processUpdateInfo(ValueMap map_response);
    
    static void playLanguageAudio(string config_name);

    static ValueMap userLevelInfo();
    static void userSaveLevelInfo(ValueMap save_map);
    static void userSaveLevelInfoWithSync(int level_id, int lesson_index, bool action_now = false);
    static void poolSprite(Node *node, string str);
    static string getRateAppUrl();
    
    template<typename Base>
    static Base getSuperParent(Node *node){
        Node *tmp = node->getParent();
        Base item = dynamic_cast<Base>(tmp);
        while(item == NULL) {
            tmp = tmp->getParent();
            item = dynamic_cast<Base>(tmp);
        }
        return item;
    }
    static void sendEmailSupport();
    static int totalSampleLesson();
    //codemoi
    int current_language_play = 0;
    int current_level_play = 0;
    int current_level_id_play = 0;
    ValueMap listGroupAudio;
    ValueMap loadGroupAudioByLevel(ValueMap dict);
    //end
    
    static void syncDataProcess(ValueMap dict_response);
    static void onDeepLink(std::string url);

private:
    GlobalFunc(){

    };
    static GlobalFunc * globalFunc;
    MK_SYNTHESIZE_WITHDEFAULT_VALUE(bool, _isClevertapProfileInitialized, IsClevertapProfileInitialized, false);
    MK_SYNTHESIZE_WITHDEFAULT_VALUE(bool, _isClevertapLoginProcessing, IsClevertapLoginProcessing, false);
    MK_SYNTHESIZE_WITHDEFAULT_VALUE(bool, _isNeedReloginClevertap, IsNeedReloginClevertap, false);
    //MK_SYNTHESIZE_WITHDEFAULT_VALUE(HandleDeepLink*, _currentDeepLink, CurrentDeepLink, nullptr);

};
#endif /* GlobalFunc_hpp */
