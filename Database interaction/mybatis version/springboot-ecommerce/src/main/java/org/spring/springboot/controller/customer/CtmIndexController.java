package org.spring.springboot.controller.customer;

//主页
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;


@Controller
@RequestMapping("/customer")
public class CtmIndexController {

    @GetMapping("/index")
    public String homepage(Model model) {

        return "customer/index";

    }



}