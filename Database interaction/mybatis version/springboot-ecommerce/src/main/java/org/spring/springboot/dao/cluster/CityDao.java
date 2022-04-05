package org.spring.springboot.dao.cluster;

import org.apache.ibatis.annotations.*;
import org.spring.springboot.domain.City;

import java.util.List;

/**
 * 城市 DAO 接口类
 *
 * Created by bysocket on 07/02/2017.
 */
@Mapper
public interface CityDao {

    /**
     * 根据城市名称，查询城市信息
     *
     * @param cityName 城市名
     */
    City findByName(@Param("cityName") String cityName);


    /**
     * 获取城市信息列表
     *
     * @return
     */
    List<City> findAllCity();


    /**
     * 根据城市 ID，获取城市信息
     *
     * @param id
     * @return
     */
    City findById(@Param("id") Long id);

    Long saveCity(City city);

    Long updateCity(City city);

    Long deleteCity(Long id);

    @Update("UPDATE city SET city_name = #{cityName} WHERE id = #{userId}")
    int updateCityName(@Param("cityName") String cityName, @Param("userId") long userId);

    @Update("UPDATE city SET address = #{address} WHERE id = #{userId}")
    int updateAddress(@Param("address") String address, @Param("userId") long userId);

    @Update("UPDATE city SET description = #{description} WHERE id = #{userId}")
    int updateDescription(@Param("description") String description, @Param("userId") long userId);

    @Update("UPDATE city SET province_id = #{provinceId} WHERE id = #{userId}")
    int updateProvinceId(@Param("provinceId") Long provinceId, @Param("userId") long userId);

    @Update("UPDATE city SET phonenumber = #{phoneNumber} WHERE id = #{userId}")
    int updatePhoneNumber(@Param("phoneNumber") Long phoneNumber, @Param("userId") long userId);


}
