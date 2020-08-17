    
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.util.HashSet;
import java.util.TreeSet;
import java.util.Set;
import java.io.*;


/**
 * Classe Contribuinte abstrata usada para tornar as informacao das empresas e dos contribuintes individuais compativeis.
 */
public abstract class Contribuinte implements Serializable
{
    private int nif;
    private String email;
    private String nome;
    private String morada;
    private String password;
    private Set<Integer> faturas;
    
    
    /**
      * Construtor sem argumentos.
    */
    
    public Contribuinte(){
        this.nif = -1;
        this.email = "";
        this.nome = "";
        this.morada = "";
        this.password = "";
        this.faturas = new TreeSet<Integer>();
        
    }

    /**
      * Construtor com argumentos.
      * @param nif1         Nif a introduzir.
      * @param nemail       Email a introduzir.
      * @param nnome        Nome a introduzir.
      * @param nmorada      Morada a introduzir.
      * @param npassword    Password a introduzir.
    */
    
    public Contribuinte(int nif1,String nemail,String nnome,String nmorada,String npassword,Set<Integer> faturas){
        this.nif = nif1;
        this.email = nemail;
        this.nome = nnome;
        this.morada = nmorada;
        this.password = npassword;
        setFaturas(faturas);
    }

    /**
      * Construtor com argumentos.
      * @param a  Contribuinte a copiar.
    */
    
    public Contribuinte(Contribuinte a){
        this.nif = a.getNif();
        this.email = a.getEmail();
        this.nome = a.getNome();
        this.morada = a.getMorada();
        this.password = a.getPassword();
        this.faturas = a.getFaturas();
    }

    /**
      * Get para a variável nif do objeto
      * @return  Nif do objeto
    */
    
    public int getNif(){
        return this.nif;
    }

    /**
      * Get para a variável email do objeto.
      * @return Email do objeto
    */
    
    public String getEmail(){
        return this.email;
    }

    /**
      * Get para a variável Nome do objeto.
      * @return Nome do objeto
    */
    
    public String getNome(){
        return this.nome;
    }

    /**
      * Get para a variável morada do objeto.
      * @return Morada do objeto
    */
    
    public String getMorada(){
        return this.morada;
    }

    /**
      * Get para a variável password do objeto.
      * @return Password do objeto
    */
    
    public String getPassword(){ /*Acho que isto não devia ter get, mas em vez disso uma função que recebe uma string e retorna um booleano (caso a string coincida com a pass)*/
        return this.password;
    }
    
    /**
      * Get para a variável faturas do objeto.
      * @return Faturas do objeto
    */
    public Set<Integer> getFaturas(){
        Set<Integer> nova = new TreeSet<>();
        Iterator <Integer> i = this.faturas.iterator();
        int aux;
        
        while(i.hasNext()){
            aux = i.next();
            nova.add(aux);
        }
        
        return nova;
    }

    /**
      * Método abstrato de clone do objeto Contribuinte.
      * @return Clone do objeto
    */
    
    public abstract Contribuinte clone();
    
    /**
     * Método abstrato para calcular o bonús do contribuinte.
     * @return Bonificacao do contribuinte
     */
    
    public abstract double bonus();

    /**
      * Método toString do objeto Contribuinte.
      * @return Objeto em modo string
    */
    
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        sb.append("Nif: ");
        sb.append(this.nif+"\n");
        sb.append("Nome: ");
        sb.append(this.nome+"\n");
        sb.append("Morada: ");
        sb.append(this.morada+"\n");
        sb.append("Email: ");
        sb.append(this.email+"\n");
        sb.append("Password: ");
        sb.append(this.password+"\n");
        sb.append("Faturas: ");
        for(int a : this.faturas)
            sb.append(a +":");
        sb.append("\n");
        
        return sb.toString();
    }

    /**
       * Método set da variável nif.
       * @param a   Valor a introduzir.
    */    
    public void setNif(int a){
        this.nif = a;
    }

    /**
       * Método set da variável email.
       * @param e   Valor a introduzir.
    */
    
    public void setEmail(String e){
        this.email = e;
    }

    /**
       * Método set da variável nome.
       * @param e   Valor a introduzir.
    */
    
    
    public void setNome(String e){
        this.nome = e;
    }

    /**
       * Método set da variável morada.
       * @param m   Valor a introduzir.
    */
    
    
    public void setMorada(String m){
        this.morada = m;
    }

    /**
       * Método set da variável password.
       * @param p   Valor a introduzir.
    */
    
    
    public void setPassword(String p){
        this.password = p;
    }
    
    /**
       * Método set para o set de faturas.
       * @param a   Valor a introduzir.
    */
    public void setFaturas(Set<Integer> a){
        this.faturas = new TreeSet<Integer>();

        Iterator <Integer> i = a.iterator();
        int aux;
        
        while(i.hasNext()){
            aux = i.next();
            this.faturas.add(aux);
        }
    }

    /**
       * Método equals para a classe Contribuinte.
       * @param o   Objeto a comparar.
       * @return    Boolean que representa se a objeto e igual.
    */

    public boolean equals(Object o){
        if(this == o)
            return true;
        
        if (o == null || (this.getClass() != o.getClass()))
            return false;
            
        Contribuinte aux = (Contribuinte) o;
        
        return aux.getNif()==this.nif
            && aux.getNome().equals(this.nome)
            && aux.getMorada().equals(this.morada)
            && aux.getEmail().equals(this.email)
            && aux.getPassword().equals(this.password)
            && aux.getFaturas().equals(this.faturas);
    }
    
    /**
       * Método hashCode para a classe Contribuinte.
       * @return hashcode do objeto
    */
    public int hashCode(){
        int hash = 7;
        
        hash = hash*31 + this.nif;
        hash = hash*31 + this.email.hashCode();
        hash = hash*31 + this.nome.hashCode();
        hash = hash*31 + this.morada.hashCode();
        hash = hash*31 + this.password.hashCode();
        hash = hash*31 + this.faturas.hashCode();
        
        
        return hash;
    }
    
    /**
       * Método de verificao de login do contribuinte.
       * @param passe a verificar.
       * @return booelan que representa que foi feito o login
    */
    public boolean trylogin(String passe) throws ErroNotFound{
        if (!this.getPassword().equals(passe))
            throw new ErroNotFound("Passe incorreta");
        return true;
        }
    
    /**
       * Método para adicionar uma fatura ao set de faturas.
       * @param id valor a introduzir.
    */
    public void setFatura(int id){
        this.faturas.add(id);
    }
    /**
      * Método abstrato para vereficar a existencia de um setor do objeto Contribuinte.
      * @param s  Fator a comparar
    */
    public abstract boolean verificaSetor(String s);
    
    /**
      * Método abstrato que adiciona um setor ao objeto Contribuinte.
      * @param S  Setor a adicionar
    */
    public abstract void addSetor(String S);
    
    
    
}