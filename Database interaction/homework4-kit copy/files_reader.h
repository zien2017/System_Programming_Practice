#include <iostream>
#include <pqxx/pqxx>
#include <string.h>
#include <fstream>


using namespace std;
using namespace pqxx;

#ifndef _FILES_READER_
#define _FILES_READER_

// void read_state (connection * C) ;
void read_files(connection * C);


#endif //_FILES_READER_       
