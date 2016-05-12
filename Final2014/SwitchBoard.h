#ifndef SWITCH_BOARD_H
#define SWITCH_BOARD_H

class SwitchBoard
{
public:
	SwitchBoard(void);
	~SwitchBoard(void);

	void SetLED(int, bool);
	void TestSwitchBoard(void);
	
	bool GetSwitch(int);

	double GetPotentiometer(int);

	int GetSelector(void);

private:
	DriverStation* mDriverStation;
};

SwitchBoard::SwitchBoard(void)
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mDriverStation = DriverStation::GetInstance();

}

SwitchBoard::~SwitchBoard(void)
{
	delete mDriverStation;
	mDriverStation = NULL;
}

bool SwitchBoard::GetSwitch(int portNumber)
{
	return mDriverStation->GetEnhancedIO().GetDigital(portNumber);
}

double SwitchBoard::GetPotentiometer(int portNumber)
{
	return min(1, max(0, mDriverStation->GetEnhancedIO().GetAnalogIn(portNumber)/3.3));
}

int SwitchBoard::GetSelector(void)
{
	int selection;
	int one, two, four;

	one = !mDriverStation->GetEnhancedIO().GetDigital(SB_SELECTOR_ONES);
	two = !mDriverStation->GetEnhancedIO().GetDigital(SB_SELECTOR_TWOS);
	four = !mDriverStation->GetEnhancedIO().GetDigital(SB_SELCETOR_FOURS);

	selection = (four * 4) + (two * 2) + (one * 1);

	return selection;
}

void SwitchBoard::SetLED(int portNumber, bool status)
{
	mDriverStation->GetEnhancedIO().SetDigitalOutput(portNumber, status);
}

void SwitchBoard::TestSwitchBoard(void)
{
	printf("Initiating SwitchBoard Test...");

	printf("Selection is: %d \n", GetSelector());
	printf("Switch Status: %d, %d, %d, %d, %d, %d, %d, %d, %d \n", GetSwitch(SB_SWITCH_1), GetSwitch(SB_SWITCH_2), GetSwitch(SB_SWITCH_3), GetSwitch(SB_SWITCH_4), GetSwitch(SB_BIG_RED_BUTTON), GetSwitch(SB_SWITCH_5), GetSwitch(SB_JOYSTICK_UP), GetSwitch(SB_JOYSTICK_DOWN), GetSwitch(SB_LITTLE_RED_BUTTON));
	printf("Pot. Values: %f, %f, %f \n", GetPotentiometer(SB_POTENTIOMETER_1), GetPotentiometer(SB_POTENTIOMETER_2), GetPotentiometer(SB_POTENTIOMETER_3));

	SetLED(SB_LITTLE_RED_BUTTON_LED, true);
	SetLED(SB_BIG_RED_BUTTON_LED, true);
	SetLED(SB_JOYSTICK_UP_LED, true);
	SetLED(SB_JOYSTICK_DOWN_LED, true);

	Wait(1.0);

	SetLED(SB_LITTLE_RED_BUTTON_LED, false);
	SetLED(SB_BIG_RED_BUTTON_LED, false);
	SetLED(SB_JOYSTICK_UP_LED, false);
	SetLED(SB_JOYSTICK_DOWN_LED, false);

	printf("SwitchBoard Test Completed");
}

#endif
