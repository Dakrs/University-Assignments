
import java.util.Set;
import java.util.HashSet;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * Classe que extende a classe Empresa para ser possivel a criacao de empresas com previlegios em termos de deducao com base na sua localizacao geografica.
 */
public class EmpInterior extends Empresa implements Serializable
{
    
    private String conselho;
    
    /**
     * Construtor sem argumentos.
     */
    public EmpInterior(){
        super();
        this.conselho="";
    }
    
    /**
     * Construtor com argumentos.
      * @param nif1         Nif a introduzir.
      * @param nemail       Email a introduzir.
      * @param nnome        Nome a introduzir.
      * @param nmorada      Morada a introduzir.
      * @param npassword    Password a introduzir.
      * @param setores      Conjunto de setores.
      * @param ncoeficiente Coeficiente de decução fiscal.
      * @param conselho     Conselho a introdur.
     */
    public EmpInterior(int nif1,String nemail,String nnome,String nmorada,String npassword,Set<String> setores,double ncoeficiente,Set<Integer> faturas,String conselho){
        super(nif1,nemail,nnome,nmorada,npassword,setores,ncoeficiente,faturas);
        this.conselho = conselho;
    }
    
    /**
     * Construtor com outra EmpInterior.
     * @param e     EmpInterior a copir.
     */
    public EmpInterior(EmpInterior e){
        super(e);
        this.conselho = e.getConselho();
    }
    
    /**
     * Método que devolve a variavel conselho da classe.
     * @return Conselho da classe,
     */
    public String getConselho(){
        return this.conselho;
    }
    
     /**
     * Método que altera a variavel conselho da classe.
     * @param s  Conselho a alterar.
     */ 
    public void setConselho(String s){
        this.conselho = s;
    }
    
    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */
    public EmpInterior clone(){
        return new EmpInterior(this);
    }
    
    /**
     * Método toString do objeto.
     * @return objeto em modo string
     */
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        sb.append(super.toString());
        sb.append("Conselho: ");
        sb.append(this.conselho + "\n");
        
        return sb.toString();
    }
    
    /**
     * Método equal do objeto.
     * @param  o    Objeto a comparar
     * @return      Booelan que verifica se o objeto e igual
     */
    public boolean equals(Object o){
        if(this == o)
            return true;
        
        if (o == null || (this.getClass() != o.getClass()))
            return false;
        
        EmpInterior aux = (EmpInterior) o;
        
        return this.conselho.equals(aux.getConselho()) && super.equals(aux);
    }
    
    /**
     * Método hashCode do objeto.
     * @return hascode do objeto
     */
    public int hashCode(){
        int hash = 7;
        hash = 31*hash + this.conselho.hashCode();
        hash = 31*hash + super.hashCode();
        
        return hash;
    }
    
    /**
     * Método que calcula a reducao do imposto da classe 
     * @param  a    Map<Conselho,taxe de reducao de imposto>
     * @return      Reducao do imposto
     */
    public double reducaoImposto(Map<String,Double> a){
        if (a.get(this.conselho.toUpperCase()) == null)
            return 0.0;
        return a.get(this.conselho.toUpperCase());
    }
    
}
