
//////Shooter////////
		if (BtnShooter && !LastButtonState){
			CurrentButtonState = true;
		}

		if (CurrentButtonState != LastButtonState){
			LastButtonState = CurrentButtonState;

		}
		if (CurrentButtonState){
			motor[launch_motor_1] = 127;
			motor[launch_motor_2] = 127;
		}
		else {
			motor[launch_motor_1] = 0;
			motor[launch_motor_2] = 0;

		}
