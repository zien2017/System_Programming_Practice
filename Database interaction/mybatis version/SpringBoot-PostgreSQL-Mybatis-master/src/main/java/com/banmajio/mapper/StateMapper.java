package com.banmajio.mapper;

import java.util.List;

import com.banmajio.bean.State;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface StateMapper {

	public void createTable ();

	public List<State> getState();

	public int insertState(State state);

	public int updateState(State state);

	public int deleteState(String name);

}
