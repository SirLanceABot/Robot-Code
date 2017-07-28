// Author: Aaron Stoll and Ben Poopzycki
// Date: 1/14/17
// Revision History:
//	BP & AS 9/2/17: changed Test and PotTest to Print and PotPrint

#ifndef SWITCHBOARD_H
#define SWITCHBOARD_H

#include <memory>

namespace frc
{
class Joystick;
}


class SwitchBoard
{
public:
	static constexpr int SWITCH_0 = 1;
	static constexpr int SWITCH_1 = 2;
	static constexpr int SWITCH_2 = 3;
	static constexpr int SWITCH_3 = 4;
	static constexpr int SWITCH_4 = 5;
	static constexpr int BIG_BUTTON = 13;
	static constexpr int SMALL_BUTTON = 12;
	static constexpr int JOYSTICK_DOWN = 7;
	static constexpr int JOYSTICK_UP = 6;
	static constexpr int POT_0 = 0;
	static constexpr int POT_1 = 1;
	static constexpr int POT_2 = 2;
	static constexpr int COUNTER_1 = 8;
	static constexpr int COUNTER_2 = 9;
	static constexpr int COUNTER_4 = 10;
	static constexpr int LED_JOYSTICK_DOWN = 1;
	static constexpr int LED_JOYSTICK_UP = 2;
	static constexpr int LED_SMALL_BUTTON = 3;
	static constexpr int LED_BIG_BUTTON = 4;

	SwitchBoard(int port);

	bool GetSwitch0();
	bool GetSwitch1();
	bool GetSwitch2();
	bool GetSwitch3();
	bool GetSwitch4();
	bool GetBig();
	bool GetSmall();
	bool GetJoystickDown();
	bool GetJoystickUp();
	bool previousswitchone();


	void SetLedJoystickDown(bool on);
	void SetLedJoystickUp(bool on);
	void SetLedSmallButton(bool on);
	void SetLedBigButton(bool on);
	void Print();
	void LedTest();
	void PotPrint();

	int GetCounter();

	double GetPot0();
	double GetPot2();
	double GetPot1();

private:
	std::shared_ptr<frc::Joystick> mSwitchBoard;
	bool GetButton(int button);
	double GetAxis(int axis);
	void SetLed(int port, bool on);
};



#endif
