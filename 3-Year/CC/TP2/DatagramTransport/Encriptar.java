package DatagramTransport;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import java.security.*;
import javax.crypto.spec.*;
import java.util.Base64;
import java.util.zip.CRC32;
import java.util.zip.Checksum;
import java.nio.Buffer;
import java.nio.ByteBuffer;


/**
 * Classe responsável por assegurar toda a encriptagem de dados assim como a sua correta desencriptacao atraves do uso de checksum
 */
public class Encriptar {

	/**
	 * Funcao que contactena 2 arrays de Bytes
	 * @param a Primeiro array de bytes
	 * @param b Segundo array de bytes
	 * @return Array de bytes concatenados ( a + b )
	 */
    byte[] concatenateByteArrays(byte[] a, byte[] b) {
        byte[] result = new byte[a.length + b.length]; 
        System.arraycopy(a, 0, result, 0, a.length); 
        System.arraycopy(b, 0, result, a.length, b.length); 
        return result;
    }

	/**
	 * Funcao que transforma um long num array de bytes
	 * @param x Long a ser convertido para bytes
	 * @return Array de bytes apos o long ser convertido
	 */
	public byte[] longToBytes(long x) {
        ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
        buffer.putLong(x);
        return buffer.array();
    }

	/**
	 * Funcao que transforma um array de bytes no respetivo long
	 * @param bytes array de bytes para ser transformado num long
	 * @return Long apos o array de bytes ser convertido
	 */
	public long bytesToLong(byte[] bytes) {
    	ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
    	buffer.put(bytes);
    	buffer.flip();//need flip 
    	return buffer.getLong();
	}


	/**
	 * Construtor
	 */
  	public Encriptar()  {
      
   	}


	/**
	 * Funcao que encrypta um pacote de dados(array de bytes) de um dado tamanho fornecido como parametros
	 * e com uma chave tambem fornecida como parametro.
	 * Esta funcao tambem adiciona um long em formato de array de bytes no final da mensagem com o valor da checksum da mensagem encriptada
	 * @param messangeEncriptar Messange a encriptar
	 * @param keyBytes			chave de encriptação
	 * @param length			tamanho do array de bytes
	 * @return array de bytes encriptados concatenado com a sua checksum no final
	 */
  	public byte[] encryptMessage(byte[] messangeEncriptar ,String keyBytes,int length){
  		try {
  		    
    		Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
    		SecretKeySpec secretKey = new SecretKeySpec(keyBytes.getBytes("UTF-8"), "AES");
    		cipher.init(Cipher.ENCRYPT_MODE, secretKey);
    		byte[] encriptado =  cipher.doFinal(messangeEncriptar,0,length);                         //encriptar trama
    		Checksum checksum = new CRC32();
    		checksum.update(encriptado,
    		    0,encriptado.length);				                                        //calcular checksum apos encriptado
    		long checksumValue = checksum.getValue();
    		return this.concatenateByteArrays(encriptado,this.longToBytes(checksumValue));  //inserir checksum na trama
  		}
 	   	catch (Exception e){
        	System.out.println(e);
    	}
    
    	return null;
	}

	/**
	 * Funcao inversa da "encryptMessage"  que desencrypta um pacote de dados(array de bytes) de um dado tamanho fornecido como parametros
	 * e com uma chave tambem fornecida como parametro.
	 * Esta função antes de efetuar qualquer processo de desencryptar a mensagem verifica a checksum separando os ultimos 8 bytes e verificando
	 * se esse valor coincide com o valor que se foi calculado na 1 primeira fase da funcao apos separar os bytes
	 * @param encryptedMessage array de bytes a ser desencriptada
	 * @param keyBytes 		   chave de encriptação
	 * @param length		   tamanho do array de bytes
	 * @return array de bytes desencriptado. Caso a checksum seja invalida retorna nulo.
	 */
	public byte[] decryptMessage(byte[] encryptedMessage, String keyBytes,int length) {
  		try {
  		    int index_checksum = length - 8;
    		byte[] only_checksum = new byte[8];
    		for(int i = index_checksum,j = 0; i < length ; i++, j++){									// retirar do array contatenado a checksum
    		    only_checksum[j] = encryptedMessage[i];																	// long logo 8 bytes
    		}
    		
    		//System.out.println("Checksum apos retirar do array de bytes : " + this.bytesToLong(only_checksum));
    
    
    		byte[] sem_checksum = new byte[index_checksum];
    		for(int i = 0; i < index_checksum; i++){		                //separar os bytes contatenados,    	                                                 
    		    sem_checksum[i] = encryptedMessage[i];		                //separando o encryptado e a checksum
    		}
    

    		Checksum checksum_compare = new CRC32();
    		checksum_compare.update(sem_checksum, 0, sem_checksum.length);				// calcular checksum da array encryptado para verificar se manteve-se
    		long checksum_compare_long = checksum_compare.getValue();					// imutavel com a transferencia de pacotes
 
    		if( this.bytesToLong(only_checksum) != checksum_compare_long) {				// caso a checksum encontre-se diferente houve um erro de transmissao
   		     	System.out.println("Erro-> checksum diferentes");
    		    return null;
    		}
			Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
			SecretKeySpec secretKey = new SecretKeySpec(keyBytes.getBytes("UTF-8"), "AES");     //desencriptar bytes sem checksum
			cipher.init(Cipher.DECRYPT_MODE, secretKey);
			return cipher.doFinal(sem_checksum); // sem checksum

    	}
    	catch (Exception e){
        	System.out.println(e);
    	}
    	return null;
	}
  
}
