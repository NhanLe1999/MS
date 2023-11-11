#pragma once
#include "cocos2d.h"
//#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"
#include "cocostudiomacro2.h"

class PipeReader : public cocostudio::NodeReader
{
public:
	static PipeReader* getInstance();
	static void purge();
	cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};
