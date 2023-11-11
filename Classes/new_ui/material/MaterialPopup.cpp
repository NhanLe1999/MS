#include "MaterialPopup.h"
#include<string>
#include "MSTabMaterial.h"
USING_NS_CC;
INITIALIZE_READER(MaterialPopup);

std::once_flag material_popup_reader_flag;

MaterialPopup* MaterialPopup::createPopup(Type type, MaterialPopupDelegate* PopupDelegate, int numberMaterialSelected)
{
    std::call_once(material_popup_reader_flag, []
        {
            REGISTER_CSB_READER(MaterialPopup);
        });
    auto p = static_cast<MaterialPopup*>(CSLoader::createNodeWithVisibleSize(getCsbFile(type)));

    if (p)
    {
        p->didLoadFromCSB(type, PopupDelegate, numberMaterialSelected);
        return p;
    }
    return nullptr;
}

void MaterialPopup::didLoadFromCSB(Type type, MaterialPopupDelegate* popupDelegate,int numberMaterialSelected) {
    m_materialPopupDelegate = popupDelegate;
    m_type = type;
    m_numberMaterialSelected = numberMaterialSelected;
    if (type == Type::BACK_WARNING) {
        auto myRichText = cocos2d::ui::RichText::create();
        auto labeMessage = utils::findChild<ui::Text*>(this,"label_message");
        ui::Layout* tmp = ui::Layout::create();
        //myRichText->setPosition(labeMessage->getPosition()/2);
        auto textElement1 = ui::RichElementText::create(1, Color3B::RED, 255, std::to_string(m_numberMaterialSelected)+' ', MSMAT_FONT_FILE_PATH_BOLD, 27);
        auto textElement2 = ui::RichElementText::create(2, Color3B::RED, 255, MSMAT_GET_TEXT("key.material.worksheet.title")+' ', MSMAT_FONT_FILE_PATH_BOLD, 27);
        auto textElement3 = ui::RichElementText::create(3, Color3B::BLACK, 255, MSMAT_GET_TEXT("key.material.worksheet.unsend"), MSMAT_FONT_FILE_PATH_BOLD, 27);
        myRichText->pushBackElement(textElement1);
        myRichText->pushBackElement(textElement2);
        myRichText->pushBackElement(textElement3);
        myRichText->setContentSize(Size(labeMessage->getContentSize().width, 80));
        myRichText->setHorizontalAlignment(cocos2d::ui::RichText::HorizontalAlignment::CENTER);
        myRichText->ignoreContentAdaptWithSize(false);
        tmp->addChild(myRichText);
        tmp->setColor(Color3B::BLUE);
        tmp->setPosition(Vec2(labeMessage->getContentSize().width / 2, labeMessage->getContentSize().height + 10));
        labeMessage->addChild(tmp);
        
        //label
        utils::findChild<ui::Text*>(this, "label_message")->setString(MSMAT_GET_TEXT("key.material.worksheet.exit.warning"));
        utils::findChild<ui::Text*>(this, "label_message")->setFontName(MSMAT_FONT_FILE_PATH);
        //label buttons
        utils::findChild<ui::Text*>(this, "exit_label")->setString(MSMAT_GET_TEXT("key.material.worksheet.exit"));
        utils::findChild<ui::Text*>(this, "exit_label")->setFontName(MSMAT_FONT_FILE_PATH);
        utils::findChild<ui::Text*>(this, "continue_label")->setString(MSMAT_GET_TEXT("key.material.worksheet.continue.select"));
        utils::findChild<ui::Text*>(this, "continue_label")->setFontName(MSMAT_FONT_FILE_PATH);

    }
}

std::string MaterialPopup::getCsbFile(Type type) {
    switch (type)
    {
    case MaterialPopup::BACK_WARNING:
        return "csb/new_ui/material/MaterialBackWarning.csb";
        break;
    default:
        break;
    }
}
void MaterialPopup::showpopup(Type type) {
    if (m_isRunning)
    {
        return;
    }

    m_isRunning = true;

    auto csb_name = mj::helper::getCSBName(getCsbFile(type));
    auto runani = CSLoader::createTimeline(csb_name);
    this->runAction(runani);

    runani->play("show", false);

    runani->setLastFrameCallFunc([=] {
        m_isRunning = false;
        });
}
cocos2d::ui::Widget::ccWidgetClickCallback MaterialPopup::onLocateClickCallback(const std::string& callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MaterialPopup, onBackWarningClose),
        CLICK_MAP(MaterialPopup, onBackWarningContinue),

    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
void MaterialPopup::onBackWarningClose(cocos2d::Ref* ref) {
    mj::helper::playButtonFX();
    CCLOG("close");
    if (m_materialPopupDelegate) m_materialPopupDelegate->onBackWarningClose1();
}
void MaterialPopup::onBackWarningContinue(cocos2d::Ref* ref) {
    mj::helper::playButtonFX();
    CCLOG("Continue");
    this->removeFromParent();
}