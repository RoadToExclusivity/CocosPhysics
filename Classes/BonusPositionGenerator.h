#pragma once
#include <random>

class BonusPositionGenerator
{
public:
	BonusPositionGenerator();
	float NextPosition(float);

private:
	std::random_device m_rd;
	std::mt19937 m_gen;
};

