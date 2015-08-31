#pragma once
#include <random>
#include "TerrainSegmentData.h"

struct SurfaceAdditionalInfo
{
	float min_point = 0;
	float max_point = 0;
	float first_point = 0;
	float last_point = 0;
};

enum SurfaceType
{
	NORMAL,
	PLAIN_DOWN,
	FLAT,
	PLAIN_UP,
	FALL
};

class SurfaceGenerator
{
public:
	SurfaceGenerator();
	std::pair<TerrainSegmentData, SurfaceAdditionalInfo> NextSegment();
	std::pair<TerrainSegmentData, SurfaceAdditionalInfo> FlatSegment();

private:
	float NextNormalPoint(float lastHeight);
	float NextPlainDownPoint(float);
	float NextPlainUpPoint(float);
	float NextFlatPoint(float);
	float NextFallPoint(float);

	float m_diff;
	bool m_wasPrevSegment;
	TerrainSegmentData m_prevSegment;
	const float MAX_DELTA = 1.0f / (TerrainSegmentData::SEGMENT_LENGTH + 5);
	std::random_device m_rd;
	std::mt19937 m_gen;

	static const float MIN_FALL_BORDER;
	static const float FALL_STEP;
};

