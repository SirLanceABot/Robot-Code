// Author: Yash Gautam, Darry-L Wong, Aaron Stoll
// Date: 1/16/17

// Revision History:
// 1/17/17 DW, YG: Added ZRotate, fixed deadzone for axes
// 1/30/17 AS, fixed Deadband.
// 2/9/17 DW YG, fixed aarons fixing deadband, changed /n to std::endl, changed test to print, and changed the float axis port to int axis port

#include "Logitech.h"
#include "WPILib.h"

Logitech::Logitech(int port):
mLogitech(std::make_shared<frc::Joystick>(port)),
mDeadzone(0.2)
{
	std::cout << "[Logitech]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}

//This function returns a value true or false if the button is pressed
bool Logitech::GetButton(int button)

{
	return mLogitech->GetRawButton(button);
}

//This function returns a value true or false if the trigger is pressed
bool Logitech::GetTrigger()
{
	return GetButton(TRIGGER);
}

//This function returns a value true or false if button 2 is pressed
bool Logitech::Get2()
{
	return GetButton(BUTTON_2);
}

//This function returns a value true or false if button 3 is pressed
bool Logitech::Get3()
{
	return GetButton(BUTTON_3);
}
//This function returns a value true or false if button 4 is pressed
bool Logitech::Get4()
{
	return GetButton(BUTTON_4);
}

//This function returns a value true or false if button 5 is pressed
bool Logitech::Get5()
{
	return GetButton(BUTTON_5);
}

//This function returns a value true or false if button 6 is pressed
bool Logitech::Get6()
{
	return GetButton(BUTTON_6);
}

//This function returns a value true or false if button 7 is pressed
bool Logitech::Get7()
{
	return GetButton(BUTTON_7);
}

//This function returns a value true or false if button 8 is pressed
bool Logitech::Get8()
{
	return GetButton(BUTTON_8);
}

//This function returns a value true or false if button 9 is pressed
bool Logitech::Get9()
{
	return GetButton(BUTTON_9);
}

//This function a returns value true or false if button 10 is pressed
bool Logitech::Get10()
{
	return GetButton(BUTTON_10);
}

//This function returns a value true or false if button 11 is pressed
bool Logitech::Get11()
{
	return GetButton(BUTTON_11);
}

//This function returns a value true or false if button 12 is pressed
bool Logitech::Get12()
{
	return GetButton(BUTTON_12);
}

//This function returns a value -1 to 1 depending on where the axis is
double Logitech::GetAxis(int axis)
{
	double value = mLogitech->GetRawAxis(axis);

	if(value < mDeadzone && value > -mDeadzone)
	{
		value = 0.0;
	}
	return value;
}

//This function returns a value -1 to 1 depending on where the x axis is
double Logitech::GetX()
{
	return GetAxis(X_AXIS);
}

//This function returns a value -1 to 1 depending on where the y axis is
double Logitech::GetY()
{
	return -GetAxis(Y_AXIS);
}

//This function returns a value -1 to 1 depending on where the Joystick is rotated
double Logitech::GetZRotate()
{
	return GetAxis(Z_ROTATE);
}

//This function returns a value -1 to 1 depending on where the slider's axis is
double Logitech::GetSlider()
{
	return -GetAxis(SLIDER);
}

//This function returns a number value from 0 to 360 depending on where the POV is
int Logitech::GetPOV()
{
	return mLogitech->GetPOV();
}

// Sets the deadband for stick
void Logitech::SetDeadZone(double deadzone)
{
	this->mDeadzone = deadzone;
}

void Logitech::Print()
{
	if(GetTrigger() == true)
		std::cout << "Trigger is pressed" << std::endl;

	if(Get2() == true)
		std::cout <<"Button 2 is pressed" << std::endl;

	if(Get3() == true)
		std::cout <<"Button 3 is pressed" << std::endl;

	if(Get4() == true)
		std::cout <<"Button 4 is pressed" << std::endl;

	if(Get5() == true)
		std::cout <<"Button 5 is pressed" << std::endl;

	if(Get6() == true)
		std::cout <<"Button 6 is pressed" << std::endl;

	if(Get7() == true)
		std::cout <<"Button 7 is pressed" << std::endl;

	if(Get8() == true)
		std::cout <<"Button 8 is pressed" << std::endl;

	if(Get9() == true)
		std::cout <<"Button 9 is pressed" << std::endl;

	if(Get10() == true)
		std::cout <<"Button 10 is pressed" << std::endl;

	if(Get11() == true)
		std::cout <<"Button 11 is pressed" << std::endl;

	if(Get12() == true)
		std::cout <<"Button 12 is pressed" << std::endl;

	if(GetX() != 0)
		std::cout << "Axis X: " << GetX() << std::endl;

	if(GetY() != 0)
		std::cout << "Axis Y: " << GetY() << std::endl;

	if(GetZRotate() != 0)
		std::cout << "Z Rotate: " << GetZRotate() << std::endl;

	if(GetSlider() != 0)
		std::cout << "Slider: " << GetSlider() << std::endl;

	if(GetPOV() != -1)
		std::cout << "POV: " << GetPOV() << std::endl;

}



