/*
 * X10_RX.h
 *
 * Created: 14-05-2020 11:11:27
 *  Author: HansK
 */ 


#ifndef X10_RX_H_
#define X10_RX_H_

class X10_RX
{
	public:
	X10_RX(int address);
	bool dataReady(bool dataBit);
	bool convertData();
	char getData();
	private:
	bool startBuffer_[8];
	bool rawData_[8];
	bool convertedData_[4];
	char convertedDataChar_;
	bool receivingData_;
	int bitCnt_;
	bool newDataReady_;

	bool startArray_[8];
	
};




#endif /* X10_RX_H_ */