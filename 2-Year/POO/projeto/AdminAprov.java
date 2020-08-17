
/**
 * Classe de controlo de erros para opcoes de admin.
 */

public class AdminAprov extends Exception
{
    public AdminAprov(String msg){
        super(msg);
    }
    
    public AdminAprov(){
        super();
    }
}
