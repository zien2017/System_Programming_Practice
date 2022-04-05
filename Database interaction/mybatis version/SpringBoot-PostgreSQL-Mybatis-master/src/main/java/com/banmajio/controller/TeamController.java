package com.banmajio.controller;

import com.banmajio.bean.Team;
import com.banmajio.mapper.TeamMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class TeamController {
	@Autowired
	private TeamMapper teamM;


	@RequestMapping("team/select")
	public List<Team> getTeams() {
		return teamM.getTeam();
	}

}
