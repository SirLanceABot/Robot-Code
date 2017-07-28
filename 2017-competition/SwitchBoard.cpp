// Author: Aaron Stoll and Ben Puzycki
// Date: 1/14/17
// Revision History:
//	BP & AS 9/2/17: removed previousswitchone and int Switch1

#include "SwitchBoard.h"
#include "WPILib.h"

SwitchBoard::SwitchBoard(int port):
mSwitchBoard(std::make_shared<frc::Joystick>(port))
{
	std::cout << "[SwitchBoard]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}

bool SwitchBoard::GetButton(int button)
{
	return mSwitchBoard->GetRawButton(button);
}

double SwitchBoard::GetAxis(int axis)
{
	return mSwitchBoard->GetRawAxis(axis);
}

void SwitchBoard::SetLed(int port, bool on)
{
	mSwitchBoard->SetOutput(port, on);
}

// Checks if Switch 0 is turned on
bool SwitchBoard::GetSwitch0()
{
	return GetButton(SWITCH_0);
}

// Checks if Switch 1 is turned on
bool SwitchBoard::GetSwitch1()
{
	return GetButton(SWITCH_1);
}

// Checks if Switch 2 is turned on
bool SwitchBoard::GetSwitch2()
{
	return GetButton(SWITCH_2);
}

// Checks if Switch 3 is turned on
bool SwitchBoard::GetSwitch3()
{
	return GetButton(SWITCH_3);
}

// Checks if Switch 4 is turned on
bool SwitchBoard::GetSwitch4()
{
	return GetButton(SWITCH_4);
}
// Checks if the Big Red Button is pressed
bool SwitchBoard::GetBig()
{
	return GetButton(BIG_BUTTON);
}

// Checks if the Small Red Button is pressed
bool SwitchBoard::GetSmall()
{
	return GetButton(SMALL_BUTTON);
}

// Checks if the joystick is in the down position
bool SwitchBoard::GetJoystickDown()
{
	return GetButton(JOYSTICK_DOWN);
}

// Checks if the joystick is in the up position
bool SwitchBoard::GetJoystickUp()
{
	return GetButton(JOYSTICK_UP);
}

// Checks what number the counter is currently on
int SwitchBoard::GetCounter()
{
	return GetButton(COUNTER_1) * 1 + GetButton(COUNTER_2) * 2 + GetButton(COUNTER_4) * 4;
}

// Checks if the pot is turned halfway or all the way
double SwitchBoard::GetPot0()
{
	return GetAxis(POT_0) * 16.9333672 - 1;
}

// Checks if the pot is turned halfway or all the way
double SwitchBoard::GetPot1()
{
	return GetAxis(POT_1) * 16.9333672 - 1;
}

// Checks if the pot is turned halfway or all the way
double SwitchBoard::GetPot2()
{
	return GetAxis(POT_2) * 16.9333672 - 1;
}

// Changes the LED to on (true) or off (false)
void SwitchBoard::SetLedJoystickDown(bool on)
{
	SetLed(LED_JOYSTICK_DOWN, on);
}

// Changes the LED to on (true) or off (false)
void SwitchBoard::SetLedJoystickUp(bool on)
{
	SetLed(LED_JOYSTICK_UP, on);
}

// Changes the LED to on (true) or off (false)
void SwitchBoard::SetLedSmallButton(bool on)
{
	SetLed(LED_SMALL_BUTTON, on);
}

// Changes the LED to on (true) or off (false)
void SwitchBoard::SetLedBigButton(bool on)
{
	SetLed(LED_BIG_BUTTON, on);
}

void SwitchBoard::LedTest()
{
	int counter;
	for(counter = 0; counter <= 15; counter++)
	{
		mSwitchBoard->SetOutput(counter, true);
		std::cout << counter << std::endl;
		Wait(3);
	}
	Wait(4);
	for(counter = 0; counter <= 15; counter++)
	{
		mSwitchBoard->SetOutput(counter, false);
	}
	Wait(4);
}

void SwitchBoard::PotPrint()
{
	std::cout << GetPot0() << "Pot 0" << std::endl;
	std::cout << GetPot1() << " Pot 1" << std::endl;
	std::cout << GetPot2() << " Pot 2" << std::endl;
}


void SwitchBoard::Print()
{
	if (GetSwitch1() == true && GetSwitch2() == true && GetSwitch3() == true)
	{
		std::cout << "Pot 0 = " << GetPot0() ;
		std::cout << "   Pot 1 = " << GetPot1() ;
		std::cout << "   Pot 2 = " << GetPot2() ;
		std::cout << "   Counter = " << GetCounter() << std::endl;
	}

	if(GetBig() == true)
	{
		SetLedBigButton(true);
		std::cout << "Big Red Button Pressed" << std::endl;
	}
	else
	{
		SetLedBigButton(false);
	}

	if(GetSmall() == true)
	{
		SetLedSmallButton(true);
		std::cout << "Small Red Button Pressed" << std::endl;
	}
	else
	{
		SetLedSmallButton(false);
	}

	if(GetJoystickDown() == true)
	{
		SetLedJoystickDown(true);
		std::cout << "Joystick Down" << std::endl;
	}
	else
	{
		SetLedJoystickDown(false);
	}

	if(GetJoystickUp() == true)
	{
		SetLedJoystickUp(true);
		std::cout << "Joystick Up" << std::endl;
	}
	else
	{
		SetLedJoystickUp(false);
	}

	if(GetSwitch0() == true)
	{
		std::cout << "Switch 0 on" << std::endl;
	}

	if(GetSwitch1() == true)
	{
		std::cout << "Switch 1 on" << std:: endl;
	}

	if(GetSwitch2() == true)
	{
		std::cout << "Switch 2 on" << std::endl;
	}

	if(GetSwitch3() == true)
	{
		std::cout << "Switch 3 on" << std::endl;
	}

	if(GetSwitch4() == true)
	{
		std::cout << "Switch 4 on" << std::endl;
	}

}
