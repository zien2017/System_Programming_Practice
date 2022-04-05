package org.spring.springboot.service;

import org.spring.springboot.domain.Transaction;

import java.util.List;

/**
 * 业务逻辑接口类
 *
 */
public interface TransactionService {


    /**
     * 根据 ID,查询信息
     *
     * @param id
     * @return
     */
    List<Transaction> findByTransactionId(Long id);

    List<Transaction> findByPayerId(Long id);

    List<Transaction> findByReceiverId(Long id);

    List<Transaction> findAll();

    /**
     * 新增
     *
     * @param transaction
     * @return
     */
    Long saveTransaction(Transaction transaction);

    /**
     * 更新
     *
     * @param transaction
     * @return
     */
//    Long updateTransaction(Transaction transaction);

    /**
     * 根据 ID,删除信息
     *
     * @param id
     * @return
     */
//    Long deleteTransaction(Long id);


}
