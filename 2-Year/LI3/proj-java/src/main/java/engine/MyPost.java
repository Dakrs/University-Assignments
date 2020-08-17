package engine;


import java.util.*;
import java.time.LocalDate;
import java.util.stream.Collectors;

public class MyPost {

    private long            id;
    private int             type_id;
    private int             ans_count;
    private int             comm_count;
    private int             fav_count;
    private int             score;
    private long            parent_id;
    private long            owner_id;
    private String          owner_name;
    private String          title;
    private LocalDate       cdate;
    private List<Long> filhos;
    private Set<String>     tags;
    
    /**
     * Construtor por argumentos.
     * @param id            Id a introduzir.
     * @param type_id       Tipo de post a introduzir.
     * @param ans_count     Answer count a introduzir.
     * @param comm_count    Comment count a introduzir.
     * @param score         Score a introduzir.
     * @param parent_id     Id do pai do post a introduzir(So relevante caso se seja uma pergunta).
     * @param owner_id      Id do user do post a introduzir.
     * @param owner_name    Nome do user do post a introduzir.
     * @param title         Titulo do post a introduzir.
     * @param cdate         Data do post a introduzir.
     * @param filhos        Conjunto de filhos do post a introduzir.
     * @param tags          Conjunto de tags do post a introduzir.
     */

    public MyPost(int id, int type_id, int ans_count, int comm_count, int fav_count, int score, long parent_id, long owner_id, String owner_name, String title, LocalDate cdate, List<Long> filhos, Set<String> tags) {
        this.id         = id;
        this.type_id    = type_id;
        this.ans_count  = ans_count;
        this.comm_count = comm_count;
        this.fav_count  = fav_count;
        this.score      = score;
        this.parent_id  = parent_id;
        this.owner_id   = owner_id;
        this.owner_name = owner_name;
        this.title      = title;
        this.cdate      = cdate;
        this.filhos     = new ArrayList<Long>();
        this.filhos.addAll(filhos);
        this.tags       = new HashSet<String>();
        this.tags.addAll(tags);
    }

    /**
     * Contrutor sem argumentos.
     */
    
    public MyPost() {
        this.id         = -2;
        this.type_id    = 0;
        this.ans_count  = 0;
        this.comm_count = 0;
        this.fav_count  = 0;
        this.score      = 0;
        this.parent_id  = -2;
        this.owner_id   = -2;
        this.owner_name = null;
        this.title      = null;
        this.cdate      = null;
        this.filhos     = new ArrayList<Long>();
        this.tags       = new HashSet<String>();
    }
    
    /**
     * Construtor com copias.
     * @param m     Classe a copiar.
     */

    public MyPost(MyPost m) {
        this.id         = m.getId();
        this.type_id    = m.getType_id();
        this.ans_count  = m.getAns_count();
        this.comm_count = m.getComm_count();
        this.fav_count  = m.getFav_count();
        this.score      = m.getScore();
        this.parent_id  = m.getParent_id();
        this.owner_id   = m.getOwner_id();
        this.owner_name = m.getOwner_name();
        this.title      = m.getTitle();
        this.cdate      = m.getCdate();
        this.filhos     = m.getFilhos();
        this.tags       = m.getTags();
    }

    
    /**
     * Get para a variável id do objeto.
     * @return  Id do objeto.
     */
    public long getId() {
        return id;
    }
    
    /**
     * Método que altera a variavel id do objeto.
     * @param id        Valor a alterar.
     */

    public void setId(long id) {
        this.id = id;
    }
    
    /**
     * Get para a variável Type_id do objeto.
     * @return  Type_id do objeto.
     */
    
    public int getType_id() {
        return type_id;
    }
    
    /**
     * Método que altera a variavel Type_id do objeto.
     * @param type_id        Valor a alterar.
     */

    public void setType_id(int type_id) {
        this.type_id = type_id;
    }
    
    /**
     * Get para a variável ans_count do objeto.
     * @return  Ans_count do objeto.
     */

    public int getAns_count() {
        return ans_count;
    }

    /**
     * Método que altera a variavel ans_count do objeto.
     * @param ans_count     Valor a alterar.
     */
    
    public void setAns_count(int ans_count) {
        this.ans_count = ans_count;
    }
    
    /**
     * Get para a variável comm_count do objeto.
     * @return  Comm_count do objeto.
     */

    public int getComm_count() {
        return comm_count;
    }
    
    /**
     * Método que altera a variavel comm_count do objeto.
     * @param comm_count   Valor a alterar.
     */

    public void setComm_count(int comm_count) {
        this.comm_count = comm_count;
    }
    
    /**
     * Get para a variável fav_count do objeto.
     * @return  Fav_count do objeto.
     */
    
    public int getFav_count() {
        return fav_count;
    }
    
    /**
     * Método que altera a variavel fav_count do objeto.
     * @param fav_count        Valor a alterar.
     */

    public void setFav_count(int fav_count) {
        this.fav_count = fav_count;
    }
    
    /**
     * Get para a variável score do objeto.
     * @return  Score do objeto.
     */

    public int getScore() {
        return score;
    }
    
    /**
     * Método que altera a variavel score do objeto.
     * @param score        Valor a alterar.
     */

    public void setScore(int score) {
        this.score = score;
    }
    
    /**
     * Get para a variável Parent_id do objeto.
     * @return  Parent_id do objeto.
     */

    public long getParent_id() {
        return parent_id;
    }
    
    /**
     * Método que altera a variavel parent_id do objeto.
     * @param parent_id    Valor a alterar.
     */
    
    public void setParent_id(long parent_id) {
        this.parent_id = parent_id;
    }
    
    /**
     * Get para a variável owner_id do objeto.
     * @return  Id do objeto.
     */
    
    public long getOwner_id() {
        return owner_id;
    }
    
    /**
     * Método que altera a variavel owner_id do objeto.
     * @param owner_id        Valor a alterar.
     */

    public void setOwner_id(long owner_id) {
        this.owner_id = owner_id;
    }
    
    /**
     * Get para a variável owner_name do objeto.
     * @return  Owner_name do objeto.
     */

    public String getOwner_name() {
        return owner_name;
    }
    
    /**
     * Método que altera a variavel owner_name do objeto.
     * @param owner_name        Valor a alterar.
     */
    
    public void setOwner_name(String owner_name) {
        this.owner_name = owner_name;
    }
    
    /**
     * Get para a variável title do objeto.
     * @return  Title do objeto.
     */

    public String getTitle() {
        return title;
    }

    /**
     * Método que altera a variavel title do objeto.
     * @param title        Valor a alterar.
     */
    
    public void setTitle(String title) {
        this.title = title;
    }
    
    /**
     * Get para a variável date do objeto.
     * @return  Date do objeto.
     */
    
    public LocalDate getCdate() {
        return LocalDate.from(cdate);
    }

    /**
     * Método que altera a variavel cdate do objeto.
     * @param cdate        Valor a alterar.
     */
    
    public void setCdate(LocalDate cdate) {
        this.cdate = cdate;
    }
    
    /**
     * Get para a variável filhos do objeto.
     * @return  Filhos do objeto.
     */
    
    public List<Long> getFilhos() {
        List<Long> list = new ArrayList<Long>();
        list.addAll(filhos);                        //podemos usar adAll porque são longs
        return list;
    }
    
    /**
     * Método que altera a variavel filhos do objeto.
     * @param filhos        Valor a alterar.
     */

    public void setFilhos(List<Long> filhos) {
        this.filhos = filhos.stream().collect(Collectors.toCollection(ArrayList::new));
    }
    
    /**
     * Método que adiciona um filho à variavel filhos do objeto.
     * @param id        Valor a adicionar.
     */

    public void addFilho(long id) {
        this.filhos.add(id);
    }
    
    /**
     * Get para a variável tags do objeto.
     * @return  Tags do objeto.
     */

    public Set<String> getTags() {
        Set<String> set = new HashSet<String>();
        set.addAll(tags);                   //podemos usar addAll porque são strings
        return set;
    }

    /**
     * Método que altera a variavel tags do objeto.
     * @param tags        Valor a alterar.
     */
    
    public void setTags(Set<String> tags) {
        if(tags != null)
            this.tags.addAll(tags);
    }
    
    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */

    public MyPost clone(){
        return new MyPost(this);
    }
    
    /**
     * Método equal do objeto.
     * @param  o     Objeto a comparar
     * @return       Booelan que verifica se o objeto e igual
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyPost myPost = (MyPost) o;
        return  getId()         == myPost.getId()               &&
                getType_id()    == myPost.getType_id()          &&
                getAns_count()  == myPost.getAns_count()        &&
                getComm_count() == myPost.getComm_count()       &&
                getFav_count()  == myPost.getFav_count()        &&
                getScore()      == myPost.getScore()            &&
                getParent_id()  == myPost.getParent_id()        &&
                getOwner_id()   == myPost.getOwner_id()         &&
                getOwner_name().equals(myPost.getOwner_name())  &&
                getTitle().equals(myPost.getTitle())            &&
                getCdate().equals(myPost.getCdate())            &&
                getFilhos().equals(myPost.getFilhos())          &&
                getTags().equals(myPost.getTags());
    }
    
    /**
     * Método toString do objeto.
     * @return Objeto em modo string.
     */

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();

        sb.append("Id: ");
        sb.append(this.id);
        sb.append("\n");
        sb.append("Type Id: ");
        sb.append(this.type_id);
        sb.append("\n");
        sb.append("Answer Count: ");
        sb.append(this.ans_count);
        sb.append("\n");
        sb.append("Comment Count: ");
        sb.append(this.comm_count);
        sb.append("\n");
        sb.append("Favorite Count: ");
        sb.append(this.fav_count);
        sb.append("\n");
        sb.append("Score: ");
        sb.append(this.score);
        sb.append("\n");
        sb.append("Parent Id: ");
        sb.append(this.parent_id);
        sb.append("\n");
        sb.append("Owner Id: ");
        sb.append(this.owner_id);
        sb.append("\n");
        sb.append("Owner Name: ");
        sb.append(this.owner_name);
        sb.append("\n");
        sb.append("Title: ");
        sb.append(this.title);
        sb.append("\n");
        sb.append("Creation Date: ");
        sb.append(this.cdate);
        sb.append("\n");
        sb.append("Tags: ");
        sb.append(this.tags);
        sb.append("\n");
        sb.append("Filhos ");
        sb.append("(" + this.filhos.size() + "): ");
        sb.append(Arrays.toString(this.filhos.toArray()));
        sb.append("\n");


        return sb.toString();
    }
    
    /**
     * Método hashCode do objeto.
     * @return hash do Objeto.
     */
    @Override
    public int hashCode(){
        int hash = 7;
        hash = hash*31 + (int) this.id;
        hash = hash*31 + this.type_id;
        hash = hash*31 + this.comm_count;
        hash = hash*31 + this.ans_count;
        hash = hash*31 + (int) this.owner_id;
        hash = hash*31 + this.score;
        
        return hash;
    }

}
