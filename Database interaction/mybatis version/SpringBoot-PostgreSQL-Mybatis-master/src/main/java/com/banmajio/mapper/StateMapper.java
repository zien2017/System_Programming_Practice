package com.banmajio.mapper;


import com.banmajio.bean.State;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface StateMapper {

	public void dropTable ();

	public void createTable ();

	public List<State> getState();

	public void insertState(State state);


}
