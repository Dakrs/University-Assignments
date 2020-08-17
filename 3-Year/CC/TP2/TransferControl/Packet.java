package TransferControl;

import java.io.*;
import java.lang.StringBuilder;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;

/**
 * Classe que implementa o nosso cabeçalho
 */

public class Packet implements Serializable {
	/**
	 * Tipo do pacote
	 */
	private int type; // 1 estabelecer a ligaçao | 2 aceitar a ligação | 3 rejeitar a ligação | 4 transferencia de data | 5 ACK para a transferencia | 6 Termino de ligação
	/**
	 * Identificador da transferência (chave IP,nºtranferenca,porta)
	 */
	private String transfer_id;
	/**
	 * Número da partição que transporta
	 */
	private int chunk;
	/**
	 * Bytes do ficheiro
	 */
	private byte [] data;
	/**
	 * Identificador para o pacote
	 */
	private int id;

	//4-type 4-chunk 4-id

	/**
	 * Construtor parameterizado para um pacote
	 * @param id Número de sequência do pacote
	 * @param type Tipo de pacote
	 * @param transfer_id Chave da tranferência
	 * @param chunk Número da partição que transporta
	 * @param data Byte a transportar
	 */

	Packet(int id, int type, String transfer_id, int chunk,byte[]data){
		this.id = id;
		this.type = type;
		this.transfer_id = transfer_id;
		this.chunk = chunk;
		this.data = data;
	}

	/**
	 * Construtor que pega num pacote em byte e transforma num objeto Packet
	 * @param array Array de bytes
	 * @throws UnknownHostException
	 */


	Packet(byte[] array) throws UnknownHostException{
		String key;

		byte [] b = new byte[4];
		System.arraycopy(array,0,b,0,4);
		key = InetAddress.getByAddress(b).getHostAddress();

		key += "-" + ByteBuffer.wrap(array,4,4).getInt();
		key += "-" + ByteBuffer.wrap(array,8,4).getInt();

		this.transfer_id = key;
		this.id = ByteBuffer.wrap(array,12,4).getInt();
		this.type = ByteBuffer.wrap(array,16,4).getInt();
		this.chunk = ByteBuffer.wrap(array,20,4).getInt();

		byte [] data = new byte[array.length-24];

		System.arraycopy(array,24,data,0,array.length-24);

		this.data = data;

	}

	/**
	 * Método para converter um objeto Packet em bytes.
	 * @return
	 * @throws IOException
	 */

	byte[] toBytes() throws IOException{

		String [] aux = this.transfer_id.split("-");
		byte[] address = InetAddress.getByName(aux[0]).getAddress(); //4
		byte[] n_transferencia = intToBytes(Integer.parseInt(aux[1])); //4
		byte[] porta = intToBytes(Integer.parseInt(aux[2])); //4
		byte[] id = intToBytes(this.id); //4
		byte[] type = intToBytes(this.type); //4
		byte[] chunk = intToBytes(this.chunk); //4

		byte [] buffer = new byte[4*6 + this.data.length];


		System.arraycopy(address,0,buffer,0,4);
		System.arraycopy(n_transferencia,0,buffer,4,4);
		System.arraycopy(porta,0,buffer,8,4);
		System.arraycopy(id,0,buffer,12,4);
		System.arraycopy(type,0,buffer,16,4);
		System.arraycopy(chunk,0,buffer,20,4);
		System.arraycopy(data,0,buffer,24,data.length);

		return buffer;
	}

	/**
	 * Método para converter um inteiro para bytes
	 * @param i Inteiro a converter
	 * @return Array de bytes com o inteiro convertido
	 */

	private byte[] intToBytes( final int i ) {
		ByteBuffer bb = ByteBuffer.allocate(4);
		bb.putInt(i);
		return bb.array();
	}

	/**
	 * Método toString para a classe Packet
	 * @return String do pacote
	 */

	public String toString(){
		StringBuilder s = new StringBuilder();

		s.append("ID: ");
		s.append(this.id);
		s.append("; ");
		s.append(transfer_id );
		s.append("; ");
		s.append(type);
		s.append("; ");
		s.append(chunk );
		s.append("; ");
		return s.toString();
	}

	/**
	 * Método get para o tipo do pacote
	 * @return Tipo do pacote
	 */

	int getType(){
		return this.type;
	}

	/**
	 * Método get para a chave da transferência a que o pacote diz respeito
	 * @return Tipo do pacote
	 */

	String getId(){
		return this.transfer_id;
	}

	/**
	 * Método get para o número da partição que o pacote transporta
	 * @return Número da partição
	 */

	int getChunk(){
		return this.chunk;
	}

	/**
	 * Método get para data do pacote
	 * @return Array de bytes para a data contina no pacote
	 */

	byte [] getData(){
		return this.data;
	}

	/**
	 * Método get para o número de sequência do pacote
	 * @return Número de sequência
	 */

	int getIdPacket(){
		return this.id;
	}
}
