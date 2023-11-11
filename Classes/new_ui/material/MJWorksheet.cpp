////NOTE!!!!
//	Khi bam vao Item trong list view cua Popular Search thi se 
//select truyen hay Input tu khoa vao textfield?
//	Vector chua selected stories se chua gi? Khi chon item tu man
//hinh da search (gom mot vai truyen) roi reloadData gom cac truyen
//khac thi lam the nao de luu trang thai (co dau tick va layout den)
//cac truyen da chon? (duyet????)
//	Gradebox

#include "MJWorksheet.h"
#include "MSComplexCollection.h"
#include "MSPopularSearchItem.h"
#include "MMFirebaseDatabase.h"
#include "APDatabase.h"
#include "MSGradeManager.h"
#include "APProfileManager.h"
#include "MSTabMaterial.h"
#include "PopularSearchTemplate.h"
#include "MJWorksheetNoResult.h"
#include "MJHelper.h"
#include "MSLessonManager.h"
#include "CleverTapManager.h"
#include "MaterialPopup.h"

using namespace mm;
USING_NS_CC;

INITIALIZE_READER(MJWorksheet);

std::once_flag mj_worksheet_reader;

#pragma mark - Initialize
MJWorksheet* MJWorksheet::createView(mj::model::MaterialInfo::Type itemType) {
	std::call_once(mj_worksheet_reader, [] {
        REGISTER_CSB_READER(MJWorksheet);
    });
    
	auto node = reinterpret_cast<MJWorksheet*>(CSLoader::createNode("csb/new_ui/material/MaterialWorksheetSearchView.csb"));
	if (node) {
		node->didLoadFromCSB(itemType);
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}

void MJWorksheet::didLoadFromCSB(mj::model::MaterialInfo::Type itemType) {
    mj::helper::SetFpsByNumberFrames(8);
    this->setName(msmat_name_page_worksheet);
    
	_itemType = itemType;
    _data_sent = this->getWorksheetItemSent();
    
    // button email
    _button_sendemail = cocos2d::utils::findChild<ui::Button *>(this, "btn_sendemail");
    _button_sendemail->setPosition(Point(this->getContentSize().width, 0));
    if (auto text_sendemail = cocos2d::utils::findChild<ui::Text *>(_button_sendemail, "text")) {
        text_sendemail->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text_sendemail->setString(MSMAT_GET_TEXT("key.material.button.sendemail"));
        
        // hieunt: căn chỉnh một chút với text "Send email" tiếng Indo
        if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
            text_sendemail->setFontSize(17);
            if (auto icon_sendemail = cocos2d::utils::findChild<ui::ImageView *>(_button_sendemail, "icon")) {
                icon_sendemail->setPosition(icon_sendemail->getPosition() - Point(4, 0));
            }
            if (auto round_number = cocos2d::utils::findChild<ui::ImageView *>(_button_sendemail, "round_number")) {
                round_number->setPosition(round_number->getPosition() + Point(4, 0));
            }
        }
    }
    
    float button_sendemail_extra_w = MJ_PLATFORM_CONFIG.getNotchHeight() / 2;
    _button_sendemail->setContentSize(_button_sendemail->getContentSize() + Size(button_sendemail_extra_w, 0));
    if (auto bg = cocos2d::utils::findChild<ui::ImageView *>(_button_sendemail, "bg")) {
        bg->setContentSize(bg->getContentSize() + Size(button_sendemail_extra_w, 0));
    }
    
    // button clear
    _button_clear = cocos2d::utils::findChild<ui::Button *>(this, "button_clear");
    
    // button back
    if (auto btn_back = cocos2d::utils::findChild(this, "btn_back")) {
        btn_back->setPosition(Point(48, btn_back->getPosition().y));
    }
    
    // round of number
    _round_number = (ui::ImageView *)_button_sendemail->getChildByName("round_number");
    _round_number->setVisible(false);
    
    // panel info
    _panel_info = cocos2d::utils::findChild<ui::ImageView *>(this, "panel_info");
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
        _panel_info->setContentSize(Size(446, _panel_info->getContentSize().height));
    } else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE) {
        _panel_info->setContentSize(Size(433, _panel_info->getContentSize().height));
    }
    
    if (auto btn_info = cocos2d::utils::findChild<ui::Button *>(_panel_info, "btn_info")) {
        btn_info->setContentSize(_panel_info->getContentSize());
    }
    
    auto readed_text = cocos2d::utils::findChild<ui::Text *>(_panel_info, "readed_text");
    auto downloaded_text = cocos2d::utils::findChild<ui::Text *>(_panel_info, "downloaded_text");
    
    if (readed_text) {
        readed_text->setFontName(MSMAT_FONT_FILE_PATH);
        readed_text->setString(MSMAT_GET_TEXT(_itemType == mj::model::MaterialInfo::Type::STORY ? "key.material.story.readed" : "key.material.lesson.learned"));
    }
    
    if (downloaded_text) {
        downloaded_text->setFontName(MSMAT_FONT_FILE_PATH);
        downloaded_text->setString(MSMAT_GET_TEXT("key.material.worksheet.downloaded"));
        
        Point padding = Point::ZERO;
        if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
            padding = Point(46, 0);
        } else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) {
            padding = Point(-8, 0);
        }
        if (auto icon = cocos2d::utils::findChild<ui::ImageView *>(_panel_info, "downloaded_icon")) {
            icon->setPosition(icon->getPosition() + padding);
        }
        downloaded_text->setPosition(downloaded_text->getPosition() + padding);
    }
    
    if (auto mid_line = cocos2d::utils::findChild<ui::Layout *>(_panel_info, "mid_line")) {
        auto downloaded_icon = cocos2d::utils::findChild<ui::ImageView *>(_panel_info, "downloaded_icon");
        const float www222 = downloaded_icon->getPosition().x - downloaded_icon->getContentSize().width/2;
        const float www111 = readed_text->getPosition().x + readed_text->getContentSize().width;
        mid_line->setPosition(Point( (www111 + www222)/2 + 1, mid_line->getPosition().y ));
    }
    
    float panel_info_extra_w = MJ_PLATFORM_CONFIG.getNotchHeight() / 2;
    if (panel_info_extra_w > 0) {
        _panel_info->setContentSize(_panel_info->getContentSize() + Size(panel_info_extra_w, 0));
        if (auto container = cocos2d::utils::findChild(_panel_info, "container")) {
            container->setPosition(container->getPosition() + Point(panel_info_extra_w, 0));
        }
    }
    
    // grade box
    _scope = _itemType == mj::model::MaterialInfo::Type::STORY ? ms::GradeManager::Scope::STORIES : ms::GradeManager::Scope::LESSONS_PHONIC;
    ms::GradeManager::getInstance().setScope(_scope);
    _gradeID = ms::GradeManager::getInstance().getCurrentGradeId();
    _gradeInfos = ms::GradeManager::getInstance().getGradesInfo();
    for (auto grade : _gradeInfos)
    {
        _gradeNames.push_back(ms::GradeManager::getInstance().getNameByLanguage(grade.name, LANGUAGE_MANAGER->getDisplayLangId()));
    }
    
	auto gradebox_layout = utils::findChild<ui::Layout*>(this, "gradebox_placeholder");
    gradebox_layout->setPosition(Point(Director::getInstance()->getVisibleSize().width - 32, gradebox_layout->getPosition().y));
    _gradeBox = MSGradeBox::createBox(true, _gradeID);
	_gradeBox->setDelegate(this);
	_gradeBox->setName("gradebox");
	_gradeBox->setAnchorPoint(gradebox_layout->getAnchorPoint());
	_gradeBox->setPosition(gradebox_layout->getPosition());
	gradebox_layout->getParent()->addChild(_gradeBox, 9999);
	gradebox_layout->removeFromParentAndCleanup(true);
    
    // search textfield
	_searchTextField = utils::findChild<ui::TextField*>(this, "search_textfield");
    _searchTextField->setFontName(MSMAT_FONT_FILE_PATH);
    _searchTextField->setPlaceHolder(MSMAT_GET_TEXT(_itemType == mj::model::MaterialInfo::Type::STORY ? "key.material.placeholder.search.story" : "key.material.placeholder.search.phonics"));
    _searchTextField->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    _searchTextField->setTextColor(Color4B::BLACK);
	_searchTextField->setCursorEnabled(true);
	_searchTextField->setCursorChar('|');
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    registerPressCtrlEventForTextField(_searchTextField, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_searchTextField->addEventListener([=](Ref* sender, ui::TextField::EventType event) {
        switch (event) {
            case ui::TextField::EventType::ATTACH_WITH_IME: {
                if (_searchTextField->getString().empty()) {
                    initPopularSearch();
                    this->changeToPageSearch();
                }
            }
                break;
                
            case ui::TextField::EventType::INSERT_TEXT:
            case ui::TextField::EventType::DELETE_BACKWARD: {
                _curSearchType = MJWorksheet::CurrentSearchType::ORGANIC;
                
                /*
                // original search text
                auto originalInput = _searchTextField->getString();
                // left trim
                _searchTextField->setString(mj::helper::str_ltrim(originalInput));
                // cut 1 whitespace on the right if contain 2 whitespaces
                if (originalInput.length() > 1 && originalInput.substr(originalInput.length()-2, 2).compare("  ") == 0) {
                    _searchTextField->setString(originalInput.substr(0, originalInput.length()-1));
                }
                 */
                
                auto input = _searchTextField->getString();
                
                if (input.empty()) {
                    this->changeToPageSearch();
                } else {
                    this->changeToPageSearchWithKeyword();
                }
            }
                break;
                
            case ui::TextField::EventType::DETACH_WITH_IME: {
                auto input = _searchTextField->getString();
                if (input.length() && _data.size()) {
                    _popularSearchTemplate->setRecentSearches(input);
                    this->pushCleverTapEvent_search(MSMAT_MATERIAL_TYPE_STR(_itemType), "organic", input);
                }
            }
                break;
                
            default:
                break;
        }
    });
    
    auto search_placeholder = cocos2d::utils::findChild(this, "collection");
    _searchResult = MSComplexCollection::createCollection(this);
    search_placeholder->addChild(_searchResult);
    
    // data
    this->resetDataRoot();
    this->changeToPageDefault();
    
    // grade changed event
    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e)
    {
        std::string* data = reinterpret_cast<std::string*>(e->getUserData());
        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == _scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MJWorksheet::initPopularSearch() {
    if (_popularSearchTemplate) {
        return;
    }
    
    if (auto collection = utils::findChild(this, "collection")) {
        auto templateType = _itemType == mj::model::MaterialInfo::Type::STORY ? PopularSearchTemplate::Type::Worksheet_Story : PopularSearchTemplate::Type::Worksheet_Phonics;
        _popularSearchTemplate = PopularSearchTemplate::createTemplate(templateType);
        _popularSearchTemplate->ignoreContentAdaptWithSize(false);
        _popularSearchTemplate->setContentSize(collection->getContentSize());
        _popularSearchTemplate->reloadData();
        _popularSearchTemplate->setVisible(false);
        collection->addChild(_popularSearchTemplate);
        
        // TODO: handling when click Recent search items
        _popularSearchTemplate->setRecentItemClickCallback([=](Ref* sender) {
            if (auto button = dynamic_cast<ui::Button *>(sender)) {
                auto keyword = button->getTitleText();
                _searchTextField->setString(keyword);
                _curSearchType = MJWorksheet::CurrentSearchType::RECENT;
                changeToPageSearchWithKeyword();
                this->pushCleverTapEvent_search(MSMAT_MATERIAL_TYPE_STR(_itemType), "recent", keyword);
            }
        });
        
        // TODO: handling when click Popular search items
        _popularSearchTemplate->setPopularItemClickCallback([=](Ref* sender) {
            if (auto popItem = dynamic_cast<MJPopularSearchItem*>(sender)) {
                auto keyword = popItem->getKeyword();
                _searchTextField->setString(keyword);
                _curSearchType = MJWorksheet::CurrentSearchType::POPULAR;
                changeToPageSearchWithKeyword();
                this->pushCleverTapEvent_search(MSMAT_MATERIAL_TYPE_STR(_itemType), "popular", keyword);
            }
        });
    }
}

void MJWorksheet::setContentSize(const Size& size) {
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
    
    if (_searchResult) {
        // because when users exit reading story scene, the scene manager will pop the current scene
        // and THE TOP SCENE ON STACK (HomeScene) will call onEnter function for all children
        // actually, have some codes, we only want to excute once!
        // so we need to use the flag m_isReloadDataCalled
        if (!_isReloadDataCalledFromOnEnterFucntion) {
            _isReloadDataCalledFromOnEnterFucntion = true;
            this->reloadData();
        }
    }
}

void MJWorksheet::resetSelectedWorksheetItems() {
    _selected_items.clear();
    this->resetSelectedWorksheetNumber();
}

void MJWorksheet::resetSelectedWorksheetNumber(float durationForChangeButton) {
    if (_selected_items.size() > 0) {
        // number of Send button
        _round_number->setVisible(true);
        auto text_number = (ui::Text *)_round_number->getChildByName("number");
        if (durationForChangeButton > 0) {
            text_number->setString(std::to_string(_selected_items.size()-1));
            text_number->runAction(Sequence::create(DelayTime::create(durationForChangeButton), CallFunc::create([=](){text_number->setString(std::to_string(_selected_items.size()));}), NULL));
        } else {
            text_number->setString(std::to_string(_selected_items.size()));
        }
        
        // order numbers in items
        for (int i=0; i<_selected_items.size(); i++) {
            auto item = _selected_items.at(i);
            item->setOrderNumber(i+1);
        }
    } else {
        _round_number->setVisible(false);
    }
}

void MJWorksheet::reloadData() {
	_ui_source.clear();
	_ui_source.resize(_data.size(), nullptr);
    
    _searchResult->reloadData();
    ui::Helper::doLayout(this);
    
    // Nếu là Phonics thì nhảy đến học liệu gần nhất chưa được tải
    if (_itemType == mj::model::MaterialInfo::Type::PHONICS) {
        for (int i=0; i<_data.size(); i++) {
            if (!this->isWorksheetItemSent(_data.at(i).mat_id)) {
                int TABLE_SIZE      = this->numberOfCells();
                int NUMB_OF_COLS    = this->numberOfColumns();
                int NUMB_OF_ROWS    = (TABLE_SIZE > NUMB_OF_COLS && TABLE_SIZE % NUMB_OF_COLS > 0 ? 1 : 0) + (TABLE_SIZE / NUMB_OF_COLS);
                int index           = i+1;
                int curRow          = (index > NUMB_OF_COLS && index % NUMB_OF_COLS > 0 ? 1 : 0) + (i / NUMB_OF_COLS);
                float percent       = curRow * (100.f / NUMB_OF_ROWS);
                
                _searchResult->scrollTo(MIN(100.f, percent));
                
                break;
            }
        }
    }
}

void MJWorksheet::changeDisplayLanguage()
{
}

void MJWorksheet::onReceivedEventGradeChanged(int grade_id) {
    _gradeID = grade_id;
    this->resetDataRoot();
    
    if (_page == MJWorksheet::Page::DEFAULT) {
        this->changeToPageDefault();
    } else if (_page == MJWorksheet::Page::SEARCH_KEYWORD) {
        this->changeToPageSearchWithKeyword();
    }
}

void MJWorksheet::hideVirtualKeyboard() {
    if (_searchTextField) {
        _searchTextField->didNotSelectSelf();
    }
}

void MJWorksheet::showPanelNoResult(const std::string& keyword) {
    if (_panel_noResult == nullptr) {
        _panel_noResult = MJWorksheetNoResult::createView(keyword);
        _panel_noResult->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
        _panel_noResult->setContentSize(this->getContentSize());
        ui::Helper::doLayout(_panel_noResult);
        if (auto collection = cocos2d::utils::findChild(this, "collection")) {
            _panel_noResult->setPosition(Point(collection->getContentSize().width/2, 0));
            collection->addChild(_panel_noResult);
        }
    }
    _panel_noResult->setKeyword(keyword);
    _panel_noResult->stopAllActions();
    _panel_noResult->setOpacity(0);
    _panel_noResult->runAction(FadeIn::create(0.25f));
}

void MJWorksheet::hidePanelNoResult() {
    if (_panel_noResult) {
        _panel_noResult->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create([=](){
            _panel_noResult->removeFromParent();
            _panel_noResult = nullptr;
        }), NULL));
    }
}

void MJWorksheet::tickWorksheetItems(const std::string& selected_ids, int grade_id) {
    auto vect_str = mj::helper::split(selected_ids, '-');
    
    /* grade_id < 0: giữ nguyên grade hiện tại
     *          = 0: all grades
     *          > 0: một grade cụ thể nào đó
     */
    if (grade_id == 0) {            /* all grades */
        _gradeBox->forceSelectGrade(-1);
    } else if (grade_id > 0) {      /* grade cụ thể */
        _gradeBox->forceSelectGrade(grade_id);
    }
    
    int TOTAL_CELLS = (int)_data.size();
    int TOTAL_COLS = this->numberOfColumns();
    int TOTAL_ROWS = TOTAL_CELLS % TOTAL_COLS == 0 ? TOTAL_CELLS / TOTAL_COLS : TOTAL_CELLS / TOTAL_COLS + 1;
    float percent_per_row = 100.f / TOTAL_ROWS;
    
    // hiện tại chỉ đánh dấu một học liệu đầu tiên thôi
    // case đánh dấu nhiều học nhiều rất phức tạp và chưa cần thiết
    for (int i=0; i<_data.size(); i++) {
        auto item_info = _data.at(i);
        
        for (auto str_id : vect_str) {
            if (mj::helper::isDigitString(str_id)) {
                int item_id = std::stoi(str_id);
                
                if (item_id == item_info.mat_id) {
                    int row = i / this->numberOfColumns();
                    float percent = row * percent_per_row;
                    _searchResult->scrollTo(percent);
                    
                    // delay một lúc thì _ui_source mới có các item khác NULL
                    this->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([=](){
                        if (auto ws_item = _ui_source[i]) {
                            ws_item->forceClickItem();
                        }
                    }), NULL));
                    
                    return;
//                    break;
                }
            }
        }
    }
}

#pragma mark - CleverTap events
void MJWorksheet::pushCleverTapEvent_notation() {
    try {
        CleverTapManager::GetInstance()->pushEvent("worksheet_notation", {
            {"click_time", cocos2d::Value(_notationCounter)},
        });
    }
    catch (const std::exception&) {
        // nothing here
    }
}

void MJWorksheet::pushCleverTapEvent_screenChooseType(const std::string& type, const std::string& action, int selected_number) {
    try {
        CleverTapManager::GetInstance()->pushEvent("worksheet_screen_choose", {
            {"type", cocos2d::Value(type)},
            {"action", cocos2d::Value(action)},
            {"number_selected", cocos2d::Value(selected_number)},
        });
    } catch (const std::exception&) {
        // nothing here
    }
}

void MJWorksheet::pushCleverTapEvent_search(const std::string& type, const std::string& search_type, const std::string& value) {
    try {
        CleverTapManager::GetInstance()->pushEvent("worksheet_search", {
            {"type", cocos2d::Value(type)},
            {"search_type", cocos2d::Value(search_type)},
            {"value", cocos2d::Value(value)},
        });
    } catch (const std::exception&) {
        // nothing here
    }
}

void MJWorksheet::pushCleverTapEvent_searchEvent() {
    try {
        if (_searchTextField->getString().empty()) {
            return;
        }
        
        std::string searchTypeStr = "organic";
        if (_curSearchType == MJWorksheet::CurrentSearchType::RECENT) {
            searchTypeStr = "recent";
        } else if (_curSearchType == MJWorksheet::CurrentSearchType::POPULAR) {
            searchTypeStr = "popular";
        } else {
            // nothing here
        }
        
        CleverTapManager::GetInstance()->pushEvent("search_event", {
            {"searching_tags",cocos2d::Value(_searchTextField->getString())},
            {"choose_item",cocos2d::Value(true)},
            {"item_id",cocos2d::Value(this->getStringMaterialIds())},
            {"item_name", cocos2d::Value("")},
            {"type",cocos2d::Value(_itemType == mj::model::MaterialInfo::Type::STORY ? "ws_story" : "ws_phonics")},
            {"search_type",cocos2d::Value(searchTypeStr)},
        });
    } catch (const std::exception&) {
    }
}

void MJWorksheet::pushCleverTapEvent_buttonSendEmail(bool is_selected){
    try {
        CleverTapManager::GetInstance()->pushEvent("worksheet_button_send_email", {
            {"is_selected", cocos2d::Value(is_selected ? "yes" : "no")},
        });
    } catch (const std::exception&) {
        // nothing here
    }
}

#pragma mark - Pages
void MJWorksheet::changeToPageDefault() {
    _page = MJWorksheet::Page::DEFAULT;
    
    _searchTextField->setString("");
    _button_clear->setVisible(false);
    _button_sendemail->setVisible(true);
    _panel_info->setVisible(true);
    _searchResult->setVisible(true);
    if (_popularSearchTemplate) {
        _popularSearchTemplate->setVisible(false);
    }
    
    this->hidePanelNoResult();
    this->resetSelectedWorksheetItems();
    this->resetDataMaterials();
    this->reloadData();
}

void MJWorksheet::changeToPageSearch() {
    _page = MJWorksheet::Page::SEARCH_DEFAULT;
    
    _searchTextField->setString("");
    _button_clear->setVisible(false);
    _button_sendemail->setVisible(false);
    _panel_info->setVisible(false);
    _searchResult->setVisible(false);
    
    _data.clear();
    this->reloadData();
    
    if (_popularSearchTemplate) {
        _popularSearchTemplate->reloadData();
        _popularSearchTemplate->setVisible(true);
    }
    
    this->hidePanelNoResult();
}

void MJWorksheet::changeToPageSearchWithKeyword() {
    if (_searchTextField->getString().empty()) {
        this->changeToPageSearch();
        return;
    }
    
    _page = MJWorksheet::Page::SEARCH_KEYWORD;
    
    _button_clear->setVisible(true);
    _button_sendemail->setVisible(true);
    _panel_info->setVisible(true);
    _searchResult->setVisible(true);
    if (_popularSearchTemplate) {
        _popularSearchTemplate->setVisible(false);
    }
    
    this->hidePanelNoResult();
    this->resetSelectedWorksheetItems();
    this->resetDataMaterials(_searchTextField->getString());
    this->reloadData();
    
    if (_data.size() == 0) {
        _button_sendemail->setVisible(false);
        _panel_info->setVisible(false);
        this->showPanelNoResult(_searchTextField->getString());
    }
}

#pragma mark - GET / SET DATA
std::vector<int> MJWorksheet::getWorksheetItemSent() {
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    auto profile_id = ap::ProfileManager::getInstance().getCurrentProfileId();
    auto str_items = PROFILE_MANAGER.getStringForKey(StringUtils::format(key_mat_worksheet_sent_x_y, user_id, profile_id), "");
    if (str_items.length()) {
        std::vector<int> vec_ids;
        auto str_ids = mj::helper::split(str_items, ',');
        for (auto item : str_ids) {
            if (item.length()) {
                vec_ids.push_back(std::stoi(item));
            }
        }
        
        return vec_ids;
    }
    
    return {};
}

void MJWorksheet::pushNewWorksheetItemSent() {
    auto user_id = ap::Database::getInstance().getUserInfo()._id;
    auto profile_id = ap::ProfileManager::getInstance().getCurrentProfileId();
    auto str_items = PROFILE_MANAGER.getStringForKey(StringUtils::format(key_mat_worksheet_sent_x_y, user_id, profile_id), "");
    
    if (str_items.empty()) {
        str_items = str_items + _str_materialIds;
    } else {
        str_items = str_items + "," + _str_materialIds;
    }
    
    PROFILE_MANAGER.setStringForKey(StringUtils::format(key_mat_worksheet_sent_x_y, user_id, profile_id), str_items);
    
    // update firebase database
    mm::RealtimeDatabase::getInstance().updateTrackingWorksheetsSended(user_id, profile_id, str_items);
}

void MJWorksheet::resetDataRoot() {
    if (_gradeID > 0) {
        _data_root = StoryDataModel::getInstance()->getAllMaterialsByTypeAndLevel(_itemType, _gradeID);
    } else {
        _data_root = StoryDataModel::getInstance()->getAllMaterialsByType(_itemType);
    }
}

bool MJWorksheet::isWorksheetItemSent(int mat_id) {
    for (auto vl : _data_sent) {
        if (vl == mat_id) {
            return true;
        }
    }
    
    return false;
}

void MJWorksheet::resetDataMaterials(const std::string& text_search) {
    _data.clear();
    
    if (text_search.empty()) {
        std::vector<mj::model::MaterialInfo> data_items_sent;
        
        for (auto data : _data_root) {
            if (this->isWorksheetItemSent(data.mat_id)) {
                data_items_sent.push_back(data);
            } else {
                _data.push_back(data);
            }
        }
        
        for (auto data : data_items_sent) {
            _data.push_back(data);
        }
    } else {
        std::vector<mj::model::MaterialInfo> data_items_sent;
        std::string text_s = mj::helper::str_standardize_keyword(text_search);
        
        for (auto mat : _data_root) {
            std::string name_worksheet = mj::helper::str_standardize_keyword(mat.name_worksheet);
            std::string name_lesson = mj::helper::str_standardize_keyword(mat.name_lesson);
            
            if (text_s.length() == 1) {
                if (text_s.front() == name_worksheet.front() || text_s.front() == name_lesson.front()) {
                    if (this->isWorksheetItemSent(mat.mat_id)) {
                        data_items_sent.push_back(mat);
                    } else {
                        _data.push_back(mat);
                    }
                }
            } else if ( (name_worksheet.find(text_s) != std::string::npos) || (name_lesson.find(text_s) != std::string::npos) ) {
                if (this->isWorksheetItemSent(mat.mat_id)) {
                    data_items_sent.push_back(mat);
                } else {
                    _data.push_back(mat);
                }
            }
        }
        
        for (auto data : data_items_sent) {
            _data.push_back(data);
        }
    }
    
    // Nếu là Phonics thì sort lại theo danh sách Bài học 1, 2,... giống như ở Lessons
    if (_itemType == mj::model::MaterialInfo::Type::PHONICS) {
        std::vector<int> data_order;
        for (auto data : _data) {
            data_order.push_back(ms::LessonManager::getInstance().getLessonById(data.lesson_id).lesson_order);
        }
        
        if (data_order.size()) {
            for (int i=0; i<data_order.size()-1; i++) {
                for (int j=i+1; j<data_order.size(); j++) {
                    if (data_order[i] > data_order[j]) {
                        std::swap(data_order[i], data_order[j]);
                        std::swap(_data[i], _data[j]);
                    }
                }
            }
        }
    }
}

#pragma mark - Handle with Buttons
cocos2d::ui::Widget::ccWidgetClickCallback MJWorksheet::onLocateClickCallback(const std::string& name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(MJWorksheet, onBack),
		CLICK_MAP(MJWorksheet, onClearText),
		CLICK_MAP(MJWorksheet, onSendEmail),
        CLICK_MAP(MJWorksheet, onClickInfo),
        CLICK_MAP(MJWorksheet, onAttachSearchField),
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}

void MJWorksheet::onAttachSearchField(cocos2d::Ref* sender) {
    _searchTextField->setEnabled(true);
    _searchTextField->attachWithIME();
}

void MJWorksheet::onBack(cocos2d::Ref* sender) {
    //mj::helper::playButtonFX();
    //if (_page == MJWorksheet::Page::SEARCH_DEFAULT) {
    //    this->changeToPageDefault();
    //}
    //else if (_page == MJWorksheet::Page::SEARCH_KEYWORD) {
    //    // Clever Tap event
    //    if (_selected_items.size() && _str_materialIds.compare(this->getStringMaterialIds())) {
    //        this->pushCleverTapEvent_searchEvent();
    //    }
    //    // set recent searches
    //    if (_searchTextField->getString().length() && _data.size()) {
    //        _popularSearchTemplate->setRecentSearches(_searchTextField->getString());
    //    }
    //    // change page
    //    this->changeToPageSearch();
    //}
    //else {
    //    if (_selected_items.size()) {
    //        this->pushCleverTapEvent_screenChooseType(MSMAT_MATERIAL_TYPE_STR(_itemType), "back", (int)_selected_items.size());
    //    }
    //    else if (_clickedCounter_button_sendEmail == 0) {
    //        this->pushCleverTapEvent_screenChooseType(MSMAT_MATERIAL_TYPE_STR(_itemType), "back", 0);
    //    }
    //    this->runAction(Sequence::create(FadeOut::create(0.25f), CallFunc::create([=] {
    //        this->removeFromParent();
    //    }), NULL));
    //}
    mj::helper::playButtonFXClose();
    if (_page == MJWorksheet::Page::SEARCH_DEFAULT) {
        this->changeToPageDefault();
    } else if (_page == MJWorksheet::Page::SEARCH_KEYWORD) {
        // Clever Tap event
        if (_selected_items.size() && _str_materialIds.compare(this->getStringMaterialIds())) {
            this->pushCleverTapEvent_searchEvent();
        }
        // set recent searches
        if (_searchTextField->getString().length() && _data.size()) {
            _popularSearchTemplate->setRecentSearches(_searchTextField->getString());
        }
        // change page
        this->changeToPageSearch();
    } else {
        if (_selected_items.size()) {
            auto backPopup = MaterialPopup::createPopup(MaterialPopup::Type::BACK_WARNING, this, _selected_items.size());//show popup back warning
            backPopup->showpopup(MaterialPopup::Type::BACK_WARNING);
            this->addChild(backPopup);
        } else {
            if (_clickedCounter_button_sendEmail == 0) {
                this->pushCleverTapEvent_screenChooseType(MSMAT_MATERIAL_TYPE_STR(_itemType), "back", 0);
            }
            this->runAction(Sequence::create(FadeOut::create(0.25f), CallFunc::create([=] {
                this->removeFromParent();
            }), NULL));
        }
    }
}

void MJWorksheet::onExit() {
    ui::Layout::onExit();
    mj::helper::SetFpsByNumberFrames(20);
    pushCleverTapEvent_notation();
}

void MJWorksheet::onBackWarningClose1() {
    this->pushCleverTapEvent_screenChooseType(MSMAT_MATERIAL_TYPE_STR(_itemType), "back", (int)_selected_items.size());

    this->runAction(Sequence::create(FadeOut::create(0.25f), CallFunc::create([=] {
        this->removeFromParent();
    }), NULL));
}

void MJWorksheet::onClearText(cocos2d::Ref* sender) {
    auto input = _searchTextField->getString();
    if (input.length() && _data.size()) {
        _popularSearchTemplate->setRecentSearches(input);
    }
    this->changeToPageSearch();
}

void MJWorksheet::onSendEmail(cocos2d::Ref * sender) {
    mj::helper::playButtonFX();
    if (_selected_items.size() > 0) {
        _clickedCounter_button_sendEmail++;
        _str_materialIds = this->getStringMaterialIds();
        
        auto popup = MSMaterialEnterEmail::createPopup(MSMaterialEnterEmail::Type::SEND_MATERIAL, this);
        popup->showPopup();
        Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX-1);
        
        this->pushCleverTapEvent_screenChooseType(MSMAT_MATERIAL_TYPE_STR(_itemType), "send_email", (int)_selected_items.size());
        
        // push event search_event lên CleverTap
        // event này được push khi bấm nút Send Email và học liệu được chọn từ kết quả tìm kiếm
        this->pushCleverTapEvent_searchEvent();
        
        this->pushCleverTapEvent_buttonSendEmail(true);
        
    } else {
        auto popup = MSMaterialEnterEmail::createPopup(MSMaterialEnterEmail::Type::NOTICE, this);
        popup->showNotice(MSMAT_GET_TEXT("key.material.choose.worksheet.first"));
        popup->showPopup();
        Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX-1);
        
        this->pushCleverTapEvent_buttonSendEmail(false);
    }
}

void MJWorksheet::onClickInfo(cocos2d::Ref * sender) {
    mj::helper::playButtonFX();
    _notationCounter++;

    auto popup = MSMaterialEnterEmail::createPopup(MSMaterialEnterEmail::Type::POPUP_NOTE, this);
    
    auto contentNode = utils::findChild<ui::Layout *>(_panel_info, "container")->clone();
    contentNode->setContentSize(_panel_info->getContentSize());
    
    popup->showPopupNote(contentNode);
    popup->showPopup();
    Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX-1);
}

#pragma mark - CollectionDataSource
Size MJWorksheet::collectionSize() {
	return utils::findChild<ui::Layout*>(this, "collection")->getContentSize();
}

Size MJWorksheet::modelSize() {
	return Size(185, 290) * MJ_PLATFORM_CONFIG.getItemScaleFactor();
}

int MJWorksheet::numberOfCells() {
	return (int)_data.size();
}

int MJWorksheet::numberOfColumns() {
	return (int)((collectionSize().width + 10) / (modelSize().width + 10));
}

ui::Widget* MJWorksheet::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) {
	if (_ui_source.empty()) {
		return nullptr;
	}
    
	auto index = row * numberOfColumns() + col;
    
    if (index < 0 || index >= _data.size()) {
        return nullptr;
    }
    
	if (_ui_source[index]) {
        // nothing here
	} else {
        auto info = _data[index];
        auto item = MJWorksheetItem::createItem(info, this, this->isWorksheetItemSent(info.mat_id) ? true : false);
        _ui_source[index] = item;
		_ui_source[index]->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
	}

	return _ui_source[index];
}

void MJWorksheet::offLoadCellAt(int index) {
    // điều kiện skip:
    //      - nếu index bị out of range
    //      - hoặc số lượng item không nhiều, 50 trở xuống chẳng hạn
    if (this->numberOfCells() <= 50 || index < 0 || index >= _ui_source.size()) {
        return;
    }
    
    MJWorksheetItem * item = _ui_source[index];
    
    if (item && !item->isSelected()) {
        item->removeFromParent();
        _ui_source[index] = nullptr;
    }
}

#pragma mark - MJWorksheetItemDelegate
void MJWorksheet::onWorksheetItemClicked(cocos2d::Ref * ref)
{
    if (auto item = (MJWorksheetItem *)ref) {
        if (item->isSelected()) {
            const float time_effect_moving = 0.55f;
            
            // in MJWorksheet
            _selected_items.push_back(item);
            this->resetSelectedWorksheetNumber(time_effect_moving);
            
            // TODO: hiệu ứng nhảy vào nút Send Email
            ccBezierConfig bezier;
            bezier.controlPoint_1 = item->getChildByName("mask")->convertToWorldSpace(utils::findChild(item, "Image_1")->getPosition());
            auto sendButtonNumber = utils::findChild<ui::ImageView*>(this, "round_number");
            bezier.endPosition = sendButtonNumber->getParent()->convertToWorldSpace(sendButtonNumber->getPosition());
            bezier.controlPoint_2 = Vec2((bezier.controlPoint_1.x + bezier.endPosition.x) / 2, Director::getInstance()->getVisibleSize().height * 1.5);

            auto obj = utils::findChild<ui::ImageView*>(item, "Image_1")->clone();
            utils::findChild<ui::Text*>(obj, "Text_1")->setString(std::to_string(_selected_items.size()));
            this->addChild(obj);
            obj->setPosition(bezier.controlPoint_1);
            obj->setVisible(true);
            obj->runAction(Sequence::create(BezierTo::create(time_effect_moving, bezier),
                                            Spawn::create(FadeOut::create(0.15f), ScaleTo::create(0.15f, 0.f), NULL),
                                            CallFunc::create([=](){obj->removeFromParentAndCleanup(true);}),
                                            NULL));
        } else {
            for (int i=0; i<_selected_items.size(); i++) {
                if (_selected_items.at(i) == item) {
                    _selected_items.erase(_selected_items.begin() + i);
                    break;
                }
            }
            this->resetSelectedWorksheetNumber();
        }
    }
}

std::string MJWorksheet::getGradeNameByGradeId(int gradeId)
{
    for (int i=0; i<_gradeInfos.size(); i++)
    {
        if (_gradeInfos.at(i).grade_id == gradeId)
        {
            return _gradeNames.at(i);
        }
    }
    return "";
}

#pragma mark - MSMaterialEnterEmailDelegate
std::string MJWorksheet::getStringMaterialIds()
{
    std::string string_ids = "";
    
    for (auto matItem : _selected_items)
    {
        if (matItem && matItem->isSelected())
        {
            string_ids += StringUtils::toString(matItem->getInfo().mat_id) + ",";
        }
    }
    
    if (string_ids.length())
    {
        string_ids.resize(string_ids.length() - 1);
    }
    
    return string_ids;
}

void MJWorksheet::onMaterialEmailSent() {
    // set danh sách học liệu đã gửi để lưu lên firebase
    this->pushNewWorksheetItemSent();
    
    // reset button email
    if (auto tab_material = cocos2d::utils::findChild<MSTabMaterial *>(Director::getInstance()->getRunningScene(), msmat_name_tab_material)) {
        tab_material->onMaterialEmailUpdated();
    }
    
    // reset vector items sent
    _data_sent = this->getWorksheetItemSent();
    
    // reset vector selected item and number
    this->resetSelectedWorksheetItems();
    
    // re-load items
    if (_page == MJWorksheet::Page::SEARCH_KEYWORD) {
        this->changeToPageSearchWithKeyword();
    } else {
        this->changeToPageDefault();
    }
}
void MJWorksheet::pushEventNotation() {
    //pushCleverTapEvent_notation();
}