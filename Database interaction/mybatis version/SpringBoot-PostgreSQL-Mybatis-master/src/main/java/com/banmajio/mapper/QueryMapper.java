package com.banmajio.mapper;

import com.banmajio.bean.*;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface QueryMapper {

	public List<Player> query_1(Q1 p);

	public List<String> query_2(String color_name);

	public List<Q3_rtn> query_3(String team_name);

	public List<Q4_rtn> query_4(Q4_arg q4_arg);

	public List<Q5_rtn> query_5(int num_wins);

}
