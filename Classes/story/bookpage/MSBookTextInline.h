//
//  MSBookTextInline.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/27/18.
//

#ifndef MSBookTextInline_hpp
#define MSBookTextInline_hpp

#include "cocostudiomacro2.h"
#include "MSBookData.h"
#include <cctype>
class MSBookTextInline : public cocos2d::ui::Layout {
public:
    static MSBookTextInline * create(mj::BOOK_TEXT_INFO info);
    void repositioning();
    void highlight(int start_idx, int stop_idx);
    void resetHighlight();
    void enableInteraction();
private:
    enum class ElementType {
        NORMAL,
        TRANSLATED,
        IMAGE
    };
    
    struct ElementInfo {
        ElementType type;
        cocos2d::Size image_size;
        std::string text;
        std::string image;
        std::string audio;
        std::string translation;
        std::string translation_audio;
        std::vector<int> position;
        int start_sync_index;
        
        friend bool operator==(const ElementInfo & a, const ElementInfo & b) {
            return a.type == b.type && a.text == b.text && a.position == b.position;
        }
    };
    
    void onClick(cocos2d::Ref * sender);
    void showTranslatedIndicator();
    void showTranslated(Node *, ElementInfo);
    void showImage(Node *, ElementInfo);
    
    void clearTranslation();
    
    bool init(mj::BOOK_TEXT_INFO info);
    mj::BOOK_TEXT_INFO _info;
    
    std::vector<ElementInfo> _elements;
    cocos2d::ui::RichText * _render;
    std::vector<std::pair<Node *, ElementInfo>> _element_renders;
};

#endif /* MSBookTextInline_hpp */
