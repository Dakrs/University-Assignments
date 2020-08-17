package TransferControl;

import DatagramTransport.Buffer;

import java.net.*;
import java.sql.SQLOutput;
import java.util.*;
import java.io.*;

public class Transfer implements Comparable<Transfer> {
	private String id;
	private InetAddress destination;
	private int port;
	private final String file_name;
	private final boolean receiver;//sou eu que recebo?

	private FileManager filemanager;
	private boolean p2p; // flag para multi servidor
	private int [] limits; // limites caso o ficheiro venha de vários sítios

	private boolean [] flags; //0 primeiro pacote de dados (firstpackage) //1 ligacao estabelecida (estabilished) //2 ack_finished //3 close
	private RTT roundTripTime; // Controlo do round trip times e dos timeout
	private WindowManager windowman; // controlo de consgestão
	private boolean accepted; // foi aceite a transferências?
	private int next_packet_id; // id do próximo pacote
	private int number_packet_to_receive; // pacotes da ultima janela por receber
	private boolean print_aceitar;
	private boolean alterado = false;


	private boolean denied = false;
	private boolean mayP2p;


	/**
	 * Construtor para iniciar uma transferência por parte da pessoa que recebe
	 * @param origin
	 * @param destination
	 * @param port
	 * @param file_name
	 * @param receiver
	 * @param n_transfers
	 * @param p2p
	 * @throws IOException
	 */
	public Transfer (InetAddress origin,InetAddress destination, int port, String file_name, boolean receiver, int n_transfers,boolean p2p) throws IOException{
		this.destination = destination;
		this.port = port;
		this.id = origin.getHostAddress() + "-" + n_transfers + "-"+ port;
		this.file_name = file_name;
		this.receiver = receiver;
		this.filemanager = new FileManager(file_name);
		this.flags = new boolean[4];
		this.roundTripTime = new RTT();
		this.accepted = true;
		this.next_packet_id = 0;
		this.windowman = new WindowManager();
		this.mayP2p = p2p;
	}

	/**
	 * Construtor para um transferência proveniente de um upload
	 * @param id
	 * @param destination
	 * @param port
	 * @param file_name
	 * @param receiver
	 * @throws IOException
	 */
	public Transfer (String id,InetAddress destination, int port, String file_name, boolean receiver) throws IOException{
		this.destination = destination;
		this.port = port;
		this.id = id;
		this.file_name = file_name;
		this.receiver = receiver;
		this.filemanager = new FileManager(file_name);
		this.flags = new boolean[4];
		this.roundTripTime = new RTT();
		this.accepted = true;
		this.next_packet_id = 0;
		this.windowman = new WindowManager();

	}

	/**
	 * Construtor para um transferência normal, entidade que envia
	 * @param id
	 * @param destination
	 * @param port
	 * @param file_name
	 * @throws IOException
	 */
	public Transfer(String id,InetAddress destination,int port,String file_name) throws IOException{
		this.id = id;
		this.destination = destination;
		this.port = port;
		this.file_name = file_name;
		this.receiver = false;
		this.filemanager = new FileManager(calculate_chunks(),file_name);
		this.flags = new boolean[4];
		this.roundTripTime = new RTT();
		this.accepted = false; // isto tem que estar a false, está a true para não dar engodos a testar.
		this.next_packet_id = 1;
		this.windowman = new WindowManager();

		File f = new File(file_name);
		if(!f.isFile()) {
			throw new FileNotFoundException();
		}
	}

	/**
	 * Construtor para o servidor primário em multiservidor
	 * @param id
	 * @param destination
	 * @param port
	 * @param file_name
	 * @param number_of_servers
	 * @throws IOException
	 */
	public Transfer(String id,InetAddress destination,int port,String file_name,int number_of_servers) throws IOException{
		this.id = id;
		this.destination = destination;
		this.port = port;
		this.file_name = file_name;
		this.receiver = false;
		int chunks = calculate_chunks();
		this.filemanager = new FileManager(chunks,file_name);
		this.flags = new boolean[4];
		this.roundTripTime = new RTT();
		this.accepted = false; // isto tem que estar a false, está a true para não dar engodos a testar.
		this.next_packet_id = 1;
		this.windowman = new WindowManager();

		this.p2p = true;
		this.limits = new int[4];
		this.limits[0] = number_of_servers;
		this.limits[1] = 0;
		this.limits[2] = chunks / number_of_servers;
		this.limits[3] = 0;

		//System.out.println("0: " + this.limits[0] + " 1:" + this.limits[1] + " 2:" + this.limits[2] + " 3:" + this.limits[3]);


		File f = new File(file_name);
		if(!f.isFile()) {
			throw new FileNotFoundException();
		}
	}

	/**
	 * Contrutor para transferência de servidores secundários Lado de quem receber
	 * @param origin
	 * @param destination
	 * @param port
	 * @param file_name
	 * @param receiver
	 * @param n_transfers
	 * @param fs
	 * @param lowerlimit
	 * @param upperlimit
	 * @throws IOException
	 */
	public Transfer (InetAddress origin,InetAddress destination, int port, String file_name, boolean receiver, int n_transfers,FileManager fs,int lowerlimit,int upperlimit) throws IOException{
		this.destination = destination;
		this.port = port;
		this.id = origin.getHostAddress() + "-" + n_transfers + "-"+ port;
		this.file_name = file_name;
		this.receiver = receiver;
		this.filemanager = fs;
		this.flags = new boolean[4];
		this.roundTripTime = new RTT();
		this.accepted = true;
		this.next_packet_id = 0;
		this.windowman = new WindowManager();

		this.p2p = true;
		this.limits = new int[4];
		this.limits[0] = 0;
		this.limits[1] = lowerlimit;
		this.limits[2] = upperlimit;
		this.limits[3] = 0;
	}

	/**
	 * Construtor para transferências de servidores secundários Lado de quem envia
	 * @param id
	 * @param destination
	 * @param port
	 * @param file_name
	 * @param lowerlimit
	 * @param upperlimit
	 * @throws IOException
	 */

	public Transfer(String id,InetAddress destination,int port,String file_name,int lowerlimit,int upperlimit) throws IOException{
		this.id = id;
		this.destination = destination;
		this.port = port;
		this.file_name = file_name;
		this.receiver = false;
		int chunks = calculate_chunks();
		this.filemanager = new FileManager(chunks,file_name);
		this.flags = new boolean[4];
		this.roundTripTime = new RTT();
		this.accepted = true; // isto tem que estar a false, está a true para não dar engodos a testar.
		this.next_packet_id = 1;
		this.windowman = new WindowManager();

		this.p2p = true;
		this.limits = new int[4];
		this.limits[0] = 0;
		this.limits[1] = lowerlimit;
		this.limits[2] = upperlimit;
		this.limits[3] = 0;

        System.out.println("0: " + this.limits[0] + " 1:" + this.limits[1] + " 2:" + this.limits[2] + " 3:" + this.limits[3]);



        File f = new File(file_name);
		if(!f.isFile()) {
			throw new FileNotFoundException();
		}
	}


	/**
	 * Método para aceitar uma ligação do lado de quem receber
	 * @param chunks Número de partições
	 * @param l_size Tamanho da última partição
	 * @return Valor lógico, flag já estava a true
	 */

	public boolean connectedDownload(int chunks,int l_size){
		if (this.flags[1] == true)
			return false;

		this.flags[1] = true;
		if (!this.p2p)
			this.filemanager.start(chunks,l_size);
		return true;
	}

	/**
	 * Método para aceitar uma ligação do lado de quem envia
	 */

	public void connectedDownload(){
		this.flags[1] = true;
	}

	/**
	 * Método para verificar se já é possível calcular a percentagem de uma transferência
	 * @return Valor lógico da verificação
	 */

	public boolean testifRatio(){
		if (this.receiver && flags[1])
			return true;
		if (!this.receiver)
			return true;
		return false;
	}

	/**
	 * Método para fazer as alterações de um ACK de data do lado de quem envia
	 * @param chunk
	 */

	public void addChunk(int chunk){
		if (this.filemanager.addChunk(chunk)){
			this.windowman.congestion();
		}
		else if (this.p2p){
			this.limits[3]++;
		}
	}

	/**
	 * Método para atualizar o ficheiro quando recebido um pacote de dados Lado de quem recebe
	 * @param chunk Número de Partição
	 * @param info Bytes contidos no pacote
	 */

	public void addChunk(int chunk,byte[] info){
		if(this.filemanager.addChunk(chunk,info) && this.p2p)
			this.limits[3]++;
	}

	public boolean isPrint_aceitar() {
		return print_aceitar;
	}

	public void setPrint_aceitar() {
		this.print_aceitar = true;
	}

	/**
	 * Método get para o id da transferência
	 * @return Id da transferência
	 */

	public String getId(){
		return this.id;
	}

	/**
	 * Método get para a porta da transferência
	 * @return Porta
	 */

	public int getPort(){
		return this.port;
	}

	/**
	 * Método get para o destino da transferência
	 * @return Destino da transferência
	 */

	public InetAddress getDestination(){
		return this.destination;
	}

	/**
	 * Método para comparar duas tranferências
	 * @param o Transferência a comparar
	 * @return Resulta da comparação
	 */

	public int compareTo(Transfer o){
			return this.id.compareTo(o.getId());
	}

	/**
	 * Método para mudar a flag do primeiro pacote
	 */

	public void gotFirstPackage(){
		this.flags[0] = true;
	}

	/**
	 * Método para verificar se já recebeu o primeiro pacote de dados
	 */

	public boolean isFirstPackage(){
		return this.flags[0];
	}

	/**
	 * Método para finalizar a sincronização no inicio da ligação
	 */

	public void finishSync(){
		this.flags[1] = true;
	}

	/**
	 * Método que gera os pacotes durante o controlo de ligação
	 * @param hosts Lista de servidores secundários
	 * @return Próximo pacote
	 */

	public Packet nextStep(List<Host> hosts){
	    Packet result = null;
		StringBuilder s = new StringBuilder();
		if (!this.flags[1] && receiver){
			if (mayP2p)
				s.append("GETP;");
			else
				s.append("GET;");
			s.append(this.id+";");
			s.append(this.file_name+";");
			if (this.p2p){
			    s.append(limits[1] + ";");
			    s.append(limits[2] + ";");
            }
			String msg = s.toString();
			result = new Packet(this.next_packet_id,1,this.id,-1,msg.getBytes());
		}
		else if (!this.flags[1] && !receiver){
			int nchunks = this.calculate_chunks();
		//	System.out.println("N-Chunks:" + nchunks);
			/*
			if (nchunks == 0)
				return new Packet(3,this.id,-1,new byte[1]);*/
			s.append("SYNREC;");
			if (!this.p2p) {
				s.append(nchunks + ";");
				s.append(this.specific_range_of_bytes(nchunks - 1).length + ";");
			}
			else if (this.limits[0] != 0){
				s.append(nchunks + ";");
				s.append(this.specific_range_of_bytes(nchunks - 1).length + ";");

				int i = 1;
				int each = this.limits[2];
				for(Host h : hosts){
					s.append(h.toString()+":");
					s.append((i*each) + ":");

					if ((i+2)*each > nchunks){
						s.append((i+1)*each + (nchunks % this.limits[0]));
					}
					else s.append((i+1)*each);


					i++;
					s.append(";");
				}
			}

			String msg = s.toString();
            result = new Packet(this.next_packet_id,2,this.id,-1,msg.getBytes());
		}
		else if (this.flags[1] && receiver && !this.flags[0]){
			String msg = "START;";
            result = new Packet(this.next_packet_id,5,this.id,-1,msg.getBytes());
		}
		else if ((!receiver && this.isFinished() && !this.flags[2]) || (receiver && this.flags[2])){
			String msg = "END;";
            result = new Packet(this.next_packet_id,6,this.id,-1,msg.getBytes());
            this.roundTripTime.setConnection_control(true);
		}

		if (result != null){
			this.roundTripTime.start(Collections.singletonList(result.getIdPacket()+1));
			this.number_packet_to_receive = 1;
		}

		return result;
	}

	/**
	 * Método para calcular o número de partições de um ficheiro
	 * @return Número de partições
	 */

	private int calculate_chunks(){
		File f = new File(this.file_name);
		int num = 0;
		if (f != null){
		 num = (int) f.length() / (4*1024);
		 if (((int) f.length() % (4*1024)) != 0)
		 	num++;
	 	}
		return num;
	}

	/**
	 * Método para dada um partição calcular os bytes para a mesma
	 * @param chunk Número da partição
	 * @return Bytes da partição
	 */

	private byte [] specific_range_of_bytes(int chunk){
		int lower_limit = (4*1024)*chunk;
		int upper_limit = (4*1024)*(chunk+1);
		byte[] buffer = null;

		try{
			RandomAccessFile f = new RandomAccessFile(this.file_name,"r");
			if (lower_limit > f.length())
				return null;
			if (upper_limit > f.length()){
				upper_limit = (int) f.length();
			}
			buffer = new byte[upper_limit-lower_limit];
			f.seek(lower_limit);
			f.read(buffer,0,upper_limit-lower_limit);
			f.close();
		}
		catch (IOException e) {
			buffer = null;
			System.out.println("IOException at specific_range_of_bytes");
		}

		return buffer;
	}

	/**
	 * Método para verificar se uma transferência já enviou ou recebeu todas as partições
	 * @return Valor lógico da verificação
	 */

	public boolean isFinished(){
		if (!this.p2p)
			return this.filemanager.isFinished();

		return (limits[2]-limits[1]) == limits[3];
	}


	/**
	 * Método que calcula os próximos pacote durante a transferência de dados
	 * @param b Buffer para enviar os pacotes
	 */

	public void nextPackages(Buffer b){
		int i = 0;
		int packets_send = 0;
		if (!this.receiver){
			this.roundTripTime.setConnection_control(false);
			List<Integer> rtt_packets = new ArrayList<>();
			List<Integer> n_packets = new ArrayList<>();
			int j = this.p2p ? this.limits[1] : 0; // 			int j = 0;
			int window = this.windowman.size();
			int n_chunks = this.p2p ? this.limits[2] : this.filemanager.getChunksLenght(); // asdasd
			while(i < window && j < n_chunks){
				if (!this.filemanager.checkChunk(j)){
					i++;
					n_packets.add(j);
				}
				j++;
			}
			for(int k : n_packets){
				byte[] buffer = specific_range_of_bytes(k);
				if (buffer == null)
					break;
				Packet p = new Packet(this.next_packet_id++,4,this.id,k,buffer);
				try{
					buffer = p.toBytes();
					b.add(new DatagramPacket(buffer,buffer.length,this.destination,this.port));
					packets_send++;
					rtt_packets.add(p.getIdPacket());
				}
				catch (IOException e) {
					System.out.println("Error while  converting a packet to bytes at next5packages");
				}
			}
			if (rtt_packets.size() > 0){
				this.roundTripTime.start(rtt_packets);
			}
		}
		this.number_packet_to_receive = packets_send;
	}

	/**
	 * Método para alterar a flag ao fechar a conexão
	 * @param state
	 */

	public void setack_finished(boolean state){
		this.flags[2] = state;
	}

	/**
	 * Método que altera o estado de uma ligação
	 * @param state Valor novo para o estado
	 */

	public void setclosed(boolean state){
		this.flags[3] = state;
	}

	/**
	 * Método para verificar se a entidade está a receber
	 * @return Valor lógico
	 */

	public boolean isReceiver(){
		return this.receiver;
	}

	/**
	 * Método para verificar se a ligação foi aceite
	 * @return Valor lógico
	 */

	public boolean isAccepted(){ return this.accepted;}

	/**
	 * Método para verificar se a ligação está fechada
	 * @return Valor lógico
	 */

	public boolean isClosed(){
		return this.flags[3];
	}

	/**
	 * Método para verificar se é possível transmitir consuante o estado da ligação
	 * @return
	 */

	public boolean ableToTransmit(){
		if (!receiver && this.accepted) return true;
		if (receiver) return true;

		return false;
	}

	/**
	 * Método para fechar a escrita num ficheiro
	 */

	public void writeFile(){
		if (this.filemanager.isFinished())
			this.filemanager.writeFile();
	}

	/**
	 * Método para aceitar a ligação
	 */

	public void setAccepted(){
		this.accepted = true;
		this.alterado = true;
	}

	/**
	 * Método para rejeita a ligação
	 */


	public void setDenied() {
		this.denied = true;
		this.alterado = true;
	}

	/**
	 * Método para verificar se a ligação foi recusada
	 * @return
	 */

	public boolean isDenied() {
		return denied;
	}
	public boolean changed() {
		return alterado;
	}

	/**
	 * Método para atualizar o id dos pacotes
	 * @param i Id do pacote que chegou
	 */

	public void update_Packet_id(int i){
		if (i > this.next_packet_id){
			this.next_packet_id = i+1;
		}
	}

	/**
	 * Método para calular o valor do próximo id
	 * @return Valor do próximo id
	 */

	public int getNext_packet_id(){return this.next_packet_id;}

	/**
	 * Método para diminui os pacotes à espera após a chegada de um ACK e para dar refresh no RTT
	 * @param id Id do pacote que chegou
	 */

	public void packet_RTT_received(int id){
		if(this.roundTripTime.refreshRTT(id))
		    this.number_packet_to_receive--;
	}

	/**
	 * Método para verficiar se uma transferência pode transmitir. Verifica se já chegaram todos os pacotes ou se deu timeout
	 * @return
	 */

	public boolean mayTransmit(){
	    if (this.number_packet_to_receive == 0) {
			if (!this.receiver && flags[1] )
				this.windowman.update();
			return true;
		}
	    if  (this.roundTripTime.timedOut()){
	    	//System.out.println(this.number_packet_to_receive);
	    	this.windowman.congestion();
	    	return true;
		}
	    return false;
    }

	/**
	 * Método para calcular o estado das partições do ficheiro
	 * @return
	 */

	public boolean[] getChunks() {
		return this.filemanager.getChunks();
	}

	/**
	 * Método para calcula as partições confirmadas
	 * @return
	 */

	public int getChunks_written() {
		return this.filemanager.getChunks_written();
	}

	/**
	 * Método get para o nome do ficheiro
	 * @return
	 */

	public String getFile_name(){return  this.file_name;}

	/**
	 * Método get para o gestor de ficheiros. Usado no multi-servidor
	 * @return
	 */

	public FileManager getFilemanager(){
		return this.filemanager;
	}

	/**
	 * Método para transformar a transferência numa multiservidor
	 * @param lower Partição inferior
	 * @param upper Partição superior
	 */

	public void setP2P(int lower,int upper){
		this.limits = new int [4];
		this.limits[1] = lower;
		this.limits[2] = upper;
		this.limits[3] = 0;

		this.p2p = true;
	}



}
