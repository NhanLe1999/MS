#include "PipeReader.h"
#include "Pipe.h"

using namespace cocos2d;

static PipeReader* _instancePipeReader = nullptr;

PipeReader* PipeReader::getInstance()
{
	if (!_instancePipeReader)
	{
		_instancePipeReader = new PipeReader();
	}
	return _instancePipeReader;
}

void PipeReader::purge()
{
	CC_SAFE_DELETE(_instancePipeReader);
}

Node* PipeReader::createNodeWithFlatBuffers(const flatbuffers::Table *nodeOptions)
{
	Pipe* node = Pipe::create();
	setPropsWithFlatBuffers(node, nodeOptions);
	return node;
}