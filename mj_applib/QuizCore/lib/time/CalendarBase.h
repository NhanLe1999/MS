//
//  CalendarBase.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 1/29/18.
//

#ifndef CalendarBase_h
#define CalendarBase_h

#include <stdio.h>
#include "cocos2d.h"
#include "Math_Libs.h"
#include "PoolManager.h"
#include "NumberUtils.h"
#include "Math_Func.h"

#define CC_SYNTHESIZE_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType& get##funName(void) { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }


namespace math {
    namespace calendar {
        class DateModel {
        public:
            explicit DateModel();
            explicit DateModel(int, int, int);
            explicit DateModel(int, int, int, int);

            CC_SYNTHESIZE_REF(int, date, Date);
            CC_SYNTHESIZE_REF(int, month, Month);
            CC_SYNTHESIZE_REF(int, year, Year);
            CC_SYNTHESIZE_READONLY(int, day_of_week, DayOfWeek);
            
            const DateModel operator +(const int &day) const;
            const DateModel operator -(const int &day) const;
            const int operator -(const DateModel date) const;
            
        private:
            tm toTimeStruct() const;
        };
        
        
        class MonthModel {
		public:
            explicit MonthModel();
            explicit MonthModel(int, int, bool);
         
            CC_SYNTHESIZE_REF(int, month, Month);
            CC_SYNTHESIZE_REF(int, year, Year);
            CC_SYNTHESIZE_REF(bool, monday_start, MondayStart);
            CC_SYNTHESIZE_READONLY(int, number_day, NumberDays);
            
        private:
            tm toTimeStruct() const;

        };
        
        
        
        class Calendar :public cocos2d::ui::Layout {
		public:
			Calendar() {};
			~Calendar() {};
			static Calendar* createCalendar(MonthModel month, int theme);
            void setCalendarSize(cocos2d::Size size);
            void show(float delay = 0, math::animation::ObjectShowType show = math::animation::ScaleUp);
            void hide(float delay = 0, math::animation::ObjectShowType revert_show = math::animation::ScaleUp);
            
            /**
             Lấy ngày đàu tiên của tháng để setup lịch.

             @return DateModel first day of month
             */
            DateModel getFirstDate();

            /**
             Lấy một ngày trong tháng.

             @param date ngày trong tháng (int)
             @return DateModel
             */
            DateModel getDateOfMonth(int date);

            MonthModel getMonthData();

            /**
             Lấy danh sách các thứ trong tuần. Sunday=0
             
             @param date số thứ tự ngày cần lấy tên, =7, lấy tât cả.
             @return Danh sách thứ trong tuần
             */
            static std::vector<math::resource::Image*> getDatesOfWeek(int date = 7);
            
            /**
             Lấy danh sách các tháng trong năm. January = 0
             
             @param date số thứ tự tháng cần lấy tên, =12, lấy tât cả.
             @return Danh sách tháng trong năm
             */
            static std::vector<math::resource::Image*> getMonthsOfYear(int month = 12);

            /**
             Lấy danh sách các mùa trong năm. Spring = 0
             
             @param date số thứ tự mùa cần lấy tên, =14, lấy tât cả.
             @return Danh sách tháng trong năm
             */
            static std::vector<math::resource::Image*> getSeasonsOfYear(int season = 4);
            
			/**
				Set màu chữ cho thứ trong tuần.Sunday = 0
				@param date  số thứ tự ngày cần highlight
			*/
			void setHighlightDateOfWeek(int date,bool highlight = true);

			/**
			Set màu chữ cho ngày trong tháng
			@param date  ngày cần highligt
			*/
			void setHighlightDateOfMonth(int date,bool highlight = true);

		private :
			//std::string date_nontation;
			bool init(MonthModel month, int theme);
            
            int numberWeeksOfMonth();
            int numberDaysInFirstWeek();

            MonthModel month;
            
            int calendar_theme;
            std::string calendar_path;
            
            cocos2d::Size calendar_size;
            cocos2d::Size day_size = cocos2d::Size(78, 57);
            cocos2d::Size padding_date = cocos2d::Size(68, 258);
            cocos2d::Size padding_title = cocos2d::Size(68, 186);

			cocos2d::ui::ImageView* top_bg;
            cocos2d::ui::ImageView* bot_bg;
            cocos2d::ui::ImageView* mid_bg;
            cocos2d::ui::Text* title;
            
			std::vector<Math_Object*> _numbers;
			std::vector<Math_Object*> _dates;			
        };
    }
}
#endif /* CalendarBase_h */
