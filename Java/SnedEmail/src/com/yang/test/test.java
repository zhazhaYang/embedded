package com.yang.test;
import javax.mail.MessagingException;
import javax.mail.internet.AddressException;

import com.yang.uart.*;
import com.yang.sendEmail.*;

public class test {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		ContactByUart contact = new ContactByUart("COM4");
		contact.setAlarm();
		new SendEmail();
	}

}
