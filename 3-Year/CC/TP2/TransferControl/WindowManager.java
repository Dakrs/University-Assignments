package TransferControl;

/**
 * Classe responsável pela implementação do contro de congestão
 */

public class WindowManager {
    /**
     * Tamanho da janela
     */
    public int size;
    /**
     * Flag para verificar se está em slow start
     */
    public boolean slow_start;

    /**
     * Construtor
     */

    public WindowManager(){
        size = 1;
        this.slow_start = true;
    }

    /**
     * Método para calcula o tamanho da janela
     * @return Tamanho atual da janela
     */

    public int size(){return this.size;}

    /**
     * Método para atualizar a janela quando não foi encontrada congestão
     */

    public void update(){
        if (this.slow_start)
            this.size *= 2;
        else
            this.size++;
    }

    /**
     * Método para atualizar o valor da janela quando é encontrada congestão
     */

    public void congestion(){
        this.size /= 2;
        this.slow_start = false;
        if(this.size <= 0)
            this.size = 1;
    }


}
