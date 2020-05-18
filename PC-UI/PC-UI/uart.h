/************************************
 * uart.h
 *
 * Created: 5/13/2020 14:46:53
 *  Author: Allan Lassesen
 *
 ************************************/
#pragma once
#include <Windows.h>

class uart
{
public:
	uart();
	void setNewUserPassword();
	void unitTest();
	void sendArduionoCommand(int);
	int getArduinoResonse();
	~uart();
private:
	int setPassword_;
	int unitTest_;
	int exitTest_;
	HANDLE hComm_;		// UART Serial Port (file)
	bool Status_;
};