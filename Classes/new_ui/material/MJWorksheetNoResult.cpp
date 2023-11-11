//
//  MJWorksheetNoResult.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 12/5/20.
//

#include "MJWorksheetNoResult.h"
#include "MSTabMaterial.h"

INITIALIZE_READER(MJWorksheetNoResult);

std::once_flag mj_worksheet_noresult_reader;

#pragma mark - Initialize
MJWorksheetNoResult * MJWorksheetNoResult::createView(const std::string& keyword, std::string nameGrade)
{
    std::call_once(mj_worksheet_noresult_reader, []
    {
        REGISTER_CSB_READER(MJWorksheetNoResult);
    });
    
    auto item = reinterpret_cast<MJWorksheetNoResult*>(CSLoader::createNode("csb/new_ui/material/WorksheetNoResult.csb"));
    if (item)
    {
        item->didLoadFromCSB(keyword, nameGrade);
        return item;
    }
    
    CC_SAFE_DELETE(item);
    return nullptr;
}

void MJWorksheetNoResult::didLoadFromCSB(const std::string& keyword, std::string nameGrade)
{
    if (nameGrade.length() > 0)
    {
        _hideText = true;
        auto panel_no_result = utils::findChild(this, "panel_no_result");
    }
    if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "text_not_found")) {
        std::string cc = StringUtils::format(MSMAT_GET_TEXT("key.notification.not.found.result.of.stories").c_str(), nameGrade.c_str());
        text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
        text->setFontSize(35);
        text->setString(MSMAT_GET_TEXT(_hideText ? StringUtils::format(MSMAT_GET_TEXT("key.notification.not.found.result.of.stories").c_str(), nameGrade.c_str()) : "key.material.noresult.title"));
    }
    if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "Text_1")) {
        text->setFontName(MSMAT_FONT_FILE_PATH);
        text->setString(MSMAT_GET_TEXT(_hideText ? "key.notification.choose.stories.another.level" : "key.material.noresult.warn1"));
    }
    if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "Text_2")) {
        text->setFontName(MSMAT_FONT_FILE_PATH);
        text->setString(MSMAT_GET_TEXT(_hideText ? "key.notification.update.list.stories" : "key.material.noresult.warn2"));
    }
    
    this->setKeyword(keyword);

    auto sizeRunningScene = Director::getInstance()->getRunningScene()->getContentSize();

    float space =  (Director::getInstance()->getVisibleSize().width/Director::getInstance()->getVisibleSize().height) <= 1.5f ? 30.f : 0.0f;

    float posTextAndStar = 0;
    auto star1 = cocos2d::utils::findChild(this, "star_1");
    auto star2 = cocos2d::utils::findChild(this, "star_2");
    auto text1 = cocos2d::utils::findChild(this, "Text_1");
    auto text2 = cocos2d::utils::findChild(this, "Text_2");

    auto posStar1 = setPositionForStart("star_1", "Text_1");
    posTextAndStar = posTextAndStar + posStar1;

    auto posStar2 = setPositionForStart("star_2", "Text_2");
    posTextAndStar = posTextAndStar + posStar2;

    posTextAndStar = (sizeRunningScene.width - posTextAndStar) / 3;

    star1->setPositionX(posTextAndStar);

    float posStarAndStar2 = star1->getPositionX() +  posTextAndStar + text1->getContentSize().width + star1->getContentSize().width;
    star2->setPositionX(posStarAndStar2);

}

void MJWorksheetNoResult::setKeyword(const std::string& keyword)
{
    if (_hideText)
    {
        float yPos = 0;
        if (auto icon = cocos2d::utils::findChild(this, "icon")) {
            yPos = icon->getPositionY() - icon->getBoundingBox().size.height / 2;
        }

        if (auto star = cocos2d::utils::findChild(this, "star_1")) {
            float yPosStar = star->getPositionY();
            yPos = yPos + yPosStar;
        }

        if (auto textNotFound = cocos2d::utils::findChild<ui::Text*>(this, "text_not_found")) {
            textNotFound->setAnchorPoint(Point(0.5, 0.5));
            textNotFound->setPosition(Vec2(Director::getInstance()->getRunningScene()->getBoundingBox().size.width / 2, yPos / 2));
        }

        if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "text_keyword")) {
            text->setVisible(false);
        }
    }
    else
    {
        if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "text_keyword")) {
            text->setFontName(MSMAT_FONT_FILE_PATH_BOLD);
            text->setString("\"" + keyword + "\"");
        }
    }
}

void MJWorksheetNoResult::setBackgroundColor(const Color3B& color)
{
    if (auto bg = cocos2d::utils::findChild<ui::Layout *>(this, "panel_no_result"))
    {
        bg->setColor(color);
    }
}

//float MJWorksheetNoResult::setPositionForStart(std::string nameStar, std::string textStar)
//{
//    auto sizeRunningScene = Director::getInstance()->getRunningScene()->getContentSize();
//    //set l?i v? trí cho 2 ngôi sao
//    if (auto star = cocos2d::utils::findChild(this, nameStar)) {
//        auto posStar = sizeRunningScene.width / 2;
//        auto sizeText = 0;
//        auto sizeWithStar = star->getContentSize().width;
//
//        if (auto text = cocos2d::utils::findChild<ui::Text*>(this, textStar)) {
//            sizeText = text->getContentSize().width;
//        }
//        return (posStar - sizeText - sizeWithStar) / 2;
//    }
//    return 0;
//}

float MJWorksheetNoResult::setPositionForStart(std::string nameStar, std::string textStar)
{
    if (auto star = cocos2d::utils::findChild(this, nameStar)) {
        auto sizeText = 0;
        auto sizeWithStar = star->getContentSize().width;

        if (auto text = cocos2d::utils::findChild<ui::Text*>(this, textStar)) {
            sizeText = text->getContentSize().width;
        }
        return sizeText + sizeWithStar + 5;
    }
    return 0;
}
