#include "ComboBox.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"

#define FONT_SIZE						25.0f
#define FONT_PATH						"fonts/Roboto-Regular.ttf"
#define UNSELECTED_COLOR				cocos2d::Color3B(226, 226, 226)
#define COMBOBOX_STRING_LENGTH_MAX		100
#define ITEM_LEFT_MARGIN				15.0f
#define MARGIN_BETWEEN_EACH_ITEM		15.0f
#define SELECTED_ITEM_SCALE_FACTOR		1.03f
#define PURPLE_COLOR					cocos2d::Color3B(79, 34, 120)
#define MS_COLOR						cocos2d::Color3B(36, 172, 222)
ComboBox * ComboBox::createComboBox(cocos2d::Size i_size, std::vector<RatingProblem> i_itemList, std::string i_imageBackgroundPath, std::string i_listViewBackgroundPath, std::string i_triangleImagePath, cocos2d::ui::Widget::TextureResType i_resType)
{
	ComboBox *ret = new (std::nothrow) ComboBox();

	if (ret && ret->init(i_size, i_itemList, i_imageBackgroundPath, i_listViewBackgroundPath, i_triangleImagePath, i_resType))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

bool ComboBox::init(cocos2d::Size i_size, std::vector<RatingProblem> i_itemList, std::string i_imageBackgroundPath, std::string i_listViewBackgroundPath, std::string i_triangleImagePath, cocos2d::ui::Widget::TextureResType i_resType)
{
	if (!Layer::init() || i_itemList.empty())
	{
		return false;
	}

	auto background = ui::Button::create(i_imageBackgroundPath, i_imageBackgroundPath, i_imageBackgroundPath, i_resType);
	background->setPressedActionEnabled(false);
	background->setName("background");
	background->setScale9Enabled(true);
	
	auto lb = Label::createWithTTF("", LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":FONT_PATH, FONT_SIZE);
	lb->setAnchorPoint(Vec2(0.0f, 0.5f));
	lb->setPositionX(0);
	background->setTitleLabel(lb);
	background->getTitleLabel()->setAlignment(TextHAlignment::LEFT);
	background->getTitleLabel()->setPosition({ ITEM_LEFT_MARGIN,background->getTitleLabel()->getPositionY() });
	background->setContentSize(i_size);
	this->addChild(background);

	addUTF8TextToComboBox(i_itemList.front().name);

	auto triangle = ui::ImageView::create(i_triangleImagePath, i_resType);
	triangle->setPosition({ background->getContentSize().width - triangle->getContentSize().width*0.75f,background->getContentSize().height / 2 });
	triangle->setName("triangle");
	background->addChild(triangle);

	std::vector< ui::Button*> items;

	m_itemListView = ui::ListView::create();
	auto img = ImageView::create(i_listViewBackgroundPath, i_resType);
	img->setName("img");
	float itemListViewHeight = MARGIN_BETWEEN_EACH_ITEM;
	float itemListViewWidth = i_size.width;

	for (int i = 0 ;i< i_itemList.size();i++)
	{
		auto button = ui::Button::create();
		button->setScale9Enabled(true);
		
		auto label = Label::createWithTTF(i_itemList.at(i).name,LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf": FONT_PATH, FONT_SIZE-2);
		label->setTextColor(Color4B::WHITE);
		button->setTitleLabel(label);
		button->setContentSize({ i_size.width,button->getTitleLabel()->getContentSize().height});
		button->setTitleAlignment(cocos2d::TextHAlignment::LEFT);
		button->setTag(i);
		button->getTitleLabel()->setPositionX(button->getTitleLabel()->getContentSize().width/2 + ITEM_LEFT_MARGIN);

		itemListViewHeight += (button->getContentSize().height + MARGIN_BETWEEN_EACH_ITEM);
		itemListViewWidth  = std::max(i_size.width, SELECTED_ITEM_SCALE_FACTOR*(button->getTitleLabel()->getContentSize().width + 2*MARGIN_BETWEEN_EACH_ITEM));
		
		button->addClickEventListener([=](Ref*) {

			//CCLOG("button %d", button->getTag());

			auto items = m_itemListView->getItems();

			//reset
			for (auto it : items)
			{
				if (auto itButton = dynamic_cast<ui::Button*>(it))
				{
					itButton->getTitleLabel()->setScale(1.0f);
					itButton->removeChildByName("whiteBg");
					itButton->setTitleColor(Color3B::WHITE);
				}
			}

			m_currentItem = button;

			addUTF8TextToComboBox(button->getTitleText());

			button->setTitleColor(CONFIG_MANAGER->isVietnameseStories()? PURPLE_COLOR: MS_COLOR);

			addWhiteBackground(button);

			button->getTitleLabel()->setScale(SELECTED_ITEM_SCALE_FACTOR);

			m_itemListView->setVisible(false);
			img->setVisible(false);
			
			triangle->runAction(RotateTo::create(0.25f, 0.0f));

		});

		if (i == 0)
		{
			m_currentItem = button;

			button->setTitleColor(CONFIG_MANAGER->isVietnameseStories() ? PURPLE_COLOR : MS_COLOR);

			addWhiteBackground(button);
		}

		items.push_back(button);
	}

	m_itemListView->ignoreAnchorPointForPosition(false);
	m_itemListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_itemListView->setName("itemListView");
	m_itemListView->setDirection(ui::ScrollView::Direction::VERTICAL);
	//itemListView->setTouchEnabled(true);
	m_itemListView->setScrollBarEnabled(true);
//	itemListView->setTouchEnabled(false);
	m_itemListView->setBounceEnabled(true);
//	itemListView->setBackGroundImage("rating/listview_bg.png", i_resType);
//	itemListView->setBackGroundImageScale9Enabled(true);
	m_itemListView->setVisible(false);
	//itemListView->setGravity(ui::ListView::Gravity::LEFT);
	//itemListView->setBackGroundColor(Color3B::WHITE);
	//itemListView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	m_itemListView->setContentSize({ itemListViewWidth,std::min(200.0f,itemListViewHeight) });
	m_itemListView->setPosition(Vec2(background->getContentSize().width/2, -background->getContentSize().height*0.15f - std::min(200.0f, itemListViewHeight)/2));
	m_itemListView->setItemsMargin(MARGIN_BETWEEN_EACH_ITEM);
	m_itemListView->setInnerContainerSize({ itemListViewWidth,itemListViewHeight });
	//itemListView->getInnerContainer()->setScale(0.9f);
	background->addChild(m_itemListView);

	img->setScale9Enabled(true);
	img->setContentSize({ m_itemListView->getContentSize().width*1.02f,m_itemListView->getContentSize().height*1.1f });
	img->setPosition(m_itemListView->getPosition());
	img->setVisible(false);
	background->addChild(img, m_itemListView->getZOrder()-1);

	for (auto item : items)
	{
		item->setContentSize({ itemListViewWidth ,item->getContentSize().height });
		item->setTitleAlignment(cocos2d::TextHAlignment::LEFT);
		item->getTitleLabel()->setPositionX(item->getTitleLabel()->getContentSize().width / 2 + 1.39f*ITEM_LEFT_MARGIN);
		m_itemListView->pushBackCustomItem(item);
	}

	m_itemListView->jumpToTop();

	background->addClickEventListener([=](Ref* i_sender) 
	{
		background->setEnabled(false);

		triangle->runAction(RotateTo::create(0.25f, -180.0f));

		m_itemListView->setVisible(true);

		img->setVisible(true);

		if (m_currentItem->getTag() == 0 || m_currentItem->getTag() == 1)
		{
			background->runAction(Sequence::create(CallFunc::create([=]
			{
				m_itemListView->jumpToTop();

			}), DelayTime::create(0.3f), CallFunc::create([=]
			{
				background->setEnabled(true);

			}), nullptr));
		}
		else
		{
			background->runAction(Sequence::create(CallFunc::create([=]
			{
				m_itemListView->scrollToItem(m_currentItem->getTag(), Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);

			}), DelayTime::create(0.3f), CallFunc::create([=]
			{
				background->setEnabled(true);

			}), nullptr)); 
		}

	});

	auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(ComboBox::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(ComboBox::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);

	return true;
}

void ComboBox::addUTF8TextToComboBox(std::string i_input)
{
	auto str = StringUtils::StringUTF8(i_input);

	while (str.getString().size() > COMBOBOX_STRING_LENGTH_MAX)
	{
		str.deleteChar(str.length() - 1);
	}

	if (auto background = this->getChildByName<ui::Button*>("background"))
	{
		background->setTitleText(str.getAsCharSequence());

		background->getTitleLabel()->setPosition({ ITEM_LEFT_MARGIN,background->getTitleLabel()->getPositionY() });
	}
}

bool ComboBox::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	return true;
}

void ComboBox::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	m_itemListView->setVisible(false);

	if (auto img = this->getChildByName("background")->getChildByName("img"))
	{
		img->setVisible(false);
	}

	if (auto triangle = this->getChildByName("background")->getChildByName("triangle"))
	{
		triangle->runAction(RotateTo::create(0.25f, 0.0f));
	}


}

void ComboBox::addWhiteBackground(cocos2d::ui::Button * i_button)
{
	if (!i_button)
	{
		return;
	}

	auto whiteBg = LayerColor::create(Color4B::WHITE);
	whiteBg->setName("whiteBg");
	whiteBg->setIgnoreAnchorPointForPosition(false);
	whiteBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	whiteBg->setOpacity(0.99f*255.0f);
	whiteBg->setContentSize(i_button->getContentSize());
	whiteBg->setPosition(i_button->getContentSize() / 2);
	i_button->addChild(whiteBg, -1);
}

void ComboBox::setComboBoxPosition(cocos2d::Vec2 i_pos)
{
	if (auto background = this->getChildByName("background"))
	{
		background->setPosition(i_pos);
	}
}

int ComboBox::getCurrentItemID()
{
	return m_currentItem!=nullptr ? m_currentItem->getTag():-1;
}
