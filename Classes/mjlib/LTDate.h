//
//  LTDate.h
//  Spring_Ninja
//
//  Created by To Ngoc Linh on 3/28/15.
//
//

#ifndef __LTDate__
#define __LTDate__

#include <stdio.h>
#include <ctime>

#include <string>
#include <assert.h>

class LTDate {
public:
    static LTDate getCurrentDate();
    LTDate();
    LTDate(const tm &t);
    LTDate(const std::string &s);
    LTDate(const int time_int);
    
    const LTDate operator +(const int &sec) const;
    const LTDate operator -(const int &sec) const;
    const int operator -(const LTDate & other) const;
    
    std::string toString();
    std::string toPrettyString();
    std::string toPrettyFileName();
    std::string toPrettyStringGB();
    int day,hour, minute, second, month, year, weekday;
    tm toTimeStruct() const;
    int toInt();
    
};

#endif /* defined(__LTDate__) */
