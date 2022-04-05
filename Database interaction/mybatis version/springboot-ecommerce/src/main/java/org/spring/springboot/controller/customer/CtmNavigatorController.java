package org.spring.springboot.controller.customer;

//主页
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;


@Controller
//@RequestMapping("/customer")
public class CtmNavigatorController {

    @GetMapping("/customer")
    public String navigator(Model model) {
        return "customer/navigator";
    }



}