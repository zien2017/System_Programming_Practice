package com.banmajio.mapper;

import com.banmajio.bean.Color;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface ColorMapper {

	public void createTable ();

	public List<Color> getColor();

	public int insertColor(Color state);


}
