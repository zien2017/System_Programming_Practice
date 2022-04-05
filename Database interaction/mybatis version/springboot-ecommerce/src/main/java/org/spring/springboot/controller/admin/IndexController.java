package org.spring.springboot.controller.admin;

//主页
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;




@Controller
@RequestMapping("/admin")
public class IndexController {

    @GetMapping("/index")
    public String homepage(Model model) {

        return "admin/index";

    }



}