package TransferControl;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.InetAddress;
import java.util.GregorianCalendar;

/**
 * Classe que implementa a parte inicial de um upload. A restante é implementada recorrendo a um download
 */

public class Upload implements Comparable<Upload> {
    private String id;
    private InetAddress destination;
    private int port;
    private String file_name;
    private boolean receiver;
    private boolean accepted;
    private boolean print_aceitar;
    private boolean denied = false;
    private long last_sent;
    private boolean alterado = false;
    private boolean closed = false;

    /**
     * Construtor para a pessoa que vai fazer o upload
     * @param origin
     * @param destination
     * @param port
     * @param file_name
     * @param n_transfers
     * @throws FileNotFoundException
     */

    public Upload (InetAddress origin,InetAddress destination, int port, String file_name, int n_transfers) throws FileNotFoundException {
        this.destination = destination;
        this.port = port;
        this.id = origin.getHostAddress() + "-" + n_transfers + "-" + port;
        this.file_name = file_name;
        this.receiver = true;
        this.last_sent = -1;

        File f = new File(file_name);
        if(!f.isFile()) {
            throw new FileNotFoundException();
        }
    }

    /**
     * Construtor para a pessoa que vai receber o upload
     * @param id
     * @param destination
     * @param port
     * @param file_name
     */


    public Upload(String id,InetAddress destination, int port, String file_name){
        this.destination = destination;
        this.id = id;
        this.port = port;
        this.file_name = file_name;
        this.receiver = false;
        this.accepted = false; // tem que estar false;
    }

    /**
     * Método para verificar se vai enviar
     * @return
     */

    public boolean isUploader(){
        return this.receiver;
    }

    /**
     * Método para comparar dois uploadas
     * @param o
     * @return
     */

    public int compareTo(Upload o){
        return this.id.compareTo(o.getId());
    }

    /**
     * Método que calcula o próximo pacote
     * @return
     */

    public Packet nextStep(){
        StringBuilder s = new StringBuilder();
        Packet p;

        s.append("UP;");
        s.append(this.id+";");
        s.append(this.file_name+";");
        String msg = s.toString();
        p = new Packet(-1,1,this.id,-1,msg.getBytes());
        this.last_sent = System.nanoTime()/1000000;

        return p;
    }

    public boolean timeOut(){
        if (last_sent == -1)
            return true;

        long now = System.nanoTime()/1000000;

        return (now - last_sent) >= 500;
    }


    public void setAccepted(){
        this.accepted = true;
        this.alterado = true;
    }


    public void setDenied() {
        this.denied = true;
        this.alterado = true;
    }
    public boolean isDenied() {
        return denied;
    }
    public boolean changed() {
        return alterado;
    }


    public InetAddress getDestination(){
        return this.destination;
    }

    public int getPort(){
        return this.port;
    }

    public boolean isAccepted() {
        return accepted;
    }

    public String getId(){
        return this.id;
    }

    public String getFileName(){
        return this.file_name;
    }

    public boolean isReceiver(){
        return this.receiver;
    }

    public boolean isPrint_aceitar() {
        return print_aceitar;
    }

    public void setPrint_aceitar() {
        this.print_aceitar = true;
    }

    public void close(){
        this.closed = true;
    }

    public boolean isClosed(){
        return this.closed;
    }
}
