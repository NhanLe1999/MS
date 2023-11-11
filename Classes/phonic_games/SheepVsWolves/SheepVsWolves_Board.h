#ifndef MJ_GAME_SHEEP_VS_WOLVES_BOARD_H_
#define MJ_GAME_SHEEP_VS_WOLVES_BOARD_H_

#include "SheepVsWolvesInfoModel.h"
#include "ui/UILayout.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class SheepVsWolves_BoardDelegate {
public:
	virtual void onClickedBoardDelegate(cocos2d::Ref * sender) {};
};
class SheepVsWolves_Board :public cocos2d::ui::Layout {
public:
	CREATE_FUNC(SheepVsWolves_Board);
	static SheepVsWolves_Board * createBoard(sheepvswolves::BOARD_INFO info);
	void setInfo(sheepvswolves::BOARD_INFO info);
	sheepvswolves::BOARD_INFO getInfo() const;
	void setDelegate(SheepVsWolves_BoardDelegate * delegate);
	void setClickedBoardEnabled(bool enabled);
protected:
	cocos2d::Size board_size;
	SheepVsWolves_BoardDelegate * _delegate{ nullptr };
private:
	void didLoadFromCSB(sheepvswolves::BOARD_INFO info);
	
	sheepvswolves::BOARD_INFO m_board_info;
	cocos2d::ui::Layout * m_layout_root{ nullptr };
	cocos2d::ui::Layout * m_layout_touch{ nullptr };
	cocos2d::ui::ImageView * m_img_board1{ nullptr };
	cocos2d::ui::ImageView * m_img_board2{ nullptr };

	cocos2d::ui::ImageView * m_img_{ nullptr };
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
};

#endif // !MJ_GAME_SHEEP_VS_WOLVES_BOARD_H_
