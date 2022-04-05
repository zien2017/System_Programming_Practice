package com.banmajio.controller;

import com.banmajio.bean.Color;
import com.banmajio.bean.State;
import com.banmajio.bean.Team;
import com.banmajio.mapper.ColorMapper;
import com.banmajio.mapper.PlayerMapper;
import com.banmajio.mapper.StateMapper;
import com.banmajio.mapper.TeamMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.List;

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

		return "readFiles() succeed.";
	}

//	@RequestMapping("/select")
//	public List<Team> select() {
//		return mapper.getTeam();
//	}

//	@RequestMapping("/insert")
//	public int insert(Team team) {
//		return mapper.insertTeam(team);
//	}



	public void readColors () throws IOException {
		InputStream is = this.getClass().getClassLoader().getResourceAsStream("color.txt");
		assert is != null;
		InputStreamReader isr = new InputStreamReader(is, StandardCharsets.UTF_8);
		BufferedReader br = new BufferedReader(isr);
		String line;
		while((line = br.readLine()) != null){
			String[] s = line.split(" ");
			Color c = new Color(s[1]);
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
			State s = new State(split[1]);
			stateM.insertState(s);
		}
		br.close();
	}

}
