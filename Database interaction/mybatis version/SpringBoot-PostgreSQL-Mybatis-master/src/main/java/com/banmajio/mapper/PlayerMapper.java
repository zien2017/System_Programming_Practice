package com.banmajio.mapper;

import com.banmajio.bean.Player;
import com.banmajio.bean.Team;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface PlayerMapper {



	public void dropTable ();

	public void createTable ();

	public List<Team> getPlayer();

	public int insertPlayer(Player player);


}
