package DatagramTransport;

import java.net.*;
import java.util.*;
import java.util.concurrent.locks.*;

/**
 * Classe responsável por implementar uma buffer concorrente para pacotes UDP
 */

public class Buffer {
	Queue <DatagramPacket> buffer;
	Lock l;
	Condition not_empty;

	/**
	 * Construtor sem argumentos
	 */

	public Buffer(){
		this.buffer = new ArrayDeque<DatagramPacket>();
		l = new ReentrantLock();
		not_empty = this.l.newCondition();
	}

	/**
	 * Métoodo para verificar se exister pacotes na buffer
	 * @return Valor booleano da verificação
	 */

	public synchronized boolean hasElements(){
		return this.buffer.size() > 0 ? true : false;
	}

	/**
	 * Método para retirar e colocar numa lista todos os Pacotes existente na buffer
	 * @return Lista com pacotes UDP
	 * @throws InterruptedException
	 */

	public List<DatagramPacket> takeAll() throws InterruptedException{
		List<DatagramPacket> aux = new ArrayList<>();
		try{
			l.lock();
			while(buffer.size() == 0)
				not_empty.await();
			while(buffer.size() > 0){
				aux.add(buffer.poll());
			}
		}
		finally{
			l.unlock();
		}
		return aux;
	}

	/**
	 * Método para retirar um Pacote existente na buffer
	 * @return Pacote retirado
	 * @throws InterruptedException
	 */

	public DatagramPacket take() throws InterruptedException{
		DatagramPacket aux;
		try{
			l.lock();
			while(buffer.size() == 0)
				not_empty.await();
			aux = buffer.poll();
		}
		finally{
			l.unlock();
		}
		return aux;
	}

	/**
	 * Método para adicionar uma pacote à buffer
	 * @param s Pacote a adicionar
	 */

	public void add(DatagramPacket s){
		try{
			l.lock();
			buffer.add(s);
			this.not_empty.signalAll();
		}
		finally{
			l.unlock();
		}
	}

	/**
	 * Método para adicionar vários pacote à buffer
	 * @param s Lista com os pacotes a enviar
	 */

	public void addAll(List<DatagramPacket> s){
		try{
			l.lock();
			for(DatagramPacket a : s)
				buffer.add(a);
			this.not_empty.signalAll();
		}
		finally{
			l.unlock();
		}
	}
}
