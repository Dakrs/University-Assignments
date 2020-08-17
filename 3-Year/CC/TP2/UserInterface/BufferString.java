package UserInterface;


import java.net.*;
import java.util.*;
import java.util.concurrent.locks.*;

public class BufferString {
	Queue <String> buffer;
	Lock l;
	Condition not_empty;

	public BufferString(){
		this.buffer = new ArrayDeque<String>();
		l = new ReentrantLock();
		not_empty = this.l.newCondition();
	}

	public synchronized boolean hasElements(){
		return this.buffer.size() > 0 ? true : false;
	}

	public List<String> takeAll() throws InterruptedException{
		List<String> aux = new ArrayList<>();
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

	public String take() throws InterruptedException{
		String aux;
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

	public void add(String s){
		try{
			l.lock();
			buffer.add(s);
			this.not_empty.signalAll();
		}
		finally{
			l.unlock();
		}
	}

	public void addAll(String [] s){
		try{
			l.lock();
			for(String a : s)
				buffer.add(a);
			this.not_empty.signalAll();
		}
		finally{
			l.unlock();
		}
	}
}
