import gnu.io.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;
import java.util.Scanner;

public class ContactByUart implements Runnable, SerialPortEventListener {

	static Enumeration<CommPortIdentifier> portList;
	static CommPortIdentifier portId;
	InputStream inputStream;
	OutputStream outputStream;
	SerialPort serialPort;
	Thread readThread;

	static int inputTime[] =  new int[3];
	
	@SuppressWarnings("unchecked")
	public static void main(String[] args) {
		/*
		 * ����������getPortIdentifiers�������һ��ö�ٶ��� �ö����ְ�����ϵͳ�й���ÿ���˿ڵ�CommPortIdentifier����
		 * ����Ķ˿ڲ�������ָ���ڣ�Ҳ��������
		 */
		portList = CommPortIdentifier.getPortIdentifiers();
		while (portList.hasMoreElements()) {
			portId = (CommPortIdentifier) portList.nextElement();
			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
				if (portId.getName().equals("COM4")) {
					ContactByUart contact = new ContactByUart();
					contact.setAlarm();
				}
			}
		}
	}

	public ContactByUart() {
		/*
		 * open������ͨѶ�˿ڣ����һ��CommPort���� ����1 Ӧ�ó�����������2 �ڶ˿ڴ�ʱ�����ȴ��ĺ�����
		 */
		try {
			serialPort = (SerialPort) portId.open("jjt", 2000);
			serialPort.setDTR(true);
			serialPort.setDTR(false);
		} catch (PortInUseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		/* ��ȡ�˿ڵ����������� */
		try {
			inputStream = serialPort.getInputStream();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		/* ע��һ��SerialPortEventListener�¼������������¼� */
		try {
			serialPort.addEventListener(this);
		} catch (TooManyListenersException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		/* ���ݿ��� */
		serialPort.notifyOnDataAvailable(true);
		/* ���ô��ڳ�ʼ������ */
		try {
			serialPort.setSerialPortParams(115200, // ������
					SerialPort.DATABITS_8, // ����λ
					SerialPort.STOPBITS_1, // ֹͣλ
					SerialPort.PARITY_NONE); // У��
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

	@Override
	public void serialEvent(SerialPortEvent event) {
		// TODO Auto-generated method stub
		switch (event.getEventType()) {
		case SerialPortEvent.BI:
		case SerialPortEvent.OE:/* Overrun error����λ���� */
		case SerialPortEvent.FE:/* Framing error����֡���� */
		case SerialPortEvent.PE:/* Parity error��У����� */
		case SerialPortEvent.CD:/* Carrier detect���ز���� */
		case SerialPortEvent.CTS:/* Clear to send��������� */
		case SerialPortEvent.DSR:/* Data set ready�������豸���� */
		case SerialPortEvent.RI:/* Ring indicator������ָʾ */
		case SerialPortEvent.OUTPUT_BUFFER_EMPTY:/* ������������ */
			break;
		case SerialPortEvent.DATA_AVAILABLE:/* �˿��п������ݡ������������飬������ն� */
			byte[] readBuffer = new byte[1024];

			int count = 0;
			try {
				/* ���ض�ȡ���ֽ��� */
				count = inputStream.read(readBuffer);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			/* �ӿ�ʼ����count���ֽ���תΪ�ַ��� */
			String readStr = new String(readBuffer, 0, count);
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
			System.out.println(inputTime[i]);
			i++;
			if(i==3)
			{
				i=0;
				if((inputTime[0]>=24)||(inputTime[1]>=60)||(inputTime[2]>=60))
					System.out.println("������Χ");
				else
				{
					try {
						outputStream.write(inputTime[0]);
						outputStream.write(inputTime[1]);
						outputStream.write(inputTime[2]);
						outputStream.flush();
						outputStream.close();
					} catch(IOException e) {}
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