package com.banmajio.mapper;

import com.banmajio.bean.Player;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface PlayerMapper {

	public void dropTable ();

	public void createTable ();

	public List<Player> getAllPlayer();

	public List<Player> getPlayer(Player p);

	public void insertPlayer(Player player);
}
