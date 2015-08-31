#include "SurfaceGenerator.h"
#include <random>

using namespace std;

const float SurfaceGenerator::MIN_FALL_BORDER = 0.25f;
const float SurfaceGenerator::FALL_STEP = 0.2f;

SurfaceGenerator::SurfaceGenerator()
	:m_diff(0), m_rd(), m_gen(0), m_wasPrevSegment(false)
{
}

pair<TerrainSegmentData, SurfaceAdditionalInfo> SurfaceGenerator::NextSegment()
{
	int type = (rand() % 8);
	SurfaceType sType = type < 3 ? NORMAL : type < 5 ? PLAIN_DOWN : SurfaceType(type - 3);

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
		if (sType == PLAIN_UP)
		{
			res.setKeypointAtIndex(-3 - indexPoint, 0);
			res.setKeypointAtIndex(indexPoint, (m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH + 3 + indexPoint)
				- m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH - indexPoint)));
		}
		else
		{
			res.setKeypointAtIndex(indexPoint, 1);
			res.setKeypointAtIndex(-3 - indexPoint, 1 - (m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH + 3 + indexPoint)
				- m_prevSegment.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH - indexPoint )));
		}
		
		h = res.getKeypointAtIndex(-1);
	}

	for (int i = start; i <= TerrainSegmentData::SEGMENT_LENGTH + 2; ++i)
	{
		switch (sType)
		{
		case NORMAL:
			h = NextNormalPoint(h);
			break;
		case PLAIN_DOWN:
			h = NextPlainDownPoint(h);
			break;
		case PLAIN_UP:
			h = NextPlainUpPoint(h);
			break;
		case FLAT:
			h = NextFlatPoint(h);
			break;
		case FALL:
			h = NextFallPoint(h);
			break;
		default:
			break;
		}

		res.setKeypointAtIndex(i, h);
	}

	m_wasPrevSegment = true;
	m_prevSegment = res;

	SurfaceAdditionalInfo info;
	info.first_point = res.getKeypointAtIndex(0);
	info.last_point = res.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH);
	float minH = 2, maxH = -1;
	for (int j = 0; j <= TerrainSegmentData::SEGMENT_LENGTH; ++j)
	{
		if (res.getKeypointAtIndex(j) < minH)
		{
			minH = res.getKeypointAtIndex(j);
		}
		if (res.getKeypointAtIndex(j) > maxH)
		{
			maxH = res.getKeypointAtIndex(j);
		}
	}
	info.min_point = minH;
	info.max_point = maxH;

	return make_pair(res, info);
}

pair<TerrainSegmentData, SurfaceAdditionalInfo> SurfaceGenerator::FlatSegment()
{
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
	}

	for (int i = start; i <= TerrainSegmentData::SEGMENT_LENGTH + 2; ++i)
	{
		res.setKeypointAtIndex(i, 1);
	}

	m_wasPrevSegment = true;
	m_prevSegment = res;

	SurfaceAdditionalInfo info;

	info.last_point = 1;
	info.min_point = 1;
	info.max_point = 1;

	return make_pair(res, info);
}

float SurfaceGenerator::NextNormalPoint(float lastHeight)
{
	float stddev = MAX_DELTA / 4.0 + m_diff;

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

float SurfaceGenerator::NextPlainDownPoint(float lastHeight)
{
	normal_distribution<float> d(MAX_DELTA, MAX_DELTA);
	float diff = d(m_gen);

	if (diff < 0)
	{
		diff = 0;
	}
	if (diff > MAX_DELTA)
	{
		diff = MAX_DELTA;
	}

	float res = lastHeight - diff;
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

float SurfaceGenerator::NextPlainUpPoint(float lastHeight)
{
	normal_distribution<float> d(MAX_DELTA / 2.0, MAX_DELTA / 2.0);
	float diff = d(m_gen);

	if (diff < 0)
	{
		diff = 0;
	}
	if (diff > MAX_DELTA / 2.0)
	{
		diff = MAX_DELTA / 2.0;
	}

	float res = lastHeight + diff;
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

float SurfaceGenerator::NextFlatPoint(float)
{
	return 1;
}

float SurfaceGenerator::NextFallPoint(float lastHeight)
{
	if (lastHeight > MIN_FALL_BORDER)
	{
		lastHeight -= FALL_STEP;
	}

	return lastHeight;
}