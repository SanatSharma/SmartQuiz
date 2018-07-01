#include "stdafx.h"
#include "Sql.h"

using namespace std;

sql::Driver *driver;
sql::Driver *driver2;
sql::Connection *con;
sql::Connection *con2;
sql::PreparedStatement *prep_stmt;
sql::PreparedStatement *prep_stmt2;
sql::PreparedStatement *attendance_statement;


int sql_connect() 
{
	try 
	{
		driver = get_driver_instance();
		try 
		{
			con = driver->connect("tcp://127.0.0.1:", "root", "");
		}
		catch (exception e)
		{
			std::cout << "Connection to database failed" << "\n";
			return 0;
		}
		con->setSchema("test");
		::prep_stmt = con->prepareStatement("INSERT INTO quizdata(rfid, remoteId, qid, studentAnswer) VALUES (?, ?, ?, ?)");
		::prep_stmt2 = con->prepareStatement("DELETE FROM quizdata WHERE rfid=? and remoteId=? and qid=?");
		
		std::cout << "Database Connection successful\n";
		return 1;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException: Database Connection not successful";
		return 0;
	}
}

int attendance_init() {
	try {
		// This will all change when connecting to the actual database
		driver2 = get_driver_instance();
		con2 = driver2->connect("tcp://127.0.0.1:", "root", "");
		con2->setSchema("test");
		::attendance_statement = con->prepareStatement("INSERT INTO students(remoteId) VALUES (?)");

		std::cout << "Attendance database connected\n";
		return 1;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException: Attendance Database Connection not successful";
		return 0;
	}
}

int sql_send_data(const char* rfid, int remoteId, int qid, const char* studentAnswer) {
	try {
		prep_stmt2->setString(1, rfid);
		prep_stmt2->setInt(2, remoteId);
		prep_stmt2->setInt(3, qid);
		prep_stmt2->execute();

		prep_stmt->setString(1, rfid);
		prep_stmt->setInt(2, remoteId);
		prep_stmt->setInt(3, qid);
		prep_stmt->setString(4, studentAnswer);
		prep_stmt->execute();
		return 1;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException: Sending data to database failed";
		return 0;
	}
}

int send_attendance(int remoteId) {
	try {
		attendance_statement->setInt(1, remoteId);
		attendance_statement->execute();
		return 1;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException: Sending attendance data to database failed";
		return 0;
	}
}

int sql_close() {
	try {
		delete prep_stmt;
		delete prep_stmt2;
		delete attendance_statement;
		delete con;
		delete con2;
		return 1;
	}
	catch (sql::SQLException &e) {
		std::cout << "SQLException: Unable to close database connection";
		return 0;
	}
}