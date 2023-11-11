//
//  CalendarBase.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 1/29/18.
//

#include "CalendarBase.h"

namespace math {
    namespace calendar {
        
#pragma mark - DATE
        //Date
        math::calendar::DateModel::DateModel() {
            time_t t = time(NULL);
            auto tm = *localtime(&t);
            this->year = tm.tm_year + 1900;
            this->month = tm.tm_mon;
            this->date = tm.tm_mday;
            this->day_of_week = tm.tm_wday;
        }
        
        math::calendar::DateModel::DateModel(int _date, int _month, int _year) :
        date(_date),
        month(_month),
        year(_year) {
            tm t = toTimeStruct();
            t.tm_mday = _date;
            t.tm_mon = _month;
            t.tm_year = _year % 1900;
            time_t res = mktime(&t);
            t = * localtime(&res);
            
            day_of_week = t.tm_wday;
        }
        
        math::calendar::DateModel::DateModel(int _date, int _month, int _year, int _day_week) :
        date(_date),
        month(_month),
        year(_year),
        day_of_week(_day_week) {
        }
        
        const DateModel DateModel::operator+(const int &day) const {
            //assert(sec >= 0);
            tm t = toTimeStruct();
            t.tm_mday += day;
            time_t res = mktime(&t);
            t = * localtime(&res);
            return DateModel(t.tm_mday, t.tm_mon, t.tm_year + 1900, t.tm_wday);
        }
        
        const DateModel DateModel::operator-(const int &day) const {
            //assert(sec >= 0);
            tm t = toTimeStruct();
            t.tm_mday -= day;
            time_t res = mktime(&t);
            t = * localtime(&res);
            return DateModel(t.tm_mday, t.tm_mon, t.tm_year + 1900, t.tm_wday);
        }
        
        const int DateModel::operator-(const DateModel date) const {
            auto time_1 = this->toTimeStruct();
            auto time_2 = date.toTimeStruct();
            auto d = difftime(mktime(&time_1), mktime(&time_2));
            //Tinh ra ngay
            d = d/86400;
            return (int) d;
        }
        
        tm DateModel::toTimeStruct() const {
            time_t t = time(NULL);
            auto tm = localtime(&t);
            tm->tm_sec = 0;
            tm->tm_min = 0;
            tm->tm_hour = 0;
            tm->tm_mday = this->date;
            tm->tm_mon = this->month;
            tm->tm_year = this->year % 1900;
            return *tm;
        }
        
        
#pragma mark - MONTH
        //Month
        math::calendar::MonthModel::MonthModel() {
            time_t t = time(NULL);
            auto tm = *localtime(&t);
            this->year = tm.tm_year + 1900;
            this->month = tm.tm_mon;
            this->monday_start = false;
            
            switch (month) {
                case 1:
                {
                    if (year % 4 == 0) {
                        number_day = 29;
                    } else {
                        number_day = 28;
                    }
                }
                    break;
                case 3:
                {
                    number_day = 30;
                }
                    break;
                case 5:
                {
                    number_day = 30;
                }
                    break;
                case 8:
                {
                    number_day = 30;
                }
                    break;
                case 10:
                {
                    number_day = 30;
                }
                    break;
                default:
                    number_day = 31;
                    break;
            }
        }
        
        math::calendar::MonthModel::MonthModel(int _month, int _year, bool monday) :
        month(_month),
        year(_year),
        
        monday_start(monday) {
            switch (month) {
                case 1:
                {
                    if (year % 4 == 0) {
                        number_day = 29;
                    } else {
                        number_day = 28;
                    }
                }
                    break;
                case 3:
                {
                    number_day = 30;
                }
                    break;
                case 5:
                {
                    number_day = 30;
                }
                    break;
                case 8:
                {
                    number_day = 30;
                }
                    break;
                case 10:
                {
                    number_day = 30;
                }
                    break;
                default:
                    number_day = 31;
                    break;
            }
        }

        tm MonthModel::toTimeStruct() const {
            time_t t = time(NULL);
            auto tm = localtime(&t);
            tm->tm_sec = 0;
            tm->tm_min = 0;
            tm->tm_hour = 0;
            tm->tm_mday = 0;
            tm->tm_mon = this->month;
            tm->tm_year = this->year % 1900;
            return *tm;
        }

        
#pragma mark - CALENDAR
        //Calendar
		Calendar * Calendar::createCalendar(MonthModel m_month, int theme)
		{
			auto p = new Calendar();
			if (p && p->init(m_month, theme)) {
			    p->autorelease();
			}
			else {
				CC_SAFE_DELETE(p);
			}
			return p;
		}
        

		bool Calendar::init(MonthModel m_month, int theme)
		{
			if (!cocos2d::ui::Layout::init()) {
				return false;
			}
            
            theme = 1;
            this->month = m_month;
			calendar_theme = theme;
            calendar_path =cocos2d::StringUtils::format("%scalendar/theme_%d/", m_theme->getThemePath().c_str(), calendar_theme);

			this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            
            //TODO: Tao cac thanh phan can thiet. Setup giá trị. Chua cần căn chỉnh.
            calendar_size = cocos2d::Size(600, 275 + day_size.height * numberWeeksOfMonth());
            this->setContentSize(calendar_size);
            
            top_bg = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%stop.png", calendar_path.c_str()));
            top_bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
            top_bg->setScale9Enabled(true);
            top_bg->setCapInsets(cocos2d::Rect(1, 1, top_bg->getContentSize().width-2, top_bg->getContentSize().height-2));
            top_bg->setContentSize(cocos2d::Size(600, 225));
            top_bg->setPosition(cocos2d::Vec2(0, this->getContentSize().height));
            this->addChild(top_bg);
            
            bot_bg = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%sbot.png", calendar_path.c_str()));
            bot_bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
            bot_bg->setScale9Enabled(true);
            bot_bg->setCapInsets(cocos2d::Rect(1, 1, bot_bg->getContentSize().width-2, bot_bg->getContentSize().height-2));
            bot_bg->setContentSize(cocos2d::Size(600, 50));
            bot_bg->setPosition(cocos2d::Vec2(0, 0));
            this->addChild(bot_bg);
                                
            mid_bg = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%smid.png", calendar_path.c_str()));
            mid_bg->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
            mid_bg->setPosition(cocos2d::Vec2(0, 50));
            mid_bg->setScale9Enabled(true);
            mid_bg->setCapInsets(cocos2d::Rect(1, 1, mid_bg->getContentSize().width-2, mid_bg->getContentSize().height-2));
            mid_bg->setContentSize(cocos2d::Size(600, day_size.height*numberWeeksOfMonth()));
            this->addChild(mid_bg);
            
            std::string title_txt = cocos2d::StringUtils::format("%s %d", math::func::upperString(getMonthsOfYear(month.getMonth()).front()->getText()).c_str(), month.getYear());
            title = cocos2d::ui::Text::create(title_txt, "calendar_text.ttf", 45);
            title->setTextColor(cocos2d::Color4B::WHITE);
            title->setPosition(cocos2d::Vec2(calendar_size.width/2, calendar_size.height - 125));
            addChild(title, 1);
            
            int index_start = 7 - numberDaysInFirstWeek()-1;
            cocos2d::Vec2 start_positon = cocos2d::Vec2(padding_date.width, calendar_size.height - padding_date.height);
            for (int i = 1; i <= month.getNumberDays(); i++) {
				auto day = mpool->getObject();
                day->setEnable(i, math::common::deny, math::number::getNumber(i), nullptr);
				day->setValueTextByKey("number");
				day->setImageVisible(false);
				day->showValueTextWithAnimation(false);
                day->getLabelValue()->setFontSize(day_size.height * 0.75);
                day->getLabelValue()->setFontName("calendar_number.ttf");
                day->setObjectSize(cocos2d::Size(day_size.height, day_size.height) * 0.95);
                day->getLabelValue()->disableEffect();
                if (getDateOfMonth(i).getDayOfWeek() == 0) {
                    day->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
                }
                day->setPosition(cocos2d::Vec2(start_positon.x + ((i + index_start)%7) * day_size.width, start_positon.y - ((i + index_start)/7)*day_size.height));
                this->addChild(day, 2);
                _numbers.push_back(day);
			}
            
            auto dates = getDatesOfWeek();
            std::sort(dates.begin(), dates.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                return a->getComparisionValue("number") < b->getComparisionValue("number");
            });
            if (month.getMondayStart()) {
                auto sun = dates.front();
                dates.push_back(sun);
                dates.erase(dates.begin());
            }
            for (int i=0; i<7; i++) {
                auto date = mpool->getObject();
                date->setEnable(i, math::common::deny, dates[i], nullptr);
                date->setImageVisible(false);
                date->showValueTextWithAnimation(false);
                date->setValueTextByKey("name_n");
                date->setObjectSize(day_size);
                date->getLabelValue()->setFontSize(day_size.height * 0.55);
                date->getLabelValue()->setFontName("calendar_text.ttf");
                date->getLabelValue()->disableEffect();
                date->getLabelValue()->setTextColor(cocos2d::Color4B::WHITE);
                if (dates[i]->getComparisionValue("number") == 0) {
                    date->getLabelValue()->setTextColor(cocos2d::Color4B::RED);
                }
                date->setPosition(cocos2d::Vec2(padding_title.width + (i%7) * day_size.width, calendar_size.height - padding_title.height));
                this->addChild(date, 4);
                _dates.push_back(date);
            }

            return true;
		}
        
        void Calendar::setCalendarSize(cocos2d::Size size) {
            float scale = size.width / 600.0;                    
            auto cal_size = calendar_size * scale;
            this->setContentSize(cal_size);
            
            auto top_size = top_bg->getContentSize() * scale;
            top_bg->setContentSize(top_size);
            top_bg->setPosition(cocos2d::Vec2(0, cal_size.height));
            
            auto bot_size = bot_bg->getContentSize() * scale;
            bot_bg->setContentSize(bot_size);
            
            auto mid_size = mid_bg->getContentSize() * scale;
            mid_bg->setContentSize(mid_size);
            mid_bg->setPosition(cocos2d::Vec2(0, bot_size.height));
            
            title->setPosition(title->getPosition() * scale);
            title->setFontSize(title->getFontSize()*scale);

            auto m_day_size = day_size * scale;
            for (int i = 0; i < month.getNumberDays(); i++) {
                auto day = _numbers[i];
                day->setObjectSize(cocos2d::Size(m_day_size.height, m_day_size.height) * 0.95);
                day->getLabelValue()->setFontSize(m_day_size.height * 0.75);
                auto position = day->getPosition() * scale;
                day->setPosition(position);
            }
            
            for (int i=0; i<7; i++) {
                auto date = _dates[i];
                date->setObjectSize(m_day_size);
                date->getLabelValue()->setFontSize(m_day_size.height * 0.55);
                auto position = date->getPosition() * scale;
                date->setPosition(position);
            }
        }

        
		void Calendar::show(float delay, math::animation::ObjectShowType show)
		{
            for (Math_Object* number : _numbers) {
                number->onShow(0, math::animation::No_Effect);
            }
            for (Math_Object* date : _dates) {
                date->onShow(0, math::animation::No_Effect);
            }
            
			//TODO: Show calendar
			// Show
			auto position_clock = this->getPosition();
			this->setOpacity(0);

			for (Node* child : getChildren()) {
				child->setOpacity(0);
				child->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
			}

			switch (show) {
			case math::animation::ScaleUp:
			{
				this->setScale(0);
				this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)))));
			}
			break;
			case math::animation::SlideUp:
			{
				this->setPosition(cocos2d::Vec2(position_clock.x, position_clock.y - 100));
				this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
			}
			break;
			case math::animation::SlideDown:
			{
				this->setPosition(cocos2d::Vec2(position_clock.x, position_clock.y + 100));
				this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
			}
			break;
			case math::animation::SlideLeft:
			{
				this->setPosition(cocos2d::Vec2(position_clock.x + 100, position_clock.y));
				this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
			}
			break;
			case math::animation::SlideRight:
			{
				this->setPosition(cocos2d::Vec2(position_clock.x - 100, position_clock.y));
				this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
			}
			break;
			default:
			{
				this->setRotation(180);
				this->setScale(0);
				this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::create(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)), cocos2d::RotateTo::create(0.4, 0), NULL)));
			}
			break;
			}

		}

        void Calendar::hide(float delay, math::animation::ObjectShowType revert_show) {
            //TODO: Hide calendar
			this->stopAllActions();

			for (Node* child : getChildren()) {
				child->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay + 0.25), cocos2d::FadeOut::create(0.25)));
			}

			auto position_clock = this->getPosition();
            switch (revert_show) {
                case math::animation::ScaleUp:
                {
                    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0))), cocos2d::CallFunc::create([=]() {
                        this->removeFromParentAndCleanup(true);
                    }), NULL));
                }
                    break;
                case math::animation::SlideUp:
                {
                    auto position = cocos2d::Vec2(position_clock.x, position_clock.y - 100);
                    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=]() {
                        this->removeFromParentAndCleanup(true);
                    }), NULL));
                }
                    break;
                case math::animation::SlideDown:
                {
                    auto position = cocos2d::Vec2(position_clock.x, position_clock.y + 100);
                    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=]() {
                        this->removeFromParentAndCleanup(true);
                    }), NULL));
                }
                    break;
                case math::animation::SlideLeft:
                {
                    auto position = cocos2d::Vec2(position_clock.x + 100, position_clock.y);
                    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=]() {
                        this->removeFromParentAndCleanup(true);
                    }), NULL));
                }
                    break;
                case math::animation::SlideRight:
                {
                    auto position = cocos2d::Vec2(position_clock.x - 100, position_clock.y);
                    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=]() {
                        this->removeFromParentAndCleanup(true);
                    }), NULL));
                }
                    break;
                default:
                {
                    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::create(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)), cocos2d::RotateTo::create(0.4, 180), NULL), cocos2d::CallFunc::create([=]() {
                        this->removeFromParentAndCleanup(true);
                    }), NULL));
                }
                    break;
            }
        }
        
        DateModel Calendar::getFirstDate() {
            return getDateOfMonth(0);
        }
        
        DateModel Calendar::getDateOfMonth(int date) {
            if (date < 1) {
                date = 1;
            }
            if (date >= month.getNumberDays()) {
                date = month.getNumberDays();
            }
            return DateModel(date, month.getMonth(), month.getYear());
        }
        
        MonthModel Calendar::getMonthData() {
            return month;
        }
        
        int Calendar::numberDaysInFirstWeek() {
            int first_date = getFirstDate().getDayOfWeek();
            int delta = 0;
            if (month.getMondayStart()) {
                delta = (7-first_date)%7 + 1;
            } else {
                delta = 7-first_date;
            }
            return delta;
        }
        
        int Calendar::numberWeeksOfMonth() {
            int number_day = month.getNumberDays()-numberDaysInFirstWeek();
            int number_week = 1;

            while (number_day > 0) {
                number_day -= 7;
                number_week++;
            }
            return number_week;
        }
        
        //Get name
        std::vector<math::resource::Image*> Calendar::getDatesOfWeek(int date) {
            auto all_images = math::resource::ResHelper::getImages("game/dates.json", "", m_theme->getThemePath());
            if (date < 7) {
                std::vector<math::resource::Image*> results;
                for (math::resource::Image* img : all_images) {
                    if (img->getComparisionValue("number") == date) {
                        results.push_back(img);
                    }
                }
                return results;
            } else {
                return all_images;
            }
        }

        std::vector<math::resource::Image*> Calendar::getMonthsOfYear(int month) {
            auto all_images = math::resource::ResHelper::getImages("game/months.json", "", m_theme->getThemePath());
            if (month < 12) {
                std::vector<math::resource::Image*> results;
                for (math::resource::Image* img : all_images) {
                    if (img->getComparisionValue("number") == month) {
                        results.push_back(img);
                    }
                }
                return results;
            } else {
                return all_images;
            }
        }

        std::vector<math::resource::Image*> Calendar::getSeasonsOfYear(int season) {
            auto all_images = math::resource::ResHelper::getImages("game/seasons.json", "", m_theme->getThemePath());
            if (season < 4) {
                std::vector<math::resource::Image*> results;
                for (math::resource::Image* img : all_images) {
                    if (img->getComparisionValue("number") == season) {
                        results.push_back(img);
                    }
                }
                return results;
            } else {
                return all_images;
            }
        }

        void Calendar::setHighlightDateOfWeek(int date, bool highlight)
		{
			if (date < 0 || date >= 7) return;
            
            for (int i=1; i<=month.getNumberDays(); i++) {
                if (getDateOfMonth(i).getDayOfWeek() == date) {
                    setHighlightDateOfMonth(i, highlight);
                }
            }
		}
        
		void Calendar::setHighlightDateOfMonth(int date, bool highlight)
		{
            if (date < 1 || date > month.getNumberDays()) {
                return;
            }
            auto object = _numbers[date-1];
			if (highlight) {
                auto img = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%shightlight.png", calendar_path.c_str()));
                object->setBackground(img, 0);
			} else {
                object->setBackground(nullptr);
			}
		}
    }
}
