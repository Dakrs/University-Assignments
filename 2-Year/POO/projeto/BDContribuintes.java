
import java.util.Set;
import java.util.Map;
import java.util.HashSet;
import java.util.TreeSet;
import java.util.HashMap;
import java.util.TreeMap;
import java.util.List;
import java.util.LinkedList;
import java.util.stream.Collectors;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Iterator;
import java.io.Serializable;

/**
 * Classe que funciona como uma base de dados onde se encontram os dados acerca dos contribuintes.
 */
public class BDContribuintes implements Serializable
{
    private Map<Integer,Contribuinte> dados;
    
    
    /**
     * Contrutor sem argumentos da classe BDContribuintes.
     */
    public BDContribuintes (){
        this.dados = new HashMap<Integer,Contribuinte>();
    }
    
    /**
      * Construtor com argumentos.
      * @param a            Map de contribuintes a introduzir.
    */
    public BDContribuintes(Map<Integer,Contribuinte> a){
        setDados(a);
    }
    
    /**
      * Construtor da classe BDContribuintes com argumentos.
      * @param a  BDContribuintes a copiar.
    */
    public BDContribuintes(BDContribuintes a){
        this.dados = a.getDados();
    }
    
    
    /**
     * Método que devolve a base de dados de contribuinte.
     * @return Base de dados de contribuinte
     */
    public Map<Integer,Contribuinte> getDados(){
        return this.dados.entrySet()
                         .stream()
                         .collect(Collectors.toMap((e)->e.getKey(),
                                                   (e)->e.getValue().clone()));
    }
    
    /**
     * Método que altera a base de dados de contribuinte.
     * @param a     Valor a inserir.
     */ 
    public void setDados(Map<Integer,Contribuinte> a){
        this.dados = new HashMap <Integer,Contribuinte>();
        a.values().stream().forEach(e -> this.dados.put(e.getNif(),e.clone()));
    }
    
    /**
     * Método que clona este objeto.
     * @return Clone do objeto
     */
    public BDContribuintes clone(){
        return new BDContribuintes(this);
    }
    
    /**
     * Método toString do objeto.
     * @return Objeto em modo string
     */
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        for(Contribuinte f : this.dados.values())
            sb.append(f.toString() + "\n");
            
        return sb.toString();
    }
    
    /**
     * Método equal do objeto.
     * @param  object   Objeto a comparar
     * @return          Booelan que verifica se o objeto e igual
     */
    public boolean equals(Object object){
        if (this == object) return true;
        if (object == null || getClass() != object.getClass()) return false;
        
        BDContribuintes aux = (BDContribuintes) object;
        Map<Integer,Contribuinte> aux1 = aux.getDados();
        
        return aux1.equals(this.dados);
    }
    
    /**
     * Método que devolve o contribuinte dado um nif, da throw a erro caso nao encontre, da throw a ErroNotFound em caso de falha.
     * @param nif   Nif a procurar
     * @return Coeficiente de deducao fiscal
     */
    public Contribuinte getContribuinte(int nif) throws ErroNotFound{
        Contribuinte aux = this.dados.get(nif);
        
        if (aux == null){
            Integer i = new Integer(nif);
            throw new ErroNotFound(i.toString());
        }
        return aux.clone();
    }
    
    /**
     * Método que adiciona um contribuinte na base de dados de contribuintes, da throw a ErroNotFound em caso de falha.
     * @param o    Contribuinte a adicionar
     */
    public void addContribuinte(Contribuinte o) throws ErroNotFound{
        Integer i = new Integer(o.getNif());
        
        if (this.dados.containsKey(o.getNif()))
            throw new ErroNotFound(i.toString());
            
        this.dados.put(o.getNif(),o.clone());
    }
    
    /**
     * Método que verifica se contem um dado nif numa base de dados.
     * @param nif   Nif a procurar
     * @return Noolean que representa se o nif encontra-se na base de dados.
     */
    public boolean contains(int nif){
        Contribuinte a = this.dados.get(nif);
        
        if (a == null)
            return false;
            
        return true;
    }
    
    /**
     * Método que altera a fatura com base num id.
     * @param id    Id da fatura 
     * @param nif   Nif do contribuinte
     */
    public void setFaturaId(int id,int nif){
        Contribuinte a = this.dados.get(nif);
        a.setFatura(id);

    }
    
    /**
     * Método que devolve o um map de <Nif,Faturas desse nif> inseridas na base de dados.
     * @return map de <Nif,Faturas desse Nif>
     */
    public Map<Integer,Set<Integer>> getFaturasIds(){
        Map<Integer,Set<Integer>> aux = new HashMap<>();

        this.dados.forEach((n,i) -> aux.put(n,i.getFaturas()));

        return aux;
    }
    
    /**
     * Método que adiciona um setor a um dado contribuinte, da throw a ErroNotFound em caso de falha.
     * @param s         Setor a adicionar
     * @param nif       Nif do contribuinte
     */
    public void addSetor(String s, int nif) throws ErroNotFound{
        Contribuinte aux = this.dados.get(nif);
        Integer i = new Integer(nif);
        if (aux == null)
            throw new ErroNotFound(i.toString());
        aux.addSetor(s);
    }
    
    /**
     * Método que adiciona um agregado a um dado contribuinte da throw a ErroNtFound/Erros em caso de falha.
     * @param nif_agregado  Agregado a adicionar
     * @param nif           Nif do contribuinte
     */
    public void addAgregado(int nif, int nif_agregado) throws ErroNotFound,Erros{
        Contribuinte aux = this.dados.get(nif);
        Integer i = new Integer(nif);
        FamiliaNum nova;
        int r1,r2 = 0;
        if (aux == null)
            throw new ErroNotFound(i.toString());
            
        if (aux.getClass().getSimpleName().equals("CIndividual") || aux.getClass().getSimpleName().equals("FamiliaNum")){
            CIndividual aux2 = (CIndividual) aux;
            r1 = aux2.getNumAgregado();
            aux2.addAgregado(nif_agregado);
            r2 = aux2.getNumAgregado();
            if (r1 != r2 && r1 == 3){
                nova = new FamiliaNum(aux2);
                this.dados.put(nova.getNif(),nova);
            }
            if (r1 == r2)
                throw new Erros();
        }
        
    }
    
    
    
    
}
