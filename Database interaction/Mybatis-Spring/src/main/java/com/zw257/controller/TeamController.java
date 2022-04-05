package com.zw257.controller;

import com.zw257.bean.Team;
import com.zw257.mapper.TeamMapper;
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
