//Author: Yash Gautam, Darry-L Wong, , Aaron Stoll
//Date: 1/14/17

//Revision History:


#ifndef LOGITECH_H
#define LOGITECH_H

#include <memory>

namespace frc
{
class Joystick;
}

class Logitech
{
public:

	static constexpr int TRIGGER = 1;
	static constexpr int BUTTON_2 = 2;
	static constexpr int BUTTON_3 = 3;
	static constexpr int BUTTON_4 = 4;
	static constexpr int BUTTON_5 = 5;
	static constexpr int BUTTON_6 = 6;
	static constexpr int BUTTON_7 = 7;
	static constexpr int BUTTON_8 = 8;
	static constexpr int BUTTON_9 = 9;
	static constexpr int BUTTON_10 = 10;
	static constexpr int BUTTON_11 = 11;
	static constexpr int BUTTON_12 = 12;
	static constexpr int X_AXIS = 0;
	static constexpr int Y_AXIS = 1;
	static constexpr int Z_ROTATE = 2;
	static constexpr int SLIDER = 3;


	Logitech(int port);

	bool GetTrigger();
	bool Get2();
	bool Get3();
	bool Get4();
	bool Get5();
	bool Get6();
	bool Get7();
	bool Get8();
	bool Get9();
	bool Get10();
	bool Get11();
	bool Get12();

	double GetX();
	double GetY();
	double GetZRotate();
	double GetSlider();

	int GetPOV();

	void SetDeadZone(double deadzone);
	void Print();

private:
	std::shared_ptr<frc::Joystick> mLogitech;
	double mDeadzone;
	bool GetButton(int button);
	double GetAxis(int axis);

};



#endif
