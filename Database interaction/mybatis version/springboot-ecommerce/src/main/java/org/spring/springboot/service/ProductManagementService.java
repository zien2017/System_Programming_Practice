package org.spring.springboot.service;

import org.spring.springboot.domain.Product;

import java.util.List;

/**
 * 业务逻辑接口类
 *
 */
public interface ProductManagementService {


    /**
     * 根据 ID,查询信息
     *
     * @param productId
     * @return
     */
    List<Product> findProductById(Long productId);

    List<Product> searchProduct(Product product);

    Long insertOrUpdateProduct(Product product);

    Long saveProductId(Product product);

    List<Product> listAll();

    Long deleteProduct(Long productId);


}
