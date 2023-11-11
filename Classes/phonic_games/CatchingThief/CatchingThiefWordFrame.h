#pragma once

#include "cocos2d.h"

namespace CatchingThief {
	class WordFrame : public cocos2d::Node {
	public:
		static WordFrame* createWithLetters(const std::vector<std::string> &letters);
		virtual bool initWithLetters(const std::vector<std::string> &letters);
		const std::vector<cocos2d::Label*>& getLetterNodes();
		void clearLetterNodes();
		void reveal();
	protected:
		WordFrame();
		void createLetterPair(cocos2d::Node* head, cocos2d::Node* tail, int offset = 1);
	private:
		cocos2d::Node* m_letterWrapperNode;
		std::vector<cocos2d::Label*> m_letterNodes;
		int revealIndex;
	};
}