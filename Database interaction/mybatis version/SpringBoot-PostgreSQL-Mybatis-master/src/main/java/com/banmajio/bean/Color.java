package com.banmajio.bean;



public class Color {
	private int color_id;
	private String name;

	public Color (int color_id, String name) {
		this.color_id = color_id;
		this.name = name;
	}

	public int getColor_id() {
		return color_id;
	}

	public void setColor_id(int color_id) {
		this.color_id = color_id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}
