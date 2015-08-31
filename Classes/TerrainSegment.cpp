#include "TerrainSegment.h"
#include "TerrainCommand.h"

USING_NS_CC;

static const int SEGMENT_LINE_TENSION = 0.7;

TerrainSegment* TerrainSegment::create(const TerrainSegmentData &data, const SurfaceAdditionalInfo& info, 
										const Size &contentSize, PhysicsEngine* engine)
{
    auto ret = new TerrainSegment(engine);
    if (engine && ret->initWithData(data, info, contentSize))
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

TerrainSegment::TerrainSegment(PhysicsEngine* engine)
	:mEngine(engine)
{
    mBlendFunc.src = CC_BLEND_SRC;
    mBlendFunc.dst = CC_BLEND_DST;
}

void TerrainSegment::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	CC_NODE_DRAW_SETUP();
	terrainCommand.init(_globalZOrder, mTexture->getName(), getGLProgramState(), mBlendFunc, mVBO, mVertexCount, transform, flags);
	renderer->addCommand(&terrainCommand);
}

void TerrainSegment::cleanup()
{
    Node::cleanup();
    cleanupOpenGL();
}

void TerrainSegment::onEnter()
{
    initBox2DBody();
    Node::onEnter();
}

void TerrainSegment::onExit()
{
    cleanupBox2DBody();
    Node::onExit();
}

Rect TerrainSegment::boundingBox()
{
    Rect rect(0, mMinumumHeight, getContentSize().width, mMaximumHeight - mMinumumHeight);
    return RectApplyAffineTransform(rect, nodeToParentTransform());
}

/// @param data Used as height map for this segment.
bool TerrainSegment::initWithData(const TerrainSegmentData &data, const SurfaceAdditionalInfo& info, const Size &contentSize)
{
    mData = data;
	mTexture = TextureCache::getInstance()->addImage("terrain.png");
    if (!mTexture)
        return false;

	setAnchorPoint(Vec2(0.5, 0.5));
    setContentSize(contentSize);

    std::vector<V2F_C4B_T2F> vertexes;
    fillVerticies(data, info, vertexes);
    initOpenGL(vertexes);
    return true;
}

Point TerrainSegment::calculateSurfacePoint(const TerrainSegmentData &data, float positionX) const
{
    Size contentSize = getContentSize();

	positionX /= contentSize.width;
    float normalizedDelta = positionX * TerrainSegmentData::SEGMENT_LENGTH;

    int pi = int(normalizedDelta);
    float dtFraq = normalizedDelta - int(normalizedDelta);

    Point pp0(0, data.getKeypointAtIndex(pi - 1));
    Point pp1(0, data.getKeypointAtIndex(pi + 0));
    Point pp2(0, data.getKeypointAtIndex(pi + 1));
    Point pp3(0, data.getKeypointAtIndex(pi + 2));
    Point point = ccCardinalSplineAt(pp0, pp1, pp2, pp3, SEGMENT_LINE_TENSION, dtFraq);
	point.y *= contentSize.height;

	return point;
}

float TerrainSegment::getSurfaceY(float positionX) const
{
    Point point = calculateSurfacePoint(mData, positionX);
    return point.y;
}

/// Fills array of verticies for triangle strip using interpolation.
void TerrainSegment::fillVerticies(const TerrainSegmentData &data, const SurfaceAdditionalInfo& info, std::vector<V2F_C4B_T2F> &vertexes)
{
    Size contentSize = getContentSize();
    const int lastChunkIndex = contentSize.width;
    vertexes.reserve(2 + 2 * lastChunkIndex);

    V2F_C4B_T2F vertex;
    vertex.colors = {255, 255, 255, 255};

	float additionalMaxHeight = (info.max_point - info.min_point) * contentSize.height;
    mMaximumHeight = 0;
    mMinumumHeight = UINT_MAX;

    for (int ci = 0; ci <= lastChunkIndex; ++ci)
    {
        float dt = float(ci) / contentSize.width;
        float normalizedDelta = dt * TerrainSegmentData::SEGMENT_LENGTH;

        int pi = int(normalizedDelta);
        float dtFraq = normalizedDelta - int(normalizedDelta);

        Point pp0(0, data.getKeypointAtIndex(pi - 1));
        Point pp1(0, data.getKeypointAtIndex(pi + 0));
        Point pp2(0, data.getKeypointAtIndex(pi + 1));
        Point pp3(0, data.getKeypointAtIndex(pi + 2));
        Point point = ccCardinalSplineAt(pp0, pp1, pp2, pp3, SEGMENT_LINE_TENSION, dtFraq);
        point.x = dt * contentSize.width;
		point.y = point.y * contentSize.height;
        if (point.y > mMaximumHeight)
            mMaximumHeight = point.y;
		if (point.y - (contentSize.height + additionalMaxHeight) < mMinumumHeight)
			mMinumumHeight = point.y - (contentSize.height + additionalMaxHeight);

        // top vertex
        vertex.vertices.x = point.x;
        vertex.vertices.y = point.y;
        vertex.texCoords = {dt, 0};
        vertexes.push_back(vertex);

        // turn top vertex into bottom
		vertex.vertices.y -= (contentSize.height + additionalMaxHeight);
        vertex.texCoords = {dt, 1};
        vertexes.push_back(vertex);
    }
}

void TerrainSegment::initOpenGL(const std::vector<V2F_C4B_T2F> &vertexes)
{
	GL::bindTexture2D(mTexture->getName());
	setShaderProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

#if CC_TEXTURE_ATLAS_USE_VAO
    glGenVertexArrays(1, &mVAO);
    ccGLBindVAO(mVAO);
#endif

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F) * vertexes.size(),
                 vertexes.data(), GL_STATIC_DRAW);
    mVertexCount = vertexes.size();

    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));

    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));

    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

#if CC_TEXTURE_ATLAS_USE_VAO
    ccGLBindVAO(0);
#endif

    CHECK_GL_ERROR_DEBUG();
}

void TerrainSegment::initBox2DBody()
{
    b2BodyDef bd;
    // TODO: set correct parent-independent position.
    float ptmRatio = mEngine->getPtmRatio();
	bd.position.Set(getPositionX() / ptmRatio, getPositionY() / ptmRatio);
	mPuppeteer = TerrainPuppeteer::create(bd, mEngine);

	Size bodySize = getContentSize() / ptmRatio;
    b2Vec2 b2vertices[TerrainSegmentData::SEGMENT_LENGTH + 1];
    for (int i = 0; i <= TerrainSegmentData::SEGMENT_LENGTH; ++i) {
        float keypoint = -0.5f + mData.getKeypointAtIndex(i);
        float relativeWidth = -0.5f + float(i) / TerrainSegmentData::SEGMENT_LENGTH;
        float x = bodySize.width * relativeWidth;
        float y = bodySize.height * keypoint;
        b2vertices[i].Set(x, y);
    }

    b2ChainShape shape;
	shape.CreateChain(b2vertices, TerrainSegmentData::SEGMENT_LENGTH + 1);
	mPuppeteer->getBody()->CreateFixture(&shape, 0);
	mPuppeteer->getBody()->SetActive(true);
}

void TerrainSegment::cleanupOpenGL()
{
    if (mVBO) {
        glDeleteBuffers(1, &mVBO);
        mVBO = 0;
    }

#if CC_TEXTURE_ATLAS_USE_VAO
    // extension GL_OES_vertex_array_object.
    if (mVAO) {
        glDeleteVertexArrays(1, &mVAO);
        ccGLBindVAO(0);
        mVAO = 0;
    }
#endif
}

void TerrainSegment::cleanupBox2DBody()
{
	mPuppeteer.Clear();
}

TerrainSegment::~TerrainSegment()
{
    cleanupOpenGL();
}
