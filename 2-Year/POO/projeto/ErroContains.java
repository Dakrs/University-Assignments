

/**
 * Classe de controlo de erros para quando um nif ja esta inserido na base de dados.
 */
public class ErroContains extends Exception
{
    public ErroContains(String msg){
        super(msg);
    }
}
