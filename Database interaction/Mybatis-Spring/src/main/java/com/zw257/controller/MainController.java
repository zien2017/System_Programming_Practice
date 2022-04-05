package com.zw257.controller;

import com.zw257.bean.Color;
import com.zw257.bean.Player;
import com.zw257.bean.State;
import com.zw257.bean.Team;
import com.zw257.mapper.ColorMapper;
import com.zw257.mapper.PlayerMapper;
import com.zw257.mapper.StateMapper;
import com.zw257.mapper.TeamMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

@RestController
public class MainController {
	@Autowired
	private StateMapper stateM;
	@Autowired
	private ColorMapper colorM;
	@Autowired
	private TeamMapper teamM;
	@Autowired
	private PlayerMapper playerM;


	@RequestMapping("/createTables")
	public String createTableState() {
		// drop
		colorM.dropTable();
		stateM.dropTable();
		teamM.dropTable();
		playerM.dropTable();
		// create
		colorM.createTable();
		stateM.createTable();
		teamM.createTable();
		playerM.createTable();
		return "createTableState() Succeed";
	}


	@RequestMapping("/readFiles")
	public String readFiles() throws IOException {
		readColors();
		readStates();
		readTeams ();
		readPlayers();
		return "readFiles() succeed.";
	}


	public void readColors () throws IOException {
		InputStream is = this.getClass().getClassLoader().getResourceAsStream("color.txt");
		assert is != null;
		InputStreamReader isr = new InputStreamReader(is, StandardCharsets.UTF_8);
		BufferedReader br = new BufferedReader(isr);
		String line;
		while((line = br.readLine()) != null){
			String[] s = line.split(" ");
			Color c = new Color(0, s[1]);
			colorM.insertColor(c);
		}
		br.close();
	}

	public void readStates () throws IOException {
		InputStream is = this.getClass().getClassLoader().getResourceAsStream("state.txt");
		assert is != null;
		InputStreamReader isr = new InputStreamReader(is, StandardCharsets.UTF_8);
		BufferedReader br = new BufferedReader(isr);
		String line;
		while((line = br.readLine()) != null){
			String[] split = line.split(" ");
			State s = new State(0, split[1]);
			stateM.insertState(s);
		}
		br.close();
	}

	public void readTeams () throws IOException {
		InputStream is = this.getClass().getClassLoader().getResourceAsStream("team.txt");
		assert is != null;
		InputStreamReader isr = new InputStreamReader(is, StandardCharsets.UTF_8);
		BufferedReader br = new BufferedReader(isr);
		String line;
		while((line = br.readLine()) != null){
			String[] sp = line.split(" ");
			Team s = new Team(0, sp[1], Integer.parseInt(sp[2]), Integer.parseInt(sp[3]), Integer.parseInt(sp[4]), Integer.parseInt(sp[5]));
			teamM.insertTeam(s);
		}
		br.close();
	}


	public void readPlayers () throws IOException {
		InputStream is = this.getClass().getClassLoader().getResourceAsStream("player.txt");
		assert is != null;
		InputStreamReader isr = new InputStreamReader(is, StandardCharsets.UTF_8);
		BufferedReader br = new BufferedReader(isr);
		String line;
		while((line = br.readLine()) != null){
			String[] sp = line.split(" ");
			Player p = new Player(0, Integer.parseInt(sp[1]),
					Integer.parseInt(sp[2]),
					sp[3],
					sp[4],
					Integer.parseInt(sp[5]),
					Integer.parseInt(sp[6]),
					Integer.parseInt(sp[7]),
					Integer.parseInt(sp[8]),
					Double.parseDouble(sp[9]),
					Double.parseDouble(sp[10])
					);
			playerM.insertPlayer(p);
		}
		br.close();
	}
}
