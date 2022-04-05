package org.spring.springboot.service.impl;

import org.spring.springboot.dao.cluster.TransactionDao;
import org.spring.springboot.domain.Transaction;
import org.spring.springboot.service.TransactionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * 业务逻辑实现类
 *
 */
@Service
public class TransactionServiceImpl implements TransactionService {

    @Autowired
    private TransactionDao transactionDao;

    @Override
    public List<Transaction> findByTransactionId(Long id) {
        return transactionDao.findByTransactionId(id);
    }

    @Override
    public List<Transaction> findAll() {
        return transactionDao.findAll();
    }

    @Override
    public List<Transaction> findByPayerId(Long payerId) {
        return transactionDao.findByPayerId(payerId);    }

    @Override
    public List<Transaction> findByReceiverId(Long receiverId) {
        return transactionDao.findByReceiverId(receiverId);    }

    @Override
    public Long saveTransaction(Transaction transaction) {
        return transactionDao.saveTransaction(transaction);
    }

//    @Override
//    public Long updateTransaction(Transaction transaction) {
//        return transactionDao.updateTransaction(transaction);
//    }
//
//    @Override
//    public Long deleteTransaction(Long id) {
//        return transactionDao.deleteTransaction(id);
//    }

}
