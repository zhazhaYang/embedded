package com.yang.sendEmail;

import java.util.Date;
import java.util.Properties;
import javax.mail.Authenticator;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
 
public class SendEmail {
	// ������ �˺ź�����
	public static final String MY_EMAIL_ACCOUNT = "18316674290@163.com";
	public static final String MY_EMAIL_PASSWORD = "qianrushi123";// ��Ȩ��
	
	// SMTP������
	public static final String MEAIL_163_SMTP_HOST = "smtp.163.com";
	public static final String SMTP_163_PORT = "25";// �˿ں�,163ʹ�õ���;
	
	// �ռ���
	public static final String RECEIVE_EMAIL_ACCOUNT = "2601645510@qq.com";
	
	public SendEmail() throws AddressException, MessagingException {
		Properties p = new Properties();
		p.setProperty("mail.smtp.host", MEAIL_163_SMTP_HOST);
		p.setProperty("mail.smtp.port", SMTP_163_PORT);
		p.setProperty("mail.smtp.socketFactory.port", SMTP_163_PORT);
		p.setProperty("mail.smtp.auth", "true");
		p.setProperty("mail.smtp.socketFactory.class", "SSL_FACTORY");
		
		Session session = Session.getInstance(p, new Authenticator() {
			// ������֤�˻���Ϣ
			@Override
			protected PasswordAuthentication getPasswordAuthentication() {
				return new PasswordAuthentication(MY_EMAIL_ACCOUNT, MY_EMAIL_PASSWORD);
			}
		});
		session.setDebug(true);
		System.out.println("�����ʼ�");
		MimeMessage message = new MimeMessage(session);
		// ������
		message.setFrom(new InternetAddress(MY_EMAIL_ACCOUNT));
		// �ռ��˺ͳ�����
		message.setRecipients(Message.RecipientType.TO, RECEIVE_EMAIL_ACCOUNT);
//		message.setRecipients(Message.RecipientType.CC, MY_EMAIL_ACCOUNT);
		
		// ����
		message.setSubject("�����¼�");
		message.setContent("<h1>�����õ��������죡</h1>", "text/html;charset=UTF-8");
		message.setSentDate(new Date());
		message.saveChanges();
		System.out.println("׼������");
		Transport.send(message);
	}
}
