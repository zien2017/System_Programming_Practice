package com.banmajio.controller;

import java.util.List;

import com.banmajio.bean.State;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.banmajio.mapper.StateMapper;

@RestController
public class StateController {

	@Autowired
	private StateMapper mapper;

	@RequestMapping("state/createTable")
	public void createTableState() {
		mapper.createTable();
	}

	@RequestMapping("state/select")
	public List<State> select() {
		return mapper.getState();
	}

	@RequestMapping("state/insert")
	public int insert(State state) {
		return mapper.insertState(state);
	}

	@RequestMapping("state/update")
	public int update(State state) {
		return mapper.updateState(state);
	}

	@RequestMapping("state/delete")
	public int delete(String name) {
		return mapper.deleteState(name);
	}
}
