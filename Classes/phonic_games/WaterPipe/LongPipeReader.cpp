#include "LongPipeReader.h"
#include "LongPipe.h"

using namespace cocos2d;

static LongPipeReader* _instanceLongPipeReader = nullptr;

LongPipeReader* LongPipeReader::getInstance()
{
	if (!_instanceLongPipeReader)
	{
		_instanceLongPipeReader = new LongPipeReader();
	}
	return _instanceLongPipeReader;
}

void LongPipeReader::purge()
{
	CC_SAFE_DELETE(_instanceLongPipeReader);
}

Node* LongPipeReader::createNodeWithFlatBuffers(const flatbuffers::Table *nodeOptions)
{
	LongPipe* node = LongPipe::create();
	setPropsWithFlatBuffers(node, nodeOptions);
	return node;
}