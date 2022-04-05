package com.banmajio.controller;

import com.banmajio.bean.*;
import com.banmajio.mapper.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.LinkedList;
import java.util.List;

@RestController
public class QueryController {
	@Autowired
	private StateMapper stateM;
	@Autowired
	private ColorMapper colorM;
	@Autowired
	private TeamMapper teamM;
	@Autowired
	private PlayerMapper playerM;
	@Autowired
	private QueryMapper qM;


	@RequestMapping("/q1")
	public List<Player> query1() {
		Q1 q = new Q1();
		q.setUse_mpg(true); q.setMin_mpg(35); q.setMax_mpg(40);
		q.setUse_rpg(false); q.setMin_rpg(5); q.setMax_rpg(10);
		return qM.query_1(q);
	}

	@RequestMapping("/q2")
	public List<String> query2() {
		String color_name = "Green";
		return qM.query_2(color_name);
	}

	@RequestMapping("/q3")
	public List<Q3_rtn> query3() {
		String team_name = "Duke";
		return qM.query_3(team_name);
	}

	@RequestMapping("/q4")
	public List<Q4_rtn> query4() {
		Q4_arg q4_arg = new Q4_arg();
		q4_arg.setTeam_state("MA");
		q4_arg.setTeam_color("Maroon");

		return qM.query_4(q4_arg);
	}


	@RequestMapping("/q5")
	public List<Q5_rtn> query5() {
		return qM.query_5(13);
	}


}
