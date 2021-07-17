// Developer: Taewook, Kang
// Date: 2021.6.29
// Email: laputa99999@gmail.com

#include "SerialPort.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/MinWindows.h"
#include "Windows/HideWindowsPlatformTypes.h"

SerialPort::SerialPort()
{
	_PortHandle = NULL;
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

	_PortHandle = CreateFile(*szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (_PortHandle == NULL)
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
	SetCommTimeouts(_PortHandle, &CommTimeOuts);

	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(_PortHandle, &dcb);
	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;

	if (!SetCommState(_PortHandle, &dcb) || !SetupComm(_PortHandle, 10000, 10000))
	{
		unsigned long dwError = GetLastError();
		CloseHandle(_PortHandle);
		_PortHandle = NULL;
		return false;
	}

	PurgeComm(_PortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	return true;
}

void SerialPort::close()
{
	if (!_PortHandle)
		return;
	CloseHandle(_PortHandle);
	_PortHandle = NULL;
}

int SerialPort::write(TArray<uint8>& Buffer)
{
	DWORD dwBytesWritten;
	BOOL bWrite = WriteFile(_PortHandle, Buffer.GetData(), Buffer.Num(), &dwBytesWritten, NULL);
	if (!bWrite && (GetLastError() == ERROR_IO_PENDING))
		return dwBytesWritten;

	return dwBytesWritten;
}

int SerialPort::read(TArray<uint8>& Buffer)
{
	DWORD dwBytesRead = 0;
	BOOL bWrite = ReadFile(_PortHandle, Buffer.GetData(), Buffer.Num(), &dwBytesRead, NULL);
	if (!bWrite)
	{
		if (GetLastError() == ERROR_IO_PENDING)
			return 0;
		return dwBytesRead;
	}

	return dwBytesRead;
}

