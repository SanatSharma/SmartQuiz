#include <uWS/uWS.h>
#include "Quiz.h"
#include <agents.h>
#include <ppltasks.h>
#include "Sql.h"
#include "Rest.h"
#include "Utilities.h"
#include <nlohmann/json.hpp>

Concurrency::call<int> *call;
Concurrency::timer<int> *timer;
const char* START = "start";
const char* NEXT = "next";
const char* STOP = "stop";
const char* ATTENDANCE = "attendance";
const char* STOPATTENDANCE = "stopattendance";
const char* QIDNUM = "Qid";
int attendance_condition = 0;
int SESSION;
int QID;
int RRQ_ID = -1;
using json = nlohmann::json;
