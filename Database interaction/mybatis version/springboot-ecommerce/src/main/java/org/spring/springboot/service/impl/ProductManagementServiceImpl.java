package org.spring.springboot.service.impl;

import org.spring.springboot.dao.master.ProductDao;
import org.spring.springboot.domain.Product;
import org.spring.springboot.service.ProductManagementService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * 业务逻辑实现类
 *
 */
@Service
public class ProductManagementServiceImpl implements ProductManagementService {

    @Autowired
    private ProductDao productDao;

    @Override
    public List<Product> findProductById(Long productId) {
        return productDao.findById(productId);
    }

    @Override
    public List<Product> searchProduct(Product product) {
        return productDao.searchProduct(product);
    }

    @Transactional
    @Override
    public Long insertOrUpdateProduct(Product product) {
        //test if there exist a product with same id
        if (productDao.findById(product.getProductId()).isEmpty()){
            //insert new product
            //check if null for some key value
            if (product.getProductQuantity() == null){
                product.setProductQuantity((long)0);
            }
            return productDao.saveProductById(product);
        }else{
            //update product
            //x-lock
            productDao.xlockById(product.getProductId());
            //check if not null then update
            long myProductId = product.getProductId();

            if (product.getProductTypeId() != -1){
                productDao.updateProductTypeId(product.getProductTypeId(),myProductId);
            }
            if (!product.getProductName().equals("-1")){
                productDao.updateProductName(product.getProductName(),myProductId);
            }
            if (product.getProductState() != -1){
                productDao.updateProductState(product.getProductState(),myProductId);
            }
            if(product.getProductPrice() != -1){
                productDao.updateProductPrice(product.getProductPrice(),myProductId);
            }
            if (! product.getProductDescription().equals("-1")){
                productDao.updateDescription(product.getProductDescription(),myProductId);
            }
            if(product.getProductQuantity() != -1) {
                productDao.updateQuantity(product.getProductQuantity(), myProductId);
            }
            if (! product.getProductTag().equals("-1")){
                productDao.updateTag(product.getProductTag(),myProductId);
            }
            return (long)0;
        }
    }

    @Override
    public Long saveProductId(Product product) {
        return productDao.saveProductById(product);
    }

    @Override
    public List<Product> listAll() {
        return productDao.listAll();
    }

    @Override
    public Long deleteProduct(Long productId) {
        return productDao.deleteProduct(productId);
    }


}
