package org.spring.springboot.dao.master;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;
import org.spring.springboot.domain.ProductType;

import java.util.List;

/**
 *  DAO 接口类
 *
 */
@Mapper
public interface ProductTypeDao {

    List<ProductType> listAll();

    List<ProductType> findById(@Param("productTypeId") Long productTypeId);

    List<ProductType> search(ProductType productType);

    Long saveById(ProductType productType);

//    Long deleteById(Long productTypeId);

    @Select("SELECT * FROM product_type WHERE product_type_id = #{productTypeId} FOR UPDATE")
    Long xlockById(@Param("productTypeId") long productTypeId);

    @Update("UPDATE product_type SET product_type_name = #{productTypeName} WHERE product_type_id = #{productTypeId}")
    Long updateProductTypeNameById(@Param("productTypeName") String productTypeName, @Param("productTypeId") long productTypeId);

    @Update("UPDATE product_type SET product_type_state = #{productTypeState} WHERE product_type_id = #{productTypeId}")
    Long updateProductTypeStateById(@Param("productTypeState") String productTypeState, @Param("productTypeId") long productTypeId);


}
