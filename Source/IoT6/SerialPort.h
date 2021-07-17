// Title: Serial (RS232) communication class
// Developer: Taewook, Kang
// Date: 2021.6.29
// Email: laputa99999@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"

/**
 * 
 */
class IOT6_API SerialPort
{
public:
	SerialPort();
	~SerialPort();

	bool open(const TCHAR* sPort, int nBaud = 9600);
	void close();

	int write(TArray<uint8>& Buffer);
	int read(TArray<uint8>& Buffer);

private:
	void* _PortHandle;
};
