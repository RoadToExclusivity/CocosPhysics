#include "SurfaceGenerator.h"
#include <random>

using namespace std;

SurfaceGenerator::SurfaceGenerator()
	:m_diff(0), m_rd(), m_gen(0), m_wasPrevSegment(false)
{
}

pair<TerrainSegmentData, float> SurfaceGenerator::NextSegment()
{
	m_gen = mt19937(m_rd()); 
	
	float h = 1.0f;
	m_diff = 0;
	
	TerrainSegmentData res;
	int start = -2;
	if (m_wasPrevSegment)
	{
		start = 0;
		int indexPoint = m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH + 1) > 
						m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH + 2) ? -2 : -1;
		res.setKeypointAtIndex(indexPoint, 1);
		res.setKeypointAtIndex(-3 - indexPoint, 1 - (m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH - indexPoint) 
							- m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH + 3 + indexPoint)));
		h = res.getKeypointAtIndex(-1);
	}

	for (int i = start; i <= TerrainSegmentData::SEGMENT_LENGTH + 2; ++i)
	{
		res.setKeypointAtIndex(i, h);
		h = NextPoint(h);
	}

	m_wasPrevSegment = true;
	m_prevSegment = res;
	float lastHeight = res.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH);

	return make_pair(res, lastHeight);
}

float SurfaceGenerator::NextPoint(float lastHeight)
{
	float stddev = MAX_DELTA / 4.0 + m_diff;
	//float stddev = MAX_DELTA + m_diff;
	if (stddev <= 0)
	{
		stddev = 0.0001;
	}

	normal_distribution<float> d(MAX_DELTA, stddev);
	m_diff = d(m_gen);

	if (m_diff < -MAX_DELTA)
	{
		m_diff = -MAX_DELTA;
	}
	if (m_diff > MAX_DELTA)
	{
		m_diff = MAX_DELTA;
	}

	float res = lastHeight - m_diff;
	if (res < 0)
	{
		res = 0;
	}
	if (res > 1)
	{
		res = 1;
	}

	return res;
}