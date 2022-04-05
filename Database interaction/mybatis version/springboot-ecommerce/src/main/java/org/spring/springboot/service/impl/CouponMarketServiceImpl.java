package org.spring.springboot.service.impl;

import org.spring.springboot.dao.master.CouponMarketDao;
import org.spring.springboot.domain.CouponMarket;
import org.spring.springboot.service.CouponMarketService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * 业务逻辑实现类
 *
 */
@Service
public class CouponMarketServiceImpl implements CouponMarketService {

    @Autowired
    private CouponMarketDao couponMarketDao;

    @Override
    public List<CouponMarket> findCouponMarketById(Long couponId) {
        return couponMarketDao.findById(couponId);
    }

    @Override
    public Long saveCouponMarketId(CouponMarket couponMarket) {
        return couponMarketDao.saveCouponMarketId(couponMarket);
    }

    @Override
    public Long updateCouponMarketById(CouponMarket couponMarket) {
        //test if posted in couponMarket's params are not -1

        if(!couponMarket.getCouponName().equals("-1") ) {
            couponMarketDao.updateCouponName(couponMarket.getCouponName(), couponMarket.getCouponId());
        }
        if(couponMarket.getCouponQuantity() != -1) {
            couponMarketDao.updateQuantity(couponMarket.getCouponQuantity(), couponMarket.getCouponId());
        }
        if(couponMarket.getExpireDate() != null){
            //TODO
        }
        if(!couponMarket.getCouponDescription().equals("-1") ){
            couponMarketDao.updateDescription(couponMarket.getCouponDescription(), couponMarket.getCouponId());
        }
        couponMarketDao.updateOnsale(couponMarket.isCouponOnsale(), couponMarket.getCouponId());
        if(couponMarket.getThresholdPrice() != -1) {
            couponMarketDao.updateThresholdPrice(couponMarket.getEquivalentPrice(), couponMarket.getCouponId());
        }
        if(couponMarket.getEquivalentPrice() != -1) {
            couponMarketDao.updateEquivalentPrice(couponMarket.getEquivalentPrice(), couponMarket.getCouponId());
        }
        return (long)0;
    }

    @Override
    public List<CouponMarket> listAll() {
        return couponMarketDao.listAll();
    }

    @Override
    public Long deleteCouponMarket(Long couponId) {
        return couponMarketDao.deleteCoupon(couponId);
    }

    @Override
    public Long checkCouponMarketIsExpired() {
        return couponMarketDao.checkCouponMarketIsExpired();
    }


}
