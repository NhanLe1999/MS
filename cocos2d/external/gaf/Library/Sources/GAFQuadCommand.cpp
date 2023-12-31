#include "GAFPrecompiled.h"
#include "GAFQuadCommand.h"

USING_NS_CC;

NS_GAF_BEGIN

GAFQuadCommand::GAFQuadCommand()
{}

GAFQuadCommand::~GAFQuadCommand()
{}

void GAFQuadCommand::init(float globalOrder, GLuint textureID, GLProgramState* glProgramState, BlendFunc blendType, V3F_C4B_T2F_Quad* quad, ssize_t quadCount, const Mat4 &mv, uint32_t materialId)
{
    CCASSERT(glProgramState, "Invalid GLProgramState");
    CCASSERT(glProgramState->getVertexAttribsFlags() == 0, "No custom attributes are supported in QuadCommand");

    QuadCommand::init(globalOrder, textureID, glProgramState, blendType, quad, quadCount, mv, 0);

    if (_materialID != materialId) 
    {
        _materialID = materialId;
        if (_materialID != cocos2d::Renderer::MATERIAL_ID_DO_NOT_BATCH)
        {
            setSkipBatching(false);
        }
    }
}

NS_GAF_END
