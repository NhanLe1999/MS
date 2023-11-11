#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/WidgetReader/NodeReader/NodeReader.h"

class LongPipeReader : public cocostudio::NodeReader
{
public:
	static LongPipeReader* getInstance();
	static void purge();
	cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};
