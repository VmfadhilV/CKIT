int Send2CKIT(char *myString);
int ComOn();
int ComOff();
HANDLE hSerial;

int Send2CKIT(char *myString)
{
    DWORD bytes_written, total_bytes_written = 0;
    WriteFile(hSerial, myString, strlen(myString), &bytes_written, NULL);
    fprintf(stderr, "%d bytes written\n", bytes_written);
    return 0;
}

int ComOn()
{
    // Declare variables and structures

    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    // Open the highest available serial port number
    fprintf(stderr, "Opening serial port...");
    hSerial = CreateFile(
                "\\\\.\\COM3", GENERIC_READ|GENERIC_WRITE, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if (hSerial == INVALID_HANDLE_VALUE)
    {
            fprintf(stderr, "Error\n");
            return 1;
    }
    else fprintf(stderr, "OK\n");

    // Set device parameters (38400 baud, 1 start bit,
    // 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 5; //50
    timeouts.ReadTotalTimeoutConstant = 5; //50
    timeouts.ReadTotalTimeoutMultiplier = 5; //10
    timeouts.WriteTotalTimeoutConstant = 5; //50
    timeouts.WriteTotalTimeoutMultiplier = 5; //10
    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }
    return 0;
}

int ComOff()
{

    // Close serial port
    fprintf(stderr, "Closing serial port...");
    if (CloseHandle(hSerial) == 0)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    fprintf(stderr, "OK\n");
    return 0;
}
