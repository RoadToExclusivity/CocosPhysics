#pragma once
#include <random>
#include "TerrainSegmentData.h"

class SurfaceGenerator
{
public:
	SurfaceGenerator();
	std::pair<TerrainSegmentData, float> NextSegment();

private:
	float NextPoint(float lastHeight);
	
	float m_diff;
	bool m_wasPrevSegment;
	TerrainSegmentData m_prevSegment;
	const float MAX_DELTA = 1.0f / (TerrainSegmentData::SEGMENT_LENGTH + 5);
	std::random_device m_rd;
	std::mt19937 m_gen;
};

