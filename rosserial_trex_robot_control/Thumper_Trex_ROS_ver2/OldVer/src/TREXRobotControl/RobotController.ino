// RobotController.ino

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "trex_robot_control/RobotController.h"
#include "trex_robot_control/IOpins.h"
#include "trex_robot_control/Constants.h"

byte LeftMotorBrake;
byte RightMotorBrake;

unsigned long RobotController::VoltageTimer;
unsigned long RobotController::AccelTimer;
unsigned long RobotController::IRangeTimer;

RobotController::RobotController()
{
	LeftMode = 0;
	RightMode = 0;		
	LeftMotorSpeed = 0;
	RightMotorSpeed = 0;

	LeftMotorBrake = 0;
	RightMotorBrake = 0;	

	LeftMotorCur = 0;
	RightMotorCur = 0;
	voltage = 0.0;

	magnitude = 0;
	Xaxis = 0;
	Yaxis = 0;
	Zaxis = 0;
	dX = 0;
	dY = 0;
	dZ = 0;

	VoltageTimer = 0;
	AccelTimer = 0;
	IRangeTimer = 0;
}

void RobotController::InitPins()
{
  pinMode(LEFTMOTORPWM_pin,OUTPUT);          // configure left  motor PWM       pin for output
  pinMode(LEFTMOTORDIRECTION_pin,OUTPUT);   // configure left  motor direction pin for output
  pinMode(LEFTMOTORBRAKE_pin,OUTPUT);      // configure left  motor brake     pin for output
  
  pinMode(RIGHTMOTORPWM_pin,OUTPUT);        // configure right motor PWM       pin for output
  pinMode(RIGHTMOTORDIRECTION_pin,OUTPUT); // configure right motor direction pin for output
  pinMode(RIGHTMOTORBRAKE_pin,OUTPUT);     // configure right motor brake     pin for output	
}

void RobotController::InitIRangeData(sensor_msgs::Range &range_msg)
{
	char frameid[] = "/ir_ranger";

	range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  	range_msg.header.frame_id =  frameid;
  	range_msg.field_of_view = 0.01;
  	range_msg.min_range = 0.03;
  	range_msg.max_range = 0.4;	
}

void RobotController::IRScan(sensor_msgs::Range &range_msg)
{
	int sample;
        // Get data
	sample = analogRead(IR_pin) / 4;
  	// if the ADC reading is too low, then we are really far away from anything
  	if(sample < SAMPLEUPPERBOUND)
  	{
	    range_msg.range = MAXRANGE;     // max range
            return;		
  	}   	

  	sample = 1309 / (sample - 3); // Magic numbers to get cm
  	range_msg.range =  (sample - 1) / 100; //convert to meters
}

int RobotController::ShutdownMotors()
{	
	LeftMotorSpeed = 0;      // set left  motor speed to 0 (off)
	RightMotorSpeed = 0;      // set right motor speed to 0 (off)
	ProcessMotorCommand(LeftMotor);       // update H bridges
	ProcessMotorCommand(RightMotor);       // update H bridges
}

int RobotController::CheckMsgValidity(const trex_robot_control::TrexMotorCmds &nmc)
{
	if((int)nmc.cmd_type == 72 || (int)nmc.cmd_type == 90) // i.e., decimal equivalents for "H" (mode and speed values for the motors) or for "Z" (hibernate/power_save mode for the robot)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int RobotController::SerialCommunicate(const trex_robot_control::TrexMotorCmds &nmc)
{
    int StatusFlag = 0;
    switch((int)nmc.cmd_type)
    {
      case 72: //i.e., "H"
      	LeftMotorBrake = 0;
		RightMotorBrake = 0;
        SetPWM(LeftMotor, nmc);
        SetPWM(RightMotor, nmc);
        StatusFlag = 1;
        break;

      case 90: //i.e., "Z"
      	ShutdownMotors();
        StatusFlag = 1;      
        break; 

      default:
        break;
     }
     
     return StatusFlag; 
}

void RobotController::SetPWM(int WhichMotor, const trex_robot_control::TrexMotorCmds &nmc)
{
	switch(WhichMotor)
	{
		case LeftMotor:
		  LeftMode = (int)nmc.left_mode;
		  LeftMotorSpeed = (int)nmc.left_speed * SpeedScale;	
		  break;

		case RightMotor:
		  RightMode = (int)nmc.right_mode;
		  RightMotorSpeed = (int)nmc.right_speed * SpeedScale;  
		  break;

		default:
		  break;
	}
}

void RobotController::ProcessMotorCommand(int WhichMotor)
{
	switch(WhichMotor)
	{
		case LeftMotor:
		  digitalWrite(LEFTMOTORBRAKE_pin,LeftMotorBrake > 0);                     // if left brake>0 then engage electronic braking for left motor
		  digitalWrite(LEFTMOTORDIRECTION_pin,LeftMode == 2);                     // if left speed>0 then left motor direction is forward else reverse
		  analogWrite (LEFTMOTORPWM_pin,abs(LeftMotorSpeed));                  // set left PWM to absolute value of left speed - if brake is engaged then PWM controls braking
		  break;

		case RightMotor:
		  digitalWrite(RIGHTMOTORBRAKE_pin,RightMotorBrake > 0);                     // if left brake>0 then engage electronic braking for left motor
		  digitalWrite(RIGHTMOTORDIRECTION_pin,RightMode == 2);                     // if left speed>0 then left motor direction is forward else reverse
		  analogWrite (RIGHTMOTORPWM_pin,abs(RightMotorSpeed));                  // set left PWM to absolute value of left speed - if brake is engaged then PWM controls braking
		  break;

	}
}

void RobotController::MonitorVoltageLevel(trex_robot_control::TrexPowerMsg &tpm)
{

	// read  left motor current sensor and convert reading to mA
	LeftMotorCur = (analogRead(LEFTMOTORCURRENT_pin) - 511) * 48.83;          
	// read right motor current sensor and convert reading to mA
	RightMotorCur = (analogRead(RIGHTMOTORCURRENT_pin) - 511) * 48.83;          
	// read battery level and convert to volts with 2 decimal places (eg. 1007 = 10.07 V)	
	voltage = analogRead(VOLTAGE_pin) * 10 / 3.357;             
	
	tpm.voltage_level = voltage;
	tpm.timing_msg = millis();

	if(voltage < LOWBAT) 
	{
		tpm.power_msg = "Out of Charge";
		ShutdownMotors();
	}		
	else
	{
		tpm.power_msg = "Charged";
	}
}

void RobotController::Accelerometer(std_msgs::Int32 &accel_msg)
{
  static int X_prev, Y_prev, Z_prev, vibration;    // local variables used for impact detection
  // number of 2mS intervals to wait after an impact has occured before a new impact can be recognized
  byte devibrate = 50; 
  
  X_prev = Xaxis;   // store previous accelerometer readings for comparison
  Y_prev = Yaxis;
  Z_prev = Zaxis;
  
  vibration--;    // loop counter prevents false triggering cause by impact vibration
  if(vibration < 0)
  {
  	 vibration = 0;             // as robot vibrates due to impact
  }

  Xaxis = analogRead(XAXIS_pin); // read accelerometer - note analog read takes 260uS for each axis
  Yaxis = analogRead(YAXIS_pin);
  Zaxis = analogRead(ZAXIS_pin);

  if(vibration > 0) 
  {
  	return;   // until vibration has subsided no further calculations required
  }
  
  dX = Xaxis-X_prev;    // difference between old and new axis readings
  dY = Yaxis-Y_prev;
  dZ = Zaxis-Z_prev;
  // magnitude of delta x,y,z using Pythagorus's Theorum  
  magnitude = sqrt(sq(dX) + sq(dY) + sq(dZ));

  accel_msg.data = magnitude;
  
  if (magnitude > sensitivity) // has a new impact occured
  {
    vibration = devibrate;   // reset anti-vibration counter
    return;
  }
  else
  {
    magnitude = 0;       // no impact detected
    dX = 0;
    dY = 0;
    dZ = 0;
  }

  // accel_msg.data = magnitude;
}
