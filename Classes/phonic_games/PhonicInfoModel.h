#ifndef PhonicInfoModel_hpp
#define PhonicInfoModel_hpp

#include <string>
#include <vector>
#include "cocos2d.h"

#define GAME_TYPE_PHONIC "GAME_TYPE_PHONIC"
#define GAME_TYPE_SIGHTWORD "GAME_TYPE_SIGHTWORD"
namespace mj {
	
    struct PHONIC_INFO {
        std::string text_full;
        std::string text_phonic;
        std::string audio_phonic;
        std::string audio_full;
        std::string image_path;
        std::string video_path;
        std::vector<int> highlights;
        cocos2d::Color3B normal_color, highlight_color;
    };

	struct PHONIC_WORD {
		std::string text;
		std::string audio_path;
	};

	struct GAME_PHONIC_INFO {
		std::vector<PHONIC_INFO> list_phonic;
		std::vector<PHONIC_WORD> list_distracting_phonic;
	};
    
    struct PHONIC_BOARD_INFO {
        std::string video_path;
        std::string image_path;
        std::string text_full;
        std::string audio_full;
    };
    
    struct SIGHTWORD_WORD {
        std::string text;
        std::string audio_path;
    };
    
    struct SIGHTWORD_INFO {
        std::vector<SIGHTWORD_WORD> list_sightword;
        std::vector<std::string> list_distracting;
    };
    
    struct GEAR_ITEM {
        bool isTrueAns;
        std::string image_path;
        std::string audio_path;
    };
    
    struct HIGHLIGHT_INFO {
        std::string word;
        int s, e, ts, te;
    };
}

#endif /* PhonicInfoModel_hpp */

