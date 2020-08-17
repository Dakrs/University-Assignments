
import java.util.Comparator;
import java.io.Serializable;


/**
 * Classe comparator para comparar faturas com base no seu valor.
 */
public class CompValor implements Comparator<Fatura>,Serializable
{   
    @Override
    public int compare(Fatura f1,Fatura f2){
         if (f1.getValor() < f2.getValor())
              return 1;
         else if (f1.getValor() == f2.getValor() && f1.equals(f2))
              return 0;
         return -1;
    }
}
