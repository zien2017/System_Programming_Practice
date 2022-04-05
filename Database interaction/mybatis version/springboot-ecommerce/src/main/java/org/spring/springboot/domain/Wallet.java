package org.spring.springboot.domain;

/**
 * 钱包实体类
 *
 */
public class Wallet {


    private long id;

    private double asset;

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public double getAsset() {
        return asset;
    }

    public void setAsset(long asset) {
        this.asset = asset;
    }
}
