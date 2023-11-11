//
//  MathNetworking.h
//  MJQuiz
//
//  Created by HuyPhuong on 12/3/17.
//

#ifndef MathNetworking_h
#define MathNetworking_h

#include <stdio.h>
#include "cocos2d.h"

#include "network/CCDownloader.h"

namespace mj {
    namespace network {
        namespace api {

            struct Activity {
                int _id;
                std::string name;
                std::string zip_link;
                int game_id;
                std::string game_name;
                
                bool isNull() {
                    return _id==-1 && name=="";
                }
            };
            
            struct Lession {
                int _id;
                std::string name;
                int order_by;
                
                bool isNull() {
                    return _id==-1 && name=="";
                }
                
            };
            
            struct Category {
                int _id;
                std::string name;
                int parent_id;
                
                bool isNull() {
                    return _id==-1 && name=="";
                }
            };
            
            struct Game {
                int _id;
                std::string name;
                std::string config_path;
                
                bool isNull() {
                    return _id==-1 && name=="";
                }
                
            };
            
            static Activity ActivityNull = {-1, ""};
            static Lession  LessionNull  = {-1, "", -1};
            static Category CategoryNull = {-1, "", -1};
            static Game     GameNull     = {-1, ""};
            
            typedef std::function<void(std::vector<api::Category>)> requestCategoriesCallback;
            typedef std::function<void(std::vector<api::Game>)> requestGamesCallback;
            typedef std::function<void(std::vector<api::Lession>)> requestLesionsCallback;
            typedef std::function<void(std::vector<api::Activity>)> requestActivitiesCallback;

            class Helper {
            public:
                static void getCategoriesFromServer(requestCategoriesCallback func);
                static void getActivitiesFromServer(requestActivitiesCallback func, Category category, Lession lession = LessionNull);
                static void getLessionsFromServer(requestLesionsCallback func, Category category);
                static void getGamesFromServer(requestGamesCallback func);
                
                static void downloadActivityZip (Activity activity, std::function<void(const cocos2d::network::DownloadTask& task)> success_call, std::function<void(const cocos2d::network::DownloadTask& task, int errorCode)> error_call);
                
                static std::vector<Category> getGrades (std::vector<Category> categories);
                static std::vector<Category> getSubCategories (Category parent, std::vector<Category> categories);
            private:
                static std::vector<Game> getGameList (char* data);
                static std::vector<Category> getCategoryList (char* data);
                static std::vector<Lession> getLessionList (char* data);
                static std::vector<Activity> getActivityList (char* data);
            };
        }
    }
    
    class ZipUtils {
    public:
        
        static bool unzip(std::string zip, std::string directory);
        static void unzipAsync(std::string zip_file, std::string directory, std::function<void(bool)> callback);
        
        static std::string getZipPath();
        static std::string getConfigPath(network::api::Activity activity = network::api::ActivityNull);
        static std::string getIconsPath();
        static std::string getFilenameFromURL(std::string url, bool include_ext = true);
        static std::string getBasenameFromURL(const std::string& path);

    };

};
#endif /* MathNetworking_h */
