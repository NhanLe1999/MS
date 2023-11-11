#pragma once

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include "manager/RatingManager.h"

USING_NS_CC;

class ComboBox : public cocos2d::Layer
{
public:

	static ComboBox *createComboBox(cocos2d::Size i_size, std::vector<RatingProblem> i_itemList,  std::string i_imageBackgroundPath, std::string i_listViewBackgroundPath, std::string i_triangleImagePath,  cocos2d::ui::Widget::TextureResType i_resType = cocos2d::ui::Widget::TextureResType::LOCAL);

	void setComboBoxPosition(cocos2d::Vec2 i_pos);

	int getCurrentItemID();

private:
	
	bool init(cocos2d::Size i_size, std::vector<RatingProblem> i_itemList, std::string i_imageBackgroundPath, std::string i_listViewBackgroundPath, std::string i_triangleImagePath, cocos2d::ui::Widget::TextureResType i_resType);

	cocos2d::ui::Button* m_currentItem = nullptr;

	cocos2d::ui::ListView* m_itemListView = nullptr;

	void addUTF8TextToComboBox(std::string i_input);

	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
	void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;

	void addWhiteBackground(cocos2d::ui::Button *i_button);
};