#ifndef MJ_GAME_SHEEP_VS_WOLVES_VM_BOARD_H_
#define MJ_GAME_SHEEP_VS_WOLVES_VM_BOARD_H_

#include "SheepVsWolves_VMInfoModel.h"
#include "ui/UILayout.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class SheepVsWolves_VM_BoardDelegate {
public:
	virtual void onClickedBoardDelegate(cocos2d::Ref * sender) {};
};
class SheepVsWolves_VM_Board :public cocos2d::ui::Layout {
public:
	CREATE_FUNC(SheepVsWolves_VM_Board);
	static SheepVsWolves_VM_Board * createBoard(SheepVsWolvesVM::BOARD_INFO info);
	void setInfo(SheepVsWolvesVM::BOARD_INFO info);
	SheepVsWolvesVM::BOARD_INFO getInfo() const;
	void setDelegate(SheepVsWolves_VM_BoardDelegate * delegate);
	void setClickedBoardEnabled(bool enabled);
protected:
	cocos2d::Size board_size;
	SheepVsWolves_VM_BoardDelegate * _delegate{ nullptr };
private:
	void didLoadFromCSB(SheepVsWolvesVM::BOARD_INFO info);
	
	SheepVsWolvesVM::BOARD_INFO m_board_info;
	cocos2d::ui::Layout * m_layout_root{ nullptr };
	cocos2d::ui::Layout * m_layout_touch{ nullptr };
	cocos2d::ui::ImageView * m_img_board1{ nullptr };
	cocos2d::ui::ImageView * m_img_board2{ nullptr };

	cocos2d::ui::ImageView * m_img_{ nullptr };
	std::string font_name = "fonts/danhvan.ttf";
};

#endif // !MJ_GAME_SHEEP_VS_WOLVES_BOARD_H_
