import java.util.Set;
import java.util.Map;
import java.util.HashSet;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;
import java.time.LocalDate;
import java.io.*;
import java.util.Scanner;

/**
 * Classe que funciona como uma base de dados onde se encontram os dados acerca das faturas.
 */
public class BDFaturas implements Serializable
{
    private Map<Integer,Fatura> faturas;
    private Set<Integer> faturas_porval;
    
    /**
     * Construtor sem argumentos.
     */
    public BDFaturas (){
        this.faturas = new HashMap<Integer,Fatura>();
        this.faturas_porval = new HashSet<Integer>();
    }
    
    /**
      * Construtor com argumentos.
      * @param a        Map de faturas a introduzir.
    */
    public BDFaturas(Map<Integer,Fatura> a){
        setFaturas(a);
        this.faturas_porval = new HashSet<Integer>();
    }
    
    /**
     * Construtor com outra BDFaturas.
     * @param a    Base de dados de faturas a copiar.
     */
    public BDFaturas(BDFaturas a){
        this.faturas_porval = a.getFaturas_porval();
        this.faturas = a.getFaturas();
    }
        
    /**
      * Get para a variável faturas do objeto.
      * @return  Faturas (Map<Integer,Fatura>) do objeto
    */
    public Map<Integer,Fatura> getFaturas(){
        return this.faturas.values()
                           .stream()
                           .collect(Collectors.toMap((e)->e.getId(),
                                                     (e)->e.clone()));
    }
    
    /**
       * Método set da variável faturas.
       * @param a       Valor a introduzir.
    */
    public void setFaturas(Map<Integer,Fatura> a){
        this.faturas = new HashMap <Integer,Fatura>();
        a.values().stream().forEach(e -> this.faturas.put(e.getId(),e.clone()));
    }
    
    /**
      * Get para a variável faturas_porval do objeto.
      * @return  Faturas por validar (Set<Integer>) do objeto
    */
    public Set<Integer> getFaturas_porval(){
        Set<Integer> aux = new HashSet<Integer>();
        this.faturas_porval.forEach( a -> aux.add(a));
        
        return aux;
    }
    
    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */
    public BDFaturas clone(){
        return new BDFaturas(this);
    }
    
    /**
     * Método toString do objeto.
     * @return objeto em modo string
     */
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        for(Fatura f : this.faturas.values())
            sb.append(f.toString() + "\n");
            
        sb.append("Por validar: ");
        for(Integer i : this.faturas_porval)
            sb.append(i + ": ");
            
        sb.append("\n");
            
        return sb.toString();
    }
    
    /**
     * Método equal do objeto.
     * @param object Objeto a comparar
     * @return       Booelan que verifica se o objeto e igual
     */
    public boolean equals(Object object){
        if (this == object) return true;
        if (object == null || getClass() != object.getClass()) return false;
        
        BDFaturas aux = (BDFaturas) object;
        Map<Integer,Fatura> aux1 = aux.getFaturas();
        
        return aux1.equals(this.faturas);
    }

    /**
     * Método que adiciona uma fatura ao objeto.
     * param a  Fatura a adicionar.
     * param i  Base de dados dos contribuintes individuais.
     * param e  Base de dados dos contribuintes empresas.
     * param d  Base de dados dos setores.
     */    
    public void addFatura(Fatura a, BDContribuintes i, BDContribuintes e, BDSetores d) throws ErroNotFound{
        Empresa aux;
        CIndividual aux2;
           
            
        try {
             aux = (Empresa) e.getContribuinte(a.getNif_emitente());
             aux2 = (CIndividual) i.getContribuinte(a.getNif_cliente());
        }
        catch (ErroNotFound l){
            throw l;
        }
            
        i.setFaturaId(a.getId(),a.getNif_cliente());
        e.setFaturaId(a.getId(),a.getNif_emitente());
            
        if (aux.getSetores().size() > 1){
            this.faturas_porval.add(a.getId());
            
        }
        else if (aux.getSetores().size()==1){
            if(!d.existeSetor(a.getCategoria()))
                d.addSetor(new Setor(a.getCategoria(), 0));
        }
            
        this.faturas.put(a.getId(),a.clone());
    }

    /**
     * Metodo que retorna uma fatura de um dado id, caso nao encontra efetua throw ErrorNotFound
     * @param   id  Id a efetuar procura.
     * @return      Fatura do id.
     */
    public Fatura getFatura(int id) throws ErroNotFound {
        Fatura a;

        a = this.faturas.get(id);
        Integer i = new Integer(id);

        if (a == null)
            throw new ErroNotFound (i.toString());

        return a;
    }
    
    /**
     * Metodo que retorna uma lista de faturas de um dado id e numa dada data.
     * @param   start   Id a efetuar procura.
     * @param   end     Id a efetuar procura.
     * @param   idlist  Id a efetuar procura.
     * @return          Lista das faturas.
     */
    public List<Fatura> faturas_no_intervalo(LocalDate start ,LocalDate end, Set<Integer> idlist){
        return this.faturas.values().stream()
                                    .filter(b -> idlist.contains(b.getId()) 
                                    && (start.compareTo(b.getEmissao()) <= 0)
                                    && (end.compareTo(b.getEmissao()) >= 0))
                                    .map( b -> b.clone())
                                    .collect(Collectors.toList());
    }
    
    
    /**
     * Metodo que retorna uma lista de faturas dos ids dados.
     * @param   islist  Ids para efetuar procura.
     * @return          Lista das faturas.
     */
    public List<Fatura> faturas_contribuinte(Set<Integer> idlist){
        return this.faturas.values().stream()
                                    .filter( b -> idlist.contains(b.getId()))
                                    .map( b -> b.clone())
                                    .collect(Collectors.toList());
    }
    
    /**
     * Metodo que verifica se existe uma data fatura esta validada.
     * @param id    Fatura a verificar.
     * @return      Boolean que representa se a fatura foi ou nao validada.
     */
    public boolean check_val_fatura(int id){
        if (this.faturas_porval.contains(id))
            return false;
        return true;
    }
    
    /**
     * Metodo que valida uma data fatura,da throw a ErrorNotFound caso nao seja encontrada a fatura e FaturaVal caso a fatura esteja valida.
     * @param id        Fatura a validar.
     * @param setor     Setor da fatura.
     * @param set       Base de dados dos setores.
     */
    public void valida_fatura(int id,String setor,BDSetores set) throws ErroNotFound,FaturaVal{
        Fatura a = this.faturas.get(id);
        Integer i = new Integer(id);
        if (a == null)
            throw new ErroNotFound(i.toString());
        if (!this.faturas_porval.contains(id))
            throw new FaturaVal(i.toString());
            
        a.setCategoria(setor);
        this.faturas_porval.remove(id);
        
        if(!set.existeSetor(a.getCategoria()))
                set.addSetor(new Setor(a.getCategoria(), 0));
    }
}
