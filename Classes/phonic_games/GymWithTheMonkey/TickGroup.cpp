#include "TickGroup.h"

bool TickGroup::init()
{
	if (!cocos2d::Node::init())
		return false;
    return setNumber(0);
}

bool TickGroup::pushTick()
{
	if (m_tickCount >= m_numberOfTicks)
		return false;

	auto tick = m_ticks[m_tickCount];
	auto facetick = m_faceticks[m_tickCount];
	facetick->playSequence("face_action");

	int column = (int)ceil(m_numberOfTicks / 3.0f);
	switch (m_tickCount/column)
	{
	case 0:
		tick->playSequence("bluelight");
		break;
	case 1:
		tick->playSequence("greenlight");
		break;
	case 2:
		tick->playSequence("redlight");
		break;
	default:
		break;
	}

	m_tickCount++;
	return true;
}

void TickGroup::resetTicks()
{
	for (int i = 0; i < m_numberOfTicks; i++) {
		auto tick = m_ticks[i];
		auto facetick = m_faceticks[i];
		int column = (int) ceil(m_numberOfTicks / 3.0f);
		switch (i / column)
		{
		case 0:
			tick->playSequence("blue");
			break;
		case 1:
			tick->playSequence("green");
			break;
		case 2:
			tick->playSequence("red");
			break;
		default:
			break;
		}
		facetick->playSequence("face_udle");
	}
	m_tickCount = 0;
}

int TickGroup::getTickCount()
{
	return m_tickCount;
}

int TickGroup::getNumberOfTicks()
{
	return m_numberOfTicks;
}

bool TickGroup::isFull()
{
	return m_tickCount >= m_numberOfTicks;
}

bool TickGroup::setNumber(int number){
    
    /*for(auto c: this->getChildren()){
        c->removeFromParent();
    }*/

	this->removeAllChildren();
    m_ticks.clear();
	m_faceticks.clear();
    m_tickCount = number;
    
    const float offset = 135.0f;
    const cocos2d::Vec2 basePos(-offset, offset);
    m_numberOfTicks = number;
    int column = (int)ceil(number / 3.0f);
    
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < column; i++) {
            // init
			//auto tickBtnSprite = cocos2d::Sprite::create();
			auto tickBtn = GAFWrapper::createGAF("gaf/gymwiththemonkey/gym_radiobtns/gym_radiobtns.gaf");
			if (!tickBtn)
                return false;
			tickBtn->setPosition(cocos2d::Vec2(i * offset + basePos.x, basePos.y - j * offset));
            this->addChild(tickBtn);

			auto facetick = GAFWrapper::createGAF("gaf/gymwiththemonkey/gym_radiobtns/gym_radiobtns.gaf");
			if (!facetick)
                return false;
			facetick->setPosition(cocos2d::Vec2(i * offset + basePos.x, basePos.y - j * offset));
            this->addChild(facetick);
            
            m_ticks.push_back(tickBtn);
			m_faceticks.push_back(facetick);

			// show
			switch (j)
			{
			case 0: 
				tickBtn->playSequence("blue");
				break;
			case 1:
				tickBtn->playSequence("green");
				break;
			case 2:
				tickBtn->playSequence("red");
				break;
			default:
				break;
			}
			facetick->playSequence("face_udle");

			if (m_ticks.size() == m_numberOfTicks) break;
		}
    }
    
    m_tickCount = 0;
    return true;
}
