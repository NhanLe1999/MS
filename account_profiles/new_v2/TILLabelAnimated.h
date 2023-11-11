#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class TILLabelBMFontAnimated : public cocos2d::Label 
{

public:

	// ONLY USE THIS FUNCTION FOR CREATION
	/*static Label * createWithTTF(const std::string& text, const std::string& fontFilePath, float fontSize,
		const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
		TextVAlignment vAlignment = TextVAlignment::TOP);*/

	static TILLabelBMFontAnimated* createWithBMFont(const std::string& bmfontFilePath, const std::string& text, const cocos2d::TextHAlignment& alignment  = TextHAlignment::LEFT, int maxLineWidth  = 0 , const cocos2d::Vec2& imageOffset  = Vec2::ZERO );
	static TILLabelBMFontAnimated* createWithTTF(const std::string& text, const std::string& fontFile, float fontSize, const cocos2d::Size& dimensions = cocos2d::Size::ZERO, cocos2d::TextHAlignment hAlignment = cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment vAlignment  = cocos2d::TextVAlignment::TOP );

	//FUNCTIONS TO SET BASIC CHARACTER SPRITE PROPERTIES AT INDEX
	void setCharScale(int index, float s);
	void setCharOpacity(int index, float o);
	void setCharRotation(int index, float r);

	//FUNCTIONS TO SET BASIC PROPERTIES OF ALL CHARACTER SPRITES
	void setAllCharsScale(float s);
	void setAllCharsOpacity(float o);
	void setAllCharsRotation(float r);
	void offsetAllCharsPositionBy(cocos2d::Point offset);

	//FUNCTIONS TO RUN CUSTOM ACTIONS ON CHARATER SPRITES
	void runActionOnSpriteAtIndex(int index, cocos2d::FiniteTimeAction* action);

	void runActionOnAllSprites(cocos2d::Action* action);
	void runActionOnAllSprites(cocos2d::Action* action, bool removeOnCompletion);
	void runActionOnAllSprites(cocos2d::Action* action, bool removeOnCompletion, cocos2d::CallFunc *callFuncOnCompletion);
	void stopActionsOnAllSprites();

	//for the 'run actions sequentially' functions, duration refers to the total time to complete actions on all letters, minus the duration of the action itself
	void runActionOnAllSpritesSequentially(cocos2d::FiniteTimeAction* action, float duration, bool removeOnCompletion, cocos2d::CallFunc *callFuncOnCompletion);
	void runActionOnAllSpritesSequentially(cocos2d::FiniteTimeAction* action, float duration, bool removeOnCompletion);
	void runActionOnAllSpritesSequentially(cocos2d::FiniteTimeAction* action, float duration);

	void runActionOnAllSpritesSequentiallyReverse(cocos2d::FiniteTimeAction* action, float duration, bool removeOnCompletion, cocos2d::CallFunc *callFuncOnCompletion);
	void runActionOnAllSpritesSequentiallyReverse(cocos2d::FiniteTimeAction* action, float duration, bool removeOnCompletion);
	void runActionOnAllSpritesSequentiallyReverse(cocos2d::FiniteTimeAction* action, float duration);

	//ANIMATIONS

	//fly ins
	void animateInFlyInFromLeft(float duration);
	void animateInFlyInFromRight(float duration);
	void animateInFlyInFromTop(float duration);
	void animateInFlyInFromBottom(float duration);

	//misc animate ins
	void animateInTypewriter(float duration);
	void animateInDropFromTop(float duration);
	void animateInSwell(float duration);
	void animateInRevealFromLeft(float duration);
	void animateInSpin(float duration, int spins);
	void animateInVortex(float duration, int spins);

	//misc animations
	void animateSwell(float duration);
	void animateJump(float duration, float height);
	void animateJumpForever(float duration, float height);
	void animateStretchElastic(float stretchDuration, float releaseDuration, float stretchAmount);
	void animateRainbow(float duration);
	void flyPastAndRemove();


	void setStrikeProperties(Color4F i_color,float i_width)
	{
		if (_underlineNode)
		{
			m_strikeWidth = i_width;
			m_strikeColor = i_color;
		}
	}

	void updateContent() override
	{
		Label::updateContent();

		if (_underlineNode)
		{
			 _underlineNode->clear();

			if (_numberOfLines)
			{
				// This is the logic for TTF fonts
				const float charheight = (_textDesiredHeight / _numberOfLines);
				_underlineNode->setLineWidth(m_strikeWidth);

				// atlas font
				for (int i = 0; i < _numberOfLines; ++i)
				{
					float offsety = 0;
					if (_strikethroughEnabled)
						offsety += charheight / 2;
					// FIXME: Might not work with different vertical alignments
					float y = (_numberOfLines - i - 1) * charheight + offsety;

					// Github issue #15214. Uses _displayedColor instead of _textColor for the underline.
					// This is to have the same behavior of SystemFonts.
					_underlineNode->drawLine(Vec2(_linesOffsetX[i], y), Vec2(_linesWidth[i] + _linesOffsetX[i], y), m_strikeColor);
				}
			}
			else if (_textSprite)
			{
				// ...and is the logic for System fonts
				float y = 0;
				const auto spriteSize = _textSprite->getContentSize();
				_underlineNode->setLineWidth(spriteSize.height / 6);

				if (_strikethroughEnabled)
					// FIXME: system fonts don't report the height of the font correctly. only the size of the texture, which is POT
					y += spriteSize.height / 2;
				// FIXME: Might not work with different vertical alignments
				_underlineNode->drawLine(Vec2(0, y), Vec2(spriteSize.width, y), m_strikeColor);
			}
			
		}
	}

private:

	float m_strikeWidth = 2.0f;
	Color4F m_strikeColor;
	int numLetters();

	void animateInVortex(bool removeOnCompletion, bool createGhosts, float duration, int spins);
};

