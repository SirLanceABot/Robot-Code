#ifndef ARM_SERVO_H
#define ARM_SERVO_H

class ArmServo
{
private:
	Joystick* mXbox;
	Servo* mServo;
	int angle;

public:
	ArmServo(Joystick*, Servo*);
	void ServoTest();
	void TestArmServo();
	void Test2();
};

ArmServo::ArmServo(Joystick* pXbox, Servo* pServo)
{
	mXbox = pXbox;
	mServo = pServo;

	angle = mServo->GetAngle();
}

void ArmServo::TestArmServo()
{
	int servoVal = mServo->GetAngle();
	int servoVal1 = mServo->Get();
	int servoVal2 = mServo->GetRaw();

	double rightX = mXbox->GetRawAxis(4);

	printf("ServoVal (GetAngle):  %d", servoVal);
	printf(", ServoVal (Get):  %d", servoVal1);
	printf(", ServoVal (GetRaw):  %d", servoVal2);
	printf(", Right X:  %f\n", rightX);

	if(fabs(rightX) >= 0.2)
	{
		mServo->Set(rightX);
	}
//	else
//	{
//		mServo->Set(0.0);
//	}

	if(mXbox->GetRawButton(XBOX_LEFT_BUMPER))
	{
		angle = angle - 30;
		mServo->Set(-1.0);
	}
	else if(mXbox->GetRawButton(XBOX_RIGHT_BUMPER))
	{
		angle = angle + 30;
		mServo->Set(1.0);
	}
	if(angle <= servoVal + 1 && angle >= servoVal -1)
	{
		mServo->Set(0);
	}
	if(mXbox->GetRawButton(1))
	{
		while(mXbox->GetRawButton(1)){}
			mServo->SetAngle(5);
	}
	if(mXbox->GetRawButton(2))
	{
		while(mXbox->GetRawButton(2)){}
			mServo->SetAngle(0.83);
	}
	if(mXbox->GetRawButton(4))
	{
		while(mXbox->GetRawButton(4)){}
			mServo->SetAngle(0);
	}
	if(mXbox->GetRawButton(3))
	{
		while(mXbox->GetRawButton(3)){}
			mServo->SetAngle(2.5);
	}
}

void ArmServo::Test2()
{
	mServo->SetAngle(0.0);
//	mServo->SetBounds(360.0, 10.0, 180.0, 10.0, 0);
}

#endif
