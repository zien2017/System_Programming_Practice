package com.banmajio.mapper;

import com.banmajio.bean.Team;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface TeamMapper {

	public void dropTable ();

	public void createTable ();

	public List<Team> getTeam();

	public void insertTeam(Team state);


}
