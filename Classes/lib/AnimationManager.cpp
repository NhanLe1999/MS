#include "AnimationManager.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#define MK_RETURN_IF(x, ...)  if(x){ CCLOG("error: return at %s", __FUNCTION__); return __VA_ARGS__;}

#define MK_RETURN_IF_WITH_CONTENT(x,__TYPE__, ...)  if(x){ CCLOG("error: return at %s", __FUNCTION__); __VA_ARGS__ return __TYPE__ ;}

USING_NS_CC;

AnimationManager::AnimationManager()
{
	init();
}

AnimationManager::~AnimationManager()
{

}

bool AnimationManager::init()
{
	_screenSize = cocos2d::Director::getInstance()->getVisibleSize();
	_origin = Director::getInstance()->getVisibleOrigin();

	return true;
}

void AnimationManager::tilesShowingActionDisplayFromTopToBottom(Node * i_tile, Node * i_animal, Vec2 i_currentPosition, int i_rowNum, int i_colNum, int i, int j, std::function<void()> endFunction, Node* i_background, float i_scaleFactor)
{
	MK_RETURN_IF(i_tile == nullptr);
	MK_RETURN_IF(i_animal == nullptr);

	const float k_showingTileTime = 0.125f;

	const float k_delayTime = (0.5f / i_rowNum);//max = 0.5s

	i_tile->setPosition(i_currentPosition + Vec2(-100, 100));
	i_tile->setOpacity(0.0f);
	i_tile->setRotation(-90.0f);
	i_tile->setScale(3.0f);
	i_tile->runAction(Sequence::create(
		DelayTime::create(i*k_delayTime),
		Spawn::create(
			FadeIn::create(k_showingTileTime),
			RotateTo::create(k_showingTileTime, 0.0f),
			ScaleTo::create(k_showingTileTime, 1.0f*i_scaleFactor),
			EaseBackIn::create(MoveTo::create(k_showingTileTime, i_currentPosition)), nullptr),
		CallFunc::create([=]() {

		if (j % 2 == 0)
		{
			addParticleEffect(i_tile);
		}

		if (i_background)
			i_background->runAction(Sequence::createWithTwoActions(EaseBackIn::create(ScaleTo::create(k_delayTime, 1.1f, 1.05f)), EaseBackOut::create(ScaleTo::create(k_delayTime, 1.0f))));
		if (i == i_rowNum && j == i_colNum)
		{
			endFunction();
			//startLogicGameFunction();
		}
	}),
		nullptr));

	i_animal->setOpacity(0);
	i_animal->runAction(Sequence::createWithTwoActions(
		DelayTime::create(i*k_delayTime),
		FadeIn::create(k_showingTileTime)));
}

void AnimationManager::tilesShowingActionDisplayFromLeftToRight(Node * i_tile, Node * i_animal, Vec2 i_currentPosition, int i_rowNum, int i_colNum, int i, int j, std::function<void()> endFunction, Node * i_background, float i_scaleFactor)
{
	MK_RETURN_IF(i_tile == nullptr);
	MK_RETURN_IF(i_animal == nullptr);

	const float k_showingTileTime = 0.125f;

	const float k_delayTime = (0.5f / i_colNum);//max = 0.5s

	i_tile->setPosition(i_currentPosition + Vec2(-100, 100));
	i_tile->setOpacity(0.0f);
	i_tile->setRotation(-90.0f);
	i_tile->setScale(3.0f);
	i_tile->runAction(Sequence::create(
		DelayTime::create(j*k_delayTime),
		Spawn::create(
			FadeIn::create(k_showingTileTime),
			RotateTo::create(k_showingTileTime, 0.0f),
			ScaleTo::create(k_showingTileTime, 1.0f*i_scaleFactor),
			EaseBackIn::create(MoveTo::create(k_showingTileTime, i_currentPosition)), nullptr),
		CallFunc::create([=]() {

		//auto particleContent = EncryptManager::getInstance()->getValueMapFromEncryptedFile("effects/wooden_box_breaking_effect.plist");

		if (j%2==0)
		{
			addParticleEffect(i_tile);
		}

			if(i_background)
			i_background->runAction(Sequence::createWithTwoActions(EaseBackIn::create(ScaleTo::create(k_delayTime, 1.1f, 1.05f)), EaseBackOut::create(ScaleTo::create(k_delayTime, 1.0f))));
			if (i == i_rowNum && j == i_colNum)
			{
				endFunction();
			}
		}),
		nullptr));

	i_animal->setOpacity(0);
	i_animal->runAction(Sequence::createWithTwoActions(
		DelayTime::create(j*k_delayTime),
		FadeIn::create(k_showingTileTime)));
}

void AnimationManager::showingTilesByRowsAndColumsAction(Node * i_tile, Node * i_animal, Vec2 i_currentPosition, int i_rowNum, int i_colNum, int i, int j, int i_currentTileNum, std::function<void()> endFunction, Node * i_background,float i_scaleFactor)
{
	MK_RETURN_IF(i_tile == nullptr);
	MK_RETURN_IF(i_animal == nullptr);

	int currentTileNum = i_currentTileNum;

	if (i_currentTileNum >= (i_rowNum*i_colNum / 2))
	{
		currentTileNum -= (i_rowNum *i_colNum - 1);
		currentTileNum *= -1;
	}

	const float k_delayTime = (0.5f / (i_rowNum*i_colNum*0.5f));//max = 0.5s
	const float k_showingTileTime = 0.125f;

	i_tile->setPosition(i_currentPosition + Vec2(-100, 100));
	i_tile->setOpacity(0.0f);
	i_tile->setRotation(-90.0f);
	i_tile->setScale(3.0f);
	i_tile->runAction(Sequence::create(
		DelayTime::create(currentTileNum*k_delayTime),
		Spawn::create(
			FadeIn::create(k_showingTileTime),
			RotateTo::create(k_showingTileTime, 0.0f),
			ScaleTo::create(k_showingTileTime, 1.0f*i_scaleFactor),
			EaseBackIn::create(MoveTo::create(k_showingTileTime, i_currentPosition)), nullptr),
		CallFunc::create([=]() {

			if (j % 2 == 0)
			{
				addParticleEffect(i_tile);
			}

			if (currentTileNum < (i_rowNum*i_colNum / 2))
			{
				if (i_background)
					i_background->runAction(Sequence::createWithTwoActions(EaseBackIn::create(ScaleTo::create(k_delayTime, 1.1f, 1.05f)), EaseBackOut::create(ScaleTo::create(k_delayTime, 1.0f))));
			}

			if (i == i_rowNum && j == i_colNum)
			{
				endFunction();
			}
		}),
		nullptr));

	i_animal->setOpacity(0);
	i_animal->runAction(Sequence::createWithTwoActions(
		DelayTime::create(currentTileNum*k_delayTime),
		FadeIn::create(k_showingTileTime)));
}

void AnimationManager::tilesShowingActionFadeFromTopAndBottomToCenter(Node * i_tile, Node * i_animal, Vec2 i_currentPosition, int i_rowNum, int i_colNum, int i, int j, int i_currentTileNum, std::function<void()> endFunction, Node * i_background)
{
	MK_RETURN_IF(i_tile == nullptr);
	MK_RETURN_IF(i_animal == nullptr);

	const float k_showingTileTime = 0.125f;

	int rowIndex = i;

	if (i > (i_rowNum / 2))
	{
		rowIndex -= i_rowNum;
		rowIndex *= -1;
		rowIndex += 1;
	}
	
	const float k_delayTime = (0.5f / (i_rowNum*0.5f));//max = 0.5s

	i_tile->setOpacity(0.0f);
	i_tile->runAction(Sequence::create(
		DelayTime::create(rowIndex*k_delayTime),
		FadeIn::create(k_showingTileTime),
		CallFunc::create([=]() {
		
		if (j % 2 == 0)
		{
			addParticleEffect(i_tile);
		}

		if (i == i_rowNum && j == i_colNum)
		{
			endFunction();
		}
	}),
		nullptr));

	i_animal->setOpacity(0);
	i_animal->runAction(Sequence::createWithTwoActions(
		DelayTime::create(rowIndex*k_delayTime),
		FadeIn::create(k_showingTileTime)));
}

void AnimationManager::tilesShowingActionFadeFromLeftToRight(Node * i_tile, Node * i_animal, Vec2 i_currentPosition, int i_rowNum, int i_colNum, int i, int j, int i_currentTileNum, std::function<void()> endFunction, Node * i_background)
{
	MK_RETURN_IF(i_tile == nullptr);
	MK_RETURN_IF(i_animal == nullptr);

	const float k_showingTileTime = 0.125f;
	
	const float k_delayTime = (0.5f/ i_colNum);

	i_tile->setOpacity(0.0f);
	i_tile->runAction(Sequence::create(
		DelayTime::create(j*k_delayTime),
		FadeIn::create(k_showingTileTime),
		CallFunc::create([=]() {

		if (j % 2 == 0)
		{
			addParticleEffect(i_tile);
		}

		if (i == i_rowNum && j == i_colNum)
		{
			endFunction();
		}
	}),
		nullptr));

	i_animal->setOpacity(0);
	i_animal->runAction(Sequence::createWithTwoActions(
		DelayTime::create(j*k_delayTime),
		FadeIn::create(k_showingTileTime)));

}

void AnimationManager::tilesShowingActionFadeFromRightToLeft(Node * i_tile, Node * i_animal, Vec2 i_currentPosition, int i_rowNum, int i_colNum, int i, int j, int i_currentTileNum, std::function<void()> endFunction, Node * i_background)
{
	MK_RETURN_IF(i_tile == nullptr);
	MK_RETURN_IF(i_animal == nullptr);

	const float k_showingTileTime = 0.125f;

	const float k_delayTime = (0.5f / i_colNum);

	i_tile->setOpacity(0.0f);
	i_tile->runAction(Sequence::create(
		DelayTime::create((i_colNum-j)*k_delayTime),
		FadeIn::create(k_showingTileTime),
		CallFunc::create([=]() {

		if (j % 2 == 0)
		{
			addParticleEffect(i_tile);
		}

		if (i == i_rowNum && j == i_colNum)
		{
			endFunction();
		}
	}),
		nullptr));

	i_animal->setOpacity(0);
	i_animal->runAction(Sequence::createWithTwoActions(
		DelayTime::create((i_colNum - j)*k_delayTime),
		FadeIn::create(k_showingTileTime)));
}

void AnimationManager::tilesShowingActionFadeFromTopToBottom(Node * i_tile, Node * i_animal, Vec2 i_currentPosition, int i_rowNum, int i_colNum, int i, int j, int i_currentTileNum, std::function<void()> endFunction, Node * i_background)
{
	MK_RETURN_IF(i_tile == nullptr);
	MK_RETURN_IF(i_animal == nullptr);

	const float k_showingTileTime = 0.125f;

	const float k_delayTime = (0.5f / i_rowNum);//max = 0.5s

	i_tile->setPosition(i_currentPosition);
	i_tile->setOpacity(0.0f);
	i_tile->runAction(Sequence::create(
		DelayTime::create(i*k_delayTime),
		FadeIn::create(k_showingTileTime),
		CallFunc::create([=]() {

			if (j % 2 == 0)
			{
				addParticleEffect(i_tile);
			}

			if (i == i_rowNum && j == i_colNum)
			{
				endFunction();
			}
		}),
		nullptr));

	i_animal->setOpacity(0);
	i_animal->runAction(Sequence::createWithTwoActions(
		DelayTime::create(i*k_delayTime),
		FadeIn::create(k_showingTileTime)));
}

Node * AnimationManager::addParticleEffectToNode(Node * i_parent, std::string i_particlePath, int i_zorder, std::string i_spriteFrameTexturePath)
{
	//auto particleContent = EncryptManager::getInstance()->getValueMapFromEncryptedFile(i_particlePath);

	//auto effect = ParticleSystemQuad::create(particleContent);
	//effect->setPosition(i_parent->getContentSize() / 2);
	////effect->setPositionType(ParticleSystem::PositionType::GROUPED);

	//if (!i_spriteFrameTexturePath.empty())
	//{
	//	effect->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(i_spriteFrameTexturePath));
	//}
	//i_parent->addChild(effect, i_zorder);

	//return effect;

	return nullptr;
}

void AnimationManager::addParticleEffectToNode(Node * i_parent, std::string i_particlePath, cocos2d::Vec2 i_pos, int i_zorder, std::string i_spriteFrameTexturePath)
{/*
	auto particleContent = EncryptManager::getInstance()->getValueMapFromEncryptedFile(i_particlePath);

	auto effect = ParticleSystemQuad::create(particleContent);
	effect->setPosition(i_pos);

	if (!i_spriteFrameTexturePath.empty())
	{
		effect->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(i_spriteFrameTexturePath));
	}
	i_parent->addChild(effect, i_zorder);*/
}

void AnimationManager::zoomInEffect(Node * i_node, std::function<void()> i_callback)
{
	MK_RETURN_IF_WITH_CONTENT(i_node == nullptr, , if (i_callback) i_callback(););

	const float k_currentScaleFactor = i_node->getScale();

	i_node->setVisible(false);

	i_node->setScale(0.0f*k_currentScaleFactor);

	i_node->runAction(Sequence::create(DelayTime::create(0.125f), Show::create(), EaseBackOut::create(ScaleTo::create(0.3f, 1.0f*k_currentScaleFactor)), CallFunc::create([=]() {
	
		if (i_callback) {
			i_callback();
		}

	}), nullptr));
}

void AnimationManager::zoomOutEffect(Node * i_node, std::function<void()> i_callback)
{
	MK_RETURN_IF_WITH_CONTENT(i_node == nullptr, , if (i_callback) i_callback(););

	auto spawn = Spawn::createWithTwoActions(FadeOut::create(0.1f), EaseBackOut::create(ScaleTo::create(0.3f, 0.0f)));

	i_node->runAction(Sequence::create(spawn, CallFunc::create([=]() {

		if (i_callback) {
			i_callback();
		}

	}), nullptr));
}

void AnimationManager::fadeInEffect(Node * i_node,std::function<void()> i_callback, float duration)
{
	MK_RETURN_IF_WITH_CONTENT(i_node == nullptr, , if (i_callback) i_callback(););

	const auto k_currentOpacity = i_node->getOpacity();

	if (auto widget = dynamic_cast<cocos2d::ui::Widget*>(i_node)) {

		widget->setEnabled(false);
	}

	i_node->setOpacity(0);

	i_node->runAction(Sequence::create(FadeTo::create(duration, k_currentOpacity), CallFunc::create([=]() {

		if (i_callback) {
			i_callback();
		}

		if (auto widget = dynamic_cast<cocos2d::ui::Widget*>(i_node)) {

			widget->setEnabled(true);
		}

	}), nullptr));
}

void AnimationManager::fadeInEffect(std::vector<Node*> i_nodes, std::function<void()> i_callback, float duration)
{
	MK_RETURN_IF_WITH_CONTENT(i_nodes.empty(), , if (i_callback) i_callback(););

	for (int i = 0; i < i_nodes.size(); i++) {

		if (auto item = i_nodes.at(i)) {

			fadeInEffect(item, i >= i_nodes.size() - 1 ? i_callback : nullptr, duration);
		}
	}
}

void AnimationManager::moveLeftEffect(Node * i_node, std::function<void()> i_callback, float duration)
{
	MK_RETURN_IF_WITH_CONTENT(i_node == nullptr, , if (i_callback) i_callback(););

	auto spawn = Spawn::createWithTwoActions(FadeOut::create(duration), Sequence::createWithTwoActions(
		EaseBackOut::create(MoveBy::create(duration*0.5f,Vec2(+_screenSize.width*0.1f,0))),
		EaseBackOut::create(MoveBy::create(duration*0.8f, Vec2(-_screenSize.width*1.0f, 0)))));

	i_node->runAction(Sequence::create(spawn, CallFunc::create([=]() {

		if (i_callback) {
			i_callback();
		}

	}), nullptr));
}

void AnimationManager::zoomInZoomOutEffect(Node * i_node, std::function<void()> i_callback, float duration)
{
	MK_RETURN_IF_WITH_CONTENT(i_node == nullptr, , if (i_callback) i_callback(););

	const float k_currentScaleFactor = i_node->getScale();

	const float k_duration = duration / 4.0f;

	i_node->runAction(Sequence::create(ScaleTo::create(k_duration, k_currentScaleFactor + 0.1f), ScaleTo::create(k_duration, k_currentScaleFactor), ScaleTo::create(k_duration, k_currentScaleFactor + 0.1f), ScaleTo::create(k_duration, k_currentScaleFactor), CallFunc::create([i_callback]() {
		if (i_callback) {
			i_callback();
		}
	}), nullptr));
}

void AnimationManager::zoomInZoomOutEffect(std::vector<Node*> i_nodes, std::function<void()> i_callback, float duration)
{
	MK_RETURN_IF_WITH_CONTENT(i_nodes.empty(), , if (i_callback) i_callback(););

	for (int i = 0; i < i_nodes.size(); i++) {

		if (auto item = i_nodes.at(i)) {

			zoomInZoomOutEffect(item, i >= i_nodes.size() - 1 ? i_callback : nullptr, duration);
		}
	}
}

void AnimationManager::fadeInAndZoomInEffectForNodes(std::vector<Node*> i_nodes , float fadeInZommInDuration , float fadeInDurationBetweenItems , std::function<void()> i_callback)
{
	MK_RETURN_IF_WITH_CONTENT(i_nodes.empty(), , if (i_callback) i_callback(););

	for (int i = 0; i < i_nodes.size(); i++) {

		if (auto item = i_nodes.at(i)) {

			if (auto  langbg = utils::findChild(item, "lang_bg")) {
				langbg->setLocalZOrder(100);
			}

			const auto k_currentScale = item->getScale();
			const auto k_currentOpacity = item->getOpacity();

			item->setOpacity(0);
			item->setScale(0.5f*k_currentScale);

			if (i >= i_nodes.size() - 1) { //end

				item->runAction(Sequence::create
				(
					DelayTime::create((i + 1)*(fadeInDurationBetweenItems / i_nodes.size())),
					EaseIn::create(Spawn::createWithTwoActions(FadeTo::create(fadeInZommInDuration, k_currentOpacity), ScaleTo::create(fadeInZommInDuration, 1.0f*k_currentScale)), 2),
					CallFunc::create([i_callback](){
				
						if (i_callback) {
							i_callback();
						}

					}),
					nullptr
				));

			} else {
				item->runAction(Sequence::createWithTwoActions
				(
					DelayTime::create((i + 1)*(fadeInDurationBetweenItems / i_nodes.size())),
					EaseIn::create(Spawn::createWithTwoActions(FadeTo::create(fadeInZommInDuration, k_currentOpacity), ScaleTo::create(fadeInZommInDuration, 1.0f*k_currentScale)), 2)
				));
			}
	
		}
	}
}

void AnimationManager::transitionSlideIn(Direction i_direction, float i_duration, std::vector<Node*> i_nodes, std::function<void()> i_callback)
{
	for (int i = 0; i < i_nodes.size(); i++) {

		transitionSlideIn(i_direction, i_duration, i_nodes.at(i), (i == i_nodes.size() -1) ? i_callback:nullptr);
	}
}

void AnimationManager::transitionSlideIn(Direction i_direction, float i_duration, std::vector<std::pair<std::vector<Node*>, float>> i_nodesAndDelaytime, std::function<void()> i_callback)
{
	MK_RETURN_IF_WITH_CONTENT( i_nodesAndDelaytime.empty(), ,if (i_callback) i_callback(););

	cocos2d::Vector<FiniteTimeAction*> actions;

    for (int i = 0; i <i_nodesAndDelaytime.size();i++ ) {

        auto info = i_nodesAndDelaytime.at(i);
        
        for (int j = 0; j < info.first.size(); j++) {
            
            if(auto node = info.first.at(j)) {
                
                if(!node) {
                    continue;
                }
                
                const auto k_currentPosition = node->getPosition();

                switch (i_direction)
                {
                case AnimationManager::Direction::LEFT:
                        node->setPositionX(k_currentPosition.x + _screenSize.width);
                    break;
                case AnimationManager::Direction::RIGHT:
                        node->setPositionX(k_currentPosition.x - _screenSize.width);
                    break;
                case AnimationManager::Direction::TOP:
                        node->setPositionY(k_currentPosition.y - _screenSize.height);
                    break;
                case AnimationManager::Direction::BOTTOM:
                        node->setPositionY(k_currentPosition.y + _screenSize.height);
                    break;
                default:
                    break;
                }

                auto moveAction = MoveTo::create(i_duration, k_currentPosition);

                if(i == i_nodesAndDelaytime.size()-1 && j == info.first.size() - 1) {
                    
                    auto sequenceAction    = Sequence::create
                    (
                        DelayTime::create(info.second*i),
                        EaseOut::create(moveAction, 2.0f),
                        CallFunc::create([=]() {
                            if (i_callback) {
                                i_callback();
                            }
                        }),
                        nullptr
                    );
                    node->runAction(sequenceAction);

                }  else {
                    auto sequenceAction    = Sequence::create
                    (
                        DelayTime::create(info.second*i),
                        EaseOut::create(moveAction, 2.0f),
                        nullptr
                    );
                    node->runAction(sequenceAction);
                }
            }
        }
	}
}

void AnimationManager::transitionSlideIn(Direction i_direction, float i_duration, Node * i_node, std::function<void()> i_callback)
{
	MK_RETURN_IF_WITH_CONTENT(i_node == nullptr, , if (i_callback) i_callback(););

	const auto k_currentPosition = i_node->getPosition();

	switch (i_direction)
	{
	case AnimationManager::Direction::LEFT:
		i_node->setPositionX(k_currentPosition.x + _screenSize.width);
		break;
	case AnimationManager::Direction::RIGHT:
		i_node->setPositionX(k_currentPosition.x - _screenSize.width);
		break;
	case AnimationManager::Direction::TOP:
		i_node->setPositionY(k_currentPosition.y - _screenSize.height);
		break;
	case AnimationManager::Direction::BOTTOM:
		i_node->setPositionY(k_currentPosition.y + _screenSize.height);
		break;
	default:
		break;
	}

	auto moveAction = MoveTo::create(i_duration, k_currentPosition);

	auto sequenceAction	= Sequence::create
	(
		EaseBackInOut::create(moveAction),
		CallFunc::create([=]() {
			if (i_callback) {
				i_callback();
			}
		}),
		nullptr
	);
	i_node->runAction(sequenceAction);
}

void AnimationManager::transitionSlideOut(Direction i_direction, float i_duration, std::vector<Node*> i_nodes, std::function<void()> i_callback)
{
	for (int i = 0; i < i_nodes.size(); i++) {

			transitionSlideOut(i_direction, i_duration, i_nodes.at(i), (i == i_nodes.size() - 1) ? i_callback : nullptr);
	}
}

void AnimationManager::transitionSlideOut(Direction i_direction, float i_duration, std::vector<std::pair<std::vector<Node*>, float>> i_nodesAndDelaytime, std::function<void()> i_callback)
{
    MK_RETURN_IF_WITH_CONTENT( i_nodesAndDelaytime.empty(), ,if (i_callback) i_callback(););

    cocos2d::Vector<FiniteTimeAction*> actions;

    for (int i = 0; i <i_nodesAndDelaytime.size();i++ ) {

        auto info = i_nodesAndDelaytime.at(i);
        
        for (int j = 0; j < info.first.size(); j++) {
            
            if(auto node = info.first.at(j)) {
                
                if(!node) {
                    continue;
                }
                
                ActionInterval* moveAction = nullptr;

                switch (i_direction)
                {
                case AnimationManager::Direction::LEFT:
                    moveAction = MoveTo::create(i_duration, Vec2(-_screenSize.width,0));
                    break;
                case AnimationManager::Direction::RIGHT:
                    moveAction = MoveBy::create(i_duration,  Vec2(+_screenSize.width, 0));
                    break;
                case AnimationManager::Direction::TOP:
                    moveAction = MoveBy::create(i_duration, Vec2(0, +_screenSize.height));
                    break;
                case AnimationManager::Direction::BOTTOM:
                    moveAction = MoveBy::create(i_duration, Vec2(0, -_screenSize.height));
                    break;
                default:
                    break;
                }

                if(i == i_nodesAndDelaytime.size()-1 && j == info.first.size() - 1) {
                    
                    auto sequenceAction = Sequence::create
                    (
                        DelayTime::create(info.second*i),
                        EaseOut::create(moveAction, 2.0f),
                        CallFunc::create([=]() {
                            if (i_callback) {
                                i_callback();
                            }
                        }),
                        nullptr
                    );
                    node->runAction(sequenceAction);

                } else {
                    auto sequenceAction    = Sequence::create
                    (
                        DelayTime::create(info.second*i),
                        EaseOut::create(moveAction, 2.0f),
                        nullptr
                    );
                    node->runAction(sequenceAction);
                }
            }
        }
    }
}

void AnimationManager::transitionSlideOut(Direction i_direction, float i_duration, Node * i_node, std::function<void()> i_callback)
{
	MK_RETURN_IF_WITH_CONTENT(i_node == nullptr, , if (i_callback) i_callback(););

	ActionInterval* moveAction = nullptr;

	switch (i_direction)
	{
	case AnimationManager::Direction::LEFT:
		moveAction = MoveBy::create(i_duration, Vec2(-_screenSize.width,0));
		break;
	case AnimationManager::Direction::RIGHT:
		moveAction = MoveBy::create(i_duration,  Vec2(+_screenSize.width, 0));
		break;
	case AnimationManager::Direction::TOP:
		moveAction = MoveBy::create(i_duration, Vec2(0, +_screenSize.height));
		break;
	case AnimationManager::Direction::BOTTOM:
		moveAction = MoveBy::create(i_duration, Vec2(0, -_screenSize.height));
		break;
	default:
		break;
	}

	if (moveAction) {

		auto sequenceAction = Sequence::create
		(
			EaseOut::create(moveAction, 2.0f),
			CallFunc::create([=]() {
			if (i_callback) {
				i_callback();
			}
		}),
			nullptr
			);
		i_node->runAction(sequenceAction);

	} else {
		if (i_callback) {
			i_callback();
		}
	}
}

void AnimationManager::popupCustomAnimation(cocos2d::Node* popupBackground,  ActionType actionType, float actionTime, std::function<void()> callback ) {

	MK_RETURN_IF(!popupBackground);

	if (actionType == ActionType::SHOW) {

		popupBackground->setRotation(70.0f);
		popupBackground->setOpacity(0.0f);

		auto spawnAction = Spawn::createWithTwoActions(FadeIn::create(actionTime), EaseBackInOut::create(RotateTo::create(actionTime, 0.0f)));

		popupBackground->runAction(Sequence::createWithTwoActions(spawnAction, CallFunc::create([=]() {

			if (callback) {
				callback();
			}

		})));
	}
	else
	{

		auto spawnAction = Spawn::create(FadeOut::create(actionTime), EaseBackInOut::create(RotateTo::create(actionTime, -180.0f)), ScaleTo::create(actionTime, 0.0f), nullptr);

		popupBackground->runAction(Sequence::createWithTwoActions(spawnAction, CallFunc::create([=]() {

			if (callback) {
				callback();
			}

		})));
	}
}

Animation* AnimationManager::createAnimation(const char* prefixName, int start, int end, float delay, std::string lastFrame)
{
	Vector<SpriteFrame*> animFrames;
	
	for (int i = start; i <= end; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(prefixName, i)->getCString());

		if (frame != nullptr)
		{
			animFrames.pushBack(frame);
		}
	}
	
	animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(lastFrame));

	return Animation::createWithSpriteFrames(animFrames, delay);
}

cocos2d::Animation * AnimationManager::createAnimationWithLoop(const char * prefixName, int start, int end, float delay)
{
	Vector<SpriteFrame*> animFrames;

	for (int i = start; i <= end; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(prefixName, i)->getCString());
		
		if (frame != nullptr)
		{
			animFrames.pushBack(frame);
		}
	}

	auto resultAni = Animation::createWithSpriteFrames(animFrames, delay);
	
	resultAni->setLoops(-1);

	return resultAni;
}

Animation* AnimationManager::createAnimation(const char* prefixName, int start, int end, float delay, int loop )
{
	Vector<SpriteFrame*> animFrames;

	for (int i = start; i <= end; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(prefixName, i)->getCString());
		
		if (frame != nullptr) {
			animFrames.pushBack(frame);
		} else {
			auto sprite = Sprite::create(String::createWithFormat(prefixName, i)->getCString());
			animFrames.pushBack(sprite->getSpriteFrame());
		}
	}

	auto resultAni = Animation::createWithSpriteFrames(animFrames, delay);

	resultAni->setLoops(loop);

	return resultAni;
}

void AnimationManager::initShockwave2Animation()
{
	if (auto animation = AnimationCache::getInstance()->getAnimation("shockwave2")) {
		return;
	}
	auto animation = AnimationManager::GetInstance()->createAnimation("shockwave2/sk_%02d.png", 1, 23, 0.75f / 23.0f, 1);
	AnimationCache::getInstance()->addAnimation(animation, "shockwave2");
}

void AnimationManager::initMonkeyLoadingAnimation()
{
	if (auto animation = AnimationCache::getInstance()->getAnimation("monkey_loading_animation")) {
		return;
	}

	AnimationCache::getInstance()->addAnimation(createAnimationWithLoop("loading/load_monkey_%d.png", 0, 7, 0.1f), "monkey_loading_animation");//Animation
}
void AnimationManager::loadingprogressAnimation() {
    
    const std::string k_loading_progress_animation = "loading_progress_animation";

    auto animation = AnimationCache::getInstance()->getAnimation(k_loading_progress_animation);
    
    if(animation) {
        return;
    }
    
	auto loading_progress_animation = Animation::create();
	loading_progress_animation->setDelayPerUnit(0.06f);
	loading_progress_animation->setLoops(-1);

	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading1.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading2.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading3.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading4.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading5.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading6.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading7.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading8.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading9.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading10.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading11.png")->getSpriteFrame());
	loading_progress_animation->addSpriteFrame(cocos2d::Sprite::create("profile_parent/learning_progress/loading12.png")->getSpriteFrame());
	
	AnimationCache::getInstance()->addAnimation(loading_progress_animation, k_loading_progress_animation);
}

void AnimationManager::initHintEffect()
{
	AnimationCache::getInstance()->addAnimation(createAnimationWithLoop("effects/selected_effect_%02d.png", 1, 9, 0.1f),"hintEffect");
}

void AnimationManager::initButterfly()
{
	Vector<SpriteFrame*> animFrames;

	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("effects/butterfly/01.png"); animFrames.pushBack(frame);
	 frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("effects/butterfly/02.png"); animFrames.pushBack(frame);
	 frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("effects/butterfly/03.png"); animFrames.pushBack(frame);
	 frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("effects/butterfly/02.png"); animFrames.pushBack(frame);

	auto resultAni = Animation::createWithSpriteFrames(animFrames, 0.1f);

	resultAni->setLoops(-1);

	AnimationCache::getInstance()->addAnimation(resultAni, "butterfly01");
}

void AnimationManager::initLightningShockwave()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("effects/clicked_eff/sk_%02d.png", 00, 22, 0.04f, 1), "effect03");
}

void AnimationManager::initLightning02()
{
	Vector<SpriteFrame*> animFrames;

	for (int i = 1; i <= 22; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("effects/lightning02/lightning_%02d.png", i)->getCString());

		if (frame != nullptr)
		{
			animFrames.pushBack(frame);
		}
	}

	//animFrames.pushBack(Sprite::create()->getSpriteFrame());
	//animFrames.pushBack(Sprite::create()->getSpriteFrame());
	//animFrames.pushBack(Sprite::create()->getSpriteFrame());
	//animFrames.pushBack(Sprite::create()->getSpriteFrame());
	//animFrames.pushBack(Sprite::create()->getSpriteFrame());

	auto resultAni = Animation::createWithSpriteFrames(animFrames, 1.0f/25.0f);

	resultAni->setLoops(-1);

	AnimationCache::getInstance()->addAnimation(resultAni, "lightning02Effect");

}

void AnimationManager::initLoadingCircle()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("circle_loading_bar_%02d.png", 1, 2, 0.1f, -1), "CircleLoadingBar");
}

void AnimationManager::addParticleEffect(Node *i_parent)
{
	//auto particleContent = EncryptManager::getInstance()->getValueMapFromEncryptedFile("effects/particle_texture.plist");

	//auto effect = ParticleSystemQuad::create(particleContent);
	//effect->setPosition(i_parent->getContentSize() / 2);
	//effect->setPositionType(ParticleSystem::PositionType::GROUPED);
	//effect->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("particle/iap_eff_texture.png"));
	//i_parent->addChild(effect, 100);
}

