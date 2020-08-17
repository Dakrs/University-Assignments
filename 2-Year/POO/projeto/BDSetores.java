import java.util.Map;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;
import java.util.Objects;
import java.util.stream.Collectors;
import java.io.*;
import java.util.Scanner;
import java.util.InputMismatchException;

/**
 * Classe que funciona como uma base de dados onde se encontram os dados acerca dos setores.
 */
public class BDSetores implements Serializable {
    private Map<String, Setor> setores;
    private Set<String> por_aprovar;
    
    /**
     * Construtor sem argumentos.
     */
    public BDSetores() {
        this.setores = new HashMap<String, Setor>();
        this.por_aprovar = new HashSet<>();
    }
    
    /**
      * Construtor com argumentos.
      * @param set        Map de setores a introduzir.
    */
    public BDSetores(Map<String,Setor> set) {
        this.setores = set.entrySet().stream()
                                     .collect(Collectors.toMap(entry->entry.getKey().toUpperCase(),entry->entry.getValue().clone()));
        this.por_aprovar = new HashSet<>();
    }
    
    /**
     * Construtor com outra BDSetores.
     * @param bdSetores    BDSetores a copiar.
     */
    public BDSetores(BDSetores bdSetores){
        this.setores = bdSetores.getSetores();
        this.por_aprovar = bdSetores.getAprov();
    }

    /**
      * Get para a variável setores do objeto.
      * @return  Setores (Map<String,Setor>) do objeto
    */
    public Map<String, Setor> getSetores(){
        return this.setores.entrySet().stream()
                                      .collect(Collectors.toMap(entry->entry.getKey().toUpperCase(),entry->entry.getValue().clone()));
    }
    
    /**
       * Método set da variável setores.
       * @param setores  Valor a introduzir.
    */
    public void setSetores(Map<String, Setor> setores) {
        this.setores = new HashMap<String, Setor>();
        setores.entrySet().forEach(entry-> this.setores.put(entry.getValue().getNome().toUpperCase(), entry.getValue().clone()));
    }
    
    /**
      * Get dos setores aprovados do objeto.
      * @return  Setores aprovados (Map<String,Setor>) do objeto.
    */    
    public Set<String> getAprov(){
        return this.por_aprovar.stream().collect(Collectors.toSet());
    }
    
    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */
    public BDSetores clone(){
        return new BDSetores(this);
    }
    
    /**
      * Metodo que adiciona um setor ao objeto.
      * @param s     Setor a adicionar.
    */  
    public void addSetor(Setor s){
        this.setores.put(s.getNome().toUpperCase(), s.clone());
        this.por_aprovar.add(s.getNome().toUpperCase());
    }
    
    /**
      * Metodo que verifica se um dado setor existe.
      * @param s     Setor a verificar.
      * @return      Boolean que retorna se existe o setor.
    */  
    public boolean existeSetor(String s){
        return this.setores.containsKey(s.toUpperCase());
    }
    
    /**
     * Método equal do objeto.
     * @param  o    Objeto a comparar.
     * @return      Booelan que verifica se o objeto e igual
     */
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        BDSetores bdSetores = (BDSetores) o;
        return this.setores.equals(bdSetores.setores);
    }
    
    /**
     * Método toString do objeto.
     * @return Objeto em modo string
     */
    public String toString() {
        StringBuilder sb = new StringBuilder();
        
        for(Setor s : this.setores.values())
            sb.append(s.toString() + "\n");
            
        sb.append("Setores por aprovar: ");
        for(String a : this.por_aprovar){
            sb.append(a + ": ");
        }
        sb.append("\n");

        return sb.toString();
    }
    
    /**
     * Método que aprova os setores ainda nao aprovados, da throw a AdminAprov no caso de nao haver mais setores para aprovar.
     */
    public void admin_aprov() throws AdminAprov{
        if (this.por_aprovar.size() == 0)
            throw new AdminAprov();
            
        Scanner in = new Scanner(System.in);
        System.out.println("Aprovar Setores");
        
        double aux = -1;
        for(String a : this.por_aprovar){
            System.out.println("Setor: " + a + " Valor:");
            while(aux == -1){
                try{
                    aux = in.nextDouble();
                }
                catch (InputMismatchException e){
                    System.out.println("Valor inválido");
                    in.next();
                }
                if (aux < 0 || aux > 0.23){
                    aux = -1;
                    System.out.println("Valor inválido. Por favor insira um valor entre 0 e 0,23");
                }
                else{
                    Setor s = this.setores.get(a);
                    s.setTaxa(aux);
                }
            }
            aux = -1;
        }
        this.por_aprovar = new HashSet<>();
    }
    
    /**
     * Método que retorna a bonificao de um dado setor.
     * @param  s        Setor a verificar.
     * @return          Bonificacao
     */
    public double getBonificacao(String s){
        if (!this.setores.containsKey(s.toUpperCase()))
            return 0;
        Setor a = this.setores.get(s.toUpperCase());
        return a.getTaxa();
    }
}
