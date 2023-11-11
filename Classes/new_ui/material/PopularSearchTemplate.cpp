#include "PopularSearchTemplate.h"
#include "RemoteImageView.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MSTabMaterial.h"

INITIALIZE_READER(PopularSearchTemplate);

std::once_flag popular_search_template_reader;

PopularSearchTemplate* PopularSearchTemplate::createTemplate(PopularSearchTemplate::Type i_type)
{
	std::call_once(popular_search_template_reader, []
    {
        REGISTER_CSB_READER(PopularSearchTemplate);
    });
	
	auto item = reinterpret_cast<PopularSearchTemplate*>(CSLoader::createNode("csb/new_ui/material/PopularSearchTemplate.csb"));
	if (item)
    {
		item->didLoadFromCSB(i_type);
		return item;
	}

	CC_SAFE_DELETE(item);
	return nullptr;
}

void PopularSearchTemplate::didLoadFromCSB(PopularSearchTemplate::Type i_type)
{
	m_type = i_type;

	switch (i_type)
	{
	case PopularSearchTemplate::Type::Story:
		//loadPopularSearch(StoryDataModel::getInstance()->getPopularSearchStories());
		m_recentLocalSavingKey = "ms_story_recent_searches";
		break;
	case PopularSearchTemplate::Type::Audiobook:
		//loadPopularSearch(StoryDataModel::getInstance()->getPopularSearchAudiobooks());
		m_recentLocalSavingKey = "ms_audiobook_recent_searches";
		break;
	case PopularSearchTemplate::Type::Worksheet_Story:
		m_recentLocalSavingKey = "ms_mat_recent_searches_story";
		break;
    case PopularSearchTemplate::Type::Worksheet_Phonics:
        m_recentLocalSavingKey = "ms_mat_recent_searches_phonics";
        break;
	default:
		break;
	}
    
    if (auto text = cocos2d::utils::findChild<ui::Text *>(this, "Recent_searches_text")) {
        text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text->setString(MSMAT_GET_TEXT("key.material.title.recent.search"));
    }
    
    if (auto text = cocos2d::utils::findChild<ui::Text *>(this, "Popular_searches_text")) {
        text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text->setString(MSMAT_GET_TEXT("key.material.title.popular.search"));
    }
}


std::vector<std::string> PopularSearchTemplate::getRecentSearches() {
    std::string str_recent_searches = MJDEFAULT->getStringForKey(m_recentLocalSavingKey, "");
    if (str_recent_searches.length()) {
        return mj::helper::split(str_recent_searches, ',');
    }
    
    return {};
}

void PopularSearchTemplate::setRecentSearches(const std::string& keyword)
{
    if (keyword.empty()) {
        return;
    }

    std::string str_saved_keywords = mj::helper::str_standardize_keyword(keyword);
    std::string str_recent_searches = MJDEFAULT->getStringForKey(m_recentLocalSavingKey, "");

    if (!str_recent_searches.empty()) {
        auto vec = mj::helper::split(str_recent_searches, ',');

        // tìm xem có từ nào trùng thì xoá đi
        for (int i=0; i<vec.size(); ) {
            if (str_saved_keywords.compare(vec.at(i)) == 0) {
                vec.erase(vec.begin() + i);
            } else {
                i++;
            }
        }

        // tạo chuỗi lưu keyword mới
        str_saved_keywords = str_saved_keywords + ",";
        const int k_wordMaxNum = std::min(MSMAT_MAX_RECENT_SEARCHES-1, (int)vec.size());
        for (int i=0; i<k_wordMaxNum; i++) {
            str_saved_keywords = str_saved_keywords + vec.at(i) + ",";
        }
        str_saved_keywords.resize(str_saved_keywords.length() - 1);
    }

    MJDEFAULT->setStringForKey(m_recentLocalSavingKey, str_saved_keywords);
}

void PopularSearchTemplate::reloadData()
{
	ui::Helper::doLayout(this);

	switch (m_type)
	{
	case PopularSearchTemplate::Type::Story:
		loadPopularSearch(StoryDataModel::getInstance()->getPopularSearchStories());
		m_recentLocalSavingKey = "ms_story_recent_searches";
		break;
	case PopularSearchTemplate::Type::Audiobook:
		loadPopularSearch(StoryDataModel::getInstance()->getPopularSearchAudiobooks());
		m_recentLocalSavingKey = "ms_audiobook_recent_searches";
		break;
	case PopularSearchTemplate::Type::Worksheet_Story:
            loadPopularSearch(StoryDataModel::getInstance()->getPopularMaterials(mj::model::MaterialPopularInfo::Type::STORY));
        m_recentLocalSavingKey = "ms_mat_recent_searches_story";
        break;
    case PopularSearchTemplate::Type::Worksheet_Phonics:
            loadPopularSearch(StoryDataModel::getInstance()->getPopularMaterials(mj::model::MaterialPopularInfo::Type::PHONICS));
        m_recentLocalSavingKey = "ms_mat_recent_searches_phonics";
        break;
	default:
		loadPopularSearch(StoryDataModel::getInstance()->getPopularSearchStories());
		m_recentLocalSavingKey = "ms_story_recent_searches";
		break;
	}
}

#pragma mark - Handle with Buttons
ui::Widget::ccWidgetClickCallback PopularSearchTemplate::onLocateClickCallback(const std::string& callback_name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		//CLICK_MAP(PopularSearchTemplate, onPopularItemClicked),
		//CLICK_MAP(PopularSearchTemplate, onRecentItemClicked),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

