#include "files_reader.h"

string sanitizer (string input) {
    auto it = input.begin();
    string result;
    while (it != input.end()) {
        if (*it == '\'') result.push_back('\'');
        result.push_back(* it);
        ++ it;
    }
    return result;
}

void read_states (connection * C) {
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
        // cout << sql << endl; 
    }

	W.commit();

    MyReadFile.close();
}

void read_colors (connection * C) {
    string line;
    ifstream MyReadFile("color.txt");
	work W(*C);

    while (getline (MyReadFile, line)) {
        std::string id;
        std::string name;
        stringstream tuple;
        tuple << line;
        tuple >> id
              >> name;
        std::string sql = "INSERT INTO COLOR (COLOR_ID, NAME) VALUES ( " + id + ", \'" + name + "\');";
        W.exec(sql);
        cout << sql << endl; 
    }

	W.commit();

    MyReadFile.close();
}

void read_teams (connection * C) {
    string line;
    ifstream MyReadFile("team.txt");
	work W(*C);

    while (getline (MyReadFile, line)) {
        string id;
        string name;
        string state_id;
        string color_id;
        string wins;
        string losses;
        stringstream tuple;
        tuple << line;
        tuple >> id
              >> name
              >> state_id
              >> color_id
              >> wins
              >> losses;
        string sql = "INSERT INTO TEAM (TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ( "
                     + id + ",\'" + name + "\'," + state_id + "," + color_id + "," + wins + "," + losses + ");";
        W.exec(sql);
        cout << sql << endl; 
    }

	W.commit();

    MyReadFile.close();
}

void read_players (connection * C) {
    string line;
    ifstream MyReadFile("player.txt");
	work W(*C);

    while (getline (MyReadFile, line)) {
        string player_id;
        string team_id;
        string uniform_num;
        string first_name;
        string last_name;
        string mpg;
        string ppg;
        string rpg;
        string apg;
        string spg;
        string bpg;
        stringstream tuple;
        tuple << line;
        tuple >> player_id
              >> team_id
              >> uniform_num
              >> first_name
              >> last_name
              >> mpg
              >> ppg
              >> rpg
              >> apg
              >> spg
              >> bpg;

        first_name = sanitizer (first_name);
        last_name = sanitizer (last_name);


        string sql = "INSERT INTO PLAYER ("
            "PLAYER_ID,"
            "TEAM_ID,"
            "UNIFORM_NUM, "
            "FIRST_NAME, "
            "LAST_NAME, "
            "MPG, "
            "PPG, "
            "RPG, "
            "APG, "
            "SPG, "
            "BPG) VALUES ( "
             + player_id + ",\'" + team_id + "\'," + uniform_num + ",\'"
             + first_name + "\',\'" + last_name + "\'," 
             + mpg + "," + ppg + "," + rpg + "," + apg + "," + spg + "," + bpg
             + ");";
        W.exec(sql);
        cout << sql << endl; 
    }

	W.commit();

    MyReadFile.close();
}

void read_files(connection * C) {

    read_states(C);
    read_colors(C);
    read_teams(C);
    read_players(C);
    

}
