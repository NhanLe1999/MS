#include "MJWorksheetItem.h"
#include "RemoteImageView.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MSLessonManager.h"
#include "MSTabMaterial.h"

INITIALIZE_READER(MJWorksheetItem);

std::once_flag mj_worksheet_item_reader;

#pragma mark - Initialize
MJWorksheetItem* MJWorksheetItem::createItem(const mj::model::MaterialInfo& info, MJWorksheetItemDelegate * delegate, bool isSent)
{
	std::call_once(mj_worksheet_item_reader, []
    {
        REGISTER_CSB_READER(MJWorksheetItem);
    });
	
	auto item = reinterpret_cast<MJWorksheetItem*>(CSLoader::createNode("csb/new_ui/material/WorksheetSearch_Item.csb"));
	if (item)
    {
		item->didLoadFromCSB(info, delegate, isSent);
		return item;
	}

	CC_SAFE_DELETE(item);
	return nullptr;
}

void MJWorksheetItem::didLoadFromCSB(const mj::model::MaterialInfo& info, MJWorksheetItemDelegate * delegate, bool isSent)
{
	_info = info;
    _delegate = delegate;
    
    _maskThumb = cocos2d::utils::findChild<ui::Layout *>(this, "mask");
    _icon_downloaded = cocos2d::utils::findChild<ui::ImageView *>(this, "icon_downloaded");
    _icon_readed = cocos2d::utils::findChild<ui::ImageView *>(this, "icon_check");
    _textLevel = cocos2d::utils::findChild<ui::Text *>(this, "text_grade");
    
    // mask
    if (auto img_mask = _maskThumb->getChildByName("image")) {img_mask->setOpacity(128);}
    
    // icon readed
    this->setReaded(info.is_readed);
    
    // icon downloaded
    this->setSent(isSent);
	
    // thumbnail
    auto thumb_placeholder = cocos2d::utils::findChild(this, "thumb_placeholder");
    if (auto thumb_text = (ui::Text *)thumb_placeholder->getChildByName("placeholder"))
    {
        thumb_text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        thumb_text->setString(_info.name_worksheet);
    }
	auto remoteView = RemoteImageView::createImage(_info.thumb, [=](RemoteImageView* img, bool is_success)
    {
		if (is_success)
        {
            img->setContentSize(thumb_placeholder->getContentSize());
			img->setOpacity(0);
            img->runAction(FadeTo::create(0.25f, (_info.type == mj::model::MaterialInfo::Type::PHONICS && _isSent) ? 85 : 255));
            
            if (auto thumb_text = (ui::Text *)thumb_placeholder->getChildByName("placeholder"))
            {
                thumb_text->setVisible(false);
            }
            
            _thumbnail = img;
		}
    }, RemoteImageView::Type::OTHER);
    remoteView->setPosition(thumb_placeholder->getContentSize() / 2);
    thumb_placeholder->addChild(remoteView, 999);
    _icon_downloaded->setLocalZOrder(remoteView->getLocalZOrder() + 1);
    
    // level/category
    _textLevel->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
    if (_info.type == mj::model::MaterialInfo::Type::STORY && _info.levels.size())
    {
        auto level_info = StoryDataModel::getInstance()->getLevelInfoById(_info.levels[0], StoryLanguageManager::getCurrentLangId());
        _textLevel->setString(level_info.name);
    }
    else if (_info.type == mj::model::MaterialInfo::Type::PHONICS && _info.categories.size())
    {
//        if (_delegate)
//        {
//            _textLevel->setString(_delegate->getGradeNameByGradeId(_info.categories[0]));
//        }
        this->setWorksheetName(MSMAT_GET_TEXT("lesson.item.title") + StringUtils::format(" %d", ms::LessonManager::getInstance().getLessonById(_info.lesson_id).lesson_order));
    }
    else
    {
        _textLevel->setString("");
    }
}

void MJWorksheetItem::setWorksheetName(const std::string& name)
{
    _textLevel->setString(name);
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE)
    {
        _textLevel->setFontSize(15);
    }
    else
    {
        _textLevel->setFontSize(18);
    }
}

void MJWorksheetItem::setSelected(bool selected)
{
    _isSelected = selected;
    _maskThumb->setVisible(_isSelected);
}

void MJWorksheetItem::setSent(bool sent)
{
    _isSent = sent;
    _icon_downloaded->setVisible(_isSent);
    
    if (_isSent && _info.type == mj::model::MaterialInfo::Type::PHONICS) {
        _textLevel->setColor(Color3B(128, 128, 128));
        
        if (auto background = utils::findChild<ui::ImageView *>(this, "background")) {
            background->loadTexture("mjstory/material/mat_bg_item_2.png");
        }
        
        if (auto top = utils::findChild<ui::ImageView *>(this, "top")) {
            top->loadTexture("mjstory/material/mat_top_item_2.png");
        }
    }
}

void MJWorksheetItem::setReaded(bool readed)
{
    _isReaded = readed;
    _icon_readed->setVisible(_isReaded);
}

void MJWorksheetItem::setOrderNumber(int numb)
{
    if (_maskThumb->isVisible())
    {
        if (auto text = utils::findChild<ui::Text*>(_maskThumb,"Text_1"))
        {
            text->setString(std::to_string(numb));
        }
    }
}

#pragma mark - Handle with Buttons
ui::Widget::ccWidgetClickCallback MJWorksheetItem::onLocateClickCallback(const std::string& callback_name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJWorksheetItem, onClickItem),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MJWorksheetItem::onClickItem(cocos2d::Ref * sender)
{
    if (_delegate)
    {
        if (_isSelected)
        {
            this->setSelected(false);
            _delegate->onWorksheetItemClicked(this);
            
            if (_thumbnail) {
                _thumbnail->setOpacity((_info.type == mj::model::MaterialInfo::Type::PHONICS && _isSent) ? 85 : 255);
            }
        }
        else
        {
            if (_delegate->getSelectedWorksheetNumber() < MSMAT_MAX_NUMBER_OF_MATERIALS)
            {
                this->setSelected(true);
                _delegate->onWorksheetItemClicked(this);
            }
            else
            {
                auto popup = MSMaterialEnterEmail::createPopup(MSMaterialEnterEmail::Type::NOTICE, nullptr);
                popup->showNotice(StringUtils::format(MSMAT_GET_TEXT("key.material.max.worksheet.canbe.selected").c_str(), MSMAT_MAX_NUMBER_OF_MATERIALS));
                popup->showPopup();
                Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX-1);
            }
            
            if (_info.type == mj::model::MaterialInfo::Type::PHONICS) {
                if (_thumbnail) {
                    _thumbnail->setOpacity(255);
                }
            }
        }
    }
}
