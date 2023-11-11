//
//  MJBookPage.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/6/17.
//

#include "MJBookPage.h"
#include "AudioEngine.h"
#include "GAFWrapper.h"

USING_NS_CC;
INITIALIZE_READER(MJBookPage);

struct TextInfo {
    std::string text;
    float start_time;
    float stop_time;
    int start_id;
    int stop_id;
};

struct GAFInfo {
    int gaf_id;
    std::string sequence;
    float start_time;
    float stop_time;
};

std::vector<TextInfo> sync_data = {
};

std::vector<GAFInfo> gaf_data {

};

std::once_flag mj_bookpage_reader;
MJBookPage * MJBookPage::createPage(const std::string & config_file) {
    std::call_once(mj_bookpage_reader, [] {
        REGISTER_CSB_READER(MJBookPage);
    });
    
    auto page = reinterpret_cast<MJBookPage *>(CSLoader::createNodeWithVisibleSize("csb/BookPage.csb"));
    if (page) {
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return nullptr;
}

void MJBookPage::onEnter() {
    Director::getInstance()->getTextureCache()->addImage("gaf/linda/test_hdr.png");
    Director::getInstance()->getTextureCache()->addImage("gaf/linda/test_hdr_2.png");
    Director::getInstance()->getTextureCache()->addImage("gaf/linda/test_hdr_3.png");
    Director::getInstance()->getTextureCache()->addImage("gaf/linda/test_hdr_4.png");
    Director::getInstance()->getTextureCache()->addImage("gaf/linda/test_hdr_5.png");
    Director::getInstance()->getTextureCache()->addImage("gaf/linda/test_hdr_6.png");
    
    ui::Layout::onEnter();
    box_id = cursor_id = 0;
    
    auto counter = 1;
    auto boxtext = utils::findChild<ui::Text *>(this, StringUtils::format("boxtext_%d", counter));
    while (boxtext) {
        texts.push_back(boxtext->getString());
        boxtext->setTextColor(Color4B::WHITE);
        boxtext->setColor(Color3B::BLACK);
        counter++;
        boxtext = utils::findChild<ui::Text *>(this, StringUtils::format("boxtext_%d", counter));
    }
    
    experimental::AudioEngine::play2d("sounds/demo.mp3");

    this->scheduleOnce([=](float) {
        auto counter = 0;
        for (auto info : sync_data) {
            this->scheduleOnce([=](float) {
                this->highlightText(info.text, box_id, cursor_id, info.start_time, info.stop_time - info.start_time, Color3B::RED);
            }, info.start_time, StringUtils::format("sync_text_%d", counter++));
        }
        auto gaf_p = utils::findChild(this, "gaf_1");
        auto gaf = GAFWrapper::createGAF("gaf/linda/linda.gaf", gaf_p);
        gaf->setName("gaf_0");
        
        for (auto info : gaf_data) {
            this->scheduleOnce([=](float) {
                auto gaf = utils::findChild<GAFWrapper *>(this, StringUtils::format("gaf_%d", info.gaf_id));
                gaf->playSequence(info.sequence, true);
                gaf->runAction(Sequence::create(DelayTime::create(info.stop_time - info.start_time), CallFunc::create([gaf, info] {
                    gaf->playSequence(StringUtils::format("s%s", info.sequence.c_str()));
                }), NULL));
            }, info.start_time, StringUtils::format("sync_anim_%d", counter++));
        }
    }, 0.1, "delay_what?");
    
}

void MJBookPage::highlightText(std::string text, int box, int cursor, float start_time, float duration, cocos2d::Color3B color) {
    //find range
    auto join_text = std::string("");
    for (auto i = 0; i < texts.size(); i++) {
        join_text += texts[i];
        join_text += " ";
        if (i < box) {
            cursor += texts[i].size() + 1;
        }
    }
    
    join_text.resize(join_text.size() - 1);
    auto p_start = join_text.find(text, cursor);
    if (p_start == std::string::npos) {
    }
    auto p_finish = p_start + text.size();
    
    auto box_start = 0;
    while (p_start > texts[box_start].size() + 1) {
        p_start -= (texts[box_start].size() + 1);
        box_start++;
    }
    
    auto box_finish = 0;
    while (p_finish > texts[box_finish].size() + 1) {
        p_finish -= (texts[box_finish].size() + 1);
        box_finish ++;
    }
    
    std::vector<std::tuple<int, int, int>> highlight_info;
    if (box_start == box_finish) {
        highlight_info.push_back(std::make_tuple(box_start, p_start, p_finish));
    } else {
        highlight_info.push_back(std::make_tuple(box_start, p_start, texts[box_start].size() - 1));
        for (auto i = box_start + 1; i < box_finish - 1l; ++i) {
            highlight_info.push_back(std::make_tuple(i, 0, texts[i].size() - 1));
        }
        highlight_info.push_back(std::make_tuple(box_finish, 0, p_finish));
    }
    
    box_id = box_finish;
    cursor_id = p_finish;
    
    for (auto t : highlight_info) {
        int box, start, finish;
        std::tie(box, start, finish) = t;

        auto boxtext = utils::findChild<ui::Text *>(this, StringUtils::format("boxtext_%d", box + 1));
        for (auto i = start; i <= finish; ++i) {
            auto letter = boxtext->getLetter(i);
            if (letter) {
                letter->runAction(Sequence::create(CallFunc::create([letter]{
                    letter->setColor(Color3B::RED);
                }), DelayTime::create(duration), CallFunc::create([letter] {
                    letter->setColor(Color3B::BLACK);
                }), NULL));
            }
        }
    }
}
