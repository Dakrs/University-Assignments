
/**
 * Classe de controlo de erros para quando nao e encontrado um dado nif.
 */

public class ErroNotFound extends Exception
{
    public ErroNotFound(String msg){
        super(msg);
    }
}
