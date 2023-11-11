#include "LongPipe.h"

bool LongPipe::init() {
	if (!Node::init()) {
		return false;
	}
	return true;
}