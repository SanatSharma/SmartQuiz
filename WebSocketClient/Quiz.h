#pragma once
#include <rf21x-api.h>
#include <gsp-api.h>

int startUp();
int quiz_start();
int quiz_stop();
int session_end();
int quiz_poll(int attendance=0);