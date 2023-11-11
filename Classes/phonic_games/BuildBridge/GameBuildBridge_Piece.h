//
//  GameBuildBridge_Piece.hpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/5/18.
//

#ifndef GameBuildBridge_Piece_hpp
#define GameBuildBridge_Piece_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GAFWrapper;

class GameBuildBridge_Piece : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(GameBuildBridge_Piece);
    static GameBuildBridge_Piece * createPiece(int idx);
    
    void show();
    void onTapRight(std::function<void()> callback = nullptr);
    void onTapWrong();
    
private:
    bool initPiece(int idx);
    
    GAFWrapper * _gaf;
};

#endif /* GameBuildBridge_Piece_hpp */
