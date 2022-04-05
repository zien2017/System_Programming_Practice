package com.zw257.controller;

import com.zw257.bean.Color;
import com.zw257.mapper.ColorMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class ColorController {
	@Autowired
	private ColorMapper colorM;


	@RequestMapping("color/select")
	public List<Color> getColors() {
		return colorM.getColor();
	}



}
