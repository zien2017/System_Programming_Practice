package org.spring.springboot.dao.master;

import org.apache.ibatis.annotations.*;
import org.spring.springboot.domain.CouponMarket;

import java.util.Date;
import java.util.List;

/**
 *  DAO 接口类
 *
 */
@Mapper
public interface CouponMarketDao {

    List<CouponMarket> listAll();

    List<CouponMarket> findById(@Param("couponId") Long couponId);

    Long saveCouponMarketId(CouponMarket coupon);

//    Long updateCoupon(CouponMarket coupon);
//
    Long deleteCoupon(Long couponId);


    @Update("UPDATE coupon_market SET coupon_onsale = '0' WHERE coupon_onsale = '1' AND expire_date <= NOW()")
    Long checkCouponMarketIsExpired();

    @Update("UPDATE coupon_market SET coupon_name = #{couponName} WHERE coupon_id = #{couponId}")
    int updateCouponName(@Param("couponName") String couponName, @Param("couponId") long couponId);

    @Update("UPDATE coupon_market SET coupon_onsale = #{couponOnsale} WHERE coupon_id = #{couponId}")
    int updateOnsale(@Param("couponOnsale") boolean couponOnsale, @Param("couponId") long couponId);

    @Update("UPDATE coupon_market SET threshold_price = #{thresholdPrice} WHERE coupon_id = #{couponId}")
    int updateThresholdPrice(@Param("thresholdPrice") double thresholdPrice, @Param("couponId") long couponId);

    @Update("UPDATE coupon_market SET equivalent_price = #{equivalentPrice} WHERE coupon_id = #{couponId}")
    int updateEquivalentPrice(@Param("equivalentPrice") double equivalentPrice, @Param("couponId") long couponId);

    @Update("UPDATE coupon_market SET coupon_quantity = #{couponQuantity} WHERE coupon_id = #{couponId}")
    int updateQuantity(@Param("couponQuantity") Long couponQuantity, @Param("couponId") long couponId);

    @Update("UPDATE coupon_market SET coupon_description = #{couponDescription} WHERE coupon_id = #{couponId}")
    int updateDescription(@Param("couponDescription") String couponDescription, @Param("couponId") long couponId);

    @Update("UPDATE coupon_market SET expire_date = #{expireDate} WHERE coupon_id = #{couponId}")
    int updateExpireDate(@Param("expireDate") Date expireDate, @Param("couponId") long couponId);

}
