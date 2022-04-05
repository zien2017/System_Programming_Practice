package org.spring.springboot.service;

import org.spring.springboot.domain.ProductType;

import java.util.List;

/**
 * 业务逻辑接口类
 *
 */
public interface ProductTypeManagementService {


    /**
     * 根据 ID,查询信息
     *
     * @param productTypeId
     * @return
     */
    List<ProductType> findById(Long productTypeId);

    List<ProductType> search(Long productTypeId, String productTypeName);

    Long insertOrUpdateById(ProductType productType);

    Long saveById(ProductType productType);

    List<ProductType> listAll();

    Long offLoad(Long productTypeId);


}
