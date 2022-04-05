//用户管理
package org.spring.springboot.controller.admin;

import org.spring.springboot.dao.cluster.CityDao;
import org.spring.springboot.domain.User;
import org.spring.springboot.service.UserService;
import org.spring.springboot.domain.City;
import org.spring.springboot.service.CityService;
import org.spring.springboot.domain.Wallet;
import org.spring.springboot.service.WalletService;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.util.List;


@Controller
@RequestMapping("/admin")
public class UserController {

    @Autowired
    private UserService userService;

    @Autowired
    private CityService cityService;

    @Autowired
    private CityDao cityDao;

    @Autowired
    private WalletService walletService;

    @GetMapping("/user")
    public String listAllUsers(Long id, Model model) {
        User tempUser = new User();
        tempUser.setCity(new City());
        model.addAttribute("user", tempUser);
        List<User> List;
        if (id != null){
            model.addAttribute("host", "搜索结果");
            List = userService.findById(id);
        }else{
            model.addAttribute("host", "所有用户");
            List = userService.findByName("%");
        }
        userService.setCityList(List);
        userService.setWalletList(List);
        model.addAttribute("List", List);
        return "admin/user";
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

    @PostMapping("/user")
    public String userRegistrationOrUpdate(Model model, @ModelAttribute User user) {
        if(user.getId() == null){
            return "admin/inputWarning";
        }
        if(userService.findById(user.getId()).isEmpty()){
            userService.saveUser(user);
            //create city
            user.getCity().setId(user.getId());
            cityService.saveCity(user.getCity());
            //create wallet
            Wallet wallet = new Wallet();
            wallet.setId(user.getId());
            wallet.setAsset(0);
            walletService.saveWallet(wallet);
        }else{
            City cityInDB = cityService.findCityById(user.getId());
            //test if city's param is not all null
            if(user.getUserName() != null && user.getUserName() != "") {
                userService.updateUser(user);
            }
            //test if city's param is not all null
            user.getCity().setId(user.getId());

            if(user.getCity().getAddress() != null && user.getCity().getAddress() != "" ) {
                cityDao.updateAddress(user.getCity().getAddress(), user.getId());
            }
            if(user.getCity().getCityName() != null && user.getCity().getCityName() != "") {
                cityDao.updateCityName(user.getCity().getCityName(),user.getId());

            }
            if(user.getCity().getProvinceId() != null){
                cityDao.updateProvinceId(user.getCity().getProvinceId(), user.getId());
            }
            if(user.getCity().getDescription() != null && user.getCity().getDescription() != ""){
                cityDao.updateDescription(user.getCity().getDescription(), user.getId());
            }
            if(user.getCity().getPhoneNumber() != null){
                cityDao.updatePhoneNumber(user.getCity().getPhoneNumber(), user.getId());
            }

//            //update city
//            cityService.updateCity(customer.getCity());

        }

        return "redirect:/admin/user";
    }

    @PostMapping("/userDelete")
    public String deleteUser(Model model, @ModelAttribute User user) {
        if(user.getId() == null){
            return "admin/inputWarning";
        }
        userService.deleteUser(user.getId());
        cityService.deleteCity(user.getId());
        walletService.deleteWallet(user.getId());
        return "redirect:/admin/user";
    }

}