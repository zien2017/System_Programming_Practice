package com.zw257.mapper;

import com.zw257.bean.Color;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface ColorMapper {

	public void dropTable ();

	public void createTable ();

	public List<Color> getColor();

	public void insertColor(Color color);


}
