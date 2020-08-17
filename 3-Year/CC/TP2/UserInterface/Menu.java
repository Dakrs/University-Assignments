package UserInterface;

import UserInterface.BufferString;
import UserInterface.IO;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.file.FileSystems;
import java.util.Scanner;


/**
 * Classe responsável pela interaceo do Utilizador com o programa
 */

public class Menu {
    int menu_type;
    BufferString buffer;
    String file_name;
    String host_ip;
    String host_port;
    String msg;
    Scanner is = new Scanner(System.in);
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    private boolean enviarmsg;
    String id_transfer;
    private  boolean aTransferir;
    private IO io ;

    /**
     * Construtor da classe
     * @param buffer Buffer partilhado para comunicacao entre esta camada e a camada de gestao do programa
     * @param aux    Classe partilhada que gere a sicronizacao entre o Input e o Ouput das camadas
     */
    public Menu(BufferString buffer, IO aux){
        this.buffer = buffer;
        this.io = aux;
    }

    /**
     * Funcao que pede ao utilizador um IP
     * @return String com o input dado pelo utilizador
     */
    public String getIP(){
        String ip = null;
        boolean teste = true;
        while (teste) {
            try {
                io.println("Insira o IP");
                ip = io.getString();
                teste = false;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return ip;
    }

    /**
     * Funcao que pede a porta do utilizador
     * @return Retorna a porta dada pelo utilizador
     */
    public int getPort(){
        int port = -1;
        boolean teste = true;
        while (teste) {
            try {
                io.println("Insira a Porta");
                String msg = is.nextLine();
                port = Integer.parseInt(msg);
                teste = false;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return port;
    }

    /**
     * Funcao que imprime o menu
     */
    public void imprime_menutype() throws Exception {
        switch (this.menu_type){
            case (0):{
                this.io.println("---------Menu Inicial--------");
                this.io.println("1 -> Descarregar um Ficheiro");
                this.io.println("2 -> Dar upload a um Ficheiro");
                this.io.println("3 -> Estado das Transferencias");
                this.io.println("4 -> Aceitar/Recusar Conecoes");
                this.io.println("5 -> Sair");
                break;
            }
            case (1):{
                this.io.println("---------Menu de Download--------");
                this.io.println("Nome do Ficheiro");
                this.file_name =io.getString();
                this.io.println("IP do Host");
                this.host_ip =io.getString();
                this.io.println("Port do Host");
                this.host_port =io.getString();
                this.io.println("Deseja tentar download p2p?\nS/s para aceitar, qualquer outra tecla para cancelar");
                this.msg =io.getString();
                if(this.msg.equals("S") || this.msg.equals("s")) {
                    this.msg = ("GETP;" + this.file_name + ";" + this.host_ip + ";" + this.host_port);
                }
                else
                    this.msg = ("GET;" + this.file_name +";"+ this.host_ip +";" + this.host_port) ;
                this.enviarmsg=true;
                this.menu_type = 5;


                break;
                //cenas descargar
            }
            case (2):{
                this.io.println("---------Menu de Upload--------");
                this.io.println("Nome do Ficheiro");
                this.file_name =io.getString();
                this.io.println("IP do Host");
                this.host_ip =io.getString();
                this.io.println("Port do Host");
                this.host_port =io.getString();
                this.msg = ("UP;" + this.file_name +";"+ this.host_ip +";" + this.host_port) ;
                this.enviarmsg=true;
                this.menu_type = 5;
                break;
                //cenas descargar
            }
            case (3):{
                this.msg =("DOWNLOADS");
                this.buffer.add(this.msg);
                this.io.waitresponse();
                this.menu_type = 5;
                break;
            }
            case (4):{
                this.msg =("WAITING");
                this.buffer.add(this.msg);
                this.io.waitresponse();
                this.menu_type = 6;
                break;
            }
            default :{
                this.io.println("Erro, Falha no comando imprime menu");
                this.menu_type = 0;
                break;
            }
        }
    }

    /**
     * Funcao que altera o menu conforme o input do utilizador no metodo anterior
     */
    public void altera_menu() throws Exception {
        switch (this.menu_type){
            case (0):{
                int res = io.getInt();
                if (res == 1){
                    this.menu_type = 1;
                    break;
                }else if (res == 2){
                    this.menu_type = 2;
                    break;
                }else if (res == 3){
                    this.menu_type = 3;
                    break;
                }else if (res == 4){
                    this.menu_type = 4;
                    break;
                }else if (res == 5){
                    this.menu_type = -1;
                    break;
                }


            }
            case (2):{break;}
            case (5):{
                this.menu_type = 0;
                break;
            }
            case ( 6):{
                String waiting_ler = io.getString();
                if (waiting_ler.equals("a"))
                    this.buffer.add("ADDALL;");
                else if(waiting_ler.equals("c"))
                    this.buffer.add("CANCELALL;");
                else if(waiting_ler.equals("x"))
                    ;
                else if(waiting_ler.equals(""))
                    ;

                else {
                    if(waiting_ler.startsWith("-")){
                        this.buffer.add("DECLINE;" + waiting_ler.substring(1) + ";");
                        System.out.println("teste decline");
                        System.out.println(waiting_ler.substring(1));
                    }else
                        this.buffer.add("ACCEPT;" + waiting_ler +";");
                }
                this.menu_type = 0;

                break;
            }
            default :{
                this.io.println("Erro, Falha no comando altera menu");
                break;
            }
        }
    }

    /**
     * Funcao principal responsavel por imprimir o menu e gerir os inputs do utilizador
     */
    public void menu(){

            boolean sair = true;

            while(sair){
                try{
                    this.imprime_menutype();
                    this.altera_menu();
                    if (this.menu_type == -1)
                        sair = false;
                    if (this.menu_type == -2) {
                        this.io.println("Erro no input");
                        this.menu_type = 0;
                    }
                    if (this.enviarmsg) {
                        this.enviarmsg = false;
                        this.buffer.add(this.msg);
                    }
                }catch (Exception e){
                    System.out.println(e);
                }

        }
    }

    /**
     * Funcao que pede ao utilizador o path para a BD local onde contem os ips e os ficheiros que este contem
     * (Apenas relevante para funcinalidades P2P)
     * @return Path para a BD Local
     */
    public String getPath(){
        io.println("Path da BD");
        String msg = null;
        try {
            msg = io.getString();
        }
        catch (Exception e){;}
        return msg;
    }


}

