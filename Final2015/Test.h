/*
 * Test.h
 *
 *  Created on: Feb 13, 2015
 *      Author: Robotics
 */

#ifndef SRC_TEST_H
#define SRC_TEST_H
using namespace std;

class TestMenu
{
public:
	TestMenu(Joystick*, Drive*, LHSVision*, Arm*, Elevator*);
	~TestMenu();
	void Menu();
private:
	int selection = 1;
	bool inMenu = true;
	Joystick* mXbox;
	Drive* mDriveClass;
	LHSVision* mLHSVisionClass;
	Arm* mArmClass;
	Elevator* mElevatorClass;
};

TestMenu::TestMenu(Joystick* pXbox, Drive* pDriveClass, LHSVision* pLHSVisionClass, Arm* pArmClass, Elevator* pElevatorClass)
{
	mXbox = pXbox;
	mDriveClass = pDriveClass;
	mLHSVisionClass = pLHSVisionClass;
	mArmClass = pArmClass;
	mElevatorClass = pElevatorClass;
}

TestMenu::~TestMenu()
{
	delete mXbox;
}

void TestMenu::Menu()
{
	for(int x = 0; x > 20; x++)
	{
		printf(" \n");
	}

	inMenu = true;
	while (inMenu == true)
	{
		for(int x = 0; x > 10; x++)
		{
			printf("\n");
		}
		if (selection == 1)
		{
			printf("-->");
		}
		printf("Drive Test\n");
		if (selection == 2)
		{
			printf("-->");
		}
		printf("Vision Test\n");
		if (selection == 3)
		{
			printf ("-->");
		}
		printf("Arm Test\n");
		if (selection == 4)
		{
			printf("-->");
		}
		printf("Elevator Test\n");

		if (mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS) < -0.5)
		{
			while (mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS) < -0.5)
			{}
			selection = max(selection - 1, 1);
		}
		if (mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS) > 0.5)
		{
			while (mXbox->GetRawButton(XBOX_LEFT_Y_AXIS) > 0.5)
			{}
			selection = min(selection + 1, 4);
		}
		if (mXbox->GetRawButton(XBOX_B_BUTTON) == true)
		{
			if (selection == 1)
			{
				printf("Drive\n");
				mDriveClass->TestDrive();
				Wait(1.0);
			}
			if (selection == 2)
			{
				printf("Vision\n");
				mLHSVisionClass->TestVision();
				Wait(1.0);
			}
			if (selection == 3)
			{
				printf("Arm\n");
				mArmClass->TestArm();
				Wait(1.0);
			}
			if (selection == 4)
			{
				printf("Elevator\n");
				mElevatorClass->TestElevator();
				Wait(1.0);
			}
		}
		Wait(0.02);
	}
}

#endif /* SRC_TEST_H_ */
