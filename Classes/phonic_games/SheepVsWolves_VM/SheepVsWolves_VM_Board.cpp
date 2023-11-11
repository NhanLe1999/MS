#include "SheepVsWolves_VM_Board.h"

#define IMG_BOARD_1 "games/SheepVsWolves_VM/board_1.png"
#define IMG_BOARD_2 "games/SheepVsWolves_VM/board_2.png"
SheepVsWolves_VM_Board * SheepVsWolves_VM_Board::createBoard(SheepVsWolvesVM::BOARD_INFO info)
{
	auto board = new SheepVsWolves_VM_Board();
	if (board && board->init()) {
		board->didLoadFromCSB(info);
		board->autorelease();
		return board;
	}
	CC_SAFE_DELETE(board);
	return nullptr;
}

void SheepVsWolves_VM_Board::setInfo(SheepVsWolvesVM::BOARD_INFO info)
{
	m_board_info = info;
	m_img_->loadTexture(info.image_path);
}

SheepVsWolvesVM::BOARD_INFO SheepVsWolves_VM_Board::getInfo() const
{
	return m_board_info;
}

void SheepVsWolves_VM_Board::setDelegate(SheepVsWolves_VM_BoardDelegate * delegate)
{
	_delegate = delegate;
}

void SheepVsWolves_VM_Board::setClickedBoardEnabled(bool enabled)
{
	m_layout_touch->setTouchEnabled(enabled);
}

void SheepVsWolves_VM_Board::didLoadFromCSB(SheepVsWolvesVM::BOARD_INFO info)
{
	m_board_info = info;
	board_size = cocos2d::Size(550, 550);
	
	this->setContentSize(board_size);
	this->setAnchorPoint(cocos2d::Vec2(0.5,0.3));

	m_layout_root = cocos2d::ui::Layout::create();
	m_layout_root->setContentSize(board_size);
	this->addChild(m_layout_root);

	m_img_board1 = cocos2d::ui::ImageView::create(IMG_BOARD_1);
	m_img_board1->setPosition(board_size / 2);
	m_layout_root->addChild(m_img_board1, 1);

	m_img_board2 = cocos2d::ui::ImageView::create(IMG_BOARD_2);
	m_img_board2->setPosition(board_size / 2);
	m_layout_root->addChild(m_img_board2, 2);

	m_img_ = cocos2d::ui::ImageView::create(m_board_info.image_path);
	m_img_->setContentSize(cocos2d::Size(225, 225));
	m_img_->ignoreContentAdaptWithSize(false);
	m_img_->setPosition(cocos2d::Vec2(board_size.width / 2, board_size.height * 0.3));
	m_layout_root->addChild(m_img_, 5);

	m_layout_touch = cocos2d::ui::Layout::create();
	m_layout_touch->setContentSize(board_size);
	m_layout_touch->setTouchEnabled(true);
	m_layout_touch->addClickEventListener([=](cocos2d::Ref *sender) {
		if (_delegate) {
			_delegate->onClickedBoardDelegate(this);
		}
	});
	this->addChild(m_layout_touch,100);
}
