#include "RatingManager.h"
#include "StoryLanguageManager.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "rating/RatingPopup.h"
#include "CleverTapManager.h"

USING_NS_CC;

#define ITEM_COUNT			1
#define ITEM_COUNT_RELEASE	5
#define RATING_TEST			0
#define IS_RATING_ENABLE	1

void RatingManager::parseProblems(char *i_json)
{
#if (!IS_RATING_ENABLE)
	return;
#endif
	rapidjson::Document doc;
	doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>(i_json);

	if (doc.HasParseError()) {
		return;
	}

	if (!doc.IsObject()) {
		return;
	}

	auto data = doc.GetObject();

	if (!JSON_SAFE_CHECK(data, "list_app_problem", Object))
	{
		CCLOG("something wrong!");

		return;
	}

	auto listAppProblem = data["list_app_problem"].GetObject();

	m_viProblem.clear();
	m_enProblem.clear();

	if (JSON_SAFE_CHECK(listAppProblem, "vi", Array))
	{
		auto viProblems = listAppProblem["vi"].GetArray();

		for (size_t i = 0; i < viProblems.Size(); i++)
		{
			auto viObj = viProblems[i].GetObject();

			m_viProblem.push_back({
				viObj["ticket_group_id"].GetInt(),
				viObj["name"].GetString(),
				});
		}
	}

	if (JSON_SAFE_CHECK(listAppProblem, "en", Array))
	{
		auto enProblems = listAppProblem["en"].GetArray();

		for (size_t i = 0; i < enProblems.Size(); i++)
		{
			auto enObj = enProblems[i].GetObject();

			m_enProblem.push_back({
				enObj["ticket_group_id"].GetInt(),
				enObj["name"].GetString(),
				});
		}
	}
    
    if (JSON_SAFE_CHECK(listAppProblem, "th", Array))
    {
        auto thProblems = listAppProblem["th"].GetArray();

        for (size_t i = 0; i < thProblems.Size(); i++)
        {
            auto obj = thProblems[i].GetObject();

            m_thProblem.push_back({
                obj["ticket_group_id"].GetInt(),
                obj["name"].GetString(),
                });
        }
    }
}

void RatingManager::setRatingConditionFromServer(bool i_ratingCondition)
{
	//CCLOG("i_ratingCondition %s", i_ratingCondition ?"true":"false");

	m_isRatingPopupShow =  i_ratingCondition;
}

void RatingManager::increaseItemCount()
{
	MJDEFAULT->setIntegerForKey(RATING_ITEM_COUNT_KEY, MJDEFAULT->getIntegerForKey(RATING_ITEM_COUNT_KEY) + 1);
}

std::vector<RatingProblem> RatingManager::getProblems()
{
	return (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? m_thProblem:((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE) ? m_viProblem : m_enProblem);
}

bool RatingManager::hasRatingPopupShow()
{
	//LAST_RATING_TIME_KEY
	auto lastRatingTime = MJDEFAULT->getIntegerForKey(LAST_RATING_TIME_KEY, LAST_RATING_DEFAULT_TIME);

	auto itemCount = MJDEFAULT->getIntegerForKey(RATING_ITEM_COUNT_KEY);

	auto currentTime = mj::helper::getTime() / 60;

#if RATING_TEST
	currentTime = mj::helper::getTime() / 60;//minute
#else
	currentTime = mj::helper::getTime() / 3600;//hour
#endif 

	CCLOG("m_isRatingPopupShow %s, itemCount %d,lastRatingTime %d ,currentTime %d", m_isRatingPopupShow==true?"true":"false",itemCount, lastRatingTime, currentTime);

#if RATING_TEST
	if (m_isRatingPopupShow && MJDEFAULT->getIntegerForKey(RATING_ITEM_COUNT_KEY) >= ITEM_COUNT && (lastRatingTime == LAST_RATING_DEFAULT_TIME || (currentTime - lastRatingTime) >= 1))
#else
	// server = true  && nghe/hoc/doc.. 5 item && ( neu chua show || lan truoc da show roi den thoi diem nay phai >= 10 ngay )
	if (m_isRatingPopupShow
        && MJDEFAULT->getIntegerForKey(RATING_ITEM_COUNT_KEY) >= ITEM_COUNT_RELEASE
        && (lastRatingTime == LAST_RATING_DEFAULT_TIME || (currentTime - lastRatingTime) >= 10 * 24))
#endif 
	{
		return true;
	}

	return false;
}

void RatingManager::reset()
{
	MJDEFAULT->setIntegerForKey(RATING_ITEM_COUNT_KEY, 0);

#if RATING_TEST
	MJDEFAULT->setIntegerForKey(LAST_RATING_TIME_KEY, mj::helper::getTime() / 60);//minute
#else
	MJDEFAULT->setIntegerForKey(LAST_RATING_TIME_KEY, mj::helper::getTime() / 3600);//hour
#endif

}
 
void RatingManager::showRatingPopup()
{
#if (!IS_RATING_ENABLE)
	return;
#endif
	
	increaseItemCount();

	if (hasRatingPopupShow())
	{
		if (auto ratingpopup = RatingPopup::createView())
		{
			Director::getInstance()->getRunningScene()->addChild(ratingpopup, INT_MAX - 1);
		}
	}
}

void RatingManager::deleteData()
{
	MJDEFAULT->deleteValueForKey(RATING_ITEM_COUNT_KEY);
	MJDEFAULT->deleteValueForKey(LAST_RATING_TIME_KEY);
}

void RatingManager::pushCleverTapTrackingData(std::string type, bool isSubmit, int prolemID, std::string problemLabel, std::string msg)
{
	try
	{
		CleverTapManager::GetInstance()->pushEvent("rating", {
				{"type", cocos2d::Value(type) },
				{"is_submit", cocos2d::Value(isSubmit) },
				{"problem_id", cocos2d::Value(prolemID)},
				{"problem_label", cocos2d::Value(problemLabel)},
				{"msg", cocos2d::Value(msg)},
			});
	}
	catch (const std::exception&)
	{

	}
}
