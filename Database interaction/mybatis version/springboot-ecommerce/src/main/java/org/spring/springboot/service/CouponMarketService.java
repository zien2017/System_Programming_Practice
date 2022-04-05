package org.spring.springboot.service;

import org.spring.springboot.domain.CouponMarket;

import java.util.List;

/**
 * 业务逻辑接口类
 *
 */
public interface CouponMarketService {


    /**
     * 根据 ID,查询信息
     *
     * @param couponId
     * @return
     */
    List<CouponMarket> findCouponMarketById(Long couponId);


    Long saveCouponMarketId(CouponMarket couponMarket);

    Long updateCouponMarketById(CouponMarket couponMarket);

    List<CouponMarket> listAll();

    Long deleteCouponMarket(Long couponId);

    Long checkCouponMarketIsExpired();

}
