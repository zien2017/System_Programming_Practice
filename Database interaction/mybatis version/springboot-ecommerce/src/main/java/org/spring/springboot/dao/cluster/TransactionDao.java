package org.spring.springboot.dao.cluster;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.spring.springboot.domain.Transaction;
import org.spring.springboot.domain.Wallet;

import java.util.List;

/**
 * DAO 接口类
 *
 * Created by bysocket on 07/02/2017.
 */
@Mapper
public interface TransactionDao {


    /**
     * 根据 ID，获取信息
     *
     * @return
     */


    List<Transaction> findAll();

    List<Transaction> findByTransactionId(@Param("id") long transactuinId);

    List<Transaction> findByPayerId(@Param("payerId") long payerId);

    List<Transaction> findByReceiverId(@Param("receiverId") long receiverId);

    long saveTransaction(Transaction transaction);

    long updateTransaction(Transaction transaction);

    long deleteTransaction(long id);
}
