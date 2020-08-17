
/**
 * Classe de controlo de erros comuns.
 */
public class ErroComum extends Exception
{
    public ErroComum (String msg){
        super(msg);
    }
    
    public ErroComum (){
        super();
    }
}
