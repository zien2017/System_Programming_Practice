package com.banmajio.controller;

import com.banmajio.bean.Player;
import com.banmajio.mapper.PlayerMapper;
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
