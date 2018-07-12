#include "stdafx.h"
#include <windows.h>
#include "Quiz.h"
#include <rf21x-api.h>
#include <gsp-api.h>

#define RF21X_SLEEP(n) (Sleep(n))

char *buf;
rf21x_Device rf21x = NULL;
rf21x_Message message = NULL;

int Quiz::startUp() {

	int i = 0;
	while (rf21x_getHidSerialNumber(i, buf, sizeof(buf)))
	{
		printf("HID Device %d: %s\n", i, buf);
		++i;
	}
	printf("Finish listing all current HID device.\n");

	char *tempPath = NULL;
	rf21x_getHidTempPath(0, tempPath, sizeof(tempPath));

	// create a device of type rf217
	rf21x = rf21x_createDevice(RF21X_DT_RF219);

	if (rf21x == NULL)
	{
		printf("Fail to create device handle!\n");
		return 0;
	}
	if (rf21x_open(rf21x, "hid://", 1, 40))
	{
		printf("Success to open device.\n");
	}
	else
	{
		printf("Fail to open device.\n");
		return 0;
	}
	return 1;
}

int Quiz::quiz_start() {

	if (rf21x_startQuiz(rf21x, RF21X_QT_Single))
	{
		printf("Success to start a quiz.\n");
	}
	else
	{
		printf("Fail to start a quiz.\n");
		return 0;
	}
	return 1;
}

int Quiz::quiz_poll(int attendance) {

	message = rf21x_createMessageBuffer();

	if (rf21x_getMessage(rf21x, message))
	{
		if (attendance) {
			int type = rf21x_getMessageType(message);
			int keypadId = rf21x_getKeypadId(message);
			const char* data = rf21x_getData(message);
			if (type == RF21X_MT_Student) {
				//send_attendance(keypadId);
				// SEND ATTENDANCE TO SERVER
			}
		}
		else {
			int type = rf21x_getMessageType(message);
			int keypadId = rf21x_getKeypadId(message);
			int quizNumber = rf21x_getQuizNumber(message);
			printf("Quiz Number: %d\n", quizNumber);
			const char* data = rf21x_getData(message);
			if (type == RF21X_MT_Teacher)
			{
				printf("Teacher: %s\n", data);
			}
			else if (type == RF21X_MT_Student)
			{
				if (quizNumber <= 0)
				{
					printf("Student %d for current question: %s\n", keypadId, data);
					//sql_send_data("RF123", keypadId, 1, data);
					if (!Rest::postResponse(keypadId, data))
					{
						throw("Quiz data could not be sent to server!"); // SHOULD THIS BE CHANGED SO APPLICATION DOESN'T BREAK
					}
				}
				else
				{
					printf("Student %d for question %d: %s\n", keypadId, quizNumber, data);
				}
			}
		}
		unsigned char *buf = NULL;
		int len = 0;
		rf21x_getRawData(message, &buf, &len);
		printf("Raw data: ");
		for (int i = 0; i < len; ++i)
		{
			printf("%02X ", buf[i]);
		}
		printf("\n");
	}

	return 1;
}

int Quiz::quiz_stop() {
	rf21x_destoryMessageBuffer(message);
	printf("Success Stop quiz and close device.\n");
	if (rf21x_stopQuiz(rf21x)) {
		printf("Successfully closed quiz!\n");
	}
	else {
		printf("Error in closing quiz!!\n");
		return 0;
	}
	return 1;
}

int Quiz::session_end(){
	
	if (rf21x_close(rf21x)) {
		printf("Device Closed.\n");
	}
	else {
		printf("Error in closing device!");
		return 0;
	}
	rf21x_destoryDevice(rf21x);
	printf("Device Destroyed");
	return 1;
}