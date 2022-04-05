package org.spring.springboot.controller.admin;

//主页
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;


@Controller
//@RequestMapping("/admin")
public class NavigatorController {

    @GetMapping("/admin")
    public String navigator(Model model) {
        return "admin/navigator";
    }



}