package org.spring.springboot.dao.cluster;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.spring.springboot.domain.Wallet;

/**
 * DAO 接口类
 *
 * Created by bysocket on 07/02/2017.
 */
@Mapper
public interface WalletDao {


    /**
     * 根据 ID，获取信息
     *
     * @param id
     * @return
     */
    Wallet findById(@Param("id") long id);

    long saveWallet(Wallet wallet);

    long updateWallet(Wallet wallet);

    long deleteWallet(long id);
}
