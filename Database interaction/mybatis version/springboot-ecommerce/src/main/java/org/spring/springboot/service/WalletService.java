package org.spring.springboot.service;

import org.spring.springboot.domain.Wallet;

import java.util.List;

/**
 * 业务逻辑接口类
 *
 */
public interface WalletService {


    /**
     * 根据 ID,查询信息
     *
     * @param id
     * @return
     */
    Wallet findWalletById(Long id);

    /**
     * 新增
     *
     * @param wallet
     * @return
     */
    Long saveWallet(Wallet wallet);

    /**
     * 更新
     *
     * @param wallet
     * @return
     */
    Long updateWallet(Wallet wallet);

    /**
     * 根据 ID,删除信息
     *
     * @param id
     * @return
     */
    Long deleteWallet(Long id);


}
