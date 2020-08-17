package engine;

import common.Pair;
import li3.TADCommunity;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import java.io.File;
import java.io.IOException;
import java.time.LocalDate;
import java.util.*;
import java.util.Map;
import java.util.List;
import java.util.stream.Collectors;
import java.util.ArrayList;
import java.util.Comparator;

/**
 * Classe contendo funções predefinidas para responder as querys bem como as suas funcoes auxiliares.
 */

public class TCDCommunity implements TADCommunity {

    private Map<Long, MyUser>                   users;
    private Map<Long, MyPost>                   posts_id;
    private Map<LocalDate, ListPost>            posts_date;
    private Map<String, Long>                   tags;
    private List<Long>                          pre_posts;
    private List<Long>                          pre_rep;

    /**
     * Contrutor sem argumentos.
     */

    public TCDCommunity() {
        this.users      = new LinkedHashMap<>();
        this.posts_id   = new LinkedHashMap<>();
        this.posts_date = new LinkedHashMap<>();
        this.tags       = new LinkedHashMap<>();
        this.pre_posts  = new ArrayList<>();
        this.pre_rep    = new ArrayList<>();
    }

    /**
     * Construtor com argumentos.
     * @param users             Map de <ID,User> a introduzir.
     * @param posts_id          Map de <ID,Post> a introduzir.
     * @param posts_date        Map de <Data,Posts> a introduzir.
     * @param tags              Map de <TAG,Contagem> a introduzir..
     * @param pre_posts         Conjunto de posts pre calculados a introduzir.
     * @param pre_rep           Conjunto de reputacao pre calculada a introduzir.
     */
    
    public TCDCommunity(Map<Long, MyUser> users, Map<Long, MyPost> posts_id, Map<LocalDate,ListPost> posts_date, Map<String, Long> tags, List<Long> pre_posts, List<Long> pre_rep) {
        this.setUsers(users);
        this.setPosts_id(posts_id);
        this.setPosts_date(posts_date);
        this.setTags(tags);
        this.setPre_posts(pre_posts);
        this.setPre_rep(pre_rep);
    }

    /**
     * Construtor por copias.
     * @param c     Classe a copiar.
     */
    
    public TCDCommunity(TCDCommunity c){
        this.users      = c.getUsers();
        this.posts_id   = c.getPosts_id();
        this.posts_date = c.getPosts_date();
        this.tags       = c.getTags();
        this.pre_posts  = c.getPre_posts();
        this.pre_rep    = c.getPre_rep();
    }

    /**
     * Get para a variável users objeto.
     * @return  Users do objeto.
     */
    
    public Map<Long, MyUser> getUsers() {
        Map<Long,MyUser> ret = new LinkedHashMap<>();
        for(MyUser u : this.users.values()){
            ret.put(u.getId(),u.clone());
        }
        return ret;
    }

    private void setUsers(Map<Long, MyUser> a){
        this.users = new LinkedHashMap<>();
        for(MyUser u : a.values()){
            this.users.put(u.getId(),u.clone());
        }
    }
    
    /**
     * Get para a variável post_id do objeto.
     * @return  Post_Id do objeto.
     */

    public Map<Long, MyPost> getPosts_id() {
        Map<Long,MyPost> ret = new LinkedHashMap<>();
        for(MyPost u : this.posts_id.values()){
            ret.put(u.getId(),u.clone());
        }
        return ret;
    }

    private void setPosts_id(Map<Long, MyPost> a) {
        this.posts_id = new LinkedHashMap<>();
        for(MyPost u : a.values()){
            this.posts_id.put(u.getId(),u.clone());
        }
    }

    /**
     * Get para a variável post_date do objeto.
     * @return  Post_date do objeto.
     */

    public Map<LocalDate,ListPost> getPosts_date() {
        Map<LocalDate,ListPost> ret = new LinkedHashMap<>();
        for(Map.Entry<LocalDate,ListPost> aux : this.posts_date.entrySet()){
            ret.put(aux.getKey(),aux.getValue().clone());
        }
        return ret;
    }

    private void setPosts_date(Map<LocalDate,ListPost> a) {
        this.posts_date = new LinkedHashMap<>();
        for(Map.Entry<LocalDate,ListPost> k : a.entrySet()){
            this.posts_date.put(k.getKey(), k.getValue().clone());
        }
    }
    
    /**
     * Get para a variável tags do objeto.
     * @return  Tags do objeto.
     */

    public Map<String, Long> getTags() {
        Map<String,Long> ret = new LinkedHashMap<>();
        this.tags.entrySet().forEach(e -> ret.put(e.getKey(),e.getValue()));
        return ret;
    }

    private void setTags(Map<String,Long> a){
        this.tags = new LinkedHashMap<>();
        this.tags.putAll(a);
    }
    
    /**
     * Get para a variável pre_posts do objeto.
     * @return  Pre_posts do objeto.
     */

    public List<Long> getPre_posts() {
        return this.pre_posts.stream().collect(Collectors.toList());
    }
    
    private void setPre_posts(List<Long> a){
        this.pre_posts = a.stream().collect(Collectors.toCollection(ArrayList::new));
    }
    
    /**
     * Get para a variável pre_red do objeto.
     * @return  Pre_rep do objeto.
     */

    public List<Long> getPre_rep() {
        return this.pre_rep.stream().collect(Collectors.toList());
    }

    private void setPre_rep(List<Long> a){
        this.pre_rep = a.stream().collect(Collectors.toCollection(ArrayList::new));
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
        TCDCommunity that = (TCDCommunity) o;
        return  this.users.equals(that.getUsers())              &&
        this.posts_id.equals(that.getPosts_id())        &&
        this.posts_date.equals(that.getPosts_date())    &&
        this.tags.equals(that.getTags())                &&
        this.pre_posts.equals(that.getPre_posts())      &&
        this.pre_rep.equals(that.getPre_rep())          ;
    }

    /**
     * Método que clona este objeto.
     * @return clone do objeto
     */

    public TCDCommunity clone(){
        return new TCDCommunity(this);
    }

    /**
     * Método toString do objeto.
     * @return Objeto em modo string.
     */
    
    public String toString(){
        StringBuilder sb = new StringBuilder();

        sb.append("Users: ");
        sb.append(this.users.toString());
        sb.append("\n");
        sb.append("Posts: ");
        sb.append(this.posts_id.toString());
        sb.append("\n");
        sb.append("Tags: ");
        sb.append(this.tags.toString());
        sb.append("\n");

        return sb.toString();
    }

    /**
     * Método dá load aos ficheiros xml.
     * @param    path        String com a diretoria onde se encontram os ficheiros.
     */
    
    public void load(String path) {

        SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();
        try {
            SAXParser saxParser = saxParserFactory.newSAXParser();
            MyUserHandler user_handler = new MyUserHandler();
            saxParser.parse(new File(path + "Users.xml"), user_handler);

            MyPostHandler post_handler = new MyPostHandler();
            saxParser.parse(new File(path + "Posts.xml"), post_handler);

            MyTagHandler tag_handler = new MyTagHandler();
            saxParser.parse(new File(path + "Tags.xml"), tag_handler);

            for (MyUser u : user_handler.getUsers())                                         //adiciona users à sua hash
                this.users.put(u.getId(), u);


            ListPost posts;
            for (MyPost p : post_handler.getPosts()) {

                try {
                    p.setOwner_name(this.users.get(p.getOwner_id()).getUsername());
                } catch (NullPointerException o) {
                }

                this.posts_id.put(p.getId(), p);                                            //adiciona post na hash por Id

                if (this.posts_date.containsKey(p.getCdate()))
                    posts = this.posts_date.get(p.getCdate());
                else
                    posts = new ListPost();
                posts.add(p);
                this.posts_date.put(p.getCdate(), posts);                                   //adiciona post na hash por datas (LocalDate -> ArrayList<Long>) datas para conjuntos de Id's
            }
            this.posts_date.values().forEach(a -> a.sort());

            this.tags = tag_handler.getTags();

            Comparator<Map.Entry<Long,Integer>> comp = (a1,a2) -> -(a1.getValue() - a2.getValue());

            List<Map.Entry<Long,Integer>> aux = new ArrayList<>(this.users.size());
            aux.addAll(user_handler.getUsers_rep().entrySet());
            aux.sort(comp);

            this.pre_rep = aux.stream().map(e -> e.getKey()).collect(Collectors.toCollection(ArrayList::new));

            for(Map.Entry<Long, List<Long>> e : post_handler.getUser_posts().entrySet())  //atribui os post aos users na hash deles
                if(users.containsKey(e.getKey()) && e.getValue() != null)
                    for(long l : e.getValue())
                        setPostToUser(l);


            aux = new ArrayList<>(this.users.size());
            aux.addAll(post_handler.getNum_posts().entrySet());
            aux.sort(comp);

            this.pre_posts = aux.stream().map(e -> e.getKey()).collect(Collectors.toCollection(ArrayList::new));

            for(MyPost p : this.posts_id.values())                                          //atribui aos posts a lista dos seus filhos
                if(this.posts_id.containsKey(p.getParent_id()))
                    setChildToPost(p.getParent_id(), p.getId());


        } catch (ParserConfigurationException | SAXException | IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Método que adicionar uma respota a um post.
     * @param parent        Pai a adiconar.
     * @param child         Filho que vai ser adicionado.
     */
    
    private void setChildToPost(long parent, long child){

        MyPost post = posts_id.get(parent);
        post.addFilho(child);

        this.posts_id.put(parent, post);

    }

    //++++++++++++++++++++++++++++++++++++++++++++++QUERIES+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    /**
     * Método mete um post no correspondete user.
     * @param post            Identificador do post.
     */


    private void setPostToUser(long post){
        this.users.get(this.posts_id.get(post).getOwner_id()).addPost(post);
    }

    // Query 1
    /**
     * Método retorna a informacao de um post.
     * @param    id        Id do post
     * @return             Pair com o title e name do user, retorna NULL em caso de nao ser encontrado.
     */
    
    public Pair<String,String> infoFromPost(long id){
        MyPost p = this.posts_id.get(id);
        if (p == null)
            return new Pair<>(null,null);

        if(p.getType_id() == 2)
            p = this.posts_id.get(p.getParent_id());

        return new Pair<>(p.getTitle(),this.users.get(p.getOwner_id()).getUsername());
    }

    // Query 2;
    /**
     * Método que calcula os N utilizadores com mais posts.
     * @param    N        Número de jogadores.
     * @return            Lista de Longs com os N users mais ativos.
     */
    
    public List<Long> topMostActive(int N){
        if (N <= 0)
            return new ArrayList<>();

        if(N > this.pre_posts.size())
            N = this.pre_posts.size();

        long k;
        List<Long> a = new ArrayList<>(N);
        Iterator i = this.pre_posts.iterator();
        while (i.hasNext() && N > 0){
            k =(long) i.next();
            N--;
            a.add(k);
        }

        return a;
    }

    // Query 3
    /**
     * Método que dado um intervalo de tempo obtem o numero total de perguntas e respostas.
     * @param    begin      Data inicial da procura
     * @param    end        Data final da procura
     * @return              Pair com o numero total de perguntas e resposta no dado intervalo.
     */
    public Pair<Long,Long> totalPosts(LocalDate begin, LocalDate end){
        if (begin.isAfter(end))
            return new Pair<>((long)0,(long)0);

        ListPost aux;
        long p = 0, r = 0;
        while(!begin.isAfter(end)){
            aux = this.posts_date.get(begin);
            if (aux != null){
                r += aux.getRespostas();
                p += aux.getPerguntas();
            }
            begin = begin.plusDays(1);
        }

        return new Pair<>(p,r);
    }
    // Query 4
    /**
     * Método que dado um intervalo de tempo retornar todas as perguntas contendo uma determinada tag.
     * @param    tag        Tag.
     * @param    begin      Data inicial da procura.
     * @param    end        Data final da procura.
     * @return              List com o id de todas as tags que ocorreram no dado intervalo de tempo.
     */
    
    public List<Long> questionsWithTag(String tag, LocalDate begin, LocalDate end) {
        ListPost aux;
        List<Long> res = new ArrayList<>();
        if (begin.isAfter(end))
            return res;

        while(end.isAfter(begin) || end.equals(begin)){
            aux = this.posts_date.get(end);
            if (aux != null){
                for(MyPost i : aux.getPosts()){
                    if ((i.getType_id() == 1) && i.getTags().contains(tag))
                        res.add(i.getId());
                }
            }
            end = end.minusDays(1);
        }

        return res;
    }

    //Query 5
    /**
     * Método que dado um id de um user devolve informacao sobre este mesmo.
     * @param    id        Id do post.
     * @return             Pair com a biografia e os 10 posts mais recentes desse mesmo user.
     */
    public Pair<String, List<Long>> getUserInfo(long id){
        Set<MyPost> posts = new TreeSet<>(new compTime());

        if (!this.users.containsKey(id))
            return new Pair<>(null,new ArrayList<>());

        MyPost aux;

        for(Long l : this.users.get(id).getPosts()) {
            aux = this.posts_id.get(l);
            if (aux.getType_id() == 1 || aux.getType_id() == 2)
                posts.add(aux);
        }

        int n = 0;
        List<Long> res = new ArrayList<>();
        Iterator i = posts.iterator();

        while(i.hasNext() && n++ < 10)
            res.add(((MyPost) i.next()).getId());



        return new Pair<>(this.users.get(id).getBio(), res);
    }

    //Query 6
    /**
     * Método que dado um intervalo de tempo calcula os N posts com melhor score.
     * @param    N          Número de respostas.
     * @param    begin      Data do começo do intervalo.
     * @param    end        Data do fim do intervalo.
     * @return              List com os N utilizadores que mais votaram no dado intervalo de tempo.
     */
    public List<Long> mostVotedAnswers(int N, LocalDate begin, LocalDate end){
        List<Long> res = new ArrayList<>();
        ListPost aux1;
        List<MyPost> aux2 = new ArrayList<>();

        if (begin.isAfter(end) || N <= 0)
            return res;

        while(!begin.isAfter(end)){
            aux1 = this.posts_date.get(begin);
            if (aux1 != null){
                for(MyPost i : aux1.getPosts()){
                    if (i.getType_id() == 2)
                        aux2.add(i);
                }
            }
            begin = begin.plusDays(1);
        }

        aux2.sort(new compScore());
        MyPost p;
        Iterator e = aux2.iterator();

        while (e.hasNext() && N > 0){
            p = (MyPost) e.next();
            res.add(p.getId());
            N--;
        }

        return res;

    }

    //Query 7
    /**
     * Método que dado um intervalo de tempo calcula as N perguntas com mais respostas.
     * @param    N          Número de posts a calcular.
     * @param    begin      Data do começo do intervalo.
     * @param    end        Data do fim do intervalo.
     * @return              List com os N utilizadores que mais votaram no intervalo dado.
     */

    public List<Long> mostAnsweredQuestions(int N, LocalDate begin, LocalDate end){
        List<Long> res = new ArrayList<>();
        ListPost aux1;
        List<MyPost> aux2 = new ArrayList<>();

        if (begin.isAfter(end) || N <= 0)
            return res;

        while(!begin.isAfter(end)){
            aux1 = this.posts_date.get(begin);
            if (aux1 != null){
                for(MyPost i : aux1.getPosts()){
                    if (i.getType_id() == 1)
                        aux2.add(i);
                }
            }
            begin = begin.plusDays(1);
        }

        aux2.sort(new compAnswer());
        MyPost p;
        Iterator e = aux2.iterator();

        while (e.hasNext() && N > 0){
            p = (MyPost) e.next();
            res.add(p.getId());
            N--;
        }

        return res;

    }

    //Query 8
    /**
     * Método que obtém os id's das N perguntas mais recentes cujo título contém uma dada palavra.
     * @param    word       Palavra a ser procurada nos títulos.
     * @param    N          Número máximo de resultados N.
     * @return              List com as N perguntas mais recentes que contêm a palavra dada.
     */
    public List<Long> containsWord(int N, String word){
        TreeSet<MyPost> posts = new TreeSet<>(new compTime());

        if (N <= 0)
            return new ArrayList<>();

        for(MyPost p : this.posts_id.values())
            if(p.getTitle()!= null && p.getTitle().contains(word))
                posts.add(p);



        List<Long> res = new ArrayList<>();
        int n = 0;
        Iterator i = posts.iterator();

        while (i.hasNext() && n++ < N)
            res.add(((MyPost) i.next()).getId());

        return res;
    }

    // Query 9
    /**
     * Método que dado 2 users retorna as N perguntas em que ambos participaram.
     * @param id1           ID do user 1.
     * @param id2           ID do user 2.
     * @param N             Número máximo de N
     * @return              List com as N perguntas mais recentes em que ambos os users participaram.
     */
    public List<Long> bothParticipated(int N, long id1, long id2){
        List<Long> res = new ArrayList<>();

        MyUser u1 = this.users.get(id1);
        MyUser u2 = this.users.get(id2);
        MyPost postaux;

        if (u2 == null || u1 == null || N <= 0)
            return res;

        Map<MyPost,Integer> aux = new HashMap<>();

        for(long i : u1.getPosts()){
            postaux = this.posts_id.get(i);
            if (postaux != null) {
                if (postaux.getType_id() == 1)
                    aux.put(postaux, 0);
                else if (postaux.getType_id() == 2) {
                    postaux = this.posts_id.get(postaux.getParent_id());
                    if (postaux != null)
                        aux.put(postaux, 0);
                }
            }
        }
        for(long k : u2.getPosts()){
            postaux = this.posts_id.get(k);
            if (postaux != null) {
                if (postaux.getType_id() == 1) {
                    if (aux.containsKey(postaux)) {
                        aux.put(postaux, 1);
                    }
                } else if (postaux.getType_id() == 2) {
                    postaux = this.posts_id.get(postaux.getParent_id());
                    if (postaux != null) {
                        if (aux.containsKey(postaux)) {
                            aux.put(postaux, 1);
                        }
                    }
                }
            }
        }
        Set<MyPost> tree = new TreeSet<>(new compTime());
        for(Map.Entry<MyPost,Integer> a : aux.entrySet()){
            if (a.getValue() == 1)
                tree.add(a.getKey());
        }
        Iterator l = tree.iterator();
        while (l.hasNext() && N > 0){
            postaux = (MyPost) l.next();
            N--;
            res.add(postaux.getId());
        }

        return res;
    }

    // Query 10
    /**
     * Método que dado um id de um post devolve a resposta melhor cotada desse post.
     * @param    id        Id do post
     * @return             ID da respota com melhor pontuacao,retorna -2 caso nao haja nenhuma respota ou o id nao corresponda a uma pergunta e -3 caso o post nao exista.
     */
    public long betterAnswer(long id){
        long res = -2;
        double max = 0, atual;
        MyPost p;

        if (!this.posts_id.containsKey(id))
            return -3;

        for(Long l : this.posts_id.get(id).getFilhos()){
            p = this.posts_id.get(l);
            atual = 0.65*p.getScore() + 0.25*this.users.get(p.getOwner_id()).getRep() + 0.1*p.getComm_count();

            if(atual > max){
                max = atual;
                res = l;
            }
        }

        return res;
    }

    // Query 11
    /**
     * Método que obtém o número de ocorrencias das N tags mais usadas num dado período de tempo pelos N users com maior reputação.
     * @param    N          Número máximo de tags.
     * @param    begin      Início do período de tempo.
     * @param    end        Final do período de tempo.
     * @return              List com as N tags mais usados num dado intervalo de tempo pelos users com mais reputacao.
     */
    public List<Long> mostUsedBestRep(int N, LocalDate begin, LocalDate end){
        Map<Long, Integer> ocorrencias = new HashMap<>();

        if (N > this.users.size())
            N = this.users.size();

        List<Long> res = new ArrayList<>(N);
        if (N <= 0 || begin.isAfter(end))
            return res;

        MyUser uaux;
        long idtag;
        int k;
        List<Long> userids = this.pre_rep.subList(0,N);
        List<MyPost> paux;
        for(long id : userids){
            uaux = this.users.get(id);
            paux = uaux.getPosts().stream().map(a-> this.posts_id.get(a))
                                           .filter(a -> a.getType_id()==1)
                                           .filter(a -> (a.getCdate().isAfter(begin)
                                                && a.getCdate().isBefore(end))
                                                || a.getCdate().equals(begin)
                                                || a.getCdate().equals(end))
                                           .collect(Collectors.toList());
            for(MyPost ps : paux){
                for(String tag : ps.getTags()){
                    idtag = this.tags.get(tag);
                    if (!ocorrencias.containsKey(idtag))
                       ocorrencias.put(idtag,0);
                    k = ocorrencias.get(idtag);
                    ocorrencias.put(idtag,k+1);
                }
            }
        }
        Set<Map.Entry<Long,Integer>> ordaux = new TreeSet<>(new Comparator<Map.Entry<Long, Integer>>() {
            @Override
            public int compare(Map.Entry<Long, Integer> o1, Map.Entry<Long, Integer> o2) {
                if (o1.getValue() > o2.getValue())
                    return -1;
                else if (o1.getValue() < o2.getValue())
                    return 1;
                return o1.getKey() > o2.getKey() ? 1 : -1;
            }
        });

        ordaux.addAll(ocorrencias.entrySet());
        res = ordaux.stream().map(a -> a.getKey()).collect(Collectors.toList());
        List<Long> fim = new ArrayList<>();
        Iterator i = res.iterator();
        while (i.hasNext() && N > 0){
            long o = (long) i.next();
            fim.add(o);
            N--;
        }

        return fim;
    }

    //          NAO SEI SE ESTA MERDA É ASSIM QUE SE FAZ
    /**
     * Método que liberta a memória do objeto.
     */
    public void clear(){
        this.pre_rep.clear();;
        this.users.clear();
        this.posts_id.clear();
        this.posts_date.clear();
        this.pre_posts.clear();
        this.tags.clear();
    }





}
