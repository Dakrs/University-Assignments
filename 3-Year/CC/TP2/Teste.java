import TransferControl.TransferCC;
import UserInterface.BufferString;
import UserInterface.Menu;
import UserInterface.IO;

import java.net.*;
import java.util.*;
import java.io.*;

public  class Teste {
	public static void main(String[] args) throws Exception {
		IO io = new IO();
		Scanner is = new Scanner(System.in);
		BufferString bs = new BufferString();
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		Menu menu = new Menu(bs,io);
		TransferCC aux = new TransferCC(menu.getPort(),InetAddress.getByName(menu.getIP()),bs,io,menu.getPath());
		new Thread(aux).start();
		menu.menu();
		System.exit(0);
	}

}
