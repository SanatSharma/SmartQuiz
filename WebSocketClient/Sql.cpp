#include "stdafx.h"
#include "Sql.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <ctime>

using namespace std;

sql::Driver *driver;
sql::Connection *con;
sql::PreparedStatement *prep_stmt;
sql::PreparedStatement *prep_stmt2;

int sql_connect() {
	try {
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:", "root", "");
		con->setSchema("test");
		::prep_stmt = con->prepareStatement("INSERT INTO quizdata(rfid, remoteId, qid, studentAnswer) VALUES (?, ?, ?, ?)");
		::prep_stmt2 = con->prepareStatement("DELETE FROM quizdata WHERE rfid=? and remoteId=? and qid=?");
		
		std::cout << "Database Connection successful\n";
		return 1;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException";
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
		std::cout << "# ERR: SQLException";
		return 0;
	}
}

int sql_close() {
	try {
		delete prep_stmt;
		delete con;
		return 1;
	}
	catch (sql::SQLException &e) {
		return 0;
	}
}
