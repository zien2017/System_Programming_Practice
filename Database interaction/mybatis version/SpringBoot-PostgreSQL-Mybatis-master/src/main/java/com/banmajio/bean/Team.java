package com.banmajio.bean;



public class Team {
	private int team_id;
	private String name;
	private int state_id;
	private int color_id;
	private int wins;
	private int losses;

	public Team(String name, int state_id, int color_id, int wins, int losses) {
		this.name = name;
		this.state_id = state_id;
		this.color_id = color_id;
		this.wins = wins;
		this.losses = losses;
	}

	public int getTeam_id() {
		return team_id;
	}

	public void setTeam_id(int team_id) {
		this.team_id = team_id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getState_id() {
		return state_id;
	}

	public void setState_id(int state_id) {
		this.state_id = state_id;
	}

	public int getColor_id() {
		return color_id;
	}

	public void setColor_id(int color_id) {
		this.color_id = color_id;
	}

	public int getWins() {
		return wins;
	}

	public void setWins(int wins) {
		this.wins = wins;
	}

	public int getLosses() {
		return losses;
	}

	public void setLosses(int losses) {
		this.losses = losses;
	}
}
