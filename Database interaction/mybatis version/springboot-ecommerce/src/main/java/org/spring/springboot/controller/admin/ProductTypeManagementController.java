//商品类型管理
package org.spring.springboot.controller.admin;


import org.spring.springboot.dao.master.ProductTypeDao;
import org.spring.springboot.domain.ProductType;
import org.spring.springboot.service.ProductTypeManagementService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import java.util.List;


@Controller
@RequestMapping("/admin")
public class ProductTypeManagementController {

    @Autowired
    private ProductTypeManagementService productTypeManagementService;

    @Autowired
    private ProductTypeDao productTypeDao;

    @GetMapping("/productTypeManagement")
    public String listAllProductTypeInMarket(Long productTypeId, String productTypeName, Model model) {
        model.addAttribute("ProductType", new ProductType());
        List<ProductType> list;
        model.addAttribute("host", "搜索结果");
        list = productTypeManagementService.search(productTypeId, productTypeName);
        model.addAttribute("list", list);
        return "admin/productTypeManagement";
    }



    @PostMapping("/productTypeManagement")
    public String productTypeInsertOrUpdate(Model model, @ModelAttribute ProductType productType) {
        if((productType.getProductTypeId() == null)) {
            model.addAttribute("host", "ID不能为空");
            return "admin/inputWarning";
        }
        productTypeManagementService.insertOrUpdateById(productType);
        return "redirect:/admin/productTypeManagement";
    }

    @PostMapping("/productTypeManagementDelete")
    public String deleteUser(Model model, @ModelAttribute ProductType productType) {
        if(productType.getProductTypeId() == null){
            model.addAttribute("host", "ID不能为空");
            return "admin/inputWarning";
        }
        if(productTypeManagementService.offLoad(productType.getProductTypeId()) == -1){
            model.addAttribute("host", "失败，该类型下仍然存在商品");
            return "admin/inputWarning";
        }
        return "redirect:/admin/productTypeManagement";
    }

}