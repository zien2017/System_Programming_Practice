#include <iostream>
#include <pqxx/pqxx>
#include <string>

#include "exerciser.h"
#include "files_reader.h"

using namespace std;
using namespace pqxx;



void tableCreator(connection *C) {
	std::string STATE, COLOR, TEAM, PLAYER;

	STATE = "CREATE TABLE STATE("
			"STATE_ID       INT PRIMARY KEY   NOT NULL,"
			"NAME           TEXT    NOT NULL);"
			;

	COLOR = "CREATE TABLE COLOR("
			"COLOR_ID       INT PRIMARY KEY   NOT NULL,"
			"NAME           TEXT    NOT NULL);"
			;

	TEAM =  "CREATE TABLE TEAM("
			"TEAM_ID        INT PRIMARY KEY   NOT NULL,"
			"NAME           TEXT    NOT NULL,"
			"STATE_ID 		INT     NOT NULL      REFERENCES STATE(STATE_ID),"
			"COLOR_ID 		INT     NOT NULL      REFERENCES COLOR(COLOR_ID),"
			"WINS           INT     NOT NULL,"
			"LOSSES         INT     NOT NULL);"
			;

	PLAYER = 	"CREATE TABLE PLAYER("
				"PLAYER_ID      INT PRIMARY KEY     NOT NULL,"
				"TEAM_ID 		INT     NOT NULL      REFERENCES TEAM(TEAM_ID),"
				"UNIFORM_NUM	INT     NOT NULL,"
				"FIRST_NAME     TEXT    NOT NULL,"
				"LAST_NAME      TEXT    NOT NULL,"
				"MPG            INT,"
				"PPG            INT,"
				"RPG            INT,"
				"APG            INT,"
				"SPG            INT,"
				"BPG            INT);"
				;

	exec_commit_sql (C, STATE);
	exec_commit_sql (C, COLOR);
	exec_commit_sql (C, TEAM);
	exec_commit_sql (C, PLAYER);

}

void tableCleaner(connection *C)
{

	std::string sql;

	sql = "DROP TABLE IF EXISTS PLAYER CASCADE;"
		  "DROP TABLE IF EXISTS TEAM CASCADE;"
		  "DROP TABLE IF EXISTS STATE CASCADE;"
		  "DROP TABLE IF EXISTS COLOR CASCADE;";

	exec_commit_sql (C, sql);

}

int main(int argc, char *argv[]) {

	// Allocate & initialize a Postgres connection object
	connection *C;

	try {
		// Establish a connection to the database
		// Parameters: database name, user name, user password
		C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
		if (C->is_open())
		{
			cout << "Opened database successfully: " << C->dbname() << endl;
		}
		else
		{
			cout << "Can't open database" << endl;
			return 1;
		}
	}
	catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return 1;
	}

	tableCleaner(C);
	// TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
	//       load each table with rows from the provided source txt files
	tableCreator(C);

	read_files(C);


	exercise(C);

	// Close database connection
	C->disconnect();

	return 0;
}
