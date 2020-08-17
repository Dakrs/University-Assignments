
/**
 * Escreva a descrição da classe Empresas aqui.
 * 
 * @author (seu nome) 
 * @version (número de versão ou data)
 */

import java.util.Set;
import java.util.HashSet;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.io.*;
/**
 * Classe que armazena a informacao de uma empresa.
 */
public class Empresa extends Contribuinte implements Serializable 
{
    private Set<String> setores;
    private double fatorcal;
    
    /**
     * Construtor sem argumentos.
     */
    
    public Empresa(){
        super();
        this.setores = new HashSet<String>();
        this.fatorcal = -1;
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
      * @param faturas      Faturas da empresa.
     */
    public Empresa(int nif1,String nemail,String nnome,String nmorada,String npassword,Set<String> setores,double ncoeficiente,Set<Integer> faturas){
        super(nif1,nemail,nnome,nmorada,npassword,faturas);
        setSetores(setores);
        this.fatorcal = ncoeficiente;
    }
    
    /**
     * Construtor com outra Empresa.
     * @param a     Empresa a copir.
     */
    
    public Empresa(Empresa a){
        super(a);
        this.setores = a.getSetores();
        this.fatorcal = a.getfatorcal();
    }
    
    /**
     * Método que devolve o fator de calculo.
     * @return coeficiente de deducao fiscal
     */
    
    public double getfatorcal(){
        return this.fatorcal;
    }
    
    /**
     * Método que altera o fator de calculo de uma empresa.
     * @param a     Valor a inserir.
     */
    
    public void setfatorcal(double a){
        this.fatorcal = a;
    }
    
    /**
     * Método que copia um set de String para o set com os setores de dedução.
     * @param setores Set a copiar.
     */
    
    public void setSetores(Set<String> setores){
        this.setores = new HashSet<String>();
        Iterator <String> i = setores.iterator();
        String aux;
        
        while(i.hasNext()){
            aux = i.next();
            this.setores.add(aux);
        }
        
    }
    
    /**
     * Método que devolve uma cópia do conjunto de setores
     * @return conjunto de setores
     */
    
    public Set<String> getSetores(){
        Set<String> nconj = new HashSet<String>();
        Iterator <String> i = this.setores.iterator();
        String aux;
        
        while(i.hasNext()){
            aux = i.next();
            nconj.add(aux);
        }
        
        return nconj;
    }
    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */
    
    public Empresa clone(){
        return new Empresa(this);
    }
    
    /**
     * Método toString do objeto.
     * @return objeto em modo string
     */
    
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        sb.append(super.toString() + "\n");
        sb.append("Setores: ");
        for(String b : this.setores)
            sb.append(b + "; ");
        sb.append("\n");
        sb.append("Fator de cálculo: ");
        sb.append(this.fatorcal + "\n");
        
        return sb.toString();
    }
    
    
    /**
     * Método equal do objeto.
     * @param   o   Objeto a comparar
     * @return      booelan que verifica se o objeto e igual
     */
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        
        if (o == null || (this.getClass() != o.getClass()))
            return false;
        
        Empresa aux = (Empresa) o;
        
        return super.equals(aux)
            && this.setores.equals(aux.getSetores())
            && this.fatorcal == aux.getfatorcal();
    }
    
    /**
     * Método hashCode do objeto.
     * @return hascode do objeto
     */
    public int hashCode(){
        int hash = 7;
        long aux;
        
        aux = Double.doubleToLongBits(this.fatorcal);
        hash = hash*31 + super.hashCode();
        hash = hash*31 + this.setores.hashCode();
        hash = 31*hash + (int)(aux^(aux >>> 32));
        
        
        
        return hash;
    }
    
    /**
     * Método que calcula a bonificacao da classe Empresa
     * @return bonificacao do objeto
     */
    public double bonus(){
        return (1 + this.fatorcal + 0.005*this.setores.size());
    }
    
    
    /**
     * Método que verifica um dada string pertence aos setores do objeto
     * @param       s  setor a vereficar 
     * @return booelan que representa se o a string pertence ao objeto
     */
    public boolean verificaSetor(String s){
        return this.setores.contains(s);
    }
    
    /**
     * Método que adiciona um setor ao set de setores do objeto
     * @param       s  setor a adicionar
     */
    public void addSetor(String s){
        this.setores.add(s);
    }
    
    
}
