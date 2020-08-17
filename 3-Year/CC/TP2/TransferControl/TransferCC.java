package TransferControl;

import DatagramTransport.Buffer;
import DatagramTransport.ReceiverAgentUDP;
import DatagramTransport.SenderAgentUDP;
import UserInterface.BufferString;
import UserInterface.IO;
import TransferControl.Transfer;
import TransferControl.Upload;
import TransferControl.Packet;

import javax.xml.crypto.dsig.TransformService;
import java.net.*;
import java.util.*;
import java.io.*;

class Host {
	private InetAddress ip;
	private int port;

	public Host(InetAddress ip,int port){
		this.ip = ip;
		this.port = port;
	}

	public boolean equals(Host h){
		return h.port == this.port && h.ip.equals(this.ip);
	}

	public String toString(){
		return this.ip.getHostAddress() + ":" + this.port;
	}
}


public class TransferCC implements Runnable {
	Buffer package_received; // Buffer para receber
	Buffer package_tosend; // Buffer para enviar
	BufferString commands_from_user; // Buffer para os comando do utilizador
	Map<String, Transfer> connections; // Map para as transferências (downloads)
	Map<String, Upload> uploads; // Map para os uploadas
	Map<String, List<Host>> peers; // Map para os servidores secundários
	int n_transfers; // Número de transferência feitas
	int port; // Porta
	InetAddress address; // Endereço IP
	IO io ; // Sincronizar com o utilizador



	public int getN_transfers(){return this.n_transfers;} // teste
    public Map<String,Transfer> getConnections(){return this.connections;}
    public Map<String,Upload> getUploads(){return this.uploads;}
	public InetAddress getAddress(){
		return this.address;
	}
	public TransferCC(int port,InetAddress address,BufferString bs, IO io,String path){
		this.package_received = new Buffer();
		this.package_tosend = new Buffer();
		this.commands_from_user = bs;
		this.connections = new TreeMap<>();
		this.uploads = new TreeMap<>();
		this.address = address;
		this.port = port;
		this.n_transfers = 0;
		this.io = io;


		this.peers = parseFile(path,port,address);
		if(!peers.isEmpty())
			for(String file_name : this.peers.keySet()) {
				System.out.println("Ficheiro : " + file_name);
				for (Host peer : this.peers.get(file_name))
					System.out.println(peer.toString());
			}
	}

	/**
	 * Método para retirar dos servidores secundários um servidores
	 * @param ip Ip do servidor
	 * @param port Porta do servidor
	 * @param filename Ficheiro do servidor
	 * @return Lista sem o host
	 */

	public List<Host> filterHost(InetAddress ip, int port,String filename){
		Host h = new Host(ip,port);
		List<Host> fim = new ArrayList<>();

		if (this.peers.containsKey(filename)) {
			List<Host> aux = this.peers.get(filename);
			for (Host a : aux)
				if (!h.equals(aux))
					fim.add(a);
		}

		return fim;
	}

	/**
	 * Método para dar parse à base de dados local
	 * @param path Path para o ficheiro
	 * @param port Porta inserida
	 * @param address Ip
	 * @return Map de hosts
	 */

	public Map<String, List<Host>> parseFile(String path,int port, InetAddress address){
		Map<String,List<Host>> res = new TreeMap<>();
		try {
			String line;
			String [] args;
			List<Host> hosts ;
			RandomAccessFile fd = new RandomAccessFile(path, "r");
			while((line = fd.readLine()) != null){
				hosts = new ArrayList<>();
				args = line.split(";");
				if(res.containsKey(args[0])){
					hosts = res.get(args[0]);
				}
				if(!InetAddress.getByName(args[1]).equals(address) || port != Integer.parseInt(args[2])){
					hosts.add(new Host(InetAddress.getByName(args[1]),Integer.parseInt(args[2])));
					res.put(args[0],hosts);
				}
			}
		} catch (Exception e) {
			System.out.println("BD nao encontrada");
		}
		return res;
	}

	/**
	 * Método principal da TransfereCC. Este método é o responsável por juntar todos os componente e tornar tudo funcional.
	 */

	public void run(){
		try{
			DatagramSocket socket = new DatagramSocket(this.port);
			Thread t1 = new Thread(new SenderAgentUDP(socket,this.package_tosend));
			Thread t2 = new Thread(new ReceiverAgentUDP(socket,this.package_received));
			t1.start();
			t2.start();

			while(true){
				//System.out.println("Phase 1");
				// Interpretar os pacotes que chegaram
				interpret_received_packet();

				//System.out.println("Phase 2");
				//Comando do utilizador

				if (commands_from_user.hasElements()){
					List<String> msg_from_user_list = this.commands_from_user.takeAll();
					for(String s : msg_from_user_list){
						command(s);
					}
				}

				//System.out.println("Phase 3");
				//Uploads
				for(Upload u : uploads.values()){
					if (!u.isClosed()) {
						if (u.isUploader()) {
							if (u.timeOut()) {
								Packet aux = u.nextStep();
								byte[] data = aux.toBytes();
								this.package_tosend.add(new DatagramPacket(data, data.length, u.getDestination(), u.getPort()));
							}
						} else if (u.isAccepted()) {
							Transfer t = new Transfer(u.getId(), u.getDestination(), u.getPort(), u.getFileName(), true);
							this.connections.put(t.getId(), t);
							u.close();
						}
					}
				}

				//Downloads
				for(Transfer t : connections.values()){
					if (!t.isClosed() && t.ableToTransmit() && t.mayTransmit() && !t.isDenied()){
						Packet aux = t.nextStep(filterHost(t.getDestination(),t.getPort(),t.getFile_name()));
						if (aux != null){
							if (aux.getType() == 3)
								this.connections.remove(aux.getId());
							byte [] data = aux.toBytes();
							this.package_tosend.add(new DatagramPacket(data,data.length,t.getDestination(),t.getPort()));
						}
						else{
							if (!t.isFinished()){
								/*
								List<DatagramPacket> pack = t.nextPackages();
								this.package_tosend.addAll(pack);*/
								t.nextPackages(this.package_tosend);
							}
						}
					}
				}
				//enviar o que tiver que ir
				//System.out.println("Going to Sleep");
				//Thread.sleep(1000);
				//System.out.println("Waking up");
			}

		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Método para interpretar um comando do utilizador
	 * @param msg Comando do utilizador
	 * @return
	 */

	private boolean command(String msg){
		String [] args = msg.split(";",2);

		switch(args[0]){
			case "ACCEPT": { accept_connection(msg); break;}
			case "DECLINE": { decline_file(msg); break;}
			case "UP": {up_file(msg); break;}
			case "GET": {get_file(msg); break;}
			case "GETP": {get_file(msg); break;}
			case "LIST": {listKeys(); break;}
			case "ADDALL" : {addAll(); break;}
			case "CANCELALL" : {cancelAll(); break;}
			case "DOWNLOADS": {listDownloads();this.io.offwaiting(); break;}
			case "WAITING": {listToAccept();this.io.offwaiting(); break;}
			default: {System.out.println("Erro"); break;}
			//falta o disconnect total e tratar das excpetions melhor
		}


		boolean result = args[0].equals("EXIT");

		return result;
	}

	/**
	 * Método para aceitar todas as transferências e uploads
	 */

	private void addAll(){
		for(Transfer transferencia : this.getConnections().values()){
			if (!transferencia.changed()){
				transferencia.setAccepted();
			}
		}
		for(Upload uploads : this.getUploads().values()){
			if (!uploads.changed()){
				uploads.setAccepted();
			}
		}
	}

	/**
	 * Método para cancelar todas as transferências
	 */

	private void cancelAll(){
		for(Transfer transferencia : this.getConnections().values()){
			if (!transferencia.changed()){
				transferencia.setDenied();
			}
		}
		for(Upload uploads : this.getUploads().values()){
			if (!uploads.changed()){
				uploads.setDenied();
			}
		}
	}

		private void listToAccept() {
			System.out.println("Menu de Gestao de Transferencia");
			if (!this.getConnections().isEmpty() || !this.getUploads().isEmpty()) {
				System.out.println("---Downloads---");
				System.out.println("NOME DE FICHEIRO    ID DONWLOAD");

				int aux = 0;
				for (TransferControl.Transfer transferencia : this.getConnections().values()) {
					if (!transferencia.isReceiver() && !transferencia.isAccepted()) {
						if (transferencia.isDenied()) {
							System.out.println("*** " + transferencia.getFile_name() + "        " + transferencia.getId() + " RECUSADO");
						}
						else{aux++;
							System.out.println(   transferencia.getFile_name() + "        " + transferencia.getId());
						}
					}
				}
				System.out.println("---Uploads---");
				System.out.println("NOME DE FICHEIRO    ID UPLOAD");
				for (TransferControl.Upload uploads : this.getUploads().values()) {
                    if (!uploads.isReceiver() && !uploads.isAccepted()) {
						if (uploads.isDenied()) {
							System.out.println("*** " + uploads.getFileName() + "        " + uploads.getId() + " RECUSADO");
						}else{aux++;
                        	System.out.println(  uploads.getFileName() + "        " + uploads.getId());
                    	}
                    }
                }
				if (aux == 0)
					System.out.println("Todos os pedidos já aceites/recusados\n Enter para continuar");
				else {
					System.out.println("a para aceitar todos \n c para recusar todos \n x ou nao listado para voltar sem tomar decisoes\n -id -> cancelar ; id -> aceitar");
				}
			}else
			System.out.println("Sem pedidos recebidos\n Clique x para voltar");
		}

	/**
	 * Método para calcular o estado de todas as transferências
	 */

	private void listDownloads(){
		if(!this.getConnections().isEmpty()) {
			for (TransferControl.Transfer transferencia : this.getConnections().values()) {
				float total;
				float atual;
				if (!transferencia.testifRatio()){
					if (transferencia.isReceiver())
							if(transferencia.isDenied())
								System.out.println("***********            " + "RECUSADO " +   "   Download    "  + transferencia.getId() + "    " + transferencia.getFile_name());
							else
								System.out.println("***********            " + "Iniciando  " + "   Download    "  + transferencia.getId() + "    " + transferencia.getFile_name());
					else
							if(transferencia.isDenied())
								System.out.println("***********            " + "RECUSADO " +   "    Upload    "  + transferencia.getId() + "    " + transferencia.getFile_name());
							else
								System.out.println("***********            " + "Iniciando  " + "    Upload    " + transferencia.getId() + "    " + transferencia.getFile_name());

				}else{
					if(transferencia.isDenied())
						System.out.println("***********            " + "CANCELADO  " + "             " + transferencia.getId() + "    " + transferencia.getFile_name());

					else if (!transferencia.isFinished() && transferencia.testifRatio()) {
						total = transferencia.getChunks().length;
						atual = transferencia.getChunks_written();
						float racio = atual / total;
						racio *= 100;
						System.out.print("[");
						int i;
						int racio_p = Math.round(racio) / 10;
						for (i = 0; i < racio_p; i++) {
							System.out.print("*");
						}
						for (; i < 10; i++) {
							System.out.print("-");
						}
						System.out.print("]");
						if (transferencia.isReceiver())
							System.out.println("           " + racio + "%   Downloading    " + transferencia.getId() + "    " + transferencia.getFile_name());
						else
							System.out.println("           " + racio + "%    Uploading    " + transferencia.getId() + "    " + transferencia.getFile_name());

					} else if (!transferencia.testifRatio()) {
						System.out.println("           " + "%   Connecting    " + transferencia.getId() + "    " + transferencia.getFile_name());
					} else {
						System.out.print("[**********");
						System.out.print("]");
						if (transferencia.isReceiver())
							System.out.println("           " + "COMPLETO  " + "   DOWNLOADED     " + transferencia.getId() + "    " + transferencia.getFile_name());
						else
							System.out.println("           " + "COMPLETO  " + "    UPLOADED      " + transferencia.getId() + "    " + transferencia.getFile_name());
					}
				}
			}
		}else{
			System.out.println("Sem transferencias efetuadas");
		}
	}
	private void listKeys(){
		for(String key : this.connections.keySet()){
			System.out.println(key);
		}
	}

	/**
	 * Método para aceitar uma ligação
	 * @param msg
	 */


	private void accept_connection(String msg){
		String [] args = msg.split(";");
		if(!this.connections.containsKey(args[1]) && !this.uploads.containsKey(args[1])){
			System.out.println("ID nao encontrado, impossivel aceitar ligacao");
			return;
		}
		if (this.connections.containsKey(args[1])) {
			Transfer t = this.connections.get(args[1]);
			if(!t.changed())

				t.setAccepted();
		}else{
			Upload t = this.uploads.get(args[1]);
			if(!t.changed())

				t.setAccepted();
		}
	}

	/**
	 * Método para rejeitar uma transferência
	 * @param msg
	 */

	private void decline_file(String msg){
		String [] args = msg.split(";");
		if(!this.connections.containsKey(args[1]) && !this.uploads.containsKey(args[1])){
			System.out.println("ID nao encontrado, impossivel recusasr ligacao");
			return;
		}
		if (this.connections.containsKey(args[1])) {
			Transfer t = this.connections.get(args[1]);
			if(!t.changed())
			t.setDenied();
		}else{
			Upload t = this.uploads.get(args[1]);
			if(!t.changed())
			t.setDenied();
		}
	}

	/**
	 * Método para interpretar um pedido de upload do utilizador
	 * @param msg Pedido
	 */

	private void up_file(String msg){
		String [] args = msg.split(";");
		int port_destination = Integer.parseInt(args[3]);
		try{
			Upload u = new Upload(this.address,InetAddress.getByName(args[2]),port_destination,args[1],this.n_transfers++);
			this.uploads.put(u.getId(),u);
		}
		catch (UnknownHostException e) {
			System.out.println("Host not found");
		}
		catch (IOException e){
			System.out.println("Erro while converting packet to bytes or destination couldn't be reached");
		}
	}

	/**
	 * Método para interpretar um pedido de download do utilizador
	 * @param msg Pedido
	 */

	private void get_file(String msg){
		String [] args = msg.split(";");
		int port_destination = Integer.parseInt(args[3]);
		try{
			Transfer t;
			if(args[0].equals("GETP"))
				t = new Transfer(this.address,InetAddress.getByName(args[2]),port_destination,args[1],true,this.n_transfers++,true);
			else
				t = new Transfer(this.address,InetAddress.getByName(args[2]),port_destination,args[1],true,this.n_transfers++,false);
			this.connections.put(t.getId(),t);
		}
		catch (UnknownHostException e) {
			System.out.println("Host not found");
		}
		catch (IOException e){
			System.out.println("Erro while converting packet to bytes or destination couldn't be reached");
		}
	}

	/**
	 * Método para interpretar todos os pacotes
	 */

	private void interpret_received_packet(){
		List<DatagramPacket> packetlist;

		try{
			if (this.package_received.hasElements()){
				packetlist = this.package_received.takeAll();
				//System.out.println("Recebi: " + packetlist.size());
				for(DatagramPacket p : packetlist){
					try{
						Packet arrived = new Packet(p.getData());
					//	System.out.println("Recebi: -> " + arrived.toString());
						packet_dealer(arrived,p);
					}
					catch (IOException a){
						System.out.println("Error converting bytes to object");
					}
				}
			}
		}
		catch(InterruptedException i){
			System.out.println("Thread interrupted error");
		}
	}

	/**
	 * Método para interpretar um pacote
	 * @param p Pacote recebido
	 * @param a Pacote UDP
	 */

	private void packet_dealer(Packet p,DatagramPacket a){
		int i = p.getType();
		try{
			switch (i) {
				case 1:{
					String msg = new String(p.getData(), "UTF-8");
					String [] args = msg.split(";");
					if (args[0].equals("GET") || args[0].equals("GETP")) {
						if (!this.connections.containsKey(p.getId())) {
							try {
								Transfer t;
								if (args.length == 5){
									int [] aux2 = new int[2];
									aux2[0] = Integer.parseInt(args[3]);
									aux2[1] = Integer.parseInt(args[4]);
									t = new Transfer(p.getId(),a.getAddress(),a.getPort(), args[2],aux2[0],aux2[1]);
								}
								else {
									if (args[0].equals("GETP") && this.peers.containsKey(args[2])) {
										t = new Transfer(p.getId(), a.getAddress(), a.getPort(), args[2], 1 + filterHost(a.getAddress(),a.getPort(),args[2]).size());
									} else t = new Transfer(p.getId(), a.getAddress(), a.getPort(), args[2]);
								}
								this.connections.put(t.getId(), t);

								if (this.uploads.containsKey(t.getId())){
									t.setAccepted();
									this.uploads.get(t.getId()).close();
                                    System.out.println("UPLOAD:Ligacao Aceitada : " + t.getId() + " " + t.getFile_name());
								}
								else{
									System.out.println("Ligacao por Aceitar : " + t.getId() + " " + t.getFile_name() );
								}

							} catch (IOException e) {
								e.printStackTrace();
								System.out.println("Error at RTT or file not found");
								Packet aux = new Packet(p.getIdPacket(), 3, p.getId(), -1, "DECLINED".getBytes());
								byte[] info = aux.toBytes();
								this.package_tosend.add(new DatagramPacket(info, info.length, a.getAddress(), a.getPort()));
							}
						}else {
							Transfer t = this.connections.get(p.getId());
							if (t.isDenied()) {
								Packet aux = new Packet(p.getIdPacket(), 3, p.getId(), -1, "DECLINED".getBytes());
								byte[] info = aux.toBytes();
								this.package_tosend.add(new DatagramPacket(info, info.length, a.getAddress(), a.getPort()));
							}
						}
					}
					else if (args[0].equals("UP")){
						if (!this.connections.containsKey(p.getId()) && !this.uploads.containsKey(p.getId())){
							Upload u = new Upload(p.getId(),a.getAddress(),a.getPort(),args[2]);
							this.uploads.put(p.getId(),u);
							System.out.println("Ligacao por Aceitar : " + u.getId() + " " + u.getFileName() );
						}
					}
					break;
				}
				case 2:{ // aceitar a conecção
					String msg = new String(p.getData(), "UTF-8");
					String [] args = msg.split(";");
					if(this.connections.containsKey(p.getId())){
						Transfer t = this.connections.get(p.getId());
						if (t.getDestination().equals(a.getAddress()) && t.getPort() == a.getPort()){
							if (!t.isPrint_aceitar()) {
								if (!this.uploads.containsKey(p.getId()))
								    System.out.println("DOWNLOAD:Ligacao Aceitada : " + t.getId() + " " + t.getFile_name());
								t.setPrint_aceitar();
							}

							if (args.length > 2) {
								int chunks = Integer.parseInt(args[1]);
								int size   = Integer.parseInt(args[2]);
								boolean flag = t.connectedDownload(chunks, size);

								if (args.length > 3 && flag){
									int [] aux2 = new int [3];
									String [] args2;
									for(int l = 3; l < args.length; l++) {
										args2 = args[l].split(":");
										aux2[0] = Integer.parseInt(args2[1]);
										aux2[1] = Integer.parseInt(args2[2]);
										aux2[2] = Integer.parseInt(args2[3]);

										if (l == 3)
											t.setP2P(0,aux2[1]);

										Transfer taux = new Transfer(this.address,InetAddress.getByName(args2[0]),aux2[0],t.getFile_name(),true,this.n_transfers++,t.getFilemanager(),aux2[1],aux2[2]);
										this.connections.put(taux.getId(),taux);
									}

								}
							}
							else if (args.length == 1){
								t.connectedDownload();
							}
							t.update_Packet_id(p.getIdPacket());
							t.packet_RTT_received(p.getIdPacket());
						}
					}
					break;
				}
				case 3:{// recusar
					if(this.connections.containsKey(p.getId())){
						Transfer t = this.connections.get(p.getId());
						if (t.getDestination().equals(a.getAddress()) && t.getPort() == a.getPort()){
							if (!t.isPrint_aceitar()) {
								System.out.println("Ligacao Recusada : " + t.getId() + " " + t.getFile_name());
								t.setDenied();
								t.setPrint_aceitar();
							}
							if (!t.changed())
							t.setDenied();


						}
					}
					break;
				}
				case 4:{
					if(this.connections.containsKey(p.getId())){
						Transfer t = this.connections.get(p.getId());
						if (t.getDestination().equals(a.getAddress()) && t.getPort() == a.getPort()){
							if (!t.isFirstPackage())
								t.gotFirstPackage();

							t.addChunk(p.getChunk(),p.getData());
							t.packet_RTT_received(p.getIdPacket());

							Packet ack = new Packet(p.getIdPacket(),5,p.getId(),p.getChunk(),"CHUNK".getBytes());
							byte[] info = ack.toBytes();
							this.package_tosend.add(new DatagramPacket(info,info.length,t.getDestination(),t.getPort()));
						}
					}
					break;
				}
				case 5:{
					String msg = new String(p.getData(), "UTF-8");
					String [] args = msg.split(";");
					if(this.connections.containsKey(p.getId())){
						Transfer t = this.connections.get(p.getId());
						if (msg.equals("START;")){
							if (t.getDestination().equals(a.getAddress()) && t.getPort() == a.getPort()){
								t.finishSync();
							}
							t.update_Packet_id(p.getIdPacket());


						}
						else if (msg.equals("END;")){
							t.setclosed(true);
						}
						else if (msg.equals("CHUNK")){
							t.addChunk(p.getChunk());
						}
						t.packet_RTT_received(p.getIdPacket());
					}
					break;
				}
				case 6:{
					String msg = new String(p.getData(), "UTF-8");
					String [] args = msg.split(";");
					if(this.connections.containsKey(p.getId())){
						Transfer t = this.connections.get(p.getId());
						if (msg.equals("END;")){
							if (t.getDestination().equals(a.getAddress()) && t.getPort() == a.getPort()){
								t.update_Packet_id(p.getIdPacket());
								t.packet_RTT_received(p.getIdPacket());
								t.setack_finished(true);
								if (!t.isReceiver()){
									t.setclosed(true);

									Packet aux = new Packet(t.getNext_packet_id(),5,t.getId(),-1,msg.getBytes());
									byte[] info = aux.toBytes();
									this.package_tosend.add(new DatagramPacket(info,info.length,t.getDestination(),t.getPort()));
								}
								else{
									t.writeFile();
								}
							}
						}
					}
					break;
				}
				default:{
				//	System.out.println("Recebi ->" + p.toString()); break;
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
}
