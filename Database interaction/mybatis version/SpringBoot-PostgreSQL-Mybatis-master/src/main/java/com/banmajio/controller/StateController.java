package com.banmajio.controller;

import com.banmajio.bean.Color;
import com.banmajio.bean.State;
import com.banmajio.mapper.ColorMapper;
import com.banmajio.mapper.StateMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class StateController {
	@Autowired
	private StateMapper stateM;


	@RequestMapping("state/select")
	public List<State> getState() {
		return stateM.getState();
	}

}
