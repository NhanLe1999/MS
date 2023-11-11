#include "Sequencing.h"
#include "AspectR.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <audio/include/AudioEngine.h>
#include "json/document.h"
#include "json/stringbuffer.h"
#include "MJHelper.h"
#include "story/HSAudioEngine.h"
USING_NS_CC;

#define MARGIN_DIRECTOR_INSTANCE_SMALL 10
#define ASPECT_RATIO_16_9 1.4
using namespace sequencing;
using namespace rapidjson;

GamePlay* GamePlay::createGame(std::vector<math::resource::Image*> images)
{
    GamePlay* obj = new GamePlay();
    if(obj->init())
    {
        obj->autorelease();
        obj->initOptions(images);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

#include "MJDefault.h"
#include "MJHelper.h"
#define key_count_guide_jumble_pages "key_count_guide_jumble_pages"

void GamePlay::initOptions(std::vector<math::resource::Image*> images)
{
    auto count = MJDEFAULT->getIntegerForKey(key_count_guide_jumble_pages, 0);
    //_is_guide_showed = !(count < 3);
    
    this->scheduleOnce([count](float) {
        MJDEFAULT->setIntegerForKey(key_count_guide_jumble_pages, count + 1);
    }, 30, "update_count_guide_jumble_pages");
    
    json_file = "config.json";
    if(MJDEFAULT->getBoolForKey(key_background_music)){
        cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/bg.mp3", true, 0.3);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->setPosition(Vec2::ZERO);
    this->setContentSize(Size(visibleSize.width,visibleSize.height));

    //assign
    _allLevels = loadLevelData(images);

    //background///////////
    auto background = Sprite::create("games/sequencing/bg.jpg");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setPosition(AspectR::getMidX(),AspectR::getMidY());
	if (background->getContentSize().width < Director::getInstance()->getVisibleSize().width){
		background->setScale(Director::getInstance()->getVisibleSize().width / background->getContentSize().width);
	}
    this->addChild(background);
    ///////////////////////

    //monkey with rope
    const string khiPath = "gaf/sequencing/khi/khi.gaf";
    GAFAsset *khiAsset = GAFAsset::create(khiPath);
    _khiObject = khiAsset->createObject();
    _khiObject->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _khiObject->setPosition(Vec2(AspectR::getMidX(),AspectR::getMidY() + AspectR::getHeight(150)));
    this->addChild(_khiObject);

    //paper fly
    const string paperFlyPath = "gaf/sequencing/paper_fly/paper_fly.gaf";
    GAFAsset *paperFlyAsset = GAFAsset::create(paperFlyPath);
    _paperFlyObject = paperFlyAsset->createObject();
    _paperFlyObject->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _paperFlyObject->setPosition(Vec2(AspectR::getMidX(),AspectR::getMidY()));
    this->addChild(_paperFlyObject);

    //title
    auto title = Sprite::create("games/sequencing/title.png");
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    title->setPosition(Vec2(AspectR::getMidX(),AspectR::getMidY()));
    this->addChild(title);

    //droppable image
    for(int i=0;i<_allLevels[_currentIndex].images.size();i++) {
        auto img = DroppableTile::create(i + 1);
        this->addChild(img);
        _droppable.push_back(img);
    }

    _overlay = Layout::create();
    _overlay->setContentSize(this->getContentSize());
//    _overlay->setBackGroundColorType(BackGroundColorType::SOLID);
//    _overlay->setBackGroundColor(Color3B::BLACK);
//    _overlay->setOpacity(200);
    _overlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _overlay->setPosition(Vec2(AspectR::getMidX(),AspectR::getMidY()));
    _overlay->setVisible(false);
    _overlay->setTouchEnabled(true);
    _overlay->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (!canTouchOut) return;
        if (type == Widget::TouchEventType::ENDED) {
                zoomOut(_dragTile);
        }
    });
    this->addChild(_overlay, 1000);

    //images
	

    float width = 0;
    for(int i=0;i<_allLevels[_currentIndex].images.size();i++) {
        auto img = ImageTile::create(_allLevels[_currentIndex].images[i]);
        img->holder = _droppable[i];
        img->audioIndex = i;
        img->setTouchEnabled(true);
        img->setTag(i);
        this->addChild(img);
        _images.push_back(img);
		float maxSize = Director::getInstance()->getVisibleSize().width / 4 -120;
		float iw = maxSize / img->getContentSize().width;
		
        width += img->getContentSize().width* iw;
		sorted_imgs.pushBack(img);
    }


    //shuffle
    mj::helper::shuffle(_images);

    //images to position
    float aspectRatio = cocos2d::Director::getInstance()->getVisibleSize().width / cocos2d::Director::getInstance()->getVisibleSize().height;
    float imagePosition = aspectRatio< ASPECT_RATIO_16_9 ? MARGIN_DIRECTOR_INSTANCE_SMALL * 3: MARGIN_DIRECTOR_INSTANCE_SMALL*1.5;
        //MARGIN_DIRECTOR_INSTANCE_SMALL * 1.5;
    for(int i=0;i<_images.size();i++) {
        auto img = _images[i];

		float maxSize = (Director::getInstance()->getVisibleSize().width - 120) / 4;
		float iw = maxSize / img->getContentSize().width;

        auto dropImg = _droppable[i];
        if(i<2) {
            img->setScale(iw);
            img->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            img->setPosition(Vec2( imagePosition , AspectR::getHeight(80)));
        } else if(i>=2) {
            img->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            img->setScale(iw);
            img->setPosition(Vec2(imagePosition + img->getContentSize().width* iw , AspectR::getHeight(80)));

        }
		img->originalPos = img->getPosition();

        dropImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        dropImg->setPosition(Vec2(imagePosition + maxSize/2, AspectR::getHeight(600)));

        imagePosition += (img->getContentSize().width *iw) + AspectR::getWidth(20);
    }

    _speaker = mj::helper::addSpeaker(this);
    _speaker->setPosition(Vec2(40, Director::getInstance()->getVisibleSize().height - 40));
    _speaker->setTouchEnabled(true);
    _speaker->setScale(0.9f);
    _speaker->setTouchEnabled(false);
    _speaker->addClickEventListener([=](cocos2d::Ref* sender) {
        _speaker->setTouchEnabled(false);
        cocos2d::experimental::AudioEngine::stop(_currentAudio);
        _currentAudio = cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/remind.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path) {
            scheduleInactiveMode();
            _speaker->setTouchEnabled(true);
            });

        });

	auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/gameName.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
		title->runAction(Sequence::create(DelayTime::create(1.5), FadeOut::create(0.5), CallFunc::create([=](){
			cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/intro.mp3");
			cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/monkey_swing_paper1.mp3");
			_khiObject->playSequence("right", false);
			_khiObject->start();
		}), DelayTime::create(0.5), CallFunc::create([=](){
			_paperFlyObject->playSequence("right", false);
			_paperFlyObject->start();
			cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/paper_in.mp3");
		}), DelayTime::create(1.3), CallFunc::create([=](){
			//applyImageEnterAnim(0, 1);
            applyImageInPosition(0, 1);
		})/*, DelayTime::create(1)*/, CallFunc::create([=](){
			//applyImageInPosition(0, 1);
		}), DelayTime::create(1.5), CallFunc::create([=](){
			_khiObject->playSequence("left", false);
			_khiObject->start();
		}), DelayTime::create(0.5), CallFunc::create([=](){
			_paperFlyObject->playSequence("left", false);
			_paperFlyObject->start();
			cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/paper_in.mp3");
		}), DelayTime::create(1.3), CallFunc::create([=](){
			//applyImageEnterAnim(2, 3);
            applyImageInPosition(2, 3);
		}), DelayTime::create(1), CallFunc::create([=](){
			//applyImageInPosition(2, 3);
		}), DelayTime::create(1), CallFunc::create([=](){
			for (auto& dimg : _droppable) {
				dimg->setScale(0.5f);
				dimg->runAction(ScaleTo::create(0.2f, 1.05f));
				dimg->setVisible(true);
			}
			auto audio_id = cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/number_out.mp3");
            cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int audio_id, std::string path){
                _speaker->setTouchEnabled(true);
                scheduleInactiveMode();
            });

			canTouch = true;
		}), nullptr));
	});
    countImageLeft = _images.size();
}

vector<LevelData> GamePlay::loadLevelData(std::vector<math::resource::Image*> images)
{
    Document document;
	std::string str = FileUtils::getInstance()->getStringFromFile(json_file);
	document.Parse(str.c_str());

    vector<LevelData> totalLevels;

    //overall array
    auto levelArray = document["data"].GetArray();
    for (auto i = 0; i < levelArray.Size(); i++) {
        auto level = LevelData();
        for (auto& v : levelArray[i].GetArray()) {
			auto info = math::resource::ResHelper::getImage(images, v.GetString());
            level.images.push_back(info->path);
			level.detailsAudio.push_back(info->getAudio());
        }
        
        totalLevels.push_back(level);
    }
    
    return totalLevels;
}

void GamePlay::applyImageEnterAnim(int index1, int index2)
{
	float maxSize = (Director::getInstance()->getVisibleSize().width - 120) / 2;
	float iw = maxSize / _images[0]->getContentSize().width;
	bool iNeedScale = (_images[0]->getContentSize().width - maxSize) > 0;
    if(index1 == 0 && index2 == 1)
    {
        this->runAction(Sequence::create(CallFunc::create([=](){
            _images[index1]->setVisible(true);
            _images[index1]->setPositionX(AspectR::getMidX() - _images[index1]->getContentSize().width*iw);
			if (iNeedScale)
				_images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
			else
			 _images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2,1)));
            _images[index1]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2,Vec2(_images[index1]->getPositionX() -20, _images[index1]->getPositionY()))));
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/paper_in.mp3");
        }),DelayTime::create(0.3),CallFunc::create([=](){
            _images[index2]->setVisible(true);
            _images[index2]->setPositionX(AspectR::getMidX());
			if (iNeedScale)
				_images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
			else
				_images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2,1)));
            _images[index2]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2,Vec2(_images[index2]->getPositionX()+20 , _images[index2]->getPositionY()))));
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/paper_in.mp3");
        }),nullptr));
    }
    else if(index1 == 2 && index2 == 3)
    {
        this->runAction(Sequence::create(CallFunc::create([=](){
            _images[index1]->setVisible(true);
            _images[index1]->setPositionX(AspectR::getMidX());
			if (iNeedScale)
				_images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
			else
				_images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2,1)));
            _images[index1]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2,Vec2(_images[index1]->getPositionX() -20, _images[index1]->getPositionY()))));
            _images[index1]->getParent()->reorderChild(_images[index1], _images[index1]->getParent()->getChildrenCount()-3);
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/paper_in.mp3");
        }),DelayTime::create(0.3),CallFunc::create([=](){
            _images[index2]->setVisible(true);
            _images[index2]->setPositionX(AspectR::getMidX() + _images[index1]->getContentSize().width*iw);
			if (iNeedScale)
				_images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
			else
				_images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2,1)));
            _images[index2]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2,Vec2(_images[index2]->getPositionX()+20 , _images[index2]->getPositionY()))));
            _images[index2]->getParent()->reorderChild(_images[index2], _images[index2]->getParent()->getChildrenCount()-3);
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/paper_in.mp3");
        }),nullptr));
    }
}

void GamePlay::applyImageInPosition(int index1, int index2)
{
	float maxSize = (Director::getInstance()->getVisibleSize().width - 120) / 4 ;
	float iw = maxSize / _images[0]->getContentSize().width;
	bool iNeedScale = (_images[0]->getContentSize().width - maxSize) > 0;

    if(index1 == 0 && index2 == 1) {
        this->runAction(Sequence::create(CallFunc::create([=](){
            _images[index1]->setVisible(true);
            _images[index1]->setPositionX(AspectR::getMidX());
            if (iNeedScale) {
                _images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
                _images[index1]->setPositionX(AspectR::getMidX());
            }
			else
			 _images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, 0.5)));
            _images[index1]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2, _images[index1]->originalPos)));
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/slide_1.mp3");
        }),DelayTime::create(0.1),CallFunc::create([=](){
            _images[index2]->setVisible(true);
            _images[index2]->setPositionX(AspectR::getMidX() + _images[index1]->getContentSize().width * iw);
			if (iNeedScale)
				_images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
			else
            _images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, 0.5)));
            _images[index2]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2, _images[index2]->originalPos)));
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/slide_1.mp3");
        }), nullptr));
    } else if(index1 == 2 && index2 == 3) {
        _images[index1]->setVisible(true);
        _images[index1]->setPositionX(AspectR::getMidX());
        this->runAction(Sequence::create(CallFunc::create([=](){
			if (iNeedScale)
				_images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
			else
            _images[index1]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, 0.5)));
            _images[index1]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2, _images[index1]->originalPos)));
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/slide_1.mp3");
        }),DelayTime::create(0.1),CallFunc::create([=](){
            _images[index2]->setVisible(true);
            _images[index2]->setPositionX(AspectR::getMidX() + _images[index1]->getContentSize().width * iw);
			if (iNeedScale)
				_images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, iw)));
			else
				_images[index2]->runAction(EaseCubicActionOut::create(ScaleTo::create(0.2, 0.5)));
            _images[index2]->runAction(EaseCubicActionOut::create(MoveTo::create(0.2, _images[index2]->originalPos)));
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/slide_1.mp3");
        }), nullptr));
    }
}

void GamePlay::zoomIn(ImageTile* img, bool inFinish /*= false*/) {
    _overlay->setTouchEnabled(false);
    _image_zoom = true;
    auto close = this->getChildByName("close_layout");
    close ? close->setVisible(false) : (void) nullptr;
    
    auto guide = Director::getInstance()->getRunningScene()->getChildByName("guide_overlay");
    guide ? guide->setVisible(false) : (void) nullptr;
    
	canTouch = false;
    _overlay->getParent()->reorderChild(_overlay, (int)_overlay->getParent()->getChildrenCount()-1);
    _overlay->setVisible(true);
	float maxHeght = Director::getInstance()->getVisibleSize().height - 150;
	float maxScaleY = maxHeght / img->getContentSize().height;
	float maxWidth = Director::getInstance()->getVisibleSize().width - 150;
	float maxScaleX = maxWidth / img->getContentSize().width;
	float maxScale = maxScaleX > maxScaleY ? maxScaleY : maxScaleX;
    
    img->getParent()->reorderChild(img, (int)img->getParent()->getChildrenCount()-2);
    
    
    auto fade_bg = ui::Layout::create();
    fade_bg->setContentSize(this->getContentSize());
    fade_bg->setBackGroundColor(Color3B::BLACK);
    fade_bg->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    fade_bg->setOpacity(0xcc);
    fade_bg->setName("fade_bg");
    this->addChild(fade_bg, (int)img->getParent()->getChildrenCount() - 3);
    
    if(img->getAnchorPoint() == Vec2::ANCHOR_BOTTOM_LEFT) {
		img->runAction(MoveTo::create(0.3, Vec2(AspectR::getMidX() - img->getContentSize().width ,
			AspectR::getMidY() - img->getContentSize().height )));
    } else if(img->getAnchorPoint() == Vec2::ANCHOR_BOTTOM_RIGHT) {
		img->runAction(MoveTo::create(0.3, Vec2(AspectR::getMidX() + img->getContentSize().width ,
			AspectR::getMidY() - img->getContentSize().height )));
    } else if(img->getAnchorPoint() == Vec2::ANCHOR_MIDDLE) {
        img->runAction(MoveTo::create(0.3,Vec2(AspectR::getMidX(), AspectR::getMidY())));
    }

	//img->stopAllActions();
	img->runAction(Sequence::create(ScaleTo::create(0.3, maxScale), CallFunc::create([=](){
        _detailsAudio = cocos2d::experimental::AudioEngine::play2d(_allLevels[_currentIndex].detailsAudio[img->audioIndex]);
		cocos2d::experimental::AudioEngine::setFinishCallback(_detailsAudio, [=](int au_id, std::string path){
			if (inFinish){
				if (sorted_imgs.contains(img)){
					sorted_imgs.eraseObject(img);
				}
				zoomOut(img, true);
            }
            else {
                _overlay->setTouchEnabled(true);
                zoomOut(img);
            }

		});
		if (inFinish){
			
		}
		else{
			//this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=](){
			canTouchOut = true;
			canTouch = false;
			//})));
		}
    }), NULL) );

    cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/zoom_in_paper.mp3");
}

void GamePlay::zoomOut(ImageTile* img, bool inFinish /*= false*/) {
    _image_zoom = false;
    this->removeChildByName("fade_bg");
    auto close = this->getChildByName("close_layout");
    close ? close->setVisible(true) : (void) nullptr;
	float maxSize = (Director::getInstance()->getVisibleSize().width - 120) / 4;
	float iw = maxSize / _images[0]->getContentSize().width ;
	bool iNeedScale = (_images[0]->getContentSize().width - maxSize) > 0;
    cocos2d::experimental::AudioEngine::stop(_detailsAudio);
    _overlay->setVisible(false);

	//img->stopAllActions();
    img->runAction(MoveTo::create(0.3,img->originalPos));
    img->runAction(Sequence::create(ScaleTo::create(0.3,iw), CallFunc::create([=](){
        img->getParent()->reorderChild(img, img->getParent()->getChildrenCount()-3);
		if (inFinish){
			if (sorted_imgs.size()>=1){
				this->scheduleOnce([=](float dt){
					zoomIn(sorted_imgs.at(0), true);
				}, 0.2f, "zom_in_next");
				
			}
			else{
				onFinish();
			}
		}
		else{
			_dragTile = NULL;
			canTouch = true;
			canTouchOut = false;
		}
    }), nullptr));


    cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/zoom_out_paper.mp3");
}

void GamePlay::onEnter() {
    ms::BaseActivity::onEnter();

    //*** Event Listenter for DRAG ***//
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = [=](Touch* touch, Event* event) {
        cocos2d::experimental::AudioEngine::stop(_currentAudio);
        unschedule("show_guide_jumble_pages");
        if(!canTouch) return false;
        Vec2 pt = convertToNodeSpace(touch->getLocation());

        for(auto& img : _images) {
            if(img->getBoundingBox().containsPoint(pt)) {
                if (!img->isTouchEnabled()) {
                    return false;
                }
                _dragTile = img;
                _dragTile->setTouchEnabled(true);
                cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/drag.mp3");
                return true;
            }
        }

        return false;
    };
    _listener->onTouchMoved = [=](Touch* touch, Event* event) {
        Vec2 pt = convertToNodeSpace(touch->getLocation());
        if(_dragTile != NULL  && !_dragTile->placed) {
            _dragTile->setPosition(Vec2(_dragTile->getPosition().x + touch->getDelta().x, _dragTile->getPosition().y + touch->getDelta().y));
            //bring to front
            _dragTile->getParent()->reorderChild(_dragTile, _dragTile->getParent()->getChildrenCount()-1);
        }
    };
    _listener->onTouchEnded = [=](Touch* touch, Event* event) {
        scheduleInactiveMode();
        Vec2 pt = convertToNodeSpace(touch->getLocation());

        if(_dragTile != NULL && _dragTile->getPosition() == _dragTile->originalPos) {
            zoomIn(_dragTile);
        }
        else if(_dragTile != NULL && !_dragTile->placed && _dragTile->holder->isOverlap(_dragTile->getBoundingBox())) {
            _countDraggedWrongImage = 0;
            countImageLeft--;
            auto draggedImage = _dragTile;
            auto holder = _dragTile->holder;
            _is_guide_showed = true;
            draggedImage->getParent()->reorderChild(draggedImage, draggedImage->getParent()->getChildrenCount() - 3);
            draggedImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            draggedImage->setPosition(holder->getPosition());
            draggedImage->originalPos = holder->getPosition();
            draggedImage->placed = true;
            Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
          //  this->unschedule("show_guide_jumble_pages");
            draggedImage->showTick();
            cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/correct_drop.mp3");
            if (!game_play_enable) {
                this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                    this->pauseGame();
                }), NULL));
            }
            _dragTile = NULL;

            //check if all solved
            bool allSolved = true;
            for(auto& timg : _images) {
                if(!timg->placed) {
                    allSolved = false;
                    break;
                }
            }
            if(allSolved) {
    
                _speaker->setTouchEnabled(false);
                unschedule("show_guide_jumble_pages");
                for(int i=0;i<_images.size();i++) {
                    _droppable[i]->setVisible(false);
                    _images[i]->runAction(Sequence::create(DelayTime::create(1), MoveTo::create(0.5,Vec2(_images[i]->getPositionX(), AspectR::getMidY())), nullptr));
                    _images[i]->originalPos = Vec2(_images[i]->getPositionX(), AspectR::getMidY());
                }
				canTouch = false;
				canTouchOut = false;
                this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create([=](){
					auto audio_id_intro = cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/complete.mp3");
					cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_intro, [=](int au_id, std::string path){
						this->scheduleOnce([=](float dt){
                            Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
                            unschedule("show_guide_jumble_pages");
							showSequence();
						}, 0.5f, "play_video");
					});
                    //cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/complete.mp3");
                }),DelayTime::create(6.5f),CallFunc::create([=](){

                }), nullptr));
            }

        } 
        else if(_dragTile != NULL && !_dragTile->placed) {
            bool draggedOnWrongImage = false;
            for(auto drop:_droppable) {
                if (drop->isOverlap(_dragTile->getBoundingBox())) {
                    draggedOnWrongImage = true;
                    break;
                }
            }

            if (draggedOnWrongImage) {
                if (_countDraggedWrongImage>2) {
                    _is_guide_showed = false;
                    showHand();
                }
                else {
                    _countDraggedWrongImage++;
                }
                cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/wrong.mp3");
            } else {
                _countDraggedWrongImage = 0;
                cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/drag.mp3");
            }

            _dragTile->runAction(Sequence::create(CallFuncN::create([](Node * n) {
                reinterpret_cast<ui::Widget *>(n)->setTouchEnabled(false);
            }), EaseBackInOut::create(MoveTo::create(0.5,_dragTile->originalPos)), CallFuncN::create([](Node * n) {
                reinterpret_cast<ui::Widget *>(n)->setTouchEnabled(true);
            }), nullptr));
            _dragTile->getParent()->reorderChild(_dragTile, _dragTile->getParent()->getChildrenCount() - 3);
        }
    };
    
    _listener->onTouchCancelled = _listener->onTouchEnded;
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, -1);
    //*********************//
}

//void GamePlay::pauseGame(){
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(cocos2d::Director::getInstance()->getWinSize());
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setPosition(layout->getContentSize()/2);
//    layout->setTouchEnabled(true);
//    this->addChild(layout, 99999999);
//    auto text = cocos2d::ui::Text::create("PAUSE Game!", "fonts/monkeystories_ultra.ttf", 50);
//    text->setScale(0);
//    layout->addChild(text);
//    text->setPosition(layout->getContentSize()/2);
//    text->setTextColor(Color4B::RED);
//    text->enableOutline(Color4B::BLACK, 3);
//    text->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 2.0f)), DelayTime::create(3.0f), CallFunc::create([=]{
//    }), NULL));
//}

void GamePlay::onExit() {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    Layout::onExit();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    _eventDispatcher->removeEventListener(_listener);
}

void sequencing::GamePlay::showSequence()
{
	zoomIn(sorted_imgs.at(0), true);
}

void sequencing::GamePlay::onFinish()
{
	auto endTxt = Label::createWithTTF("Can you tell the sequence?", "fonts/Linotte_Bold.otf", AspectR::getFontSize(30));
	endTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	endTxt->setTextColor(Color4B::BLACK);
    endTxt->setPosition(Point(Director::getInstance()->getVisibleSize().width / 2, 600));
	endTxt->setAlignment(TextHAlignment::LEFT);
	this->addChild(endTxt);

	auto doneButton = ImageView::create("games/sequencing/button.png");
	doneButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	doneButton->setPosition(Vec2(AspectR::getMidX(), AspectR::getHeight(120)));
	doneButton->setTouchEnabled(true);
	doneButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (!canTouch) return;
		if (type == Widget::TouchEventType::ENDED) {
			canTouch = false;
			CCLOG("@@@@ GAME OVER @@@@");
            this->unscheduleAllCallbacks();
            this->unscheduleUpdate();
            this->stopAllActions();
			this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=] {
				//_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
				commonEndGame(ms::BaseActivity::EGT_CHAR, "gaf/end_game/KhiQuanLa_standalone_new/KhiQuanLa_standalone_new.gaf", true);
			}), NULL));
		}
	});
	this->addChild(doneButton);
	doneButton->setOpacity(0);
	doneButton->setCascadeOpacityEnabled(true);

	auto doneButtonCaption = Label::createWithTTF("DONE", "fonts/Linotte_Bold.otf", AspectR::getFontSize(24));
	doneButtonCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	doneButtonCaption->setTextColor(Color4B::BLACK);
	doneButtonCaption->setPosition(Point(doneButton->getContentSize() / 2) + Point(1, -3));
	doneButtonCaption->setAlignment(TextHAlignment::CENTER);
	doneButton->addChild(doneButtonCaption);
	
	auto audio_id_intro = cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/complete2.mp3");
	cocos2d::experimental::AudioEngine::setFinishCallback(audio_id_intro, [=](int au_id, std::string path){
		canTouch = true;
		doneButton->runAction(FadeIn::create(0.5f));
	});
}

/*
 * Image Tile
 */

ImageTile* ImageTile::create(string src)
{
    ImageTile* obj = new ImageTile();
    if(obj->init(src))
    {
        obj->autorelease();
        obj->setVisible(false);
        obj->initOptions();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void ImageTile::initOptions()
{
    this->setScale(0.5);

    //tick
    const string tickPath = "gaf/sequencing/tick/tick.gaf";
    GAFAsset *tickAsset = GAFAsset::create(tickPath);
    _tickObject = tickAsset->createObject();
    _tickObject->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _tickObject->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    this->addChild(_tickObject);
}

void ImageTile::showTick() {
    _tickObject->start();
}
//-------------------------------------------------

/*
 * Droppable
 */
DroppableTile* DroppableTile::create(int index)
{
    DroppableTile* obj = new DroppableTile();
    if(obj->init("games/sequencing/dropbox.png"))
    {
        obj->autorelease();
        obj->initOptions(index);
        obj->setVisible(false);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void DroppableTile::initOptions(int index)
{
    auto numTxt = Label::createWithTTF(StringUtils::toString(index), "fonts/Linotte_Heavy.otf", AspectR::getFontSize(50));
    numTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    numTxt->setTextColor(Color4B::BLACK);
    numTxt->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
    numTxt->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    numTxt->setOpacity(200);
    this->addChild(numTxt);
}

bool DroppableTile::isOverlap(Rect otherRect)
{
    if(this->getBoundingBox().intersectsRect(otherRect)) {
        return true;
    }
    return false;
}
//-------------------------------------------------

void GamePlay::inactiveMode() {
    if (countImageLeft < _images.size()) {
        cocos2d::experimental::AudioEngine::stop(_currentAudio);
        _currentAudio = cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/remind.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path) {
            scheduleInactiveMode();
        });
    }
    else{
        showHand();
        cocos2d::experimental::AudioEngine::stop(_currentAudio);
        _currentAudio = cocos2d::experimental::AudioEngine::play2d("sounds/sequencing/remind.mp3");
        cocos2d::experimental::AudioEngine::setFinishCallback(_currentAudio, [=](int au_id, std::string path) {
            scheduleInactiveMode();
        });
    }
}
void GamePlay::showHand() {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    Vec2 start;
    Vec2 stop;
    bool  flag = false;
    for (int i = 0; i <= 3; i++) {
        for (auto img : _images) {
            if (img->getTag() == i && !img->placed) {

                auto start_bb = img->getBoundingBox();
                start = Point(start_bb.getMidX(), start_bb.getMidY());
                start = img->getParent()->convertToWorldSpace(start);

                stop = img->holder->getPosition();
                stop = img->holder->getParent()->convertToWorldSpace(stop) - Point(0, 100);

                auto overlay = cocos2d::ui::Layout::create();
                overlay->setContentSize(Director::getInstance()->getVisibleSize());
                overlay->setName("guide_overlay");
                Director::getInstance()->getRunningScene()->addChild(overlay, INT_MAX - 1);

                auto hand = cocos2d::ui::ImageView::create("mjstory/new_ui/hand.png");
                overlay->addChild(hand, INT_MAX - 1);
                hand->setPosition(start - Point(0, 25));
                overlay->setVisible(!_image_zoom);

                hand->runAction(Sequence::create(MoveTo::create(1.5, stop), DelayTime::create(0.5), RemoveSelf::create(), nullptr));

                flag = true;

                break;
            }
        }
        if (flag == true) {
            break;
        }
    }
}
void GamePlay::scheduleInactiveMode() {
    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    unschedule("show_guide_jumble_pages");
    this->schedule([=](float) {
        this->inactiveMode();
        }, 10, 100, 0, "show_guide_jumble_pages");
}