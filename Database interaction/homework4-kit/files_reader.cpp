#include "files_reader.h"


void read_state (connection * C) {
    string line;
    ifstream MyReadFile("state.txt");
	work W(*C);

    while (getline (MyReadFile, line)) {
        std::string id;
        std::string name;
        stringstream tuple;
        tuple << line;
        tuple >> id
              >> name;
        std::string sql = "INSERT INTO STATE (STATE_ID, NAME) VALUES ( " + id + ", \'" + name + "\');";
        W.exec(sql);
        cout << sql << endl; 
    }

	W.commit();

    MyReadFile.close();
}

void read_files(connection * C) {

    read_state(C);

}
