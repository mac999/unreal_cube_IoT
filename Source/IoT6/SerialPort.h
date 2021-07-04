// Fill out your copyright notice in the Description page of Project Settings.

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
	void* m_hIDComDev;
};
