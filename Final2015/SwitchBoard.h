#ifndef SWITCHBOARD_H
#define SWITCHBOARD_H

class Switchboard

{
public:
	Switchboard(Joystick*);
	bool TestSwitchboard();
	int GetCounter();
	bool GetRawButton(int);
	float GetPotentiometer(int);
	void SetLED(int, bool);

private:
	Joystick* mLaunchpad;
	int mCounter;
};

Switchboard::Switchboard(Joystick* pLaunchpad)
{
	mLaunchpad = pLaunchpad;
	mCounter = GetCounter();
}

bool Switchboard::TestSwitchboard()
{
	int counter;

	if (GetRawButton(SWITCHBOARD_BUTTON_1))
	{
		printf ("Button 1 Pressed \n");
	}
	if (GetRawButton(SWITCHBOARD_BUTTON_2))
	{
		printf ("Button 2 Pressed \n");
	}
	if (GetRawButton(SWITCHBOARD_BUTTON_3))
	{
		printf ("Button 3 Pressed\n");
	}
	if (GetRawButton(SWITCHBOARD_BUTTON_4))
	{
		printf ("Button 4 Pressed\n");
	}
	if (GetRawButton(SWITCHBOARD_BUTTON_5))
	{
		printf ("Button 5 Pressed\n");
	}
	if (GetRawButton(SWITCHBOARD_JOYSTICK_UP))
	{
		printf ("Joystick up\n");
		SetLED(SWITCHBOARD_JOYSTICK_UP_LED, true);
	}
	else
	{
		SetLED(SWITCHBOARD_JOYSTICK_UP_LED, false);
	}
	if (GetRawButton(SWITCHBOARD_JOYSTIC_DOWN))
	{
		printf ("Joystick down\n");
		SetLED(SWITCHBOARD_JOYSTICK_DOWN_LED, true);
	}
	else
	{
		SetLED(SWITCHBOARD_JOYSTICK_DOWN_LED, false);
	}
	if (GetRawButton(SWITCHBOARD_LITTLE_RED_BUTTON))
	{
		printf ("Little Red Button Pressed\n");
		SetLED(SWITCHBOARD_LITTLE_RED_BUTTON_LED, true);
	}
	else
	{
		SetLED(SWITCHBOARD_LITTLE_RED_BUTTON_LED, false);
	}
	if (GetRawButton(SWITCHBOARD_BIG_RED_BUTTON))
	{
		printf ("Big Red Button Pressed\n");
		SetLED(SWITCHBOARD_BIG_RED_BUTTON_LED, true);
	}
	else
	{
		SetLED(SWITCHBOARD_BIG_RED_BUTTON_LED, false);
	}

	counter = GetCounter();
	if(counter == 1)
	{
		printf("Potentiometer 1:  %f\n", GetPotentiometer(SWITCHBOARD_POT_1));
	}
	else if(counter == 2)
	{
		printf("Potentiometer 2:  %f\n", GetPotentiometer(SWITCHBOARD_POT_2));
	}
	else if(counter == 3)
	{
		printf("Potentiometer 3:  %f\n", GetPotentiometer(SWITCHBOARD_POT_3));
	}
	if(mCounter != counter)
	{
		printf("Counter:  %d\n", counter);
		mCounter = counter;
	}
	return GetRawButton(SWITCHBOARD_BIG_RED_BUTTON) && GetRawButton(SWITCHBOARD_LITTLE_RED_BUTTON);
}

int Switchboard::GetCounter()
{
	int counter = 0;
	if(mLaunchpad->GetRawButton(SWITCHBOARD_COUNTER_ONES))
	{
		counter = counter + 1;
	}
	if(mLaunchpad->GetRawButton(SWITCHBOARD_COUNTER_TWOS))
	{
		counter = counter + 2;
	}
	if(mLaunchpad->GetRawButton(SWITCHBOARD_COUNTER_FOURS))
	{
		counter = counter + 4;
	}
	return counter;
}

bool Switchboard::GetRawButton(int button)
{
	return mLaunchpad->GetRawButton(button);
}

float Switchboard::GetPotentiometer(int pot)
{
	return mLaunchpad->GetRawAxis(pot);
}

void Switchboard::SetLED(int LED, bool state)
{
	mLaunchpad->SetOutput(LED, state);
}

#endif
