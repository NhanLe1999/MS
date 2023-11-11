//
//  MJ_Snail_Character.h
//

#ifndef Snail_Character_h
#define Snail_Character_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
class Snail_CharacterDelegate {
public:
    virtual void onCharacterPlayDone() {};
};

class Snail_Character : public cocos2d::ui::Layout
{
public:
    
    enum {
        TYPE_BEGIN,
        TYPE_MIDLE,
        TYPE_END
    };
    
    struct ITEM_INFO {
        std::string character;
        std::string audio_path;
    };
    
    CREATE_FUNC(Snail_Character);
    static Snail_Character *createItem(ITEM_INFO item_info, int type, bool resize);
    void setDelegate(Snail_CharacterDelegate *delegate);
    ITEM_INFO item_info;
    void playCharacter();
    void playText(float duration);
	cocos2d::ui::Text* mText;
private:
	
    void didLoadFromCSB(ITEM_INFO item_info, int type, bool resize);
    void onEnter();
    Snail_CharacterDelegate *delegate = nullptr;
};
CREATE_READER(Snail_Character);
#endif /* MJ_Snail_Character_h */
