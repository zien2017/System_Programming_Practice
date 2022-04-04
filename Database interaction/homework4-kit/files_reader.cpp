#include "files_reader.h"
#include "query_funcs.h"

string sanitizer (string input) {
    auto it = input.begin();
     while (it != input.end()) {

        if (*it == '\'') {
            input.insert(it, 1, '\'');
            ++ it;
        }
        ++ it;
    }
    return input;
}

void read_states (connection * C) {
    string line;
    ifstream MyReadFile("state.txt");

    while (getline (MyReadFile, line)) {
        std::string id;
        std::string name;
        stringstream tuple;
        tuple << line;
        tuple >> id
              >> name;
        add_state(C, name);
    }

    MyReadFile.close();
}

void read_colors (connection * C) {
    string line;
    ifstream MyReadFile("color.txt");

    while (getline (MyReadFile, line)) {
        std::string id;
        std::string name;
        stringstream tuple;
        tuple << line;
        tuple >> id
              >> name;
        add_color (C, name);

    }


    MyReadFile.close();
}

void read_teams (connection * C) {
    string line;
    ifstream MyReadFile("team.txt");

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
        
        add_team (C, name,  stoi(state_id), stoi(color_id), stoi(wins), stoi(losses));
    }

    MyReadFile.close();
}

void read_players (connection * C) {
    string line;
    ifstream MyReadFile("player.txt");


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

        add_player (C, stoi(team_id), stoi(uniform_num), first_name, last_name,
         stoi(mpg), stoi(ppg), stoi(rpg), stoi(apg), stod(spg), stod(bpg));

    }


    MyReadFile.close();
}

void read_files(connection * C) {

    read_states(C);
    read_colors(C);
    read_teams(C);
    read_players(C);
    

}
