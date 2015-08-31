#include "BonusPositionGenerator.h"

using namespace std;

BonusPositionGenerator::BonusPositionGenerator()
	:m_rd(), m_gen(0)
{
	m_gen = mt19937(m_rd());
}

float BonusPositionGenerator::NextPosition(float maxDiff)
{
	uniform_real_distribution<float> d(0, maxDiff);

	return d(m_gen);
}
