/************************************
 * uart.cpp
 *
 * Created: 5/13/2020 14:46:53
 *  Author: Allan Lassesen
 *
 ************************************/
 
#include<windows.h>
#include<iostream>
#include<string>
#include<math.h>
#include "uart.h"
using namespace std;

uart::uart()
{
	// UART Communication Setup - Create a "file-configuration" for Serial communication
	hComm_ = CreateFile(L"\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	// Print communication status
	if (hComm_ == INVALID_HANDLE_VALUE)
		cout << "          Error in opening serial port" << endl;
	else
		cout << "          opening serial port successful" << endl;
	// Initializing DCB structure	- with UART settings
	DCB dcbSerialParams = { 0 };						
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	Status_ = GetCommState(hComm_, &dcbSerialParams);		// Communication status
	dcbSerialParams.BaudRate = CBR_9600;					// Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;							// Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;					// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;						// Setting Parity = None

	SetCommState(hComm_, &dcbSerialParams);
	// command settings
	setPassword_ = 150;
	unitTest_ = 50;
	exitTest_ = 255;
}

void uart::setNewUserPassword()
{
	/// Set Arduino ready to change password
	system("cls");
	sendArduinoCommand(setPassword_);
	// Get new password
	cout << "     Insert a series of 8  '1' and '0' as the new password:" << endl;
	cin.clear();
	string userInput;
	cout << "     "; 
	cin >> userInput;
	// Binary ASCII to int
	char newPassword = 0;
	for (size_t i = 0; i < 8; i++)
		newPassword = newPassword + pow(2 * (userInput.at(i) == '1' ? 1 : 0), i);

	// Print new Password
	cout << endl << endl << "     new password is " << stoi(userInput) << " as decimal " << newPassword << endl << endl;
	// Send new defined user password
	sendArduinoCommand(newPassword);
	system("cls");
	// Receive confirmation from Arduino
	char response = getArduinoResponse();

	cout << endl << "     User input was " << stoi(userInput) << endl << endl;
}

void uart::unitTest()
{

		system("cls");
		sendArduinoCommand(unitTest_);
		cout << "No test available...!" << endl << endl;
		cout << endl << "     ------------------------------------------------------------" << endl;
		cout <<  "     --------------------  To X10 Unit Test  --------------------" << endl;
		cout  << "     ------------------------------------------------------------" << endl << endl;
		cout << "     Press a number, to choose a device to test:" << endl;
		cout << "     1. Lamp 1 - with dimming" << endl;
		cout << "     2. Lamp 2 - I/O" << endl;
		cout << "     3. Curtain - Up/Down" << endl;
		cout << "     4. MP3 - Play song" << endl;
		cout << "     99. Exit Test" << endl;
		int device;
		cout << "     ";
		cin >> device;
		sendArduinoCommand(device);

				system("cls");
				cout << "No test available...!" << endl << endl;
				cout << endl << "     ------------------------------------------------------------" << endl;
				cout << "     --------------------  To X10 Unit opions  ------------------" << endl;
				cout << "     ------------------------------------------------------------" << endl << endl;
				cout << "     Press a number, to choose a device to test:" << endl;
				cout << "     1. Off" << endl;
				cout << "     2. On" << endl;
				cout << "     3. Pause" << endl;
				cout << "     8. 12.5% active" << endl;
				cout << "     9. 25% active" << endl;
				cout << "     10. 37.5% active" << endl;
				cout << "     11. 50% active" << endl;
				cout << "     12. 62.5% active" << endl;
				cout << "     13. 75% active" << endl;
				cout << "     14. 87.5% active" << endl;
				cout << "     15. 100% active" << endl;
				cout << "     99. Exit test" << endl;
				int command;
				cout << "     ";
				cin >> command;
				sendArduinoCommand(command);
}

void uart::sendArduinoCommand(int command)
{
	int lpBuffer[1] = { command };
	DWORD dNoOfBytesWritten;			// No of bytes written to the port
	Status_ = WriteFile(hComm_,			 // Handle to the Serial port
		lpBuffer,						// Data to be written to the port
		1,								//No of bytes to write
		&dNoOfBytesWritten,				//Bytes written
		NULL);
}

int uart::getArduinoResponse()
{
	//// RECIEVE DATA
	int response; // Arduino response
	DWORD NoBytesRead;

	bool bResult = ReadFile(hComm_,  //Handle of the Serial port
		&response,					//Temporary character
		1,							//Size of TempChar
		&NoBytesRead,				//Number of bytes read
		NULL);
	if (!bResult)
	{
		printf("READ ERROR");
	}
	return response;
}

uart::~uart()
{
	CloseHandle(hComm_);	//Closing the Serial Port
}