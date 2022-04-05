package org.spring.springboot.dao.master;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;
import org.spring.springboot.domain.Product;

import java.util.List;

/**
 *  DAO 接口类
 *
 */
@Mapper
public interface ProductDao {

    List<Product> listAll();

    List<Product> findById(@Param("productId") Long productId);

    List<Product> findByProductTypeId(@Param("productTypeId") Long productTypeId);

    List<Product> searchProduct(Product product);

    Long saveProductById(Product product);

    Long deleteProduct(Long productId);

    @Select("SELECT * FROM product WHERE product_id = #{productId} FOR UPDATE")
    int xlockById(@Param("productId") Long productId);

    @Update("UPDATE product SET product_type_id = #{productTypeId} WHERE product_id = #{productId}")
    int updateProductTypeId(@Param("productTypeId") Long productTypeId, @Param("productId") Long productId);

    @Update("UPDATE product SET product_name = #{productName} WHERE product_id = #{productId}")
    int updateProductName(@Param("productName") String productName, @Param("productId") Long productId);

    @Update("UPDATE product SET product_state = #{productState} WHERE product_id = #{productId}")
    int updateProductState(@Param("productState") Integer productState, @Param("productId") Long productId);

    @Update("UPDATE product SET product_price = #{productPrice} WHERE product_id = #{productId}")
    int updateProductPrice(@Param("productPrice") Double productPrice, @Param("productId") Long productId);

    @Update("UPDATE product SET product_quantity = #{productQuantity} WHERE product_id = #{productId}")
    int updateQuantity(@Param("productQuantity") Long productQuantity, @Param("productId") Long productId);

    @Update("UPDATE product SET product_description = #{productDescription} WHERE product_id = #{productId}")
    int updateDescription(@Param("productDescription") String productDescription, @Param("productId") Long productId);

    @Update("UPDATE product SET product_tag = #{productTag} WHERE product_id = #{productId}")
    int updateTag(@Param("productTag") String productTag, @Param("productId") Long productId);

}
