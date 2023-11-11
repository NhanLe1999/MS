//
//  Fraction.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/12/17.
//

#ifndef Fraction_h
#define Fraction_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Math_Libs.h"
#include "PoolManager.h"

namespace math {
    class Fraction : public cocos2d::ui::Layout {
    public:
        Fraction(){};
        ~Fraction(){};
        
        static Fraction* createFraction();
        
        int numerator = 1; //tử số
        int denominator = 1; //Mẫu số # 0
        int number = 0; //Số nguyên của hỗn số.
        bool negative = false;
        bool fraction_anyway = false; //Nếu tử số =0, có hiển thị kiểu phân só hay k?
        
        Math_Object* number_object;
        Math_Object* numerator_object;
        Math_Object* denominator_object;
        
        void setFraction(int nume, int deno, int num=0);
        void setShowFractionAnyway(bool show);
        void setFractionSize(int height);
        
        //Chuyển đổi
        void convertToFraction();
        void convertToMixedNumeral();
        
        void reduce();
        void reciprocal();
        
        void add(Fraction* other);
        void sub(Fraction* other);
        void div(Fraction* other);
        void mul(Fraction* other);
        
        void add(int numb);
        void sub(int numb);
        void div(int numb);
        void mul(int numb);
        
    private:
        bool init() override;
        cocos2d::DrawNode* drawer;
        void updateFraction();
        
        int calculate_gcd(int a, int b);
        int calculate_lcm(int a, int b);
        
    };
}
#endif /* Fraction_h */

