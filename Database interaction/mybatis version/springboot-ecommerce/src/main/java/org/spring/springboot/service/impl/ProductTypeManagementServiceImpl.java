package org.spring.springboot.service.impl;

import org.spring.springboot.dao.master.ProductDao;
import org.spring.springboot.dao.master.ProductTypeDao;
import org.spring.springboot.domain.ProductType;
import org.spring.springboot.service.ProductTypeManagementService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * 业务逻辑实现类
 *
 */
@Service
public class ProductTypeManagementServiceImpl implements ProductTypeManagementService {

    @Autowired
    private ProductTypeDao productTypeDao;

    @Autowired
    private ProductDao productDao;

    @Override
    public List<ProductType> findById(Long productTypeId) {
        return productTypeDao.findById(productTypeId);
    }

    @Override
    public List<ProductType> search(Long productTypeId, String productTypeName) {
        ProductType productType = new ProductType();
        productType.setProductTypeId(productTypeId);
        productType.setProductTypeName(productTypeName);
        return productTypeDao.search(productType);
    }

    @Transactional
    @Override
    public Long insertOrUpdateById(ProductType productType) {
        //search if there exist a productType with same id
        if (productTypeDao.findById(productType.getProductTypeId()).isEmpty()){
            //insert new productType
            //check if null for some key value
            return productTypeDao.saveById(productType);
        }else{
            //update productType
            //x-lock
            productTypeDao.xlockById(productType.getProductTypeId());
            //check if not null then update
            long productTypeId = productType.getProductTypeId();

            if (! productType.getProductTypeName().equals("-1")){
                productTypeDao.updateProductTypeNameById(productType.getProductTypeName(),productTypeId);
            }
            return (long)0;
        }
    }

    @Override
    public Long saveById(ProductType productType) {
        return productTypeDao.saveById(productType);
    }

    @Override
    public List<ProductType> listAll() {
        return productTypeDao.listAll();
    }

    @Transactional
    @Override
    public Long offLoad(Long productTypeId) {
        productTypeDao.xlockById(productTypeId);
        if (!productDao.findByProductTypeId(productTypeId).isEmpty()) {
            return (long)-1;
        }
        productTypeDao.updateProductTypeStateById("Deleted",productTypeId);
        return (long)0;
    }

}
