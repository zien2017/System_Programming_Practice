package org.spring.springboot.service;

import org.spring.springboot.domain.City;
import org.spring.springboot.domain.User;

import java.util.List;

/**
 * 用户业务接口层
 *
 */
public interface UserService {

    /**
     * 根据用户名获取用户信息，包括从库的地址信息
     *
     * @param userName
     * @return
     */
    List<User> findById(long userName);
    List<User> findByName(String userName);
    void setCityList(List<User> list);
    void setWalletList(List<User> list);
    Long saveUser(User user);
    Long updateUser(User user);
    Long deleteUser(Long id);

}
