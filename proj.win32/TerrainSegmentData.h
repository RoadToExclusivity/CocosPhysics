#pragma once

struct TerrainSegmentData
{
    static const int SEGMENT_LENGHT = 16;

    TerrainSegmentData();

    /// Keypoint values should be in range [0..1].
    float keypoints[SEGMENT_LENGHT + 5];

    // Index can be in range [-2...SEGMENT_LENGHT+2]
    void setKeypointAtIndex(int index, float height);
    float getKeypointAtIndex(int index) const;
};
