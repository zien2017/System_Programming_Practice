package com.banmajio.bean;



public class State {
	private int state_id;
	private String name;

	public State (int state_id, String name) {
		this.state_id = state_id;
		this.name = name;
	}

	public int getState_id() {
		return state_id;
	}

	public void setState_id(int state_id) {
		this.state_id = state_id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}
