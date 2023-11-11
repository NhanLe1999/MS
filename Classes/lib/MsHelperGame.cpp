
#include "MsHelperGame.h"
#include "AppDelegate.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "StoryParentNotification_Landscape.h"


USING_NS_CC;

namespace ms {
    namespace game {

         std::vector<std::pair<int, int>> getTextIndexSentence(std::string text, std::vector<math::resource::Sync_Text> syncDatas) {
            std::vector<std::pair<int, int>> result;
            int startIndex = 0, endIndex = 0;
            text = mj::helper::to_lower(text);

            for (auto str : syncDatas) {
                auto word = mj::helper::to_lower(str.w);
                if (!std::isalpha(word[word.length() - 1])) {
                    word.pop_back();
                }
                startIndex = text.find(word, startIndex ? startIndex + 1 : 0);
                endIndex = startIndex + word.length();
                if (startIndex == std::string::npos) return { {-1,-1} };
                result.push_back({ startIndex,endIndex });
                startIndex = endIndex;
            }
            return result;
        }

         void highLightText(cocos2d::ui::Text* text, cocos2d::Node* currentNode, cocos2d::Color4B textOriginColor, cocos2d::Color3B highlightCorlor, int index, std::vector<math::resource::Sync_Text> syncDatas, std::vector<std::pair<int, int>> vectorTextIndex, std::function<void(int,std::string)> callback)
        {
            if (index >= syncDatas.size() || index >=vectorTextIndex.size()) {
                //restore text state
                text->setTextColor(textOriginColor);
        //        for (int i = 0; i < text->getString().length(); i++) {
        //            if (text->getLetter(i) != nullptr) {
        //                text->getLetter(i)->setColor(TEXT_COLOR);
        //            }
        //        }
                if (callback) {
                    callback(-1,"");
                    return;
                }
            }
            for (auto p : vectorTextIndex) {
                if (p.first < 0 || p.second < 0) {
                    if (callback) {
                        callback(-1, "");
                    }
                    return;
                }
            }
            
            if (index == 0) {//prepare text
                text->setTextColor(Color4B::WHITE);
                for (int i = 0; i < text->getString().length(); i++) {
                    if (text->getLetter(i) != nullptr) {
                        text->getLetter(i)->setColor(Color3B(textOriginColor));
                    }
                }
            }

            std::vector<Sprite*> textElement;

            for (int i = vectorTextIndex[index].first; i <= vectorTextIndex[index].second; i++) {
                if (text->getLetter(i)) {
                    textElement.push_back(text->getLetter(i));
                }

            }

            for (auto it = textElement.begin(); it != textElement.end(); it++) {
                if (*it != nullptr) (*it)->setColor(highlightCorlor);
            }
            float duration = (syncDatas.at(index).e - syncDatas.at(index).s) / 1000.0f;
            if(index == 0)
            {
                duration = syncDatas.at(index).e/1000.0f;
            }
             //index++;
            currentNode->scheduleOnce([=](float dt) {
                for (auto it = textElement.begin(); it != textElement.end(); it++) {
                    if (*it != nullptr) (*it)->setColor(Color3B(textOriginColor));
                }
                highLightText(text, currentNode, textOriginColor, highlightCorlor, index+1, syncDatas, vectorTextIndex, callback);
            }, duration, "waitSync" + std::to_string(index));

        }

    
        void startHighlightText(std::vector<math::resource::Sync_Text> listTextHight, cocos2d::ui::Text* text, std::string audioPath, std::function<void(int auId, std::string auPath)> callback, std::string answer, int spaceBetweenWords, cocos2d::Color4B startSyncTextColor , cocos2d::Color4B endSyncTextColor)
        {
            if (!text || listTextHight.empty())
            {
                if (!audioPath.empty()) {

                    auto id = cocos2d::experimental::AudioEngine::play2d(audioPath);

                    if (callback) {

                        cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int auId, std::string auPath) {

                            if (callback) {
                                callback(auId, auPath);
                            }

                            });
                    }
                }
                return;
            }
            auto layout = text->getParent();
            if (!layout)
                return;

            auto fontSize = text->getFontSize();
            auto fontName = text->getFontName();
            auto listSync = listTextHight;
            std::vector<float> listTimeDelay = {};
            std::vector<std::string> listText = {};
            std::vector<int> listIndex = {};
            std::vector<float> listDelayTimeLetter = {};

            for (int i = 0; i < listSync.size(); i++)
            {
                auto sync = listSync[i];
                float timeDelay = sync.e - sync.s;
                float timeDelayLetter = sync.s;
                listTimeDelay.push_back(sync.e - sync.s);
                listText.push_back(sync.w);
                listIndex.push_back(sync.te - sync.ts);
                listDelayTimeLetter.push_back(sync.s);
            }

            int postX = 0;

            auto anchorPontText = text->getAnchorPoint();

            float anchorPontTextX = anchorPontText.x;
            float anchorPontTextY = anchorPontText.y;

            if (anchorPontText.x != 0 && anchorPontText.x != 1)
            {
                postX = text->getPosition().x - text->getBoundingBox().size.width * anchorPontTextX;
            }
            else {
                if (anchorPontText.x == 0)
                {
                    postX = text->getPositionX();
                }
            }

            if (auto textTrue = utils::findChild<Text*>(layout, answer))
            {
                textTrue->setVisible(false);
            }
            Vector<FiniteTimeAction*> actions;
            for (int i = 0; i < listText.size(); i++)
            {
                auto textLabel = cocos2d::ui::Text::create(listText[i], fontName, fontSize);
   
                textLabel->setAnchorPoint(Point(0, anchorPontTextY));
                textLabel->setPosition(cocos2d::Vec2(postX, text->getPosition().y));
                textLabel->setTextColor(text->getTextColor());
                postX = textLabel->getBoundingBox().size.width + postX + spaceBetweenWords;
                layout->addChild(textLabel, INT_MAX);

                //add action
                float timedelay = listTimeDelay[i] / 1000.f;
                float timeDelayL = listIndex[i] / 1000.f;

                actions.pushBack(CallFunc::create([=] {

                    textLabel->setTextColor(startSyncTextColor);
                    }));
                actions.pushBack(cocos2d::DelayTime::create(timedelay));
                actions.pushBack(CallFunc::create([=] {

                    textLabel->setTextColor(endSyncTextColor);
                }));
            }

            if (!audioPath.empty()) {

                  auto id =  cocos2d::experimental::AudioEngine::play2d(audioPath);

                  if (callback) {

                      cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int auId, std::string auPath) {

                          if (callback) {
                              callback(auId, auPath);
                          }

                      });
                  }
            }

            layout->runAction(Sequence::create(actions));
        }

        void startHighlightText1(std::vector<math::resource::Sync_Text> listTextHight, cocos2d::ui::Text* text, std::string audioPath, std::function<void(int auId, std::string auPath)> callback, std::string answer, int spaceBetweenWords, cocos2d::Color4B startSyncTextColor , cocos2d::Color4B endSyncTextColor)
        {
            if (!text || listTextHight.empty())
            {
                if (!audioPath.empty()) {

                    auto id = cocos2d::experimental::AudioEngine::play2d(audioPath);

                    if (callback) {

                        cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int auId, std::string auPath) {

                            if (callback) {
                                callback(auId, auPath);
                            }

                            });
                    }
                }
                return;
            }
            auto layout = text->getParent();
            if (!layout)
                return;

            auto fontSize = text->getFontSize();
            auto fontName = text->getFontName();
            auto listSync = listTextHight;
            std::vector<float> listTimeDelay = {};
            std::vector<std::string> listText = {};
            std::vector<int> listIndex = {};
            std::vector<float> listDelayTimeLetter = {};

            for (int i = 0; i < listSync.size(); i++)
            {
                auto sync = listSync[i];
                float timeDelay = sync.e - sync.s;
                float timeDelayLetter = sync.s;
                listTimeDelay.push_back(sync.e - sync.s);
                listText.push_back(sync.w);
                listIndex.push_back(sync.te - sync.ts);
                listDelayTimeLetter.push_back(sync.s);
            }

            int postX = 0;

            auto anchorPontText = text->getAnchorPoint();

            float anchorPontTextX = anchorPontText.x;
            float anchorPontTextY = anchorPontText.y;

            if (anchorPontText.x != 0 && anchorPontText.x != 1)
            {
                postX = text->getPosition().x - text->getBoundingBox().size.width * anchorPontTextX;
            }
            else {
                if (anchorPontText.x == 0)
                {
                    postX = text->getPositionX();
                }
            }

            

            Vector<FiniteTimeAction*> actions;
            for (int i = 0; i < listText.size(); i++)
            {
                auto textLabel = cocos2d::ui::Text::create(listText[i], fontName, fontSize);

                textLabel->setAnchorPoint(Point(0, anchorPontTextY));
                textLabel->setPosition(cocos2d::Vec2(postX, text->getPosition().y));
                textLabel->setTextColor(text->getTextColor());
                postX = textLabel->getBoundingBox().size.width + postX + spaceBetweenWords;
                layout->addChild(textLabel, INT_MAX);

                //add action
                float timedelay = listTimeDelay[i] / 1000.f;
                float timeDelayL = listIndex[i] / 1000.f;

                actions.pushBack(CallFunc::create([=] {

                    textLabel->setTextColor(startSyncTextColor);
                    }));
                actions.pushBack(cocos2d::DelayTime::create(timedelay));
                actions.pushBack(CallFunc::create([=] {

                    textLabel->setTextColor(endSyncTextColor);
                }));
                actions.pushBack(CallFunc::create([=] {

                    textLabel->removeFromParent();
                }));
                
            }

            if (!audioPath.empty()) {

                  auto id =  cocos2d::experimental::AudioEngine::play2d(audioPath);

                  if (callback) {

                      cocos2d::experimental::AudioEngine::setFinishCallback(id, [=](int auId, std::string auPath) {

                          if (callback) {
                              callback(auId, auPath);
                          }

                      });
                  }
            }

            layout->runAction(Sequence::create(actions));
        }
    }
}
