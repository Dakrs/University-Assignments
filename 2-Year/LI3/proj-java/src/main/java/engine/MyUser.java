package engine;

import java.util.List;
import java.util.ArrayList;


/**
 * Classe onde está definido tipo adotado para os users e funções relativas ao mesmo.
 */

public class MyUser {

    private long    id;
    private int     rep;
    private String  username;
    private String  bio;
    List<Long>      posts;

    /**
     * Contrutor sem argumentos.
     */
    
    public MyUser() {
        this.id         = -2;
        this.rep        = 0;
        this.username   = null;
        this.bio        = null;
        this.posts      = new ArrayList<Long>();
    }

    /**
     * Construtor com argumentos.
     * @param id            Id a introduzir.
     * @param rep           Reputacao a introduzir.
     * @param username      Username a introduzir.
     * @param bio           Bio a introduzir.
     * @param posts         Conjunto de posts a introduzir.
     */
    
    public MyUser(int id, int rep, String username, String bio, List<Long> posts) {
        this.id         = id;
        this.rep        = rep;
        this.username   = username;
        this.bio        = bio;
        this.posts      = new ArrayList<Long>();
        this.posts.addAll(posts);
    }
    
    /**
     * Construtor por copias.
     * @param m     Classe a copiar.
     */

    public MyUser(MyUser m){
        this.id         = m.getId();
        this.rep        = m.getRep();
        this.username   = m.getUsername();
        this.bio        = m.getBio();
        this.posts      = m.getPosts();
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
     * Get para a variável rep do objeto.
     * @return  Rep do objeto.
     */

    public int getRep() {
        return rep;
    }
    
    /**
     * Método que altera a variavel rep do objeto.
     * @param rep        Valor a alterar.
     */

    public void setRep(int rep) {
        this.rep = rep;
    }
    
    /**
     * Get para a variável username do objeto.
     * @return  Username do objeto.
     */

    public String getUsername() {
        return username;
    }
    
    /**
     * Método que altera a variavel username do objeto.
     * @param username        Valor a alterar.
     */

    public void setUsername(String username) {
        this.username = username;
    }

    /**
     * Get para a variável bio do objeto.
     * @return  Bio do objeto.
     */
    
    public String getBio() {
        return bio;
    }

    /**
     * Método que altera a variavel bio do objeto.
     * @param bio        Valor a alterar.
     */
    
    public void setBio(String bio) {
        this.bio = bio;
    }
    
    /**
     * Get para a variável posts do objeto.
     * @return  Posts do objeto.
     */

    public List<Long> getPosts() {
        ArrayList<Long> list = new ArrayList<Long>();
        list.addAll(this.posts);
        return list;
    }
    
    /**
     * Método que altera a variavel posts do objeto.
     * @param list        Valor a alterar.
     */

    public void setPosts(List<Long> list) {
        this.posts.addAll(list);
    }
    
    /**
     * Método que adiciona um post à variavel de posts do objeto.
     * @param id        Valor a adicionar.
     */

    public void addPost(long id){
        this.posts.add(id);
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
        MyUser myUser = (MyUser) o;
        return  getId()     == myUser.getId()               &&
                getRep()    == myUser.getRep()              &&
                getUsername().equals(myUser.getUsername())  &&
                getBio().equals(myUser.getBio())            &&
                getPosts().equals(myUser.getPosts())        ;
    }

    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */
    
    public MyUser clone(){
        return new MyUser(this);
    }
    
    /**
     * Método toString do objeto.
     * @return Objeto em modo string.
     */

    public String toString(){
        StringBuilder sb = new StringBuilder();

        sb.append("Id: ");
        sb.append(this.id);
        sb.append("\n");
        sb.append("Username: ");
        sb.append(this.username);
        sb.append("\n");
        sb.append("Reputacao: ");
        sb.append(this.rep);
        sb.append("\n");
        sb.append("Biografia: ");
        sb.append(this.bio);
        sb.append("\n");
        sb.append("Posts criados ");
        sb.append("(" + this.posts.size() + "): ");
        sb.append(this.posts);
        sb.append("\n");

        return sb.toString();
    }
    
    /**
     * Método hashCode do objeto.
     * @return hash do objeto.
     */
    
    public int hashCode(){
        int hash = 7;
        hash = hash*31 + (int) this.id;
        hash = hash*31 +  this.rep;
        hash = hash*31 +  this.username.hashCode();
        
        return hash;
    }

}
