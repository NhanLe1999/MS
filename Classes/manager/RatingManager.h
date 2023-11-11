//
//  Created by NguyenHoangThienPhuoc on 24/03/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "BaseSingleton.h"
//#include "PopupBase.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#define LAST_RATING_TIME_KEY		"last_rating_time"
#define RATING_ITEM_COUNT_KEY		"rating_item_count" //github/document [VM va MS thi item dc dinh nghia la 1 bai hoc 1 bai Phonics 1 bai truyen hoac 1 bai doc]
#define LAST_RATING_DEFAULT_TIME	-1

struct RatingProblem
{
	int id;
	std::string name;
};

class RatingManager : public BaseSingleton<RatingManager>
{
public:

	void parseProblems(char *doc);

	void setRatingConditionFromServer(bool i);

	void increaseItemCount();

	std::vector<RatingProblem> getProblems();

	bool hasRatingPopupShow();

	void reset();

	void showRatingPopup();

	void deleteData();

	void pushCleverTapTrackingData(std::string type, bool isSubmit, int prolemID, std::string problemLabel, std::string msg);

private:

	std::vector< RatingProblem> m_viProblem;
	std::vector< RatingProblem> m_enProblem;
    std::vector< RatingProblem> m_thProblem;

	bool m_isRatingPopupShow = false;

	int m_itemCount = 0;

};


