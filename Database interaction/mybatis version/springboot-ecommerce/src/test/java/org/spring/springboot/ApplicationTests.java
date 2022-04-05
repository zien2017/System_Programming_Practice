package org.spring.springboot;

import com.alibaba.druid.pool.DruidDataSource;
import org.spring.springboot.*;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.transaction.annotation.Transactional;

import javax.activation.DataSource;
import java.sql.Connection;


@RunWith(SpringRunner.class)
@SpringBootTest
public class ApplicationTests {

//	@Autowired
//	private UserRepository userRepository;


	@Test
	@Transactional
	public void test() throws Exception {



	}


}
