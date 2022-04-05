package com.zw257.controller;

import com.zw257.bean.*;
import com.zw257.mapper.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

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

	@RequestMapping(value = {"/q2"}, method = {RequestMethod.GET})
	@ResponseBody
	public List<String> query2(@RequestParam(value = "color", defaultValue = "%") String color) {
		return qM.query_2(color);
	}


	@RequestMapping(value = {"/q3"}, method = {RequestMethod.GET})
	@ResponseBody
	public List<Q3_rtn> query3(@RequestParam(value = "team", defaultValue = "%") String team) {
		return qM.query_3(team);
	}

	@RequestMapping(value = {"/q4"}, method = {RequestMethod.GET})
	@ResponseBody
	public List<Q4_rtn> query4(@RequestParam(value = "state", defaultValue = "%") String state,
							   @RequestParam(value = "color", defaultValue = "%") String color) {
		Q4_arg q4_arg = new Q4_arg();
		q4_arg.setTeam_state(state);
		q4_arg.setTeam_color(color);
		return qM.query_4(q4_arg);
	}


	@RequestMapping(value = {"/q5"}, method = {RequestMethod.GET})
	@ResponseBody
	public List<Q5_rtn> query5(@RequestParam(value = "wins", defaultValue = "-1") int wins) {
		return qM.query_5(wins);
	}


}
