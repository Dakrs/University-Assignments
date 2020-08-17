package DatagramTransport;

import java.net.*;
import java.io.*;

/**
 * Classe responsável por receber pacotes UDP provenientes de outros clientes
 */

public class ReceiverAgentUDP implements Runnable {
	private Buffer buffer;
	private DatagramSocket socket;


	/**
	 * Construtor parameterizado
	 * @param socket Socket onde serão recebidos os pacotes
	 * @param b Buffer onde serão colocados os pacotes recebidos
	 */

	public ReceiverAgentUDP(DatagramSocket socket, Buffer b){
		this.buffer = b;
		this.socket = socket;
	}

	/**
	 * Método que a thread responsável por este objeto utilizará para saber o que fazer. Receber pacotes
	 */

	public void run(){
		try{
			while(true){
				byte[] receiving = new byte[(4*1024)+200];
				DatagramPacket packet = new DatagramPacket(receiving,receiving.length);
				socket.receive(packet);
				Encriptar crypto = new Encriptar();
				byte[] aux = packet.getData();
				byte[] aux2 = crypto.decryptMessage(aux,"chavebonitabytes",packet.getLength());
				if(aux2 != null){
					packet.setData(aux2);
					buffer.add(packet);
				}
			}
		}
		catch (IOException e) {
			System.out.println("Erro no receiver");
		}
	}

}
