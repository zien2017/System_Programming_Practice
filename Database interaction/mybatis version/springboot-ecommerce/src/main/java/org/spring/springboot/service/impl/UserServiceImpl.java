package org.spring.springboot.service.impl;

import org.spring.springboot.dao.cluster.WalletDao;
import org.spring.springboot.domain.Wallet;
import org.spring.springboot.dao.cluster.CityDao;
import org.spring.springboot.domain.City;
import org.spring.springboot.dao.master.UserDao;
import org.spring.springboot.domain.User;

import org.spring.springboot.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * 用户业务实现层
 *
 */
@Service
public class UserServiceImpl implements UserService {

    @Autowired
    private UserDao userDao; // 主数据源

    @Autowired
    private CityDao cityDao; // 从数据源

    @Autowired
    private WalletDao walletDao; // 从数据源


    @Override
    public List<User> findById(long id) {
        List<User> list = userDao.findById(id);
        return list;
    }

    @Override
    public List<User> findByName(String userName) {
        List<User> list = userDao.findByName(userName);
        return list;
    }

    @Override
    public void setCityList(List<User> list) {
        for(User myUser : list){
            if (cityDao.findById(myUser.getId()) != null){
                myUser.setCity(cityDao.findById(myUser.getId()));
            }
        }
    }

    @Override
    public void setWalletList(List<User> list) {
        for(User myUser : list){
            if (walletDao.findById(myUser.getId()) != null){
                myUser.setWallet(walletDao.findById(myUser.getId()));
            }
        }
    }

    @Override
    public Long saveUser(User user) {
        return userDao.saveUser(user);
    }


    @Override
    public Long updateUser(User user) {
        return userDao.updateUser(user);
    }

    @Override
    public Long deleteUser(Long id) {
        return userDao.deleteUser(id);
    }


}
