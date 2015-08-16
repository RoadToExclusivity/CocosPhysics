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
	GLuint _textureID;
	cocos2d::GLProgramState* _glProgramState = nullptr;
	cocos2d::BlendFunc _blendType;

	GLuint _textrueID;

	GLuint _vertexBuffer;
	size_t _vertexCount;

	cocos2d::Mat4 _mv;
};

