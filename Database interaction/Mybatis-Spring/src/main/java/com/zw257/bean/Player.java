package com.zw257.bean;

public class Player {
	private Integer player_id;
	private Integer team_id;
	private Integer uniform_num;
	private String first_name;
	private String last_name;

	private Integer mpg;
	private Integer ppg;
	private Integer rpg;
	private Integer apg;
	private Double spg;
	private Double bpg;

	public Player(int player_id, int team_id, int uniform_num, String first_name, String last_name, int mpg, int ppg, int rpg, int apg, double spg, double bpg) {
		this.player_id = player_id;
		this.team_id = team_id;
		this.uniform_num = uniform_num;
		this.first_name = first_name;
		this.last_name = last_name;
		this.mpg = mpg;
		this.ppg = ppg;
		this.rpg = rpg;
		this.apg = apg;
		this.spg = spg;
		this.bpg = bpg;
	}

	public Integer getPlayer_id() {
		return player_id;
	}

	public void setPlayer_id(Integer player_id) {
		this.player_id = player_id;
	}

	public Integer getTeam_id() {
		return team_id;
	}

	public void setTeam_id(Integer team_id) {
		this.team_id = team_id;
	}

	public Integer getUniform_num() {
		return uniform_num;
	}

	public void setUniform_num(Integer uniform_num) {
		this.uniform_num = uniform_num;
	}

	public String getFirst_name() {
		return first_name;
	}

	public void setFirst_name(String first_name) {
		this.first_name = first_name;
	}

	public String getLast_name() {
		return last_name;
	}

	public void setLast_name(String last_name) {
		this.last_name = last_name;
	}

	public Integer getMpg() {
		return mpg;
	}

	public void setMpg(Integer mpg) {
		this.mpg = mpg;
	}

	public Integer getPpg() {
		return ppg;
	}

	public void setPpg(Integer ppg) {
		this.ppg = ppg;
	}

	public Integer getRpg() {
		return rpg;
	}

	public void setRpg(Integer rpg) {
		this.rpg = rpg;
	}

	public Integer getApg() {
		return apg;
	}

	public void setApg(Integer apg) {
		this.apg = apg;
	}

	public Double getSpg() {
		return spg;
	}

	public void setSpg(Double spg) {
		this.spg = spg;
	}

	public Double getBpg() {
		return bpg;
	}

	public void setBpg(Double bpg) {
		this.bpg = bpg;
	}
}
