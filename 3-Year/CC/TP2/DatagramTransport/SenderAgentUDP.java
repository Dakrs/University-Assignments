package DatagramTransport;

import java.net.*;
import java.io.*;

/**
 * Classe responsável por enviar pacotes UDP para outros clientes
 */

public class SenderAgentUDP implements Runnable {
	private Buffer buffer;
	private DatagramSocket socket;

	/**
	 * Construtor parameterizado
	 * @param socket Socket por onde serão enviados os pacotes
	 * @param b Buffer com os pacotes a enviar
	 */

	public SenderAgentUDP(DatagramSocket socket, Buffer b){
		this.buffer = b;
		this.socket = socket;
	}


	/**
	 * Método que a thread responsável por este objeto utilizará para saber o que fazer. Enviar pacotes
	 */

	public void run(){
		try{
			while(true){
				DatagramPacket p = this.buffer.take();
				//System.out.println(new String(p.getData()));
				Encriptar crypto = new Encriptar();
				byte[] aux = crypto.encryptMessage(p.getData(),"chavebonitabytes",p.getLength());
				p.setData(aux);

				this.socket.send(p);
			}
		}
		catch (IOException e) {
			System.out.println("Erro no sender");
		}
		catch (InterruptedException a){
			System.out.println("Erro na buffer");
		}
	}

}
