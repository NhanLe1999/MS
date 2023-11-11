#pragma once

#include "cocos2d.h"
#include "GameData.h"

namespace WizardCauldron_VM {
	class WordFrame_VM : public cocos2d::Node {
	public:
		static WordFrame_VM* createWithLetters(const std::vector<std::string> &letters, bool co_dau = false, int pos_dau = -1);
		static WordFrame_VM* createWithLetters(const std::vector<mj::GameObject> &objs, bool co_dau = false,int pos_dau = -1);
		virtual bool initWithLetters(const std::vector<std::string> &letters);
		const std::vector<cocos2d::Label*>& getLetterNodes();
		void clearLetterNodes();
		void reveal();
	protected:
		WordFrame_VM();
		void createLetterPair(cocos2d::Node* head, cocos2d::Node* tail, int offset = 1);
	private:
        bool _co_dau;
        int _pos_dau;
		cocos2d::Node* m_letterWrapperNode;
		std::vector<cocos2d::Label*> m_letterNodes;
        std::vector<cocos2d::Sprite*> unders;
		int revealIndex;
	};
}
