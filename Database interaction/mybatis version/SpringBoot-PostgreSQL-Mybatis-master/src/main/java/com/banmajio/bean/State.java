package com.banmajio.bean;



public class State {
	private int state_id;
	private String name;

	public State (String name) {
		this.name = name;
	}


	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}
