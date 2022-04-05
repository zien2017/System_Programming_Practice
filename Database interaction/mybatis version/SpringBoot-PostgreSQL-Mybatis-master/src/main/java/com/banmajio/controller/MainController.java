package com.banmajio.controller;

import com.banmajio.bean.Team;
import com.banmajio.mapper.ColorMapper;
import com.banmajio.mapper.PlayerMapper;
import com.banmajio.mapper.StateMapper;
import com.banmajio.mapper.TeamMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

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




//	@RequestMapping("/select")
//	public List<Team> select() {
//		return mapper.getTeam();
//	}

//	@RequestMapping("/insert")
//	public int insert(Team team) {
//		return mapper.insertTeam(team);
//	}

}
