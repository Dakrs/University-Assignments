package TransferControl;

import java.io.IOException;
import java.util.*;

/**
 * Classe que implementa os round trip times
 */

public class RTT {
    private long estimatedRTT;
    private long devRTT;
    private long timeout;
    /**
     * Flag que identifica a altura da transferência atual, True refere-se a controlo de ligações, False transferência da informação
     */
    private boolean connection_control;
    /**
     * Mapa com os números de sequência dos pacotes enviados em cada janela
     */
    private TreeMap<Long,List<Integer>> packets_sent;

    /**
     * Construtor para a classe RTT
     */

    public RTT() {
        this.estimatedRTT = 500;
        this.connection_control = true;
        this.packets_sent = new TreeMap<>();
    }

    /**
     * Método para atualizar o estimatedRTT sempre que um pacote novo chega
     * @param sequence Número de sequência de um pacote
     * @return
     */

    public boolean refreshRTT(int sequence){
        List<Integer> aux = this.packets_sent.lastEntry().getValue();
        if (aux.contains(sequence)) {
            long last_sent = this.packets_sent.lastKey();
            long now = System.nanoTime() / 1000000;
            this.estimatedRTT = (long) Math.ceil(this.estimatedRTT * 0.875 + 0.125 * (now - last_sent));
            this.devRTT = (long) Math.ceil(0.75 * this.devRTT + 0.25 * Math.abs(now - last_sent - this.estimatedRTT));

            return true;
        }

        return false;
    }

    /**
     * Método para iniciar a contagem de tempo quando é enviada uma janela nova e para estabelecimento de timeouts
     * @param list Números de sequência dos pacotes enviados
     */

    public void start(List<Integer> list){
        this.packets_sent.put(System.nanoTime() / 1000000,list);
        this.timeout = 4 * (this.estimatedRTT + 4*devRTT);

    }

    /**
     * Método para verificar se a janela enviada deu time out.
     * @return Valor booleano do resultado
     */

    public boolean timedOut(){
        try {
            long last_sent = this.packets_sent.lastKey();
            long now = System.nanoTime() / 1000000;
            if (!this.connection_control) {
                if ((now - last_sent) >= timeout) {
                    return true;
                }
                else return false;
            }
            return (now - last_sent) >= 500;
        }
        catch (NoSuchElementException e){
            return false;
        }
    }

    /**
     * Método set para alterar a altura da transferência em que o RTT se encontra
     * @param flag Novo valor para a altura
     */

    public void setConnection_control(boolean flag){
        this.connection_control = flag;
    }
}
