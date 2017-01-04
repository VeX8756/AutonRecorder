// ------------------------------------------------------>>> PASTE <<<------------------------------------------------------ //
// -------->>>                       replace this comment with motor and sensor setup code                      <<<--------- //
// ------------------------------------------------------>>> PASTE <<<------------------------------------------------------ //

#pragma DebuggerWindows("debugStream")

// -------------------------------------------------- AUTONOMOUS RECORDER -------------------------------------------------- //
//            Records driver control actions to motor statements that can be used as an autonomous program.                  //
//                              Version 1.01, Doug Moyers, Mentor, Team 8756, December 2016                                  //
// ------------------------------------------------------------------------------------------------------------------------- //

// HOW TO USE THIS RECORDER:
// 1. Paste the motor control code from your competition template at line 1 above.
// 2. Paste the user control code from your competition template in the userControlCode function below.
// 3. Adjust R_mSec or R_Freq values if you want a different length or frequency of recording
// 4. Adjust minBattVoltage as desired.  Using a high-charge battery for recording and playback will improve playback consistancy.
// 5. Have a game field set up and a driver ready to drive the bot for an autonomous run.
// 6. While using the programming cable with the controller connected to the robot via vexNet, download this code.
// 7. Press start to run the code. There will be a 3 second count down before user control and recording starts.
// 8. Copy the text from the debug stream window into your cometition template's autonomous task.
// 9. Downdoad the competition template and test the autonomous using the competition control debug window or a field switch.


int R_mSec = 15000; 				// 15000, length of autonomous redording in miliseconds
int R_Freq = 50; 						// 50, the frequency of the recording in milisecons. A lower number may result in errors in the recording.
int minBattVoltage = 7800; 	// 7800, the minimum battery voltage at which a recording will be made

// function to hold user control code
void userControlCode()
{
	// ------------------------------------------------------>>> PASTE <<<------------------------------------------------------ //
	// ---->>>       Replace this comment with your user control code, everything within the while(true) brackets        <<<---- //
	// ---->>>   If you are using globals, functions, or tasks they must be coppied into this code above this function   <<<---- //
	// ------------------------------------------------------>>> PASTE <<<------------------------------------------------------ //
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                        NOTHING BELOW NEEDS TO BE EDITED FOR THE PROGRAM TO WORK                         ////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int loopCounter; 							// the number of each loop of code
int	loopOfLastChange; 				// last loop any motor value changed
int motorValue_Last[10]; 			// motor values of the previous loop
int sensorValue_Last[20]; 		// sensor values of the previous loop
int iemValue_Last[10]; 				// iem values of the previous loop

bool motorChange = false; 		// any motor change within the current loop sets to true
bool sensorPresent = false; 	// if any sensor is installed, code sets to true
bool sensorChange = false; 		// any sensor change within the current loop sets to true
bool iemPresent = false; 			// if any IEM is installed, code sets to true
bool iemChange = false; 			// any IEM change within the current loop sets to true

// task to record motor and sensor value changes to the debug stream
task recordAutonomous()
{
	// write initial sensor values for all sensors to debug stream as comments
	if (sensorPresent == true) // only record sensors if present
	{
		writeDebugStream("// Initial values for Sensors: ");
		for (int s = 0; s < 8; s++) // analogue sensors
		{
			if (SensorType[s] != 0) // sensor exists
			{
				writeDebugStream("in %d", s+1); // write values to the debugStream as comments
				writeDebugStream(" = %d  ", SensorValue(s));
				sensorValue_Last[s] = SensorValue(s);
			}
		}
		for (int s = 8; s < 20; s++)  // digital sensors
		{
			if (SensorType[s] != 0 && SensorType[s] != 81 && SensorType[s] != 56) // sensor exists, is not OSE/sonic 2
			{
				writeDebugStream("dgtl %d", s-7); // write values to the debugStream as comments
				writeDebugStream(" = %d  ", SensorValue(s));
				sensorValue_Last[s] = SensorValue(s);
			}
		}
		writeDebugStreamLine(" ");
	}
	if (iemPresent == true) // only record iem if present
	{
		writeDebugStream("// Initial values for IEMs: ");
		for (int m = 0; m < 10; m++)
		{
			if (getEncoderForMotor(m) != -1) // IEM exists
			{
				writeDebugStream("port %d", m+1); // write IME values to the debugStream as comments
				writeDebugStream(" = %d  ", nMotorEncoder[m]);
				iemValue_Last[m] = nMotorEncoder[m];
			}
		}
		writeDebugStreamLine(" ");
	}

	while(true)
	{
		for (int m = 0; m < 10; m++)
		{
			// create comments following and preceeding the motor command lines
			if(abs(motor[m] - motorValue_Last[m]) >= 3 && motorChange == false) // first motor change in a loop
			{
				// wait command and comment appended to previous loop motor lines so that wait of that loop can be calculated
				writeDebugStream("wait1Msec( P_Freq * %d );", (loopCounter - loopOfLastChange)); // wait command
				writeDebugStreamLine("		// wait %d", (loopCounter - loopOfLastChange) * R_Freq);	// wait comment
				writeDebugStreamLine(" ");
				writeDebugStream("// %d  ", loopCounter * R_Freq);	//print the current time of the autonomous

				// RECORD SENSOR VALUES
				if (sensorPresent == true) // only record sensors if present
				{
					writeDebugStream("Sensors: ");
					for (int s = 0; s < 8; s++)  // analogue sensors
					{
						if (SensorType[s] != 0 && abs(SensorValue(s) - sensorValue_Last[s]) >= 3) // sensor exists and has changed by 3
						{
							writeDebugStream("in %d", s+1); // write values to the debugStream as comments
							writeDebugStream(" = %d  ", SensorValue(s));
							sensorValue_Last[s] = SensorValue(s);
							sensorChange = true;
						}
					}
					for (int s = 8; s < 20; s++)   // digital sensors
					{
						if (SensorType[s] != 0 && SensorType[s] != 81 && SensorType[s] != 56 && abs(SensorValue(s) - sensorValue_Last[s]) >= 3) // sensor exists, is not OSE/sonic 2 and has changed by 3
						{
							writeDebugStream("dgtl %d", s-7); // write values to the debugStream as comments
							writeDebugStream(" = %d  ", SensorValue(s));
							sensorValue_Last[s] = SensorValue(s);
							sensorChange = true;
						}
						if (s == 19 && sensorChange == false) // all sensors checked and none changed
						{
							writeDebugStream("no change  ");
						}
					}
				}

				// RECORD IEM VALUES
				if (iemPresent == true) // only record iem if present
				{
					writeDebugStream("IEMs: ");
					for (int m = 0; m < 10; m++)
					{
						if (getEncoderForMotor(m) != -1 && nMotorEncoder[m] != iemValue_Last[m]) // IEM exists and has changed
						{
							writeDebugStream("port %d", m+1); // write IME values to the debugStream as comments
							writeDebugStream(" = %d  ", nMotorEncoder[m]);
							iemValue_Last[m] = nMotorEncoder[m];
							iemChange = true;
						}
						if (m == 9 && iemChange == false) // all IEMs checked and none changed
						{
							writeDebugStream("no change  ");
						}
					}
				}
				writeDebugStreamLine(" ");
			}

			// CREATE MOTOR COMMANDS
			if(abs(motor[m] - motorValue_Last[m]) >= 3 ) // if motorValue changes from last loop by 3 or more
			{
				writeDebugStream("motor[ port%d", m+1); // write changed values to the debugStream as motor[] commands
				writeDebugStream(" ] = %d;", motor[m]);
				writeDebugStreamLine(" ");
				motorValue_Last[m] = motor[m]; // update motorVaule_Last
				motorChange = true;
			}
		}

		// UPDATE VARIABLES
		if ( motorChange == true ) // if any motor value changed this loop
		{
			loopOfLastChange = loopCounter; // restart LastChange count
		}
		motorChange = false;
		sensorChange = false;
		iemChange = false;
		loopCounter++;

		wait1Msec(R_Freq);
	}
}

task main()
{
	clearDebugStream();	// clear previous recordings

	if (nImmediateBatteryLevel >= minBattVoltage)
	{
		// initialize sensorValue_Last array to 0
		for (int s = 0; s < 20; s++)
		{
			sensorValue_Last[s] = 0;
			if (SensorType[s] == 46)
			{
				writeDebugStreamLine("// Hang on 2 seconds, resetting gyro.");
				SensorType[s] = sensorNone;
				wait1Msec(500);
				SensorType[s] = sensorGyro; // reset gyro(s) to 0
				wait1Msec(1500);
			}
			if (SensorType[s] == 80)
			{
				SensorValue(s) = 0; // set OSE sensors to 0
			}
			if (SensorValue[s] != 0)
			{
				if (sensorPresent == false){writeDebugStreamLine("// Sensor(s) detected, nice!");}
				sensorPresent = true;
			}
		}

		// initialize motorValue_Last array to 0
		for (int m = 0; m < 10; m++)
		{
			motorValue_Last[m] = 0;
			iemValue_Last[m] = 0;
			if (getEncoderForMotor(m) != -1) // if there is an IME
			{
				nMotorEncoder[m] = 0; // set IEM to 0
				if (iemPresent == false){writeDebugStreamLine("// Integeated Encoder Module(s) detected, sweet!");}
				iemPresent = true;
			}
		}

		// write sensor status to debug stream
		if (sensorPresent == false)
		{
			writeDebugStreamLine("// Sensors NOT detected.");
		}
		if (iemPresent == false)
		{
			writeDebugStreamLine("// Integeated Encoder Module(s) NOT detected.");
		}
		if (sensorPresent == false && iemPresent == false)
		{
			writeDebugStreamLine("// If you install sensors or IEMs, this program will record their value at each motor change as a comment.");
		}

		// countdown in debug stream
		writeDebugStream("// Begin recording in: ");
		wait1Msec(250);
		for (int n = 3; n>0 ; n--)
		{
			writeDebugStream (" %d", n);
			wait1Msec(250);
			for (int i = 0; i<4 ; i++)
			{
				writeDebugStream (" .");
				wait1Msec(250);
			}
		}

		// write header for autonomous code
		writeDebugStreamLine(" ");
		writeDebugStreamLine("// BEGIN AUTONOMOUS RECORDING");
		writeDebugStreamLine("/*<<<<---- START COPY ----------------------------------------------------------------------*/");
		writeDebugStreamLine("int R_Freq = %d;", R_Freq);
		writeDebugStreamLine("int R_Batt = %d;", nImmediateBatteryLevel);
		writeDebugStreamLine("int P_Freq = R_Freq * (R_Batt / nImmediateBatteryLevel)");

		// start recording
		startTask (recordAutonomous);

		// allow driver control for R_mSec miliseconds
		clearTimer(T1);
		while (time1[T1] < R_mSec)
		{
			userControlCode();
		}

		// stop recording
		stopTask (recordAutonomous);

		// write footer for recording to debug window
		writeDebugStream("wait1Msec( P_Freq * %d );", (loopCounter - loopOfLastChange)); // wait command
		writeDebugStreamLine("		// wait %d", (loopCounter - loopOfLastChange) * R_Freq);	// wait comment
		writeDebugStreamLine(" ");
		writeDebugStreamLine("// %d ", R_mSec); // end time comment

		writeDebugStreamLine("for (int i = 0; i < 10; i++){ motor[i] = 0; } // stop all motors");
		writeDebugStreamLine("/*--------------------------------------------------------------------------END COPY---->>>>*/");
		writeDebugStreamLine("// END AUTONOMOUS RECORDING");

		// stop all motors
		for (int i; i < 10; i++){	motor[i] = 0; }
	}
	else
	{
		writeDebugStream("RECORDING ABORTED, BATTERY VOLTAGE IS BELOW %d", minBattVoltage);
		writeDebugStreamLine(" ( %d )", nImmediateBatteryLevel);
		writeDebugStream("replace battery and try again");
	}
}
