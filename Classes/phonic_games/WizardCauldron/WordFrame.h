#pragma once

#include "cocos2d.h"
#include "GameData.h"

namespace WizardCauldron {
	class WordFrame : public cocos2d::Node {
	public:
		static WordFrame* createWithLetters(const std::vector<std::string> &letters, bool hideText = false);
		static WordFrame* createWithLetters(const std::vector<mj::GameObject>& objs, bool hideText = false);
		virtual bool initWithLetters(const std::vector<std::string> &letters, bool hideText = false);
		const std::vector<cocos2d::Label*>& getLetterNodes();
		void clearLetterNodes();
		void reveal();
	protected:
		WordFrame();
		void createLetterPair(cocos2d::Node* head, cocos2d::Node* tail, int offset = 1);
	private:
		int _max;
		bool _hideText;
		cocos2d::Node* m_letterWrapperNode;
		std::vector<cocos2d::Label*> m_letterNodes;
		int revealIndex;
	};
}