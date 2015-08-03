#pragma once
#include "cocos2d.h"
#include "renderer\CCCustomCommand.h"

class TerrainCommand : public cocos2d::CustomCommand
{
public:
	TerrainCommand();
	~TerrainCommand();
	
	void init(float globalZOrder, GLuint textureID, cocos2d::GLProgramState* glProgramState, cocos2d::BlendFunc blendType,
		GLuint vertexBuffer, size_t vertexCount, const cocos2d::Mat4 &mv, uint32_t flags);

	void execute();

private:
	void applyRenderState();
	void restoreRenderState();

	GLuint _textureID;
	cocos2d::GLProgramState* _glProgramState = nullptr;
	cocos2d::BlendFunc _blendType;

	GLuint _textrueID;

	GLuint _vertexBuffer;
	size_t _vertexCount;

	// ModelView transform
	cocos2d::Mat4 _mv;

	bool _cullFaceEnabled;
	GLenum _cullFace;
	bool _depthTestEnabled;
	bool _depthWriteEnabled;
	bool _forceDepthWrite;

	bool _renderStateCullFaceEnabled;
	bool _renderStateDepthTest;
	GLboolean _renderStateDepthWrite;
	GLenum    _renderStateCullFace;
};

