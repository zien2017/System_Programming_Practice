package com.zw257.controller;

import com.zw257.bean.Player;
import com.zw257.mapper.PlayerMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class PlayerController {
	@Autowired
	private PlayerMapper playerM;


	@RequestMapping("player/select")
	public List<Player> getAllPlayer() {
		return playerM.getAllPlayer();
	}


}
