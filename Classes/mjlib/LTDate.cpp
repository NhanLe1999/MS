//
//  LTDate.cpp
//  Spring_Ninja
//
//  Created by To Ngoc Linh on 3/28/15.
//
//

#include "LTDate.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <assert.h>
#include <vector>

LTDate LTDate::getCurrentDate() {
    time_t t = time(NULL);
    auto tm = localtime(&t);
    
    return LTDate(*tm);
}

LTDate::LTDate() {
    
}

LTDate::LTDate(const tm &t) {
    this->hour = t.tm_hour;
    this->minute = t.tm_min;
    this->second = t.tm_sec;
    this->year = t.tm_year;
    this->month = t.tm_mon;
    this->day = t.tm_mday;
    this->weekday = t.tm_wday;
}

LTDate::LTDate(const int time_int){
    time_t abc = time_t(time_int);
    struct tm * timeinfo;
    timeinfo = localtime(&abc);
    this->hour = timeinfo->tm_hour;
    this->minute = timeinfo->tm_min;
    this->second = timeinfo->tm_sec;
    this->year = timeinfo->tm_year;
    this->month = timeinfo->tm_mon;
    this->day = timeinfo->tm_mday;
    this->weekday = timeinfo->tm_wday;
}

LTDate::LTDate(const std::string &s) {
    std::stringstream ss(s);
    ss >> year >> month >> day >> hour >> minute >> second;
}

const LTDate LTDate::operator+(const int &sec) const {
    //assert(sec >= 0);
    tm t = toTimeStruct();
    t.tm_sec += sec;
    time_t res = mktime(&t);
    t = * localtime(&res);
    return LTDate(t);
}

const LTDate LTDate::operator-(const int &sec) const {
    assert(sec < 0);
    tm t = toTimeStruct();
    t.tm_sec += sec;
    time_t res = mktime(&t);
    t = * localtime(&res);
    return LTDate(t);
}

const int LTDate::operator-(const LTDate &other) const {
    auto time_1 = this->toTimeStruct();
    auto time_2 = other.toTimeStruct();
    auto d = difftime(mktime(&time_1), mktime(&time_2));
    return (int) d;
}

std::string LTDate::toString() {
    std::stringstream ss;
    ss << year << ' ' << month << ' ' << day << ' ' << hour << ' ' << minute << ' ' << second;
    return ss.str();
}

int LTDate::toInt(){
    auto int_date = year*10000+month*100+day;
    return int_date;
}

std::string LTDate::toPrettyString() {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << day << '/' << month + 1 << '/' << year + 1900;
    return ss.str();
}

std::string LTDate::toPrettyStringGB() {

    //std::array<std::string, 12> gb_month = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    std::vector<std::string> gb_month = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << gb_month[month] << ' ' << day << ", " << year + 1900;
    return ss.str();

}

std::string LTDate::toPrettyFileName() {
    
    std::vector<std::string> gb_month = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << gb_month[month] << day << "_" << year + 1900 << "_" << hour << "." << minute << "." << second;
    return ss.str();
}

tm LTDate::toTimeStruct() const {
    time_t t = time(NULL);
    auto tm = localtime(&t);
    tm->tm_hour = this->hour;
    tm->tm_min = this->minute;
    tm->tm_sec = this->second;
    tm->tm_year = this->year;
    tm->tm_mon = this->month;
    tm->tm_mday = this->day;
    tm->tm_wday = this->weekday;
    return *tm;
}
