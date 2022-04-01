#include <iostream>
#include <pqxx/pqxx>
#include <string>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

int tableCreator(connection * C) {

	/* Create SQL statement */

	std::string sql;

	sql = "CREATE TABLE COMPANY("
		  "ID INT PRIMARY KEY     NOT NULL,"
		  "NAME           TEXT    NOT NULL,"
		  "AGE            INT     NOT NULL,"
		  "ADDRESS        CHAR(50),"
		  "SALARY         REAL );";

	/* Create a transactional object. */
	work W(*C);

	/* Execute SQL query */
	W.exec(sql);
	W.commit();
	cout << "Table created successfully" << endl;
}

int tableCleaner(connection * C) {

	/* Create SQL statement */

	std::string sql;

	sql = "DROP TABLE IF EXISTS COMPANY"
		  "  CASCADE ";

	/* Create a transactional object. */
	work W(*C);

	/* Execute SQL query */
	W.exec(sql);
	W.commit();
	cout << "Table dropped successfully" << endl;
}


int main(int argc, char *argv[])
{

	// Allocate & initialize a Postgres connection object
	connection *C;

	try
	{
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
	catch (const std::exception &e)
	{
		cerr << e.what() << std::endl;
		return 1;
	}


	tableCleaner(C);
	// TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
	//       load each table with rows from the provided source txt files
	tableCreator(C);

	exercise(C);

	// Close database connection
	C->disconnect();

	return 0;
}
