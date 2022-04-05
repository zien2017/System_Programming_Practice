package com.banmajio.controller;

import com.banmajio.mapper.ColorMapper;
import com.banmajio.mapper.PlayerMapper;
import com.banmajio.mapper.StateMapper;
import com.banmajio.mapper.TeamMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

@RestController
public class FilesReaderController {

    @Autowired
    private ColorMapper colorM;
    @Autowired
    private StateMapper stateM;
    @Autowired
    private TeamMapper teamM;
    @Autowired
    private PlayerMapper playerM;

    public void readColor () throws IOException {
        InputStream is = this.getClass().getClassLoader().getResourceAsStream("color.txt");
        assert is != null;
        InputStreamReader isr = new InputStreamReader(is, StandardCharsets.UTF_8);
        BufferedReader br = new BufferedReader(isr);

        String line;
        while((line = br.readLine()) != null){
            //process the line
//            System.out.println(line);
            
        }
        br.close();
    }

    @RequestMapping("/readFiles")
    public String readFiles() throws IOException {
        FilesReaderController f = new FilesReaderController();
        f.readColor();
        return "readFiles() succeed.";
    }

}
