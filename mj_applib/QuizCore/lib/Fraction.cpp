//
//  Fraction.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "Fraction.h"
using namespace math;

Fraction* Fraction::createFraction() {
    auto p = new Fraction();
    if (p && p->init()) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool Fraction::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    
    drawer = cocos2d::DrawNode::create();
    this->addChild(drawer);
    
    cocos2d::Size obj_size = cocos2d::Size(100, 100);
    
    auto img_number = math::resource::ResHelper::createImage("");
    number_object = mpool->getObject();
    number_object->setEnable(0, math::common::deny, img_number, nullptr);
    number_object->setObjectSize(obj_size);
    number_object->setComparisionValue("number", 0);
    number_object->setImageVisible(false);
    number_object->showValueTextWithAnimation(false);
    number_object->setValueText("");
    this->addChild(number_object, 2);
    number_object->onShow(0);

    auto img_nume = math::resource::ResHelper::createImage("");
    numerator_object = mpool->getObject();
    numerator_object->setEnable(0, math::common::deny, img_nume, nullptr);
    numerator_object->setObjectSize(obj_size);
    numerator_object->setComparisionValue("number", numerator);
    numerator_object->setImageVisible(false);
    numerator_object->showValueTextWithAnimation(false);
    numerator_object->setValueTextByKey("number");
    this->addChild(numerator_object, 2);
    numerator_object->onShow(0);

    auto img_deno = math::resource::ResHelper::createImage("");
    denominator_object = mpool->getObject();
    denominator_object->setEnable(0, math::common::deny, img_deno, nullptr);
    denominator_object->setObjectSize(obj_size);
    denominator_object->setComparisionValue("number", denominator);
    denominator_object->setImageVisible(false);
    denominator_object->showValueTextWithAnimation(false);
    denominator_object->setValueTextByKey("number");
    this->addChild(denominator_object, 2);
    denominator_object->onShow(0);
    
    setFractionSize(200);
    return true;
}

void Fraction::setShowFractionAnyway(bool show) {
    fraction_anyway = show;
}

void Fraction::setFraction(int nume, int deno, int num) {
    numerator = nume;
    denominator = deno;
    number = num;
    
    number_object->setComparisionValue("number", number);
    numerator_object->setComparisionValue("number", numerator);
    denominator_object->setComparisionValue("number", denominator);
    
    if (numerator !=0 || fraction_anyway) {
        if (number != 0) {
            number_object->setValueTextByKey("number");
        } else {
            number_object->setValueText("");
        }
        numerator_object->setValueTextByKey("number");
        denominator_object->setValueTextByKey("number");
    } else {
        number_object->setValueTextByKey("number");
        numerator_object->setValueTextByKey("");
        denominator_object->setValueTextByKey("");
    }
    
    updateFraction();
}

void Fraction::setFractionSize(int height) {
    setContentSize(cocos2d::Size(height, height));
    
    denominator_object->setObjectSize(cocos2d::Size(height, height)*0.45);
    denominator_object->getLabelValue()->setFontSize(height*0.4);
    
    numerator_object->setObjectSize(cocos2d::Size(height, height)*0.45);
    numerator_object->getLabelValue()->setFontSize(height*0.4);
    
    number_object->setObjectSize(cocos2d::Size(height, height)*0.45);
    number_object->getLabelValue()->setFontSize(height*0.4);
    
    updateFraction();
}

void Fraction::updateFraction() {
    float height = getContentSize().height;
    
    float deno_width = denominator_object->getLabelValue()->getContentSize().width;
    float nume_width = numerator_object->getLabelValue()->getContentSize().width;
    
    float fraction_width = deno_width>nume_width?deno_width:nume_width;
    float numb_width = number_object->getLabelValue()->getContentSize().width;
    float distance = height*0.1;
    if (numb_width==0 || fraction_width==0) {
        distance=0;
    }
    float total_width = fraction_width + numb_width + distance;
    float delta = (height - total_width)/2;

    drawer->clear();

    if (numerator !=0 || fraction_anyway) {
        number_object->setPosition(cocos2d::Vec2(delta + numb_width/2, height*0.5));
        float fraction_x = delta + numb_width + distance + fraction_width/2;
        numerator_object->setPosition(cocos2d::Vec2(fraction_x, height*0.775));
        denominator_object->setPosition(cocos2d::Vec2(fraction_x, height*0.225));
        
        int line_weight = height*0.02;
        line_weight = line_weight>2?line_weight:2;
        drawer->drawSegment(cocos2d::Vec2(delta+numb_width+distance, height*0.5), cocos2d::Vec2(delta+numb_width+distance+fraction_width, height*0.5), line_weight, cocos2d::Color4F(34/255.0, 31/255.0, 127/255.0, 1));
    } else {
        number_object->setPosition(cocos2d::Vec2(delta + numb_width/2, height*0.5));
        float fraction_x = delta + numb_width + distance + fraction_width/2;
        numerator_object->setPosition(cocos2d::Vec2(fraction_x, height*0.775));
        denominator_object->setPosition(cocos2d::Vec2(fraction_x, height*0.225));
    }
    //drawer->drawRect(cocos2d::Vec2::ZERO, cocos2d::Vec2(height, height), cocos2d::Color4F::RED);
}

#pragma mark - Convert
void Fraction::convertToFraction() {
    if (number!=0) {
        numerator += (denominator*number);
        setFraction(numerator, denominator);
    }
}

void Fraction::convertToMixedNumeral() {
    if (numerator>denominator) {
        number+=numerator/denominator;
        numerator = numerator%denominator;
        setFraction(numerator, denominator, number);
    }
}

void Fraction::reduce() {
    convertToFraction();
    int gcd = calculate_gcd(numerator, denominator);
    numerator = numerator / gcd;
    denominator = denominator / gcd;
    setFraction(numerator, denominator);
}

void Fraction::reciprocal() {
    convertToFraction();
    setFraction(denominator, numerator);
}

#pragma mark - tinh toan
void Fraction::add(Fraction *other) {
    int common_deno = calculate_lcm(denominator, other->denominator);
    
    int delta_1 = common_deno / denominator;
    int delta_2 = common_deno / other->denominator;
    setFraction(numerator * delta_1 + delta_2 * other->numerator, common_deno, number + other->number);
}

void Fraction::sub(Fraction *other) {
    int common_deno = calculate_lcm(denominator, other->denominator);
    
    int delta_1 = common_deno / denominator;
    int delta_2 = common_deno / other->denominator;
    setFraction(numerator * delta_1 - delta_2 * other->numerator, common_deno, number + other->number);
}

void Fraction::mul(Fraction *other) {
    convertToFraction();
    other->convertToFraction();

    setFraction(numerator * other->numerator, denominator * other->denominator);
}

void Fraction::div(Fraction *other) {
    convertToFraction();
    other->convertToFraction();
    
    setFraction(numerator * other->denominator, denominator * other->numerator);
}

void Fraction::add(int numb) {
    if (number !=0 ) {
        setFraction(numerator, denominator, number + numb);
    } else {
        setFraction(numerator + numb * denominator, denominator);
    }
}

void Fraction::sub(int numb) {
    if (number !=0 ) {
        setFraction(numerator, denominator, number - numb);
    } else {
        setFraction(numerator - numb * denominator, denominator);
    }
}

void Fraction::mul(int numb) {
    convertToFraction();
    setFraction(numerator * numb, denominator);
}

void Fraction::div(int numb) {
    convertToFraction();
    setFraction(numerator, denominator * numb);
}

#pragma mark - support method
int Fraction::calculate_gcd (int a, int b) {
    int temp;
    int m = std::abs(a);
    int n = std::abs(b);
    
    if (n > m) {
        temp = m;
        m = n;
        n = temp;
    }
    
    while(n != 0) {
        temp = n;
        n = m % n;
        m = temp;
    }
    return m;
}

int Fraction::calculate_lcm (int a, int b) {
    int m = std::abs(a);
    int n = std::abs(b);

    while(m != n) {
        if(m < n) {
            m = m + std::abs(a);
        } else {
            n = n + std::abs(b);
        }
    }
    return m;
}


