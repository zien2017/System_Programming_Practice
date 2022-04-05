package com.zw257.mapper;

import com.zw257.bean.Team;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface TeamMapper {

	public void dropTable ();

	public void createTable ();

	public List<Team> getTeam();

	public void insertTeam(Team state);


}
