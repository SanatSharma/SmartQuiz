#pragma once

#include "Quiz.h"
#include "Rest.h"
#include "Utilities.h"
#include <nlohmann/json.hpp>

extern int SESSION;
extern int Q_ID;
extern int RRQ_ID;
using json = nlohmann::json;

enum Events { CONNECTION, MESSAGE, DISCONNECTION, CLOSE };
enum Profile { RRQ };
enum Action { START, STOP, END, NEXT, TEACHERCONNECTION, CLASSROOMCONNECTION };
enum Message { PROFILE, TYPE, SESSIONID, ACTION, RRQID, QID, WSID };
