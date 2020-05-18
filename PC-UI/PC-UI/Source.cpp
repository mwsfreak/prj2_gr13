//		https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
//		https://www.codeproject.com/Questions/282238/Serial-COM-Port-readfile-reads-only-30-bytes
/************************************
 * Source.cpp
 *
 * Created: 5/13/2020 14:46:53
 *  Author: Allan Lassesen
 *
 ************************************/
#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<string>
#include"uart.h"
using namespace std;

int main()
{
	string username = "admin";
	string password = "1234";
	string usernameAttempt;
	string passwordAttempt;
	bool passwordIncorrect = true;
	int loginMenu = 0;
	do
	{
		switch (loginMenu)
		{
		case 1:
			// Login
			loginMenu = (usernameAttempt != username ? 3 : (passwordAttempt != password ? 2 : 4));
			break;
		case 2:
			// incorrect password
			system("cls");
			cout << endl << "Incorrect password...! try again" << endl << endl;
			loginMenu = 0;
			break;
		case 3:
			// incorrect username
			system("cls");
			cout << endl << "Incorrect username...! try again" << endl << endl;
			loginMenu = 0;
			break;
		case 4:
			// close program
			system("cls");
			passwordIncorrect = false;
			break;
		default:
			cout << endl << "     ------------------------------------------------------------" << endl;
			cout << "     -------  Please login to use Home Automation System  -------" << endl;
			cout << "     ------------------------------------------------------------" << endl << endl;
			cout << "     Username: ";
			std::cin >> usernameAttempt;
			cout << endl << "     Password: ";
			std::cin >> passwordAttempt;
			loginMenu = 1;
			break;
		}

	} while (passwordIncorrect);


	uart Arduino;		// setting up Serial communication to Arduino
	bool menuOn = true;	
	int choice = 0;

	do
	{
		switch (choice)
		{
		case 1:
			Arduino.setNewUserPassword();	// Save new user password on Arduino
			choice = 0;
			break;
		case 2:
			Arduino.unitTest();
			system("cls");
			choice = 0;
			break;
		case 3:
			menuOn = false;
			//Arduino.~uart(); // close UART communication
			break;
		default:
			cout << endl << "     ------------------------------------------------------------------" << endl;
			cout << "     -------  Welcome to CMD controlled Home Automation System  -------" << endl;
			cout << "     ------------------------------------------------------------------" << endl << endl;
			cout << "     Press a number, to choose an event:" << endl;
			cout << "     1. Change Password" << endl;
			cout << "     2. Unit testing mode" << endl;
			cout << "     3. Shut Down" << endl;
			cout << "     ";
			std::cin >> choice;
			break;
		}
	
	} while (menuOn);

	return 0;
}




