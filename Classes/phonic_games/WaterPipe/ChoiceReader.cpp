#include "ChoiceReader.h"
#include "Choice.h"

using namespace cocos2d;

static ChoiceReader* _instanceChoiceReader = nullptr;

ChoiceReader* ChoiceReader::getInstance()
{
	if (!_instanceChoiceReader)
	{
		_instanceChoiceReader = new ChoiceReader();
	}
	return _instanceChoiceReader;
}

void ChoiceReader::purge()
{
	CC_SAFE_DELETE(_instanceChoiceReader);
}

Node* ChoiceReader::createNodeWithFlatBuffers(const flatbuffers::Table *nodeOptions)
{
	Choice* node = Choice::create();
	setPropsWithFlatBuffers(node, nodeOptions);
	return node;
}