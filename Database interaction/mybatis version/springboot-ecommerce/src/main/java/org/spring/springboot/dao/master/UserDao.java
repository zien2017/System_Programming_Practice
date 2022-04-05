package org.spring.springboot.dao.master;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.spring.springboot.domain.User;

import java.util.List;

/**
 * 用户 DAO 接口类
 *
 * Created by bysocket on 07/02/2017.
 */
@Mapper
public interface UserDao {

    /**
     * 根据用户名获取用户信息
     *
     * @param userName
     * @return
     */
    List<User> findByName(@Param("userName") String userName);
    List<User> findById(@Param("id") long id);
    Long saveUser(User user);
    Long deleteUser(@Param("id") Long id);
    Long updateUser(User user);
}
