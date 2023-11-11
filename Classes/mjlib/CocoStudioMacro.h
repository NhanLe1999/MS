//
//  CocoStudioMacro.h
//  BHPikachu
//
//  Created by To Ngoc Linh on 12/27/14.
//
//

#ifndef CocoStudioMacro_h
#define CocoStudioMacro_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "ui/CocosGUI.h"
#define CREATE_READER_CLASS(__READER_TYPE__, __NODE_TYPE__) \
class __READER_TYPE__ : public cocostudio::NodeReader {\
public:\
static __READER_TYPE__ * __instance_##__READER_TYPE__;\
__READER_TYPE__() {}; \
~__READER_TYPE__() {}; \
\
static __READER_TYPE__ * getInstance() {\
if (!__instance_##__READER_TYPE__) {\
__instance_##__READER_TYPE__ = new __READER_TYPE__();\
}\
return(__instance_##__READER_TYPE__);\
};\
\
static void purge() {\
CC_SAFE_DELETE(__instance_##__READER_TYPE__);\
}; \
cocos2d::Node * createNodeWithFlatBuffers(const flatbuffers::Table * node_options) {\
__NODE_TYPE__ * node = __NODE_TYPE__::create();\
setPropsWithFlatBuffers(node, node_options);\
return node;\
};\
};\

#define INITIALIZE_READER_INSTANCE(__READER_TYPE__) __READER_TYPE__ * __READER_TYPE__::__instance_##__READER_TYPE__ = nullptr

#define CLICK_MAP(class_name, func_name) \
{#func_name, CC_CALLBACK_1(class_name::func_name, this)}

#define REGISTER_READER(class_name) \
cocos2d::CSLoader::getInstance()->registReaderObject(#class_name, (cocos2d::ObjectFactory::Instance)class_name::getInstance)

#endif
