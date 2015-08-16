#include "CarSetup.h"

void CarSetup::SetWheel(WheelSetup setup)
{
	m_wheel = setup;
}

WheelSetup CarSetup::GetWheel() const
{
	return m_wheel;
}

void CarSetup::SetClearance(ClearanceSetup setup)
{
	m_cl = setup;
}

ClearanceSetup CarSetup::GetClearance() const
{
	return m_cl;
}