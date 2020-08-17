package li3;

import engine.TCDCommunity;

/**
 * Classe com o simples intuito de por em execução a nossa implementação do modelo MVC
 */
public class MVC {

    /**
     * Método main que cria um model, uma view, e depois os utiliza para construir um controller
     */
    public static  void main(String[] args){
        View view = new View();
        TCDCommunity model = new TCDCommunity();
        Controller controller = new Controller(model, view);
    }


}
