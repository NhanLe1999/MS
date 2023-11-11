//
//  MSOnboard.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/1/18.
//

#include "MSOnboard.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryLanguageManager.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "StoryConfigManager.h"
#include "MSEventLogger.h"
#include "MSOnboardDelegate.h"

USING_NS_CC;
INITIALIZE_READER(MSOnboard);

std::once_flag ms_onboard_reader;

const std::vector<OnboardInfo> ONBOARD_INFO_VN_VI_STORIES = {
    {
        {},
        "Kho truyện tranh tương tác khổng lồ",
        "Chia theo cấp độ tuổi thuộc 10 chủ đề, được cập nhật hàng tuần. \n"
        "Trải nghiệm đọc độc đáo đa giác quan: nghe, nhìn, chạm giúp trẻ vừa chơi vừa học.",
        Point(0, 0.85),
        Point(160, 690),
        Rect(0, 768 - 120, 150, 120)
    },
    {
        {},
        "Hệ thống bài học chuẩn Mỹ",
        "1000 hoạt động tương tác thú vị giúp trẻ phát triển toàn diện 4 kỹ năng quan trọng: Nghe, Nói, Đọc, Viết",
        Point(0, 0.6),
        Point(160, 590),
        Rect(0, 768 - 240, 150, 120)
    },
    {
        {},
        "Kho sách nói chọn lọc",
        "Giàu tính giáo dục và nhân văn, phù hợp với văn hoá Việt.\nGiọng đọc truyền cảm và hiệu ứng âm thanh phong phú kích thích khả năng tưởng tượng của trẻ.",
        Point(0, 0.3),
        Point(160, 470),
        Rect(0, 768 - 360, 150, 120)
    },
    {
        {},
        "Thay đổi cấp độ học cho trẻ",
        "",
        Point(0, 0.5),
        Point(430, 525),
        Rect(175, 503, 250, 49)
    },
    {
        {},
        "Khám phá thêm nhiều nội dung đặc sắc",
        "",
        Point(0.55, 0),
        Point(587, 190),
        Rect(462, 119, 250, 49)
    },
};


const std::vector<OnboardInfo> ONBOARD_INFO_VN = {
    {
        {},
        "Kho truyện tranh tương tác khổng lồ",
        "Trên 500 truyện thuộc 11 chủ đề được cập nhật hàng tuần. \nTrải nghiệm đọc độc đáo đa giác quan với tính năng tương tác: nghe, nhìn, chạm, chơi mà học",
        Point(0, 0.85),
        Point(160, 690),
        Rect(0, 768 - 120, 150, 120)
    },
    {
        {},
        "Hệ thống bài học chuẩn Mỹ",
        "3000 hoạt động tương tác giúp trẻ hoàn thiện cách phát âm và phát triển toàn diện 4 kỹ năng quan trọng: Nghe, Nói, Đọc, Viết",
        Point(0, 0.6),
        Point(160, 590),
        Rect(0, 768 - 240, 150, 120)
    },
    {
        {},
        "Kho sách nói chọn lọc",
        "Tuyển chọn những câu chuyện và thơ kinh điển trên thế giới.\nGiọng đọc truyền cảm và hiệu ứng âm thanh phong phú kích thích khả năng tưởng tượng của trẻ.",
        Point(0, 0.3),
        Point(160, 470),
        Rect(0, 768 - 360, 150, 120)
    },
    {
        {},
        "Thay đổi cấp độ học cho trẻ",
        "",
        Point(0, 0.5),
        Point(430, 525),
        Rect(175, 503, 250, 49)
    },
    {
        {},
        "Khám phá thêm nhiều nội dung đặc sắc",
        "",
        Point(0.55, 0),
        Point(587, 190),
        Rect(462, 119, 250, 49)
    },
};

const std::vector<OnboardInfo> ONBOARD_INFO = {
    {
        {},
        "Picture stories",
        "500+ levelled e-books with new stories added weekly.\nA unique reading experience with interactive features to inspire your child to explore, read, learn and love",
        Point(0, 0.85),
        Point(160, 690),
        Rect(0, 768 - 120, 150, 120)
    },
    {
        {},
        "Guided Lessons",
        "3000 game-based activities for hundreds of phonics and comprehensive lessons aligned with US Common Core State Standard",
        Point(0, 0.6),
        Point(160, 590),
        Rect(0, 768 - 240, 150, 120)
    },
    {
        {},
        "Audiobooks",
        "Collection of famous tales, fables and stories all over the world.\nBeautiful voice and lively music and sound effects that help to stimulate kids' imagination.",
        Point(0, 0.3),
        Point(160, 470),
        Rect(0, 768 - 360, 150, 120)
    },
    {
        {},
        "Select your grade",
        "",
        Point(0, 0.5),
        Point(430, 525),
        Rect(175, 503, 250, 49)
    },
    {
        {},
        "Explore more content",
        "",
        Point(0.55, 0),
        Point(587, 190),
        Rect(462, 119, 250, 49)
    }
};

MSOnboard * MSOnboard::createSingleBoard(OnboardInfo info) {
    std::call_once(ms_onboard_reader, [] {
        REGISTER_CSB_READER(MSOnboard);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/intro/Onboard.csb");
    auto board = reinterpret_cast<MSOnboard *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (board) {
        board->didLoadFromCSB(info);
    } else {
        CC_SAFE_DELETE(board);
    }
    
    return board;
}

MSOnboard * MSOnboard::createBoard(int step) {
    std::call_once(ms_onboard_reader, [] {
        REGISTER_CSB_READER(MSOnboard);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/intro/Onboard.csb");
    auto board = reinterpret_cast<MSOnboard *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (board) {
        board->didLoadFromCSB(step);
    } else {
        CC_SAFE_DELETE(board);
    }
    
    return board;
}

#define SAFE_FUNCTION(p, f) if (p) { p->f; }

void MSOnboard::didLoadFromCSB(OnboardInfo info)
{
    auto title = utils::findChild<ui::Text *>(this, "title");

    title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

    title->setString(info.title);
    
    auto label_next = utils::findChild<ui::Text *>(this, "label_next");

    label_next->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");

//    label_next->setString(LANGUAGE_MANAGER->getDisplayTextByKey("onboard.next"));
    label_next->setString(LANGUAGE_MANAGER->getDisplayTextByKey("sidebar.des.close"));
    
    auto rich = ui::RichText::create();
    for (auto t : info.texts)
    {
        if (t.text == "##") {
            rich->pushBackElement(ui::RichElementNewLine::create(2, Color3B::BLACK, 255));
            auto p = ui::Layout::create();
            p->setContentSize(Size(10, 10));
            rich->pushBackElement(ui::RichElementCustomNode::create(3, Color3B::BLACK, 255, p));
            rich->pushBackElement(ui::RichElementNewLine::create(2, Color3B::BLACK, 255));
            continue;
        }
        auto font_name = t.is_bold ? "fonts/Montserrat-Bold.ttf" : "fonts/Montserrat-Light.ttf";
        auto text = ui::RichElementText::create(0, Color3B::BLACK, 255, t.text, font_name, t.size);
        rich->pushBackElement(text);
    }
    
    rich->setWrapMode(ui::RichText::WRAP_PER_WORD);
    rich->ignoreContentAdaptWithSize(false);
    rich->setName("content");
    
    auto placeholder = utils::findChild(this, "placeholder");
    
    auto plain = ui::Text::create(info.plain_text, "fonts/Montserrat-Light.ttf", 20);
    plain->setTextAreaSize(Size(placeholder->getContentSize().width, 0));
    plain->setContentSize(plain->getVirtualRendererSize());
    plain->setPosition(placeholder->getPosition());
    plain->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    plain->setTextColor(Color4B::BLACK);
    plain->ignoreContentAdaptWithSize(false);
    placeholder->getParent()->addChild(plain);
    
    auto bg = utils::findChild(this, "background");
    bg->setContentSize(Size(bg->getContentSize().width, plain->getContentSize().height + 240));
    if (title->getString().empty()) {
        bg->setContentSize(Size(bg->getContentSize().width, bg->getContentSize().height - 60));
    }
    plain->setPositionY(160);
    
    ui::Helper::doLayout(bg);
    
    auto arrow_v = utils::findChild(this, "arrow_v");
    arrow_v->setContentSize(Size(bg->getContentSize().height + 20, 50));
    arrow_v->setPositionY((bg->getContentSize().height - 20) / 2);
    
    rich->setPosition(placeholder->getPosition());
    rich->setAnchorPoint(placeholder->getAnchorPoint());
    rich->setContentSize(placeholder->getContentSize());
    placeholder->getParent()->addChild(rich);
    rich->setVisible(false);
    placeholder->removeFromParent();
    
    auto bt = utils::findChild<ui::Button *>(this, "button_next");
    bt->setCascadeColorEnabled(false);
    bt->addClickEventListener([this] (Ref * sender) {
        this->removeFromParent();
    });
    
    this->setHighlightRect(Rect(0, 0, 150, 200));
    
    auto optout = utils::findChild(this, "optout");
    optout->removeFromParent();
    utils::findChild(this, "panel_dot")->removeFromParent();
    
    bg->setAnchorPoint(info.anchor);
    bg->setPosition(info.position);
    
    auto highlight_rect = info.highlight;
    highlight_rect.size.width += MJ_PLATFORM_CONFIG.getNotchHeight();
    setHighlightRect(highlight_rect);
    
    bg->setPosition(bg->getPosition() + Point(MJ_PLATFORM_CONFIG.getNotchHeight(), 0));
    
    auto arrow = utils::findChild(this, "arrow");
    arrow->setPositionY(bg->getContentSize().height * info.anchor.y);
    
    arrow_v->setVisible(bg->getAnchorPoint().y == 0);
    arrow->setVisible(bg->getAnchorPoint().x == 0);
}

void MSOnboard::didLoadFromCSB(int step) {
    _onboard_info = LANGUAGE_MANAGER->getDisplayLangId() == 1 ? ONBOARD_INFO : (CONFIG_MANAGER->isVietnameseStories() ? ONBOARD_INFO_VN_VI_STORIES : ONBOARD_INFO_VN);
    if (CONFIG_MANAGER->isVietnameseStories()) {
        _onboard_info.erase(_onboard_info.begin()+1);
        _onboard_info[1].position = Point(160, 590);
        _onboard_info[1].anchor = Point(0, 0.6);
        _onboard_info[1].highlight = Rect(0, 768 - 240, 150, 120);
        
        auto end_index = (int)_onboard_info.size() -1;
        _onboard_info[end_index].position.y = 165;
        _onboard_info[end_index].highlight = Rect(462, 94, 250, 49);

    }
    _step = step;
    assert(step < (int)_onboard_info.size());
    OnboardInfo info = _onboard_info[step];
    
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setString(info.title);
    
    auto label_next = utils::findChild<ui::Text *>(this, "label_next");
    label_next->setString(LANGUAGE_MANAGER->getDisplayTextByKey("onboard.next"));
    if (step == _onboard_info.size() - 1) {
        label_next->setString(LANGUAGE_MANAGER->getDisplayTextByKey("onboard.finish"));
    }
    
    auto rich = ui::RichText::create();
    for (auto t : info.texts) {
        if (t.text == "##") {
            rich->pushBackElement(ui::RichElementNewLine::create(2, Color3B::BLACK, 255));
            auto p = ui::Layout::create();
            p->setContentSize(Size(10, 10));
            rich->pushBackElement(ui::RichElementCustomNode::create(3, Color3B::BLACK, 255, p));
            rich->pushBackElement(ui::RichElementNewLine::create(2, Color3B::BLACK, 255));
            continue;
        }
        auto font_name = t.is_bold ? "fonts/Montserrat-Bold.ttf" : "fonts/Montserrat-Light.ttf";
        auto text = ui::RichElementText::create(0, Color3B::BLACK, 255, t.text, font_name, t.size);
        rich->pushBackElement(text);
    }
    
    rich->setWrapMode(ui::RichText::WRAP_PER_WORD);
    rich->ignoreContentAdaptWithSize(false);
    rich->setName("content");
    
    auto placeholder = utils::findChild(this, "placeholder");
    
    auto plain = ui::Text::create(info.plain_text, "fonts/Montserrat-Light.ttf", 20);
    plain->setTextAreaSize(Size(placeholder->getContentSize().width, 0));
    plain->setContentSize(plain->getVirtualRendererSize());
    plain->setPosition(placeholder->getPosition());
    plain->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    plain->setTextColor(Color4B::BLACK);
    plain->ignoreContentAdaptWithSize(false);
    placeholder->getParent()->addChild(plain);
    
    auto bg = utils::findChild(this, "background");
    bg->setContentSize(Size(bg->getContentSize().width, plain->getContentSize().height + 240));
    if (title->getString().empty()) {
        bg->setContentSize(Size(bg->getContentSize().width, bg->getContentSize().height - 60));
    }
    plain->setPositionY(160);
    
    ui::Helper::doLayout(bg);
    
    auto arrow_v = utils::findChild(this, "arrow_v");
    arrow_v->setContentSize(Size(bg->getContentSize().height + 20, 50));
    arrow_v->setPositionY((bg->getContentSize().height - 20) / 2);
    
    rich->setPosition(placeholder->getPosition());
    rich->setAnchorPoint(placeholder->getAnchorPoint());
    rich->setContentSize(placeholder->getContentSize());
    placeholder->getParent()->addChild(rich);
    rich->setVisible(false);
    placeholder->removeFromParent();
    
    auto bt = utils::findChild(this, "button_next");
    bt->setCascadeColorEnabled(false);
//    bt->setColor(Color3B(38, 176, 230));
    
    this->setHighlightRect(Rect(0, 0, 150, 200));
    
    auto rich2 = ui::RichText::create();
    
    auto text_1 = ui::RichElementText::create(0, Color3B::BLACK, 255, LANGUAGE_MANAGER->getDisplayTextByKey("onboard.optout.1"), "fonts/Montserrat-Light.ttf", 15);
    auto text_2 = ui::RichElementText::create(0, Color3B(38, 176, 230), 255, LANGUAGE_MANAGER->getDisplayTextByKey("onboard.optout.2"), "fonts/Montserrat-SemiBold.ttf", 15);
    rich2->pushBackElement(text_1);
    rich2->pushBackElement(text_2);
    auto optout = utils::findChild(this, "optout");
    rich2->setPosition(optout->getPosition());
    rich2->setAnchorPoint(optout->getAnchorPoint());
    optout->getParent()->addChild(rich2);
    rich2->setContentSize(optout->getContentSize());
    optout->removeFromParent();
    
    auto counter = 0;
    auto n = utils::findChild(this, "dot_" + StringUtils::toString(counter));
    while (n) {
        if (CONFIG_MANAGER->isVietnameseStories()) {
            if (counter == (int)_onboard_info.size()){
                n->setVisible(false);
            }else {
                n->setPositionX(n->getPositionX()+20);
            }
            
        }
        n->setColor(Color3B(100, 100, 100));
        counter ++;
        n =  utils::findChild(this, "dot_" + StringUtils::toString(counter));
    }
    
    n = utils::findChild(this, "dot_" + StringUtils::toString(step));
    auto color = CONFIG_MANAGER->isVietnameseStories()? Color3B(255, 203, 0) : Color3B(38, 176, 230);
    n ? n->setColor( color) : (void) nullptr;
    
    bg->setAnchorPoint(info.anchor);
    bg->setPosition(info.position);
    
    if (_step == 4 || (_step == 3 && CONFIG_MANAGER->isVietnameseStories())) {
        auto bt = utils::findChild(Director::getInstance()->getRunningScene(), "button_more_fixed");
        Rect bb;
        if (bt) {
            bb = bt->getBoundingBox();
            bb.origin = bt->getParent()->convertToWorldSpace(bb.origin);
        } else {
            bb.origin = Vec2(Director::getInstance()->getVisibleSize().width - 50, 0);
            bb.size = Vec2(100, 40);
        }
        setHighlightRect(bb);
        
        bg->setPosition(Point(bb.getMidX(), bb.getMaxY() + 20));
    } else if (_step == 3) {
        auto highlight_rect = info.highlight;
        highlight_rect.origin.x += MJ_PLATFORM_CONFIG.getNotchHeight();
        setHighlightRect(highlight_rect);
        
        bg->setPosition(bg->getPosition() + Point(MJ_PLATFORM_CONFIG.getNotchHeight(), 0));
    } else {
        auto highlight_rect = info.highlight;
        highlight_rect.size.width += MJ_PLATFORM_CONFIG.getNotchHeight();
        setHighlightRect(highlight_rect);
        
        bg->setPosition(bg->getPosition() + Point(MJ_PLATFORM_CONFIG.getNotchHeight(), 0));
    }
    
    auto arrow = utils::findChild(this, "arrow");
    arrow->setPositionY(bg->getContentSize().height * info.anchor.y);
    
    arrow_v->setVisible(bg->getAnchorPoint().y == 0);
    arrow->setVisible(bg->getAnchorPoint().x == 0);
    
//    auto rendered = utils::captureNode(bg);
//
//    auto texture = new (std::nothrow) Texture2D();
//    texture->initWithImage(rendered);
//
//    auto sprite = Sprite::createWithTexture(texture);
//    this->addChild(sprite);
}

void MSOnboard::setHighlightRect(Rect r) {
    auto bottom = utils::findChild<ui::Layout *>(this, "bottom");
    bottom->setContentSize(Size(this->getContentSize().width, r.getMinY()));
    
    auto top = utils::findChild<ui::Layout *>(this, "top");
    top->setContentSize(Size(this->getContentSize().width, this->getContentSize().height - r.getMaxY()));
    top->setPositionY(this->getContentSize().height);

    auto left = utils::findChild<ui::Layout *>(this, "left");
    left->setContentSize(Size(r.getMinX(), r.size.height));
    left->setPosition(Point(0, r.getMinY()));
    
    auto right = utils::findChild<ui::Layout *>(this, "right");
    right->setContentSize(Size(this->getContentSize().width - r.getMaxX(), r.size.height));
    right->setPosition(Point(this->getContentSize().width, r.getMinY()));
}

ui::Widget::ccWidgetClickCallback MSOnboard::onLocateClickCallback(const std::string &name) {
    if (name == "onNext") {
        return CC_CALLBACK_1(MSOnboard::onNext, this);
    }
    
    if (name == "onSkip") {
        return CC_CALLBACK_1(MSOnboard::onSkip, this);
    }
    
    return nullptr;
}


void MSOnboard::onNext(cocos2d::Ref *sender) {
    if(_delegate) CCLOG("delegate's not null");
    if (_step == (int)_onboard_info.size() - 1) {
        this->removeFromParent();
        MJDEFAULT->setBoolForKey(key_onboarding_part_2, true);
        MJDEFAULT->setBoolForKey(key_onboarding_part_3, true);
        MS_LOGGER.logEventTutorialCompleted();
//        _delegate ? _delegate->onFinishOnboard(): (void) nullptr;
        return;
    }
    
    utils::findChild(this, "background")->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this]() {
        auto next_onboard = MSOnboard::createBoard(_step + 1);
        next_onboard->setDelegate(_delegate);
        this->getParent()->addChild(next_onboard, 10);
        utils::findChild(next_onboard, "background")->setOpacity(0);
        utils::findChild(next_onboard, "background")->runAction(FadeIn::create(0.25));
        this->removeFromParent();
    }), NULL));
}

void MSOnboard::onSkip(cocos2d::Ref * sender) {
    MS_LOGGER.logEventTutorialSkip();
    _delegate ? _delegate->onFinishOnboard(): (void) nullptr;
    this->removeFromParent();
    MJDEFAULT->setBoolForKey(key_onboarding_part_2, true);
    MJDEFAULT->setBoolForKey(key_onboarding_part_3, true);
}
