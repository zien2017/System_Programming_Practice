package com.zw257.controller;

import com.zw257.bean.State;
import com.zw257.mapper.StateMapper;
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
