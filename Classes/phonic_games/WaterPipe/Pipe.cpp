#include "Pipe.h"
using namespace cocos2d;

bool Pipe::init() {
	if (!Node::init()) {
		return false;
	}
	this->pipeTimeline = cocos2d::CSLoader::createTimeline("Pipes.csb");
	this->pipeTimeline->retain();
	return true;
}

void Pipe::viberate() {
	Vector<FiniteTimeAction*> vecs;
	for (int i = 0; i < 20; i++) {
		float delta = 0.25f;
		if (i % 2) delta = -delta;
		auto move = MoveBy::create(0.04f, Vec2(0, delta));
		auto delay = DelayTime::create(0.04f);
		vecs.pushBack(move);
		vecs.pushBack(delay);
		this->runAction(Sequence::create(vecs));
	}
}

void Pipe::showUp() {

	Vector<FiniteTimeAction*> vecs;
	
	auto listChild = this->getChildren();
	std::vector<float> vecWidths;
	for (auto child : listChild) {
		vecWidths.push_back(child->getContentSize().height);
	}

	int maxActs = 20;

	for (int i = 0; i < maxActs; i++) {		
		Vector<FiniteTimeAction*> vecSpawn;

		for (int j = 0; j < vecWidths.size(); j++) {
			float len = vecWidths[j];
			auto child = listChild.at(j);
			float each = len / maxActs;
			float height = child->getContentSize().width;			
			auto setLength = CallFunc::create([=]() {
				child->setContentSize(Size(height, each * (i + 1)));
			});
			vecSpawn.pushBack(setLength);
		}
		auto delay = DelayTime::create(0.75f / maxActs);
		vecs.pushBack(Spawn::create(vecSpawn));
		vecs.pushBack(delay);
	}

	this->runAction(Sequence::create(vecs));
	
}

void Pipe::onExit() {
	this->pipeTimeline->release();
	Node::onExit();
}

