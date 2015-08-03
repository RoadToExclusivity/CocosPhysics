#include "TerrainSegmentData.h"

TerrainSegmentData::TerrainSegmentData()
{
    for (float &point : keypoints)
        point = 0;
}

void TerrainSegmentData::setKeypointAtIndex(int index, float height)
{
    keypoints[index + 2] = height;
}

float TerrainSegmentData::getKeypointAtIndex(int index) const
{
    return keypoints[index + 2];
}
