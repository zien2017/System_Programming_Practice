#include "query_funcs.h"

void exec_commit_sql (connection *C, string sql) {
	work W(*C);
	W.exec(sql);
	W.commit();
	// cout << "succeed" << endl;
}


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
}


void add_state(connection *C, string name)
{
}


void add_color(connection *C, string name)
{
}


void query1(connection *C,
	        int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            ) {
    string sql = "SELECT * FROM PLAYER WHERE 0 = 0 ";
    if (use_mpg) sql.append(" AND MPG BETWEEN " + to_string(min_mpg) + " AND " + to_string(max_mpg));
    if (use_ppg) sql.append(" AND PPG BETWEEN " + to_string(min_ppg) + " AND " + to_string(max_ppg));
    if (use_rpg) sql.append(" AND RPG BETWEEN " + to_string(min_rpg) + " AND " + to_string(max_rpg));
    if (use_apg) sql.append(" AND APG BETWEEN " + to_string(min_apg) + " AND " + to_string(max_rpg));
    if (use_spg) sql.append(" AND SPG BETWEEN " + to_string(min_spg) + " AND " + to_string(max_spg));
    if (use_bpg) sql.append(" AND BPG BETWEEN " + to_string(min_bpg) + " AND " + to_string(max_bpg));
    sql.append(" ORDER BY MPG DESC, PPG DESC, RPG DESC, APG DESC, SPG DESC, BPG DESC;");
    sql.push_back(';');

    nontransaction nt (*C);

    result res (nt.exec(sql));
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << "\n";
    for (auto it = res.begin(); it!=res.end(); ++ it){
        for (int i = 0; i < it.size(); ++ i) {
            cout << it[i].as<string>() << ' ';
        }
        cout << endl;
    }

}


void query2(connection *C, string team_color) {
    string sql = "SELECT TEAM.NAME, COLOR.NAME FROM COLOR JOIN TEAM ON TEAM.COLOR_ID = COLOR.COLOR_ID WHERE 0 = 0 ";
    sql.append(" AND COLOR.NAME LIKE \'" + team_color + "\'");
    // sql.append(" ORDER BY MPG DESC, PPG DESC, RPG DESC, APG DESC, SPG DESC, BPG DESC;");
    sql.push_back(';');

    nontransaction nt (*C);

    result res (nt.exec(sql));
    cout << "TEAM.NAME COLOR" << "\n";
    for (auto it = res.begin(); it!=res.end(); ++ it){
        for (int i = 0; i < it.size(); ++ i) {
            cout << it[i].as<string>() << ' ';
        }
        cout << endl;
    }
}


void query3(connection *C, string team_name) {

    string sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME  FROM PLAYER JOIN TEAM ON TEAM.TEAM_ID = PLAYER.TEAM_ID WHERE 0 = 0 ";
    sql.append(" AND TEAM.NAME LIKE \'" + team_name + "\'");
    sql.append(" ORDER BY PLAYER.PPG DESC ");
    sql.push_back(';');

    nontransaction nt (*C);

    result res (nt.exec(sql));
    cout << "FIRST_NAME LAST_NAME" << "\n";
    for (auto it = res.begin(); it!=res.end(); ++ it){
        for (int i = 0; i < it.size(); ++ i) {
            cout << it[i].as<string>() << ' ';
        }
        cout << endl;
    }
}


void query4(connection *C, string team_state, string team_color) {


}


void query5(connection *C, int num_wins) {


}
