#ifndef MSBookData_hpp
#define MSBookData_hpp

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "cocos2d.h"

namespace mj {
    
    struct BOOK_INLINE_IMAGE {
        std::string text;
        int image_width;
        int image_height;
        std::vector<int> position;
        std::string image_path;
        std::string audio_path;
    };
    
    struct BOOK_INLINE_TRANSLATION {
        std::string text;
        std::vector<int> position;
        std::string translation;
        std::string audio_path;
        std::string translation_path;
    };
    
    struct BOOK_TEXT_INFO {
        std::string text;
        std::string boundingbox;
        std::string highlight_color;
        std::string normal_color;
        std::string fontname;
        int fontsize;
        float start, end;
        int line_space;
        cocos2d::Color3B highlight_color_3b;
        cocos2d::Color3B normal_color_3b;
        
        std::vector<BOOK_INLINE_IMAGE> inline_images;
        std::vector<BOOK_INLINE_TRANSLATION> inline_translations;
    };
    
    struct BOOK_SYNC_TEXT {
        std::string w;
        int ts, te;
        float s, e;
    };
    
    struct BOOK_AUDIO_INFO {
        std::string path;
        float start;
        std::vector<BOOK_SYNC_TEXT> list_sync;
        std::vector<std::pair<float, float>> delay_config;
    };
    
    struct BOOK_BG_IMG
    {
        std::string path;
        std::string position;
    };
    
    struct BOOK_TOUCH
    {
        std::string boundingbox;
        std::string star_position;
    };
    
    struct BOOK_IMAGE_AUDIO
    {
        std::string path;
        std::string text;
        float duration;
        int fontsize;
        float w_text;
    };
    
    struct BOOK_IMAGE_TOUCH
    {
        std::string boundingbox, star_position;
        std::vector<cocos2d::Point> vertices;
    };
    
    struct BOOK_IMAGE_SEQUENCE
    {
        std::string name;
        float start, end;
        bool loop;
        std::string effect;
    };
    
    struct BOOK_IMAGE_INFO
    {
        std::string path;
        std::string type;
        bool animation_reset, touchable;
        int z_order, animation_order, star_order;
        std::string position, contentsize, animation_type, effect;
        BOOK_IMAGE_AUDIO audio_info;
        std::vector<BOOK_IMAGE_TOUCH> list_touch;
        std::vector<BOOK_IMAGE_SEQUENCE> list_sequence;
    };
    
    struct BOOK_DATA
    {
        std::string box_type;
        BOOK_BG_IMG book_bg_img;
        std::vector<BOOK_TEXT_INFO> list_text;
        std::vector<BOOK_AUDIO_INFO> list_audio;
        std::vector<BOOK_IMAGE_INFO> list_image;
        
    };
    
}

#endif /* MSBookData_hpp */
