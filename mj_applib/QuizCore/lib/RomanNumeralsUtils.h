//
//  RomanNumeralsUtils.h
//  Monkey_Maths
//
//  Created by HuyPhuong on 11/21/17.
//

#ifndef RomanNumeralsUtils_h
#define RomanNumeralsUtils_h

#include <string>
#include <map>

namespace math {
    
    class RomanNumeralsUtils {
        
    public:
        static std::string int_2_roman (int a) {
            int num=a;
            int decimal[] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
            std::string symbol[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
            int i = 0;
            
            std::string ans;
            while(num){
                while(num/decimal[i]){
                    ans = ans+symbol[i];
                    num -= decimal[i];
                }
                i++;
            }
            return ans;
        }
        
    public:
        static int roman_2_int (std::string s) {
            std::map<char, int> roman;
            roman['M'] = 1000;
            roman['D'] = 500;
            roman['C'] = 100;
            roman['L'] = 50;
            roman['X'] = 10;
            roman['V'] = 5;
            roman['I'] = 1;
            
            int res = 0;
            for (int i = 0; i < s.size() - 1; ++i)
            {
                if (roman[s[i]] < roman[s[i+1]])
                    res -= roman[s[i]];
                else
                    res += roman[s[i]];
            }
            res += roman[s[s.size()-1]];
            return res;
        }
    };
    
}

#endif /* RomanNumeralsUtils_h */