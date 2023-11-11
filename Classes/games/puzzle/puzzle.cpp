#include "puzzle.h"

#define bg_sound "sounds/puzzle/pz_bgm2.mp3"
#define intro_sound "sounds/puzzle/JP.mp3"
#define vnflag_sound "sounds/puzzle/flag.mp3"
#define vnmap_sound "sounds/puzzle/vnmap.mp3"
#define truongsa_sound "sounds/puzzle/truongsa.mp3"
#define guide_sound "sounds/puzzle/guide.mp3"
#define split_sound "sounds/puzzle/pz_split.mp3"
#define join_sound "sounds/puzzle/pz_join2.mp3"
#define wrong_sound "sounds/puzzle/pz_wrong.mp3"
#define correct_sound "sounds/puzzle/pz_correct2.mp3"
#define interact_sound "sounds/puzzle/pz_interact.mp3"
#define showup_sound "sounds/puzzle/pz_fadein.mp3"
#define getout_sound "sounds/puzzle/pz_fadeout.mp3"
#define goodjob_sound "sounds/puzzle/gj.mp3"

#define effect_gaf "gaf/puzzle/effec/effec.gaf"
#define firewo_gaf "gaf/puzzle/phaohoa/phaohoa.gaf"

INITIALIZE_READER(Puzzle);

std::once_flag Puzzle_reader;

Puzzle * Puzzle::createGame() {
	std::call_once(Puzzle_reader, [] {
		REGISTER_CSB_READER(Puzzle);		
	});
	Puzzle* ans = static_cast<Puzzle*>(CSLoader::createNode("csb/game/puzzle/puzzle.csb"));
	if (ans->init()) {
		ans->makeGame();
		return ans;
	}
	CC_SAFE_DELETE(ans);
	return nullptr;
}

void Puzzle::introduction() {
	
	auto gameName = ui::ImageView::create(this->searchFolder + "puzzlefun.png");
	gameName->setPosition(this->rootNode->getContentSize() * 0.5);
	gameName->setContentSize(Size(this->rootNode->getContentSize().width * 0.6, this->rootNode->getContentSize().height * 0.2));
	gameName->setOpacity(0);
	this->rootNode->addChild(gameName);

	gameName->runAction(
		Sequence::create(
			CallFunc::create([=]() {
				AUDIO_ENGINE->playEffect(intro_sound);
				gameName->runAction(FadeIn::create(1.0f));
			}),
			DelayTime::create(2.0f),
			CallFunc::create([=]() {
				AUDIO_ENGINE->playEffect(guide_sound);
			}),
			DelayTime::create(3.5f),
			FadeOut::create(1.0f),
			CallFunc::create([=]() {
				gameName->removeFromParent();
				this->triggerStart();
			}),
			nullptr
		)
	);

}

ui::ImageView* Puzzle::createImage(std::string fileName) {

	ui::ImageView* ans = ui::ImageView::create(this->searchFolder + fileName);	

	auto originalSize = ans->getContentSize();
	this->originalImageContentSize = originalSize;

	float Width, Height;

	if (this->level == 0) {
		Width = this->rootNode->getContentSize().width * 0.55;
		Height = Width * originalSize.height / originalSize.width;
	}
	else if (this->level == 1) {
		Height = this->rootNode->getContentSize().height * 0.6;
		Width = Height * originalSize.width / originalSize.height;
	}
	else {
		Width = this->rootNode->getContentSize().width * 0.45;
		Height = Width * originalSize.height / originalSize.width;
	}
	
	ans->setPosition(this->rootNode->getContentSize() * 0.5);
	ans->setScale(Width / originalSize.width);
	ans->setAnchorPoint(Vec2(0.5, 0.5));
	ans->setPosition(Vec2(Width * 0.5, Height * 0.5));

	this->targetSize = Size(Width, Height);

	return ans;
}

cocos2d::Vector<cocos2d::FiniteTimeAction*> Puzzle::makeThisSize(cocos2d::Size _size1, cocos2d::Size _size2, float duration, ui::ImageView* imageThis) {
	Vector<FiniteTimeAction*> ans;
	int numFrame = duration * 30;
	float eachWidth = (_size2.width - _size1.width) / numFrame;
	float eachHeight = (_size2.height - _size1.height) / numFrame;
	for (int i = 0; i < numFrame; i++) {
		auto _bigger = CallFunc::create([=]() {
			imageThis->setContentSize(_size1 + Size((i + 1) * eachWidth, (i + 1) * eachHeight));
		});
		ans.pushBack(_bigger);
		ans.pushBack(DelayTime::create(duration / numFrame));
	}
	return ans;
}

cocos2d::Vector<cocos2d::FiniteTimeAction*> Puzzle::makeThisSize(cocos2d::Size _size1, cocos2d::Size _size2, float duration, PartChoice* imageThis) {
	Vector<FiniteTimeAction*> ans;
	int numFrame = duration * 30;
	float eachWidth = (_size2.width - _size1.width) / numFrame;
	float eachHeight = (_size2.height - _size1.height) / numFrame;
	for (int i = 0; i < numFrame; i++) {
		auto _bigger = CallFunc::create([=]() {
			imageThis->makeSize(_size1 + Size((i + 1) * eachWidth, (i + 1) * eachHeight));
		});
		ans.pushBack(_bigger);
		ans.pushBack(DelayTime::create(duration / numFrame));
	}
	return ans;
}

void Puzzle::initImages(cocos2d::Size _size) {
	

	this->vecChoices.clear();	

	if (this->level == 0) {		

		for (int i = 1; i <= 4; i++) {
            std::string name = this->searchFolder + StringUtils::toString(i) + ".png";
			PartChoice* choice = PartChoice::createView();
			choice->initImage(Size(_size.width * 0.5f, _size.height * 0.5f), name);
			choice->initPuzzleDelegate(this);
			this->vecChoices.pushBack(choice);
		}

	}
	else
		if (this->level == 1) {
			for (int i = 6; i <= 11; i++) {
				std::string name = this->searchFolder + StringUtils::toString(i) + ".png";
				PartChoice* choice = PartChoice::createView();
				choice->initImage(Size(_size.width * 0.5f, _size.height * 1.0f / 3), name);
				choice->initPuzzleDelegate(this);
				this->vecChoices.pushBack(choice);
			}
		}
		else {
			for (int i = 13; i <= 18; i++) {
				std::string name = this->searchFolder + StringUtils::toString(i) + ".png";
				PartChoice* choice = PartChoice::createView();
				choice->initImage(Size(_size.width * 1.0f / 3, _size.height * 1.0f / 2), name);
				choice->initPuzzleDelegate(this);
				this->vecChoices.pushBack(choice);
			}
		}
}

std::vector<std::pair<float, float> > Puzzle::getPositions(cocos2d::Size _size) {
	std::vector<std::pair<float, float> > ans;
	if (this->level == 0) {
		std::pair<float, float> pos;
		pos = make_pair(_size.width * 0, _size.height * 0.5f); 
		ans.push_back(pos);
		pos = make_pair(_size.width * 0.5f, _size.height * 0.5f); 
		ans.push_back(pos);
		pos = make_pair(_size.width * 0, _size.height * 0); 
		ans.push_back(pos);
		pos = make_pair(_size.width * 0.5f, _size.height * 0); 
		ans.push_back(pos);
	}
	else
		if (this->level == 1) {
			std::pair<float, float> pos;
			pos = make_pair(_size.width * 0, _size.height * 2.0f / 3);
			ans.push_back(pos);
			pos = make_pair(_size.width * 0.5f, _size.height * 2.0f / 3);
			ans.push_back(pos);
			pos = make_pair(_size.width * 0, _size.height * 1.0f / 3);
			ans.push_back(pos);
			pos = make_pair(_size.width * 0.5f, _size.height * 1.0f / 3);
			ans.push_back(pos);
			pos = make_pair(_size.width * 0, _size.height * 0.0f / 3);
			ans.push_back(pos);
			pos = make_pair(_size.width * 0.5f, _size.height * 0.0f / 3);
			ans.push_back(pos);
		}
		else {
			std::pair<float, float> pos;
			pos = make_pair(_size.width * 0, _size.height * 0.5f);
			ans.push_back(pos);
			pos = make_pair(_size.width * 1.0f / 3, _size.height * 0.5f);
			ans.push_back(pos);
			pos = make_pair(_size.width * 2.0f / 3, _size.height * 0.5f);
			ans.push_back(pos);
			pos = make_pair(_size.width * 0, _size.height * 0);
			ans.push_back(pos);
			pos = make_pair(_size.width * 1.0f / 3, _size.height * 0);
			ans.push_back(pos);
			pos = make_pair(_size.width * 2.0f / 3, _size.height * 0);
			ans.push_back(pos);
		}
	return ans;
}

void Puzzle::makeLines(cocos2d::ui::ImageView* parent, cocos2d::Size targetSize) {
	float lineWidth = 1.05 * CC_CONTENT_SCALE_FACTOR();
	if (this->level == 0) {		
		DrawNode* line = DrawNode::create();
		line->drawSegment(Point(targetSize.width * 0.5, 0), Point(targetSize.width * 0.5, targetSize.height), lineWidth, Color4F::ORANGE);
		parent->addChild(line);
		line->setOpacity(0);
		line->runAction(FadeIn::create(1.0f));
		DrawNode* line2 = DrawNode::create();
		line2->drawSegment(Point(0, targetSize.height * 0.5), Point(targetSize.width, targetSize.height * 0.5), lineWidth, Color4F::ORANGE);
		parent->addChild(line2);
		line2->setOpacity(0);
		line2->runAction(FadeIn::create(1.0f));		
	}
	else
		if (this->level == 1) {
			DrawNode* line = DrawNode::create();
			line->drawSegment(Point(targetSize.width * 0.5, 0), Point(targetSize.width * 0.5, targetSize.height), lineWidth, Color4F::ORANGE);
			parent->addChild(line);
			line->setOpacity(0);
			line->runAction(FadeIn::create(1.0f));
			DrawNode* line2 = DrawNode::create();
			line2->drawSegment(Point(0, targetSize.height * 1.0f / 3), Point(targetSize.width, targetSize.height * 1.0f / 3), lineWidth, Color4F::ORANGE);
			parent->addChild(line2);
			line2->setOpacity(0);
			line2->runAction(FadeIn::create(1.0f));
			DrawNode* line3 = DrawNode::create();
			line3->drawSegment(Point(0, targetSize.height * 2.0f / 3), Point(targetSize.width, targetSize.height * 2.0f / 3), lineWidth, Color4F::ORANGE);
			parent->addChild(line3);
			line3->setOpacity(0);
			line3->runAction(FadeIn::create(1.0f));
		}
		else {
			DrawNode* line = DrawNode::create();
			line->drawSegment(Point(targetSize.width * 1.0f / 3, 0), Point(targetSize.width * 1.0f / 3, targetSize.height), lineWidth, Color4F::ORANGE);
			parent->addChild(line);
			line->setOpacity(0);
			line->runAction(FadeIn::create(1.0f));
			DrawNode* line3 = DrawNode::create();
			line->drawSegment(Point(targetSize.width * 2.0f / 3, 0), Point(targetSize.width * 2.0f / 3, targetSize.height), lineWidth, Color4F::ORANGE);
			parent->addChild(line3);
			line3->setOpacity(0);
			line3->runAction(FadeIn::create(1.0f));
			DrawNode* line2 = DrawNode::create();
			line2->drawSegment(Point(0, targetSize.height * 0.5), Point(targetSize.width, targetSize.height * 0.5), lineWidth, Color4F::ORANGE);
			parent->addChild(line2);
			line2->setOpacity(0);
			line2->runAction(FadeIn::create(1.0f));
		}
}

void Puzzle::makeLines(cocos2d::ui::Layout* parent, cocos2d::Size targetSize) {
	this->vecDraws.clear();
	float lineWidth = 0.5 * CC_CONTENT_SCALE_FACTOR();
	if (this->level == 0) {
		DrawNode* line = DrawNode::create();
		line->drawSegment(Point(targetSize.width * 0.5, 0), Point(targetSize.width * 0.5, targetSize.height), lineWidth, Color4F::ORANGE);
		parent->addChild(line);
		line->setOpacity(0);
		line->runAction(FadeIn::create(1.0f));
		DrawNode* line2 = DrawNode::create();
		line2->drawSegment(Point(0, targetSize.height * 0.5), Point(targetSize.width, targetSize.height * 0.5), lineWidth, Color4F::ORANGE);
		parent->addChild(line2);
		line2->setOpacity(0);
		line2->runAction(FadeIn::create(1.0f));
		this->vecDraws.pushBack(line);
		this->vecDraws.pushBack(line2);
	}
	else
		if (this->level == 1) {
			DrawNode* line = DrawNode::create();
			line->drawSegment(Point(targetSize.width * 0.5, 0), Point(targetSize.width * 0.5, targetSize.height), lineWidth, Color4F::ORANGE);
			parent->addChild(line);
			line->setOpacity(0);
			line->runAction(FadeIn::create(1.0f));
			DrawNode* line2 = DrawNode::create();
			line2->drawSegment(Point(0, targetSize.height * 1.0f / 3), Point(targetSize.width, targetSize.height * 1.0f / 3), lineWidth, Color4F::ORANGE);
			parent->addChild(line2);
			line2->setOpacity(0);
			line2->runAction(FadeIn::create(1.0f));
			DrawNode* line3 = DrawNode::create();
			line3->drawSegment(Point(0, targetSize.height * 2.0f / 3), Point(targetSize.width, targetSize.height * 2.0f / 3), lineWidth, Color4F::ORANGE);
			parent->addChild(line3);
			line3->setOpacity(0);
			line3->runAction(FadeIn::create(1.0f));
			this->vecDraws.pushBack(line);
			this->vecDraws.pushBack(line2);
			this->vecDraws.pushBack(line3);
		}
		else {
			DrawNode* line = DrawNode::create();
			line->drawSegment(Point(targetSize.width * 1.0f / 3, 0), Point(targetSize.width * 1.0f / 3, targetSize.height), lineWidth, Color4F::ORANGE);
			parent->addChild(line);
			line->setOpacity(0);
			line->runAction(FadeIn::create(1.0f));
			DrawNode* line3 = DrawNode::create();
			line->drawSegment(Point(targetSize.width * 2.0f / 3, 0), Point(targetSize.width * 2.0f / 3, targetSize.height), lineWidth, Color4F::ORANGE);
			parent->addChild(line3);
			line3->setOpacity(0);
			line3->runAction(FadeIn::create(1.0f));
			DrawNode* line2 = DrawNode::create();
			line2->drawSegment(Point(0, targetSize.height * 0.5), Point(targetSize.width, targetSize.height * 0.5), lineWidth, Color4F::ORANGE);
			parent->addChild(line2);
			line2->setOpacity(0);
			line2->runAction(FadeIn::create(1.0f));
			this->vecDraws.pushBack(line);
			this->vecDraws.pushBack(line2);
			this->vecDraws.pushBack(line3);
		}
}

std::vector<std::pair<float, float> > Puzzle::getMoveSize(cocos2d::Size _size, cocos2d::Vec2 pos, cocos2d::Size _oSize) {
	std::vector<std::pair<float, float> > ans;
	if (this->level == 0) {
		float trailing = pos.y - _size.height * 0.5;
		int num = this->numChoices + 1;
		float eachBlank = this->rootNode->getContentSize().width * 4.0f / 100;
		float eachImg = (this->rootNode->getContentSize().width - num * eachBlank) / this->numChoices;
		float height = eachImg * _oSize.height / _oSize.width;
		height = min(height, trailing * 0.8f);
		eachImg = height * _oSize.width / _oSize.height;
		for (int i = 0; i < this->numChoices; i++) {			
			ans.push_back(make_pair(eachImg, height));
		}
	}
	else
		if (this->level == 1) {
			float trailing = pos.x - _size.width * 0.5;
			int num = 4;						
			float eachImg = (pos.x - _size.width * 0.5) * 0.8;
			float height = eachImg * _oSize.height / _oSize.width;			
			height = min(height, _size.height * 0.3f);
			eachImg = height * _oSize.width / _oSize.height;
			for (int i = 0; i < this->numChoices; i++) {
				ans.push_back(make_pair(eachImg, height));
			}
		}
		else {
			float trailing = pos.y - _size.height * 0.5;
			int num = this->numChoices + 1;
			float eachBlank = this->rootNode->getContentSize().width * 3.0f / 100;
			float eachImg = (this->rootNode->getContentSize().width - num * eachBlank) / this->numChoices;
			float height = eachImg * _oSize.height / _oSize.width;
			height = min(height, trailing * 0.8f);
			eachImg = height * _oSize.width / _oSize.height;
			for (int i = 0; i < this->numChoices; i++) {
				ans.push_back(make_pair(eachImg, height));
			}
		}
	return ans;
}

std::vector<std::pair<float, float> > Puzzle::getMovePositions(cocos2d::Size _size, cocos2d::Vec2 pos, cocos2d::Size _oSize) {
	std::vector<std::pair<float, float> > ans;
	if (this->level == 0) {
		Vec2 bot = Vec2(pos.x - _size.width * 0.5, pos.y - _size.height * 0.5);
		float trailingBot = pos.y - _size.height * 0.5;
		int num = this->numChoices + 1;
		float eachBlank = this->rootNode->getContentSize().width * 4.0f / 100;
		float eachImg = (this->rootNode->getContentSize().width - num * eachBlank) / this->numChoices;
		for (int i = 0; i < this->numChoices; i++) {
			float worldPosX = (i + 1) * eachBlank + i * eachImg + 0.5 * eachImg;
			float worldPosY = trailingBot * 0.5;
			ans.push_back(make_pair(worldPosX - bot.x, worldPosY - bot.y));
		}
	} else
		if (this->level == 1) {
			Vec2 bot = Vec2(pos.x - _size.width * 0.5, pos.y - _size.height * 0.5);
			int num = 4;
			
			float eachImg = (pos.x - _size.width * 0.5) * 0.8;
			float height = eachImg * _oSize.height / _oSize.width;
			float eachBlank = (this->rootNode->getContentSize().height - 3 * height) / 4;
			float trailing = (pos.x - _size.width * 0.5) * 0.5;
			for (int i = 0; i < 3; i++) {
				float worldPosY = (i + 1) * eachBlank + i * height + 0.5 * height;
				float worldPosX = trailing;
				ans.push_back(make_pair(worldPosX - bot.x, worldPosY - bot.y));
			}
			for (int i = 0; i < 3; i++) {
				float worldPosY = (i + 1) * eachBlank + i * height + 0.5 * height;
				float worldPosX = this->rootNode->getContentSize().width - trailing;
				ans.push_back(make_pair(worldPosX - bot.x, worldPosY - bot.y));
			}
		}
		else {
			Vec2 bot = Vec2(pos.x - _size.width * 0.5, pos.y - _size.height * 0.5);
			float trailingBot = pos.y - _size.height * 0.5;
			int num = this->numChoices + 1;
			float eachBlank = this->rootNode->getContentSize().width * 3.0f / 100;
			float eachImg = (this->rootNode->getContentSize().width - num * eachBlank) / this->numChoices;
			for (int i = 0; i < this->numChoices; i++) {
				float worldPosX = (i + 1) * eachBlank + i * eachImg + 0.5 * eachImg;
				float worldPosY = trailingBot * 0.5;
				ans.push_back(make_pair(worldPosX - bot.x, worldPosY - bot.y));
			}
		}
	return ans;
}

void Puzzle::animateNewLevel() {

	Vector<FiniteTimeAction*> vecs;
	
	Vec2 framePosition;
	this->numChoices = 4;

	this->imageThis = ui::Layout::create();
	this->imageThis->setAnchorPoint(Vec2(0.5, 0.5));
	this->imageThis->setPosition(this->rootNode->getContentSize() * 0.5);

	if (this->level == 0) {
		auto img = this->createImage("laco.png");
		this->imageThis->addChild(img);
		framePosition = Vec2(this->rootNode->getContentSize().width * 0.5, this->rootNode->getContentSize().height * 0.58);
		this->numChoices = 4;
	} else
		if (this->level == 1) {
			this->numChoices = 6;
			auto img = this->createImage("5.png");
			this->imageThis->addChild(img);
			framePosition = Vec2(this->rootNode->getContentSize().width * 0.5, this->rootNode->getContentSize().height * 0.5);
		}
		else {
			this->numChoices = 6;
			auto img = this->createImage("12.png");
			this->imageThis->addChild(img);
			framePosition = Vec2(this->rootNode->getContentSize().width * 0.5, this->rootNode->getContentSize().height * 0.58);
		}

	this->rootNode->addChild(this->imageThis);
	this->frameBg->removeAllChildren();
	this->frame->setAnchorPoint(Vec2(0.5, 0.5));	
	this->frameBg->setAnchorPoint(Vec2(0.5, 0.5));
	this->centerPosition = this->imageThis->getPosition();
	
	this->imageThis->setContentSize(this->targetSize);

	this->frame->setContentSize(this->targetSize * 1.00f);
	this->frame->getChildren().at(0)->setContentSize(Size(this->targetSize.width * 1.04f, this->targetSize.height * 1.045));
	this->frame->getChildren().at(0)->setPosition(Size(this->targetSize.width * 0.5f, this->targetSize.height * 0.497f));
	this->frame->getChildren().at(0)->setScale(1.2f);
	this->frameBg->setContentSize(this->targetSize);
	this->frameBg->setScale(1.2f);

	this->imageThis->setScale(0.1f);

	this->initImages(this->targetSize);
	


	auto imageShowUp = CallFunc::create([=]() {
		
		this->imageThis->runAction(
			Sequence::create(
				ScaleTo::create(0.4f, 1.35f),		
				ScaleTo::create(0.2f, 1.1f),
				ScaleTo::create(0.1f, 1.25f),
				ScaleTo::create(0.05f, 1.2f),
				nullptr
			)
		);
		
		AUDIO_ENGINE->playEffect(showup_sound);
	});
	vecs.pushBack(imageShowUp);
	vecs.pushBack(DelayTime::create(1.25f));

	this->frame->setPosition(this->imageThis->getPosition());
	this->frameBg->setPosition(this->imageThis->getPosition());
	this->frame->setOpacity(0);
	this->frameBg->setOpacity(0);
	this->frame->setVisible(true);
	this->frameBg->setVisible(true);

	auto levelName = CallFunc::create([=]() {
		if (this->level == 0) {
			AUDIO_ENGINE->playEffect(vnflag_sound);
		}
		else
			if (this->level == 1) {
				AUDIO_ENGINE->playEffect(vnmap_sound);
			}
			else {
				AUDIO_ENGINE->playEffect(truongsa_sound);
			}
	});
	vecs.pushBack(levelName);
	vecs.pushBack(DelayTime::create(1.75f));
	

	auto separate = CallFunc::create([=]() {

		this->frame->runAction(FadeIn::create(1.0f));
		this->frameBg->runAction(FadeIn::create(1.0f));		
		
		this->makeLines(this->imageThis, this->targetSize);
		this->makeLines(this->frameBg, this->targetSize);
		
	});
	vecs.pushBack(separate);
	vecs.pushBack(DelayTime::create(1.0f));

	auto moveAss = CallFunc::create([=]() {
		auto frameSmaller = CallFunc::create([=]() {
			this->frame->getChildren().at(0)->runAction(ScaleTo::create(0.5f, 1));
		});
		this->frame->runAction(Spawn::createWithTwoActions(
			MoveTo::create(0.5f, framePosition),
			frameSmaller
		));
		this->frameBg->runAction(Spawn::createWithTwoActions(
			MoveTo::create(0.5f, framePosition),
			ScaleTo::create(0.5f, 1)
		));
		this->imageThis->runAction(Spawn::createWithTwoActions(
			MoveTo::create(0.5f, framePosition),
			ScaleTo::create(0.5f, 1)
		));
	});
	vecs.pushBack(moveAss);
	vecs.pushBack(DelayTime::create(0.5f));

	auto changeForm = CallFunc::create([=]() {		
		auto pos = this->getPositions(this->targetSize);
		
		for (int i = 0; i < this->numChoices; i++) {
			auto choice = this->vecChoices.at(i);
			auto curPos = pos[i];
			
			choice->setPosition(Vec2(curPos.first, curPos.second));
			choice->setAnchorPoint(Vec2(0.5, 0.5));
			choice->setPosition(Vec2(curPos.first + choice->getContentSize().width * 0.5, curPos.second + choice->getContentSize().height * 0.5));
			choice->setParentPosition(framePosition - Vec2(this->targetSize) * 0.5);
			choice->setOriginalPosition(choice->getPosition());
			this->frame->addChild(choice);
		}
		this->imageThis->runAction(FadeOut::create(0.1f));		
	});
	vecs.pushBack(changeForm);
	vecs.pushBack(DelayTime::create(0.1f));
	
	auto moveAll = CallFunc::create([=]() {
		
		this->imageThis->setVisible(false);
		this->imageThis->runAction(FadeIn::create(0.1f));

		auto movePosition = this->getMovePositions(this->targetSize, framePosition, this->vecChoices.at(0)->getContentSize() * this->vecChoices.at(0)->getScale());
		auto moveSize = this->getMoveSize(this->targetSize, framePosition, this->vecChoices.at(0)->getContentSize() * this->vecChoices.at(0)->getScale());
		std::random_device rd;
		std::mt19937 rng(rd());
		std::shuffle(std::begin(movePosition), std::end(movePosition), rng);
		int i = 0;

		Vector<FiniteTimeAction*> moveAlls;

		for (auto choice: this->vecChoices) {
						
			choice->setNewSize(Size(moveSize[i].first, moveSize[i].second));
			choice->setNewPosition(Vec2(movePosition[i].first, movePosition[i].second));			
			choice->newScale = moveSize[i].first / choice->getOriginalSize().width;
			
			auto moveThis = CallFunc::create([=]() {
				auto scaleTo = ScaleTo::create(0.15, moveSize[i].first / choice->getOriginalSize().width);
				auto spawn = Spawn::createWithTwoActions(
					scaleTo,
					MoveTo::create(0.25f, Vec2(movePosition[i].first, movePosition[i].second))
				);
				choice->runAction(spawn);
				AUDIO_ENGINE->playEffect(split_sound);
			});
			moveAlls.pushBack(moveThis);
			moveAlls.pushBack(DelayTime::create(0.2f));
					
			++i;			
		}
		this->runAction(Sequence::create(moveAlls));
	});
	vecs.pushBack(moveAll);
	vecs.pushBack(DelayTime::create(this->numChoices * 0.25f + 0.25f));	

	auto makeTouchable = CallFunc::create([=]() {
		for (auto choice : this->vecChoices) {
			choice->makeTouchable();
		}
	});
	vecs.pushBack(makeTouchable);
	
	auto seq = Sequence::create(vecs);
	this->runAction(seq);

}

void Puzzle::triggerStart() {
	this->cntCorrect = 0;
	this->animateNewLevel();
}

void Puzzle::initAll() {
	
	playBackgroundMusic(bg_sound);

	this->level = 0;
	this->rootNode = this->getChildByName<ui::Layout*>("bg");
	Size szN = Director::getInstance()->getVisibleSize();
	this->rootNode->setContentSize(szN);

	this->searchFolder = "games/puzzle/";	
	this->frame = this->rootNode->getChildByName<ui::ImageView*>("frame");	
	this->frameBg = this->rootNode->getChildByName<ui::ImageView*>("framebg");
	/*
	Size sz = Director::getInstance()->getVisibleSize();
	if (sz.width < 1024 && sz.height < 768) {
		this->searchFolder = "hd/games/puzzle/";
	}
	*/
}

void Puzzle::makeGame() {	
	this->initAll();
	this->introduction();	
}

void Puzzle::touched(PartChoice* partChoice) {	

	AUDIO_ENGINE->playEffect(interact_sound);

	for (auto choice : this->vecChoices) {
		if (choice != partChoice) {
			choice->makeUntouchable();
		}
	}	
	partChoice->runAction(
		ScaleTo::create(0.1f, 1.0f)
	);
}

void Puzzle::released(PartChoice* partChoice) {	
	

	if (partChoice->inItsRect()) {
		this->madeCorrect(partChoice);
	}
	else {
		this->madeWrong(partChoice);
	}	
}

void Puzzle::madeWrong(PartChoice* partChoice) {	
	AUDIO_ENGINE->playEffect(wrong_sound);
	partChoice->setLocalZOrder(5);
	this->runWrongAnimation(partChoice);
}

void Puzzle::madeCorrect(PartChoice* partChoice) {	
	++this->cntCorrect;
	partChoice->setLocalZOrder(5 + this->cntCorrect);
	AUDIO_ENGINE->playEffect(correct_sound);
	this->runCorrectAnimation(partChoice);
}

void Puzzle::runWrongAnimation(PartChoice* partChoice) {
	Vector<FiniteTimeAction*> vecs;

	auto moveTo = CallFunc::create([=]() {
		auto _moveTo = MoveTo::create(0.25f, partChoice->getNewPosition());
		auto ease = EaseBackOut::create(_moveTo);
		partChoice->makeUntouchable();
		partChoice->runAction(ease);
		partChoice->runAction(
			ScaleTo::create(
				0.18f, 
				partChoice->newScale
			)
		);
	});
	vecs.pushBack(moveTo);
	vecs.pushBack(DelayTime::create(0.6f));

	auto makeTouchable = CallFunc::create([=]() {
		for (auto choice : this->vecChoices) {
			choice->makeTouchable();
		}
	});
	vecs.pushBack(makeTouchable);	

	partChoice->runAction(Sequence::create(vecs));
}

void Puzzle::runCorrectAnimation(PartChoice* partChoice) {
	Vector<FiniteTimeAction*> vecs;

	auto moveTo = CallFunc::create([=]() {
		auto _moveTo = MoveTo::create(0.15f, partChoice->getOriginalPosition());
		partChoice->runAction(_moveTo);
		partChoice->makeCannotBeTouchedAnymore();		
	});
	vecs.pushBack(moveTo);
	vecs.pushBack(DelayTime::create(0.15f));

    float smaller = 0.010f;
    if (this->level > 0) {
        smaller = 0.015f;
    }
    
	auto toNho = CallFunc::create([=]() {
		float scale = 1.0f;
		partChoice->runAction(
			Sequence::create(
				ScaleTo::create(0.06f, scale - 0.025f),
				ScaleTo::create(0.12f, scale + 0.025f),
                ScaleTo::create(0.06f, scale - smaller),
				nullptr
			)
		);
		auto gaf = GAFWrapper::createGAF(effect_gaf);
		gaf->play(false);
		gaf->setPosition(partChoice->getContentSize() * 0.5);
		gaf->setScale(0.45f);
		partChoice->addChild(gaf);		
		this->gaff = gaf;
	});
	vecs.pushBack(toNho);
	vecs.pushBack(DelayTime::create(0.6f));

	auto makeTouchable = CallFunc::create([=]() {
		
		this->gaff->removeFromParent();

		for (auto choice : this->vecChoices) {
			choice->makeTouchable();
		}
		if (this->cntCorrect == this->numChoices) {
			this->runEndingAnimation();
		}		
	});
	vecs.pushBack(makeTouchable);	

	partChoice->runAction(Sequence::create(vecs));
	
}

void Puzzle::runEndingAnimation() {
	
	Vector<FiniteTimeAction*> vecs;

	vecs.pushBack(DelayTime::create(0.75f));
	auto fade = CallFunc::create([=]() {		
		for (auto line : this->vecDraws) {
			line->removeFromParent();
		}
		for (auto choice : this->vecChoices) {
			choice->setVisible(false);
		}
		this->frame->runAction(FadeOut::create(0.5f));
		this->frameBg->runAction(FadeOut::create(0.5f));
		this->imageThis->setVisible(true);		
		AUDIO_ENGINE->playEffect(join_sound);
	});
	vecs.pushBack(fade);
//    vecs.pushBack(DelayTime::create(1.25f));

	auto moveTo = CallFunc::create([=]() {

		auto spawn = Spawn::createWithTwoActions(
			MoveTo::create(0.75f, this->centerPosition),
			ScaleTo::create(0.75f, 1.25f)
		);
		this->imageThis->runAction(spawn);		

		auto gaf = GAFWrapper::createGAF(firewo_gaf);
		gaf->play(false);
		gaf->setPosition(this->rootNode->getContentSize() * 0.5);		
		this->rootNode->addChild(gaf);
		this->gaff = gaf;
		auto gaf1 = GAFWrapper::createGAF(firewo_gaf);
		gaf1->play(false);
		gaf1->setPosition(Size(this->rootNode->getContentSize().width * 0.2, this->rootNode->getContentSize().height * 0.25));
		this->rootNode->addChild(gaf1);
		this->gaff1 = gaf1;
		auto gaf2 = GAFWrapper::createGAF(firewo_gaf);
		gaf2->play(false);
		gaf2->setPosition(Size(this->rootNode->getContentSize().width * 0.2, this->rootNode->getContentSize().height * 0.75));
		this->rootNode->addChild(gaf2);
		this->gaff2 = gaf2;
		auto gaf3 = GAFWrapper::createGAF(firewo_gaf);
		gaf3->play(false);
		gaf3->setPosition(Size(this->rootNode->getContentSize().width * 0.7, this->rootNode->getContentSize().height * 0.25));
		this->rootNode->addChild(gaf3);
		this->gaff3 = gaf3;
		auto gaf4 = GAFWrapper::createGAF(firewo_gaf);
		gaf4->play(false);
		gaf4->setPosition(Size(this->rootNode->getContentSize().width * 0.7, this->rootNode->getContentSize().height * 0.75));
		this->rootNode->addChild(gaf4);
		this->gaff4 = gaf4;
	});
	vecs.pushBack(moveTo);
//    vecs.pushBack(DelayTime::create(1.5f));
    
    auto gjSound = CallFunc::create([=]() {
        AUDIO_ENGINE->playEffect(goodjob_sound);
    });
    vecs.pushBack(gjSound);
    vecs.pushBack(DelayTime::create(2.0f));

	auto gafFadeout = CallFunc::create([=]() {				
		this->gaff->runAction(FadeOut::create(0.75f));
		this->gaff1->runAction(FadeOut::create(0.75f));
		this->gaff2->runAction(FadeOut::create(0.75f));
		this->gaff3->runAction(FadeOut::create(0.75f));
		this->gaff4->runAction(FadeOut::create(0.75f));
	});
	vecs.pushBack(gafFadeout);
	vecs.pushBack(DelayTime::create(0.00f));

	auto gameName = CallFunc::create([=]() {
		if (this->level == 0) {
			AUDIO_ENGINE->playEffect(vnflag_sound);
		}
		else
			if (this->level == 1) {
				AUDIO_ENGINE->playEffect(vnmap_sound);
			}
			else {
				AUDIO_ENGINE->playEffect(truongsa_sound);
			}
	});
	vecs.pushBack(gameName);
	vecs.pushBack(DelayTime::create(0.75f));

	auto rmGaf = CallFunc::create([=]() {
		this->gaff->removeFromParent();
		this->gaff1->removeFromParent();
		this->gaff2->removeFromParent();
		this->gaff3->removeFromParent();
		this->gaff4->removeFromParent();
	});
	vecs.pushBack(rmGaf);
	vecs.pushBack(DelayTime::create(2.0f));

	auto moveDown = CallFunc::create([=]() {
				
		AUDIO_ENGINE->playEffect(getout_sound);
		
		this->imageThis->runAction(MoveTo::create(0.1f, this->centerPosition - Vec2(0, 150)));
	});
	vecs.pushBack(moveDown);
	vecs.pushBack(DelayTime::create(0.185f));

	auto moveUp = CallFunc::create([=]() {
		this->imageThis->runAction(MoveTo::create(0.25f, this->centerPosition + Vec2(0, 1500)));
	});
	vecs.pushBack(moveUp);
	vecs.pushBack(DelayTime::create(0.25f));

	auto newLevel = CallFunc::create([=]() {

		++this->level;
		if (this->level < 3) {
			this->triggerStart();
		}
		else {
			this->onFinishGame();
		}
	});
	vecs.pushBack(newLevel);

	this->runAction(Sequence::create(vecs));
}
