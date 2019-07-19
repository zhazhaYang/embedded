package com.yang.uart;

import gnu.io.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;

import javax.mail.MessagingException;
import javax.mail.internet.AddressException;

import java.util.Scanner;

import com.yang.sendEmail.*;

public class ContactByUart implements Runnable, SerialPortEventListener {

	static Enumeration<CommPortIdentifier> portList;
	static CommPortIdentifier portId;
	InputStream inputStream;
	OutputStream outputStream;
	SerialPort serialPort;
	Thread readThread;

	static int inputTime[] =  new int[3];
	
	@SuppressWarnings("unchecked")
	public ContactByUart(String uartName) {
		portList = CommPortIdentifier.getPortIdentifiers();
		while (portList.hasMoreElements()) {
			portId = (CommPortIdentifier) portList.nextElement();
			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
				if (portId.getName().equals(uartName)) {
					/*open方法打开通讯端口，获得一个CommPort对象 参数1 应用程序名，参数2 在端口打开时阻塞等待的毫秒数*/
					try {
						serialPort = (SerialPort) portId.open("yang", 2000);
						serialPort.setDTR(true);
						serialPort.setDTR(false);
					} catch (PortInUseException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					/* 获取端口的输入流对象 */
					try {
						inputStream = serialPort.getInputStream();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					/* 注册一个SerialPortEventListener事件来监听串口事件 */
					try {
						serialPort.addEventListener(this);
					} catch (TooManyListenersException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					/* 数据可用 */
					serialPort.notifyOnDataAvailable(true);
					/* 设置串口初始化参数 */
					try {
						serialPort.setSerialPortParams(115200, // 波特率
								SerialPort.DATABITS_8, // 数据位
								SerialPort.STOPBITS_1, // 停止位
								SerialPort.PARITY_NONE); // 校验
					} catch (UnsupportedCommOperationException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					int i = 0;
					while (i < 1000000) {
						i++;
					}
					readThread = new Thread(this);
					readThread.start();
				}
			}
		}
		serialPort.setDTR(true);
		serialPort.setDTR(false);
	}

	@Override
	public void serialEvent(SerialPortEvent event) {
		// TODO Auto-generated method stub
		switch (event.getEventType()) {
		case SerialPortEvent.BI:
		case SerialPortEvent.OE:/* Overrun error，溢位错误 */
		case SerialPortEvent.FE:/* Framing error，传帧错误 */
		case SerialPortEvent.PE:/* Parity error，校验错误 */
		case SerialPortEvent.CD:/* Carrier detect，载波检测 */
		case SerialPortEvent.CTS:/* Clear to send，清除发送 */
		case SerialPortEvent.DSR:/* Data set ready，数据设备就绪 */
		case SerialPortEvent.RI:/* Ring indicator，响铃指示 */
		case SerialPortEvent.OUTPUT_BUFFER_EMPTY:/* 输出缓冲区清空 */
			break;
		case SerialPortEvent.DATA_AVAILABLE:/* 端口有可用数据。读到缓冲数组，输出到终端 */
			byte[] readBuffer = new byte[1024];

			int count = 0;
			try {
				/* 返回读取的字节数 */
				count = inputStream.read(readBuffer);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			/* 从开始读到count个字节再转为字符串 */
			String readStr = new String(readBuffer, 0, count);
			if(count==1)
			{
				System.out.println("闹钟  "+(int)readStr.charAt(0));
				if(((int)readStr.charAt(0))==100)//0xFF//闹钟发生
				{
					try {
						new SendEmail();
					} catch (AddressException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (MessagingException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
					
			}
			else //时间
			{
				int ch;
				String hex = "";
				for (int i = 0; i < readStr.length(); i++) {
					ch = (int) readStr.charAt(i);
					hex += Integer.toString(ch);
					if (i < readStr.length() - 1)
						hex += ":";
				}
				System.out.println(hex);
				//System.out.println(readStr);
				hex = "";
			}
			serialPort.setDTR(true);
			serialPort.setDTR(false);
		}
	}

	@SuppressWarnings("resource")
	public void setAlarm() 
	{
		try {
			outputStream = serialPort.getOutputStream();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		Scanner input = new Scanner(System.in);
		int i=0;
		while(true)
		{
			inputTime[i]=input.nextInt();
			i++;
			if(i==3)
			{
				i=0;
				if((inputTime[0]>=24)||(inputTime[1]>=60)||(inputTime[2]>=60))
					System.out.println("超出时钟范围！");
				else
				{
					try {
						outputStream.write(inputTime[0]);
						outputStream.write(inputTime[1]);
						outputStream.write(inputTime[2]);
						outputStream.flush();
						outputStream.close();
					} catch(IOException e) {}
					System.out.println("设置的闹钟为："+inputTime[0]+" : "+inputTime[1]+" : "+inputTime[2]);
				}
			}
		}
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
