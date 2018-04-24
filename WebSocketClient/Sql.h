#pragma once
int sql_connect();
int sql_send_data(const char* rfid, int remoteId, int qid,const char* studentAnswer);
int sql_close();
int attendance_init();
int send_attendance(int remoteId);