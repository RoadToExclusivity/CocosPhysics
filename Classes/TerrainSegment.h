#pragma once

#include "cocos2d.h"
#include "TerrainSegmentData.h"
#include "SceneObjectPuppeteers.h"
#include "PhysicsEngine.h"
#include "Pointer.h"
#include "TerrainCommand.h"
#include "SurfaceGenerator.h"

class TerrainSegment : public cocos2d::Node
{
public:
	static TerrainSegment *create(const TerrainSegmentData &data, const SurfaceAdditionalInfo& info, 
								 const cocos2d::Size &contentSize, PhysicsEngine* engine);

	TerrainSegment(PhysicsEngine* engine);
	void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    void cleanup() override;
    void onEnter() override;
    void onExit() override;

    cocos2d::Rect boundingBox();
    float getSurfaceY(float positionX) const;

private:
	bool initWithData(const TerrainSegmentData &data, const SurfaceAdditionalInfo& info, const cocos2d::Size &contentSize);
	void fillVerticies(const TerrainSegmentData &data, const SurfaceAdditionalInfo& info,
                       std::vector<cocos2d::V2F_C4B_T2F> &vertexes);
	void initOpenGL(const std::vector<cocos2d::V2F_C4B_T2F> &vertexes);
    void initBox2DBody();
    void cleanupBox2DBody();
    void cleanupOpenGL();
    ~TerrainSegment();
    
    cocos2d::Point calculateSurfacePoint(const TerrainSegmentData &data, float positionX) const;

	TerrainCommand terrainCommand;

    GLuint mVAO = 0;
    GLuint mVBO = 0;
    size_t mVertexCount = 0;
    cocos2d::BlendFunc mBlendFunc;
	Pointer<cocos2d::Texture2D> mTexture = nullptr;
    TerrainSegmentData mData;
    float mMaximumHeight = 0;
    float mMinumumHeight = 0;
	Pointer<TerrainPuppeteer> mPuppeteer = nullptr;
	PhysicsEngine* mEngine = nullptr;
};
