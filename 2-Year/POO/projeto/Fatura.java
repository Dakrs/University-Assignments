import java.time.LocalDate;
import java.util.Set;
import java.util.HashSet;
import java.io.*;

/**
 * Classe que armazena a informacao de uma fatura.
 */
public class Fatura implements Serializable {

    private int id;
    private int nif_emitente;
    private int nif_cliente;
    private double valor;
    private String nome_emitente;
    private String categoria;
    private String descricao;
    private Set<String> old_cat;
    private LocalDate emissao;
    
    private static int nextid = 0;
    
    
    /**
     * Construtor sem argumentos.
     */
    public Fatura(){
        this.id             = nextid;
        this.nif_emitente   = 0;
        this.nif_cliente    = 0;
        this.valor          = 0;
        this.nome_emitente  = "";
        this.descricao      = "";
        this.categoria      = "";
        this.old_cat        = new HashSet<String>();
        this.emissao        = null;
        
        nextid++;
    }
    
    /**
     * Construtor com argumentos.
      * @param nif_emitente         Nif do emitente.
      * @param nome_emitente        Nome do emitente.
      * @param emissao              Data de emissao.
      * @param nif_cliente          Nif do cliente.
      * @param descricao            Descricao da fatura.
      * @param categoria            Categoria da fatura.
      * @param valor                Valor da fatura.
     */
    public Fatura(int nif_emitente, String nome_emitente, LocalDate emissao, int nif_cliente, String descricao, String categoria, double valor) {
        this.id             = nextid;
        this.nif_emitente   = nif_emitente;
        this.nome_emitente  = nome_emitente;
        this.emissao        = emissao;
        this.nif_cliente    = nif_cliente;
        this.descricao      = descricao;
        this.categoria      = categoria;
        this.old_cat       = new HashSet<String>();
        this.valor          = valor;
        
        nextid++;
    }
    
    /**
     * Construtor com outra Fatura.
     * @param f    Fatura a copir.
     */
    public Fatura(Fatura f) {
        this.id             = f.getId();
        this.nif_emitente   = f.getNif_emitente();
        this.nome_emitente  = f.getNome_emitente();
        this.emissao        = f.getEmissao();
        this.nif_cliente    = f.getNif_cliente();
        this.descricao      = f.getDescricao();
        this.categoria      = f.getCategoria();
        this.old_cat       = f.getOld_cat();
        this.valor          = f.getValor();
    }
    
    /**
     * Metodo que devolve o registo de todas as alteracoes da categoria de uma fatura
     * @return Registo das alteracoes da categoria da fatura
     */
    public Set<String> getOld_cat(){
        Set<String> aux = new HashSet <String>();
        this.old_cat.forEach(a -> aux.add(a));
        
        return aux;
    }
    
    /**
     * Metodo que devolve o id de uma fatura
     * @return Id da fatura
     */
    public int getId() {
        return id;
    }
    
    /**
     * Metodo que devolve o nif do cliente de uma fatura
     * @return Nif do cliente da fatura
     */
    public int getNif_cliente() {
        return nif_cliente;
    }
    
    /**
     * Metodo que devolve o nif do emitente de uma fatura
     * @return Nif do emitente da fatura
     */
    public int getNif_emitente() {
        return nif_emitente;
    }
    
    /**
     * Metodo que devolve o valor de uma fatura
     * @return Valor da fatura
     */
    public double getValor() {
        return valor;
    }
    
    /**
     * Metodo que devolve o nif do cliente de uma fatura
     * @return Nif da fatura
     */
    public LocalDate getEmissao() {
        return emissao;
    }
    
    /**
     * Metodo que devolve a categoria de uma fatura
     * @return Categoria da fatura
     */
    public String getCategoria() {
        return categoria;
    }
    
    /**
     * Metodo que devolve a descricao de uma fatura
     * @return Descricao da fatura
     */
    public String getDescricao() {
        return descricao;
    }
    
    /**
     * Metodo que devolve a nome do emitente de uma fatura
     * @return Nome o emitente da fatura
     */
    public String getNome_emitente() {
        return nome_emitente;
    }
    
    /**
     * Método que altera a categoria de uma fatura.
     * @param categoria     Categoria a inserir.
     */
    public void setCategoria(String categoria) {
        this.old_cat.add(this.categoria);
        this.categoria = categoria;
    }

    /**
     * Método que altera a Descricao de uma fatura.
     * @param descricao     Descricao a inserir.
     */
    public void setDescricao(String descricao) {
        this.descricao = descricao;
    }
    
    /**
     * Método que altera a data de emissao de uma fatura.
     * @param emissao     Data de emissao a inserir.
     */
    public void setEmissao(LocalDate emissao) {
        this.emissao = emissao;
    }

    /**
     * Método que altera o nif do cliente de uma fatura.
     * @param nif_cliente     Nif do cliente a inserir.
     */
    public void setNif_cliente(int nif_cliente) {
        this.nif_cliente = nif_cliente;
    }
    
    /**
     * Método que altera o nif do emitente de uma fatura.
     * @param nif_emitente     Nif do emitente a inserir.
     */
    public void setNif_emitente(int nif_emitente) {
        this.nif_emitente = nif_emitente;
    }
    
    /**
     * Método que altera o nome do emitente de uma fatura.
     * @param nome_emitente     Nome do emitente a inserir.
     */
    public void setNome_emitente(String nome_emitente) {
        this.nome_emitente = nome_emitente;
    }
    
    /**
     * Método que altera a valor de uma fatura.
     * @param valor     Valor a inserir.
     */
    public void setValor(double valor) {
        this.valor = valor;
    }
    
    /**
     * Método equal do objeto.
     * @param object    Objeto a comparar
     * @return          Booelan que verifica se o objeto e igual
     */
    public boolean equals(Object object) {
        if (this == object) return true;
        if (object == null || getClass() != object.getClass()) return false;
        Fatura fatura = (Fatura) object;
        
        Set<String> aux = fatura.getOld_cat();
        if (aux.size() != this.old_cat.size())
            return false;
        
        for(String a : aux)
            if (!this.old_cat.contains(a))
                return false;
            
        return  this.id              == fatura.getId()           &&
                this.nif_emitente    == fatura.getNif_emitente() &&
                this.nif_cliente     == fatura.getNif_cliente()  &&
                this.valor           == fatura.getValor()        &&
                this.descricao       == fatura.getDescricao()    &&
                this.categoria       == fatura.getCategoria()    &&
                this.nome_emitente   == fatura.getNome_emitente()&&
                this.emissao.equals(fatura.getEmissao())         ;
    }
    
    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */
    public Fatura clone(){
        return new Fatura(this);
    }
    
    /**
     * Método toString do objeto.
     * @return objeto em modo string
     */
    public String toString(){
        StringBuilder sb = new StringBuilder();

        sb.append("Id: ");
        sb.append(this.id);
        sb.append("\n");
        sb.append("Emitente: ");
        sb.append(this.nome_emitente);
        sb.append("\n");
        sb.append("NIF do emitente: ");
        sb.append(this.nif_emitente);
        sb.append("\n");
        sb.append("NIF do cliente: ");
        sb.append(this.nif_cliente);
        sb.append("\n");
        sb.append("Valor da transação: ");
        sb.append(this.valor);
        sb.append("\n");
        sb.append("Data de emissão: ");
        sb.append(this.emissao);
        sb.append("\n");
        sb.append("Categoria: ");
        sb.append(this.categoria);
        sb.append("\n");
        sb.append("Descrição: ");
        sb.append(this.descricao);
        sb.append("\n");

        return sb.toString();
    }
    
    /**
     * Método hashCode do objeto.
     * @return hascode do objeto
     */
    public int hashCode(){
        int hash = 7;
        long aux;
        
        aux = Double.doubleToLongBits(this.valor);
        hash = hash*31 + this.id;
        hash = hash*31 + this.nif_emitente;
        hash = hash*31 + this.nif_cliente;
        hash = hash*31 + (int)(aux^(aux >>> 32));
        hash = hash*31 + this.nome_emitente.hashCode();
        hash = hash*31 + this.categoria.hashCode();
        hash = hash*31 + this.descricao.hashCode();
        hash = hash*31 + this.emissao.hashCode();
        
        for(String a : this.old_cat){
            hash = hash*31 + a.hashCode();
        }
        
        return hash;
    }
    
    /**
     * Método compare  do objeto.
     * @param   h   Fatura a comparar
     * @return      Valor de comparacao
     */
    public int compareTo(Fatura h) {
        return h.getId() - this.id;
    }


}

