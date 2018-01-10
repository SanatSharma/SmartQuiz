// WebSocketClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <uWS/uWS.h>
#include "Quiz.h"
#include <agents.h>
#include <ppltasks.h>
#include "Sql.h"

Concurrency::call<int> *call;
Concurrency::timer<int> *timer;

int main() {
	uWS::Hub h;
	std::mutex m;

	if (startUp()) {
		std::cout << "Initialization is complete" << std::endl;
		if (!sql_connect())
			throw std::exception();
	}
	else {
		std::cout << "Initialization ERROR!" << std::endl;
		throw std::exception();
	}

	h.onConnection([](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
		std::cout << "Connected!" << std::endl;
	});

	h.onMessage([&m](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
		std::cout << "Message received!" << std::endl;
		if (strncmp(message, "start", length) == 0) {
			std::cout << "Time to start quiz" << std::endl;
			timer->start();
			if (!quiz_start()) {
				throw std::exception();
			}
		}
		else if (strncmp(message, "next", length) == 0) {
			std::cout << "Next Question" << std::endl;
			timer->start();
			if (!quiz_start()) {
				throw std::exception();
			}
		}
		else if (strncmp(message, "stop", length) == 0) {
			std::cout << "Stop quiz" << std::endl;
			timer->pause();
			if (!quiz_stop()) {
				throw std::exception();
			}
		}

	});

	h.onDisconnection([](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
		std::cout << "CLIENT CLOSE: " << code << std::endl;
	});

	h.connect("ws://localhost:3000");

	h.onError([](void *user) {
		int protocolErrorCount = 0;
		switch ((long)user) {
		case 1:
			std::cout << "Client emitted error on invalid URI" << std::endl;
			getchar();
			break;
		case 2:
			std::cout << "Client emitted error on resolve failure" << std::endl;
			getchar();
			break;
		case 3:
			std::cout << "Client emitted error on connection timeout (non-SSL)" << std::endl;
			getchar();
			break;
		case 5:
			std::cout << "Client emitted error on connection timeout (SSL)" << std::endl;
			getchar();
			break;
		case 6:
			std::cout << "Client emitted error on HTTP response without upgrade (non-SSL)" << std::endl;
			getchar();
			break;
		case 7:
			std::cout << "Client emitted error on HTTP response without upgrade (SSL)" << std::endl;
			getchar();
			break;
		case 10:
			std::cout << "Client emitted error on poll error" << std::endl;
			getchar();
			break;
		case 11:
			protocolErrorCount++;
			std::cout << "Client emitted error on invalid protocol" << std::endl;
			if (protocolErrorCount > 1) {
				std::cout << "FAILURE:  " << protocolErrorCount << " errors emitted for one connection!" << std::endl;
				getchar();
			}
			break;
		default:
			std::cout << "FAILURE: " << user << " should not emit error!" << std::endl;
			getchar();
		}
	});
	
	// callback function polls HID device for data
	std::function<void()> callback = [] {
		quiz_poll();
	};

	call = new concurrency::call<int>(
		[callback](int)
	{
		callback();
	});

	// create a concurrent timer that polls the HID every time_ms ms
	// this is important since it leaves the main thread open to receive calls on WebSocket
	int time_ms = 200;
	timer = new concurrency::timer<int>(time_ms, 0, call, true); 

	h.run();

	// The following code is run when the program shuts down

	std::cout << "*************************\n";
	timer->stop();
	delete timer; // delete timer object to free memory
	session_end();
	sql_close();
	getchar();
}

