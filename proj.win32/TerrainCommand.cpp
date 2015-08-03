#include "TerrainCommand.h"

USING_NS_CC;

TerrainCommand::TerrainCommand()
:_textureID(0)
, _glProgramState(nullptr)
, _blendType(BlendFunc::DISABLE)
, _cullFaceEnabled(false)
, _cullFace(GL_BACK)
, _depthTestEnabled(false)
, _depthWriteEnabled(false)
, _forceDepthWrite(false)
, _renderStateCullFaceEnabled(false)
, _renderStateDepthTest(false)
, _renderStateDepthWrite(GL_FALSE)
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

void TerrainCommand::applyRenderState()
{
	_renderStateCullFaceEnabled = glIsEnabled(GL_CULL_FACE) != GL_FALSE;
	_renderStateDepthTest = glIsEnabled(GL_DEPTH_TEST) != GL_FALSE;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &_renderStateDepthWrite);
	GLint cullface;
	glGetIntegerv(GL_CULL_FACE_MODE, &cullface);
	_renderStateCullFace = (GLenum)cullface;

	if (_cullFaceEnabled != _renderStateCullFaceEnabled)
	{
		_cullFaceEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	}

	if (_cullFace != _renderStateCullFace)
	{
		glCullFace(_cullFace);
	}

	if (_depthTestEnabled != _renderStateDepthTest)
	{
		_depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	if (_depthWriteEnabled != _renderStateDepthWrite)
	{
		glDepthMask(_depthWriteEnabled);
	}
}

void TerrainCommand::restoreRenderState()
{
	if (_cullFaceEnabled != _renderStateCullFaceEnabled)
	{
		_renderStateCullFaceEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	}

	if (_cullFace != _renderStateCullFace)
	{
		glCullFace(_renderStateCullFace);
	}

	if (_depthTestEnabled != _renderStateDepthTest)
	{
		_renderStateDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	if (_depthWriteEnabled != _renderStateDepthWrite)
	{
		glDepthMask(_renderStateDepthWrite);
	}
}

void TerrainCommand::execute()
{	
	applyRenderState();
	GL::bindTexture2D(_textureID);
	GL::blendFunc(_blendType.src, _blendType.dst);

#if CC_TEXTURE_ATLAS_USE_VAO
	ccGLBindVAO(mVAO);
#else
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	_glProgramState->apply(_mv);
	// vertex
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));

	// color
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));

	// texcood
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
#endif

	glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//restore render state
	restoreRenderState();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
