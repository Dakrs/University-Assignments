package UserInterface;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Scanner;

/**
 * Classe partilhada que gere a sicronizacao entre o Input e o Ouput das camadas
 */

public class IO {

    private boolean waiting_response = false;
    Scanner is = new Scanner(System.in);
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

    /**
     * Construtor da classe
     */
    public IO() {
    }

    /**
     * Metodo que imprime uma string com um /n no fim
     * @param a String a imprimir
     */
    public synchronized void println(String a){
        System.out.println(a);
    }

    /**
     * Metodo que imprime uma string
     */
    public synchronized void print(String a){
        System.out.print(a);
    }

    /**
     * Metodo que le um inteiro do input do utilizador
     * @return Inteiro Lido pelo input
     * @throws Exception Erro ao ler o inteiro(Formato invalido,...)
     */
    public synchronized int getInt() throws  Exception{

        String msg = is.nextLine();
        return Integer.parseInt(msg);
    }

    /**
     * Meodo que le uma String o input do utilizador
     * @return String liga do input
     * @throws Exception Erro ao ler a String
     */
    public synchronized String getString() throws Exception{
        return in.readLine();
    }

    /**
     * Funcao que permite desbloqueia a classe caso esta esteja bloqueada
     */
    public synchronized void offwaiting(){
        this.waiting_response = false;
        notifyAll();
    }

    /**
     * Funcao que permite bloquear a classe de maneira a ser possivel a espera pelo output de outras camadas
     * @throws InterruptedException
     */
    public synchronized  void waitresponse() throws InterruptedException{
        this.waiting_response = true;
        while(waiting_response)
            wait();
    }
}
