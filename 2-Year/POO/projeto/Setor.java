import java.io.Serializable;

/**
 * Classe que armazena a informacao de um setor.
 */
public class Setor implements Serializable {
    private String nome;
    private double taxa;
    
    /**
     * Construtor sem argumentos.
     */
    public Setor(){
        this.nome   = "";
        this.taxa   = 0;
    }
    
    /**
     * Construtor com argumentos.
      * @param nome         Nome do setor.
      * @param tax          Taxa do setor.
     */
    public Setor(String nome, double taxa) {
        this.nome   = nome;
        this.taxa   = taxa;
    }
    
    /**
     * Construtor com outra Fatura.
     * @param f    Fatura a copir.
     */
    public Setor(Setor setor){
        this.nome   = setor.getNome();
        this.taxa   = setor.getTaxa();
    }


    /**
     * Metodo que nome do setor.
     * @return Nome do setor
     */
    public String getNome() {
        return nome;
    }
    
    /**
     * Metodo altera o nome do setor.
     * @param nome  Nome do setor
     */
    public void setNome(String nome) {
        this.nome = nome;
    }
    
    /**
     * Metodo que taxa do setor.
     * @return Taxa do setor
     */
    public double getTaxa() {
        return taxa;
    }
    
    /**
     * Metodo altera a taxa do setor.
     * @param taxa  Taxa do setor
     */
    public void setTaxa(double taxa) {
        this.taxa = taxa;
    }

    /**
     * Método equal do objeto.
     * @param o Objeto a comparar.
     * @return  Booelan que verifica se o objeto e igual
     */
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || this.getClass() != o.getClass()) return false;
        Setor setor = (Setor) o;
        return  this.getNome() == setor.getNome() &&
                this.getTaxa() == setor.getTaxa();
    }
    
    /**
     * Método hashCode do objeto.
     * @return hascode do objeto
     */
    public int hashCode() {

        int hash = 7;
        long aux;

        aux = Double.doubleToLongBits(this.taxa);

        hash = hash*31 + this.nome.hashCode();
        hash = 31*hash + (int)(aux^(aux >>> 32));

        return hash;
    }
    
    /**
     * Método que clona este objeto.
     * @return Clone do objeto
     */
    public Setor clone(){
        return new Setor(this);
    }
    
    /**
     * Método toString do objeto.
     * @return Objeto em modo string
     */
    public String toString(){
        StringBuilder sb = new StringBuilder();

        sb.append("Setor{ \nNome: ");
        sb.append(this.getNome());
        sb.append("\n");
        sb.append("Taxa: ");
        sb.append(this.getTaxa());
        sb.append("}\n");

        return sb.toString();
    }

}
