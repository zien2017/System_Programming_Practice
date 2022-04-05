package com.zw257.mapper;


import com.zw257.bean.State;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface StateMapper {

	public void dropTable ();

	public void createTable ();

	public List<State> getState();

	public void insertState(State state);


}
