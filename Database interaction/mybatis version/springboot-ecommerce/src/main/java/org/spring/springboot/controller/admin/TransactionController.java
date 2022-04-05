//交易管理
package org.spring.springboot.controller.admin;

import org.spring.springboot.domain.User;
import org.spring.springboot.service.UserService;
import org.spring.springboot.domain.Transaction;
import org.spring.springboot.service.TransactionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.util.List;


@Controller
@RequestMapping("/admin")
public class TransactionController {

    @Autowired
    private UserService userService;

    @Autowired
    private TransactionService transactionService;



    @GetMapping("/transaction")
    public String findByTransactionId(Long id, Model model) {

        model.addAttribute("transaction", new Transaction());
        List<Transaction> List;
        if (id != null){
            model.addAttribute("host", "搜索结果");
            List = transactionService.findByTransactionId(id);
        }
        else{
            model.addAttribute("host", "所有交易");
            List = transactionService.findAll();
        }
        model.addAttribute("List", List);
        return "admin/transaction";
    }

    @GetMapping("/transaction/payer_id")
    public String findByPayerId(Long payer_id, Model model) {
        model.addAttribute("transaction", new Transaction());
        List<Transaction> List;
        if (payer_id != null){
            model.addAttribute("host", "搜索结果");
            List = transactionService.findByPayerId(payer_id);
        }
        else{
            model.addAttribute("host", "所有交易");
            List = transactionService.findAll();
        }
        model.addAttribute("List", List);
        return "admin/transaction";
    }

    @GetMapping("/transaction/receiver_id")
    public String findByReceiverId(Long receiver_id, Model model) {
        model.addAttribute("transaction", new Transaction());
        List<Transaction> List;
        if (receiver_id != null){
            model.addAttribute("host", "搜索结果");
            List = transactionService.findByReceiverId(receiver_id);
        }
        else{
            model.addAttribute("host", "所有交易");
            List = transactionService.findAll();
        }
        model.addAttribute("List", List);
        return "admin/transaction";
    }

//    @GetMapping("/customer/{id}")
//    public String findUsers(Model model, @PathVariable("id") Long id) {
//        model.addAttribute("host", "搜索结果");
//        model.addAttribute("customer", new User());
//        List<User> List;
//        List = userService.findById(id);
//        userService.setCity(List);
//        model.addAttribute("List", List);
//        return "admin/customer";
//    }

//    @PostMapping("/customer")
//    public String userRegistration(Model model, @ModelAttribute User customer) {
//        if(customer.getId() == null){
//            return "admin/inputWarning";
//        }
//        else if(!userService.findById(customer.getId()).isEmpty()){
//            return "admin/inputWarning";
//        }
//
//        userService.saveUser(customer);
//        //create city
//        City city = new City();
//        city.setId(customer.getId());
//        cityService.saveCity(city);
//        //create wallet
//        Wallet wallet = new Wallet();
//        wallet.setId(customer.getId());
//        wallet.setAsset(0);
//        walletService.saveWallet(wallet);
//        return "redirect:/customer";
//    }



}