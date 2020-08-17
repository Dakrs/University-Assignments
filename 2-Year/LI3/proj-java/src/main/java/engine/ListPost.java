package engine;

import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;
import java.util.ArrayList;

/**
 * Classe onde está definido tipo adotado para os posts e funções relativas ao mesmo.
 */

public class ListPost {

    private int perguntas;
    private int respostas;
    private List<MyPost> posts;

    /**
     * Contrutor sem argumentos.
     */
    public ListPost(){
        this.posts = new ArrayList<>();
        this.perguntas = 0;
        this.respostas = 0;
    }

    /**
     * Construtor por argumentos.
     * @param a          Lista de posts a inserir.
     */
    public ListPost(List<MyPost> a){
        this.posts = a.stream().map(MyPost::clone).collect(Collectors.toCollection(ArrayList::new));
        for(MyPost i : this.posts){
            if (i.getType_id() == 1)
                perguntas++;
            else if (i.getType_id() == 2)
                respostas++;
        }
    }

    /**
     * Construtor por copia.
     * @param a     Classe a copiar.
     */
    public ListPost (ListPost a){
        this.posts = a.getPosts().stream().map(MyPost::clone).collect(Collectors.toCollection(ArrayList::new));
        this.perguntas = a.getPerguntas();
        this.respostas = a.getRespostas();
    }

    /**
     * Get para a variável posts do objeto.
     * @return  Lista de posts do objeto.
     */
    public List<MyPost> getPosts(){
        return posts;
    }

    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */
    public ListPost clone(){
        return  new ListPost(this);
    }

    /**
     * Get para a variável perguntas do objeto.
     * @return  Perguntas do objeto.
     */
    public int getPerguntas(){
        return this.perguntas;
    }

    /**
     * Get para a variável respostas do objeto.
     * @return  Respotas do objeto.
     */
    public int getRespostas(){
        return this.respostas;
    }

    /**
     * Metodo que adiciona um post ao objeto.
     * @param p     Objeto a adicionar.
     */
    public void add(MyPost p){
        this.posts.add(p);
        if (p.getType_id() == 1)
            perguntas++;
        else if (p.getType_id() == 2)
            respostas++;
    }

    /**
     * Metodo comparador de 2 posts conforme o id.
     * @param a1    Post 1.
     * @param a2    Post 2.
     * @return  Inteiro a ser utilizado por um comparator.
     */
    private int compaux(MyPost a1, MyPost a2){
        if (a1.getId() > a2.getId())
            return -1;
        else if (a1.getId() < a2.getId())
            return 1;
        return 0;
    }

    /**
     * Metodo que ordena a lista de posts conforme os ids.
     */
    public void sort(){
        Comparator<MyPost> comp = (a1,a2) -> compaux(a1,a2);
        this.posts.sort(comp);
    }




}
