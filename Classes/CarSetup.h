#pragma once

enum WheelSetup
{
	SMALL_WHEELS,
	BIG_WHEELS
};

enum ClearanceSetup
{
	LOW_CLEARANCE,
	HIGH_CLEARANCE
};

class CarSetup
{
public:
	void SetWheel(WheelSetup setup);
	WheelSetup GetWheel() const;

	void SetClearance(ClearanceSetup setup);
	ClearanceSetup GetClearance() const;

private:
	WheelSetup m_wheel = WheelSetup::SMALL_WHEELS;
	ClearanceSetup m_cl = ClearanceSetup::LOW_CLEARANCE;
};

