/*
 * lampDriver.h
 *
 * Created: 14-05-2020 11:22:06
 *  Author: HansK
 */ 


#ifndef LAMPDRIVER_H_
#define LAMPDRIVER_H_

class dimmedLampDriver
{
public:
	dimmedLampDriver();
	void turnOnLED();
	void turnOffLED();
	void setDimValue(int dimValue);
private:
	int dimValue_;
};


#endif /* LAMPDRIVER_H_ */