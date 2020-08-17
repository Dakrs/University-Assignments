
import java.util.Set;
import java.util.HashSet;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.io.*;

/**
 * Classe que extende a classe CIndividual para ser possivel a criacao de contribuintes com familias numerosas.
 */
public class FamiliaNum extends CIndividual implements Serializable
{
    /**
     * Contrutor sem argumentos da classe FamiliaNum.
     */
    public FamiliaNum(){
        super();
    }
    
    /**
      * Construtor com argumentos.
      * @param nif1         Nif a introduzir.
      * @param nemail       Email a introduzir.
      * @param nnome        Nome a introduzir.
      * @param nmorada      Morada a introduzir.
      * @param npassword    Password a introduzir.
      * @param nn_agregado  Número de elementos no agregado.
      * @param agregados    Conjunto com os nifs dos agredados.
      * @param ncoeficiente   Coeficiente de dedução.
      * @param nsetores     Conjunto de setores em que pode haver descontos.
      * @param faturas      Conjunto de faturas.
    */
    public FamiliaNum(int nif1,String nemail,String nnome,String nmorada,String npassword,int nn_agregado,Set<Integer> agregados,double ncoeficiente,Set<String> nsetores,Set<Integer> faturas){
        super(nif1,nemail,nnome,nmorada,npassword,nn_agregado,agregados,ncoeficiente,nsetores,faturas);
    }
    
    /**
      * Construtor da classe FamliaNum com o mesmo objeto.
      * @param a  FamliaNum a copiar.
    */
    public FamiliaNum(FamiliaNum a){
        super(a);
    }
    
    /**
      * Construtor da classe FamliaNum com um objeto CIndividual.
      * @param a  FamliaNum a copiar.
    */
    
    public FamiliaNum(CIndividual a){
        super(a);
    }
    
    /**
     * Método que produz um clone do objeto.
     * @return clone do objeto
     */
    public FamiliaNum clone(){
        return new FamiliaNum(this);
    }
    
    /**
     * Método toString.
     * @return objeto em modo string
     */
    public String toString(){
        return super.toString();
    }
    
    /**
     * Método equals da classe FamiliaNum
     * @param   Objeto a comparar.
     * @return  Booelan que representa se o objeto e igual 
     */
    public boolean equals(Object o){
        if(this == o)
            return true;
        
        if (o == null || (this.getClass() != o.getClass()))
            return false;
        
        FamiliaNum aux = (FamiliaNum) o;
        
        return super.equals(aux);
    }
    
    /**
     * Método hashCode da classe FamiliaNum
     * @return hashcode do objeto
     */
    public int hashCode(){
        return super.hashCode();
    }
    
    /**
     * Método de reducao de imposto da classe FamliaNum
     * @return bonificacao do objeto
     */
    public double reducaoImposto(){
        return 0.05*getNumAgregado();
    }
}
