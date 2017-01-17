#include <iostream>
#include <memory>
#include <string>

#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <VictorSP.h>
#include <DIgitalInput.h>
#include <Relay.h>
#include <CameraServer.h>

class Robot: public SampleRobot
{
	RobotDrive myRobot {3, 2, 1 ,0};
	Joystick stick {0};
    frc::VictorSP MainShooterRight {7};
    frc::VictorSP MainShooterLeft {8};
    frc::VictorSP MainShooterAngle {6};
    frc::VictorSP Arm {5};
    frc::Relay Trigger {0, Relay::Direction::kBothDirections};
    frc::DigitalInput LimtSwitch {5};

	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";

public:
	Robot(){
		//Note SmartDashboard is not initialized here, wait until RobotInit to make SmartDashboard calls
		myRobot.SetExpiration(0.1);
	}

	void RobotInit()
	{
		chooser.AddObject(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		SmartDashboard::PutData("Auto Modes", &chooser);
		frc::CameraServer::GetInstance()->StartAutomaticCapture();
	}

	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void Autonomous()
	{
		auto autoSelected = chooser.GetSelected();
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 1.0); 	// spin at half speed
			Wait(2.0); 				//    for 2 seconds
			myRobot.Drive(0.0, 0.0); 	// stop robot
		} else {
			//Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			Wait(2.0); 				//    for 2 seconds
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}

	}

	/**
	 * TeliopCode
	 */
	void OperatorControl()
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled())
		{
			float stickY = (float)stick.GetY();//converting to float
			float stickX = (float)stick.GetX();

			myRobot.ArcadeDrive(stickY, -stickX); // drive with arcade style (use right stick)

			if (stick.GetRawButton(1)) //Fire solenoid
			{
                Trigger.Set(Relay::Value::kReverse);
                Wait(0.01);
                Trigger.Set(Relay::Value::kOff);
			}

			if (stick.GetRawButton(6)) //Adjust Arm angle
			{
				Arm.Set(0.25);
			}else if (stick.GetRawButton(4) && LimtSwitch.Get() != false)
			{
				Arm.Set(-0.25);
			}else
			{
				Arm.Set(0);
			}

            if (stick.GetRawButton(5)) //Adjust Shooter angle
            {
                MainShooterAngle.Set(-0.25);
            }
            else if (stick.GetRawButton(3))
            {
                MainShooterAngle.Set(0.25);
            }
            else
            {
                MainShooterAngle.Set(0);
            }

            if (stick.GetRawButton(2)) //Spin up wheels for shooter
            {
                MainShooterLeft.Set(-1);
                MainShooterRight.Set(1);
                Wait(0.1);
                MainShooterLeft.Set(0);
                MainShooterRight.Set(0);
            }else if (stick.GetRawButton(10)) //Spin up wheels for intake
            {
                MainShooterLeft.Set(0.40);
                MainShooterRight.Set(-0.40);
                Wait(0.1);
                MainShooterLeft.Set(0);
                MainShooterRight.Set(0);
            }

			Wait(0.005);
		}
	}

	/**
	 * Runs during test mode
	 */
	void Test()
	{
	}
};

START_ROBOT_CLASS(Robot)
