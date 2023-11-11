//
//  MJ_PageLoadResource.hpp
//  M
//
//  Created by To Ngoc Linh on 5/12/17.
//
//

#ifndef MJ_PageLoadResource_hpp
#define MJ_PageLoadResource_hpp

#include <stdio.h>
#include <string>
#include <vector>
namespace mj {
    class PageLoadResource {
    public:

        static PageLoadResource & getInstance() {
            static PageLoadResource pl_instance;
            return pl_instance;
        }

        static void parseStory(int story_id, int lang_id, int total_page);
        static void releaseAllResource();
        static void deleteCacheOfStory(int _id, std::string file_zip = "");
        static void deleteCacheAllStory();
        static void deleteCacheOfAudiobook(int _id);
        static void deleteCacheAllAudiobooks();
        static void deleteCacheAllLessons();
        
    private:
        PageLoadResource();
        PageLoadResource(PageLoadResource const &);
        void operator = (PageLoadResource const &);

        void parsePagePlist(std::string name);
        void parseLearnPlist(std::string name);
        void parseGamePlist(std::string name);
        
        void parsePageJson(std::string name);
    
        std::vector<std::string> audios;
        std::vector<std::string> images;
        std::vector<std::string> animations;
        std::vector<std::string> plist_files;
    };
}

#endif /* MJ_PageLoadResource_hpp */
