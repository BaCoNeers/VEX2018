#pragma config(Sensor, in1,    claw_rollS,     sensorPotentiometer)
#pragma config(Sensor, in2,    claw_pitchS,    sensorPotentiometer)
#pragma config(Sensor, dgtl1,  testTouch,      sensorTouch)
#pragma config(Motor,  port1,           harvester_2,   tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           left_drive,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           right_drive,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           launch_motor_1, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           launch_motor_2, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           claw_roll,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           left_lift,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           right_lift,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           claw_pitch,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          harvester_1,   tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#define sign(value) (value >= 0 ? 1 : -1)
#define max(a,b) (a > b ? a : b)
#define controller_launch_forwards Btn5UXmtr2
#define controller_launch_backwards Btn5DXmtr2
#define controller_drive_left Ch3
#define controller_drive_right Ch2
#define controller_arm Ch2Xmtr2
#define controller_harvester_toggle Btn7UXmtr2
#define controller_harvester2 Btn7DXmtr2
#define controller_claw_rotate_left Btn8LXmtr2
#define controller_claw_rotate_right Btn8RXmtr2
#define controller_claw_pitch_up Btn8UXmtr2
#define controller_claw_pitch_down Btn8DXmtr2

float get_power_smooth(float controller_input)
{
	float controller_sign = sign(controller_input);
	float new_input = controller_input * controller_sign;

	new_input = max(0.0, new_input - 0.15) / (1.0 - 0.15);
	return controller_sign * new_input;
}

float get_power(float yAxis)
{
	float powers[6] = {0.0,0.3,0.5,0.7,1.0,1.0};
	float s = sign(yAxis);

	float y = abs(yAxis);

	int i = ((int)(y * 10.0))/2;

	if (i > 5) {
		return powers[5]*s;
		} else {
		return powers[i]*s;
	}
}

void set_motor_speed(int _motor, float speed)
{
	int maximum_speed = speed > 0 ? 127 : 128;
	int motor_speed = maximum_speed * speed;
	motor[_motor] = motor_speed;
}

void set_launch_motor(float speed)
{
	set_motor_speed(launch_motor_1, speed);
	set_motor_speed(launch_motor_2, speed);
}

void set_left_drivetrain(float speed)
{
	set_motor_speed(left_drive, speed);
}

void set_right_drivetrain(float speed)
{
	set_motor_speed(right_drive, speed);
}

float get_joystick_axis(int index)
{
	float maximum_range = vexRT[index] > 0 ? -127.0 : -128.0;
	float joy_pos = (float)vexRT[index];
	return get_power_smooth(joy_pos / maximum_range);
}

float get_drive_left()
{
	return get_joystick_axis(controller_drive_left);
}

float get_drive_right()
{
	return get_joystick_axis(controller_drive_right);
}

float get_arm()
{
	return get_joystick_axis(controller_arm);
}

float get_potentiometer_degrees(int sensor)
{
	return 250.0 * (float)SensorValue[sensor] / 4095.0;
}

bool harvester_running = false;
bool previous_harvester_button_state = false;
void harvester_update()
{
	bool current_harvester_button_state = (bool)vexRT[controller_harvester_toggle];
	if(previous_harvester_button_state == true && current_harvester_button_state == false)
	{
		harvester_running = !harvester_running;
	}
	previous_harvester_button_state = current_harvester_button_state;
}

bool claw_roll_toggle = false;
bool previous_claw_roll_button_state = false;
void claw_roll_update()
{
	bool current_claw_roll_button_state = (bool)vexRT[controller_harvester2];
	if(previous_claw_roll_button_state == true && current_claw_roll_button_state == false)
	{
		claw_roll_toggle = !claw_roll_toggle;
	}
	previous_claw_roll_button_state = current_claw_roll_button_state;
}



task main()
{
	while(true)
	{
		harvester_update();
		//claw_roll_update();

		if(harvester_running)
		{
			set_motor_speed(harvester_1, 1.0);
		}
		else
		{
			set_motor_speed(harvester_1, 0.0);
		}

		if(vexRT[controller_harvester2])
		{
			set_motor_speed(harvester_2, -1.0);
		}
		else
		{
			set_motor_speed(harvester_2, 0.0);
		}

		if(vexRT[controller_claw_rotate_left])
		{
			set_motor_speed(claw_roll, -0.4);
		}
		else if(vexRT[controller_claw_rotate_right])
		{
			set_motor_speed(claw_roll, 0.4);
		}
		else
		{
			set_motor_speed(claw_roll, 0.0);
		}

		if(vexRT[controller_claw_pitch_up])
		{
			set_motor_speed(claw_pitch, 0.4);
		}
		else if(vexRT[controller_claw_pitch_down])
		{
			set_motor_speed(claw_pitch, -0.4);
		}
		else
		{
			set_motor_speed(claw_pitch, 0.0);
		}

		float arm_speed = get_joystick_axis(controller_arm);
		set_motor_speed(left_lift, arm_speed);
		set_motor_speed(right_lift, arm_speed);


		/*
		float claw_roll_angle = get_potentiometer_degrees(claw_roll);
		if(claw_roll_toggle)
		{
			float target_angle = 180.0;
			float relative_angle = abs(target_angle - claw_roll_angle);

			if(relative_angle > 3.0)
			{
				set_motor_speed(claw_roll, 0.5);
			}
			else
			{
				set_motor_speed(claw_roll, 0.0);
			}

		}
		else
		{
			float target_angle = 0.0;
			float relative_angle = abs(target_angle - claw_roll_angle);

			if(relative_angle > 3.0)
			{
				set_motor_speed(claw_roll, -0.5);
			}
			else
			{
				set_motor_speed(claw_roll, 0.0);
			}
		}*/


		float speed_left = get_drive_left();
		set_left_drivetrain(speed_left);

		float speed_right = get_drive_right();
		set_right_drivetrain(speed_right);


		int test2 = SensorValue[testTouch];


		if(vexRT[controller_launch_forwards])
		{
			set_launch_motor(1.0);
		}
		else if(vexRT[controller_launch_backwards])
		{
			set_launch_motor(-1.0);
		}
		else
		{
			set_launch_motor(0.0);
		}


	}
}
