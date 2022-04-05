package com.banmajio.mapper;

import java.util.List;

import com.banmajio.bean.State;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface StateMapper {

	public void dropTable ();

	public void createTable ();

	public List<State> getState();

	public int insertState(State state);


}
