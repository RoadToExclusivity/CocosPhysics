#include "TerrainCommand.h"

USING_NS_CC;

TerrainCommand::TerrainCommand()
:_textureID(0)
, _glProgramState(nullptr)
, _blendType(BlendFunc::DISABLE)
{
	func = CC_CALLBACK_0(TerrainCommand::execute, this);
}

TerrainCommand::~TerrainCommand()
{
}

void TerrainCommand::init(float globalZOrder, GLuint textureID, GLProgramState* glProgramState, BlendFunc blendType,
	GLuint vertexBuffer, size_t vertexCount, const Mat4 &mv, uint32_t flags)
{
	CustomCommand::init(globalZOrder, mv, flags);

	_globalOrder = globalZOrder;
	_textureID = textureID;
	_glProgramState = glProgramState;
	_blendType = blendType;
	_vertexBuffer = vertexBuffer;
	_vertexCount = vertexCount;
	_mv.set(mv);
}

void TerrainCommand::execute()
{	
	GL::blendFunc(_blendType.src, _blendType.dst);
	GL::bindTexture2D(_textureID);

#if CC_TEXTURE_ATLAS_USE_VAO
	ccGLBindVAO(mVAO);
#else
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	_glProgramState->apply(_mv);
	// vertex
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));

	// color
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));

	// texcood
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
#endif

	glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CHECK_GL_ERROR_DEBUG();
}
