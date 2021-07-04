// Fill out your copyright notice in the Description page of Project Settings.


#include "SerialPort.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/MinWindows.h"
#include "Windows/HideWindowsPlatformTypes.h"

SerialPort::SerialPort()
{
	m_hIDComDev = NULL;
}

SerialPort::~SerialPort()
{
	close();
}

bool SerialPort::open(const TCHAR* sPort, int nBaud)
{
	int nPort = 1;
	FString szPort = sPort;
	// DCB dcb;

	m_hIDComDev = CreateFile(*szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hIDComDev == NULL)
	{
		unsigned long dwError = GetLastError();
		UE_LOG(LogTemp, Error, TEXT("Failed to open port %s. Error: %08X"), *szPort, dwError);
		return false;
	}


	COMMTIMEOUTS CommTimeOuts;
	//CommTimeOuts.ReadIntervalTimeout = 10;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 10;
	SetCommTimeouts(m_hIDComDev, &CommTimeOuts);

	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hIDComDev, &dcb);
	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;

	if (!SetCommState(m_hIDComDev, &dcb) || !SetupComm(m_hIDComDev, 10000, 10000))
	{
		unsigned long dwError = GetLastError();
		CloseHandle(m_hIDComDev);
		m_hIDComDev = NULL;
		return false;
	}

	PurgeComm(m_hIDComDev, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	return true;
}

void SerialPort::close()
{
	if (!m_hIDComDev)
		return;
	CloseHandle(m_hIDComDev);
	m_hIDComDev = NULL;
}

int SerialPort::write(TArray<uint8>& Buffer)
{
	DWORD dwBytesWritten;
	BOOL bWrite = WriteFile(m_hIDComDev, Buffer.GetData(), Buffer.Num(), &dwBytesWritten, NULL);
	if (!bWrite && (GetLastError() == ERROR_IO_PENDING))
		return dwBytesWritten;

	return dwBytesWritten;
}

int SerialPort::read(TArray<uint8>& Buffer)
{
	DWORD dwBytesRead = 0;
	BOOL bWrite = ReadFile(m_hIDComDev, Buffer.GetData(), Buffer.Num(), &dwBytesRead, NULL);
	if (!bWrite)
	{
		if (GetLastError() == ERROR_IO_PENDING)
			return 0;
		return dwBytesRead;
	}

	return dwBytesRead;
}

