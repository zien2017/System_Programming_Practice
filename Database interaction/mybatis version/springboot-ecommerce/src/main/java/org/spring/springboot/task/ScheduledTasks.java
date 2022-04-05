package org.spring.springboot.task;

import org.spring.springboot.service.CouponMarketService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import java.text.SimpleDateFormat;
import java.util.Date;

@Component
public class ScheduledTasks {


    @Autowired
    private CouponMarketService couponMarketService;

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");

//    @Scheduled(fixedRate = 5000)
//    public void reportCurrentTime() {
//        System.out.println("当前时间：" + dateFormat.format(new Date()));
//    }

    @Scheduled(fixedRate = 900000)//900000 => per 15min
    //reference:https://www.jianshu.com/p/4d9c9b08111d
    public void checkCouponMarcketIsExpired(){
        System.out.println(dateFormat.format(new Date()) + "  Check if coupon in market is expired.");
        couponMarketService.checkCouponMarketIsExpired();
    }

}
