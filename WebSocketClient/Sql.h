#pragma once
int sql_connect();
int sql_send_data(const char* rfid, int remoteId, int qid,const char* studentAnswer);
int sql_close();