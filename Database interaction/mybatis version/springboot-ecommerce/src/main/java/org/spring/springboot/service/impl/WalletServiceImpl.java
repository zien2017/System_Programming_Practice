package org.spring.springboot.service.impl;

import org.spring.springboot.dao.cluster.WalletDao;
import org.spring.springboot.domain.Wallet;
import org.spring.springboot.service.WalletService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * 业务逻辑实现类
 *
 */
@Service
public class WalletServiceImpl implements WalletService {

    @Autowired
    private WalletDao walletDao;

    @Override
    public Wallet findWalletById(Long id) {
        return walletDao.findById(id);
    }

    @Override
    public Long saveWallet(Wallet wallet) {
        return walletDao.saveWallet(wallet);
    }

    @Override
    public Long updateWallet(Wallet wallet) {
        return walletDao.updateWallet(wallet);
    }

    @Override
    public Long deleteWallet(Long id) {
        return walletDao.deleteWallet(id);
    }

}
