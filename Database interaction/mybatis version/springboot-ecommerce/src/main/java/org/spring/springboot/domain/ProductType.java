package org.spring.springboot.domain;

/**
 * 实体类
 *
 */
public class ProductType {

    private Long productTypeId;

    private String productTypeName = "-1";

    private String productTypeState = "正常";

    public Long getProductTypeId() {
        return productTypeId;
    }

    public void setProductTypeId(Long productId) {
        this.productTypeId = productId;
    }

    public String getProductTypeName() {
        return productTypeName;
    }

    public void setProductTypeName(String productName) {
        this.productTypeName = productName;
    }

    public String getProductTypeState() {
        return productTypeState;
    }

    public void setProductTypeState(String productTypeState) {
        this.productTypeState = productTypeState;
    }
}
