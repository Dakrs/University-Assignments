package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.text.SimpleDateFormat;
import java.time.LocalDate;
import java.util.*;
import java.time.ZoneId;

/**
 * Classe onde está definido o comportamento que o SAXParser adota quando efetuar o parsing do Posts.xml
 */
public class MyPostHandler extends DefaultHandler {

    private List<MyPost> posts = null;
    private Map<Long, Integer> num_posts;
    private Map<Long, List<Long>> user_posts;
    private MyPost p = null;

    /**
     * Get para a lista de posts que o handler recolher ao longo do ficheiro xml.
     * @return  Lista de posts recolhidos pelo handler
     */
    public List<MyPost> getPosts() {
        return posts;
    }

    /**
     * Get para o map que ao longo do ficheiro associa a cada id o número de respostas ao respetivo post
     * @return  Mapa com (Id do post -> Nº de respostas ao post)
     */
    public Map<Long, Integer> getNum_posts() {
        return num_posts;
    }

    /**
     * Get para o map que ao longo do ficheiro associa a cada id o número de posts do respetivo user
     * @return  Mapa com (Id do user -> Nº de posts criados)
     */
    public Map<Long, List<Long>> getUser_posts() {
        return user_posts;
    }

    /**
     * Método da interface DefaultHandler, que define o comportamento do parser no início de cada elemento (ou tag)
     */
    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {

        if (posts == null)
            posts = new LinkedList<MyPost>();

        if (num_posts == null)
            num_posts = new HashMap<Long, Integer>();

        if (user_posts == null)
            user_posts = new HashMap<Long, List<Long>>();


        if (qName.equalsIgnoreCase("row")) {

            long n;
            int count;
            List<Long> longs;
            p = new MyPost();

            String str = attributes.getValue("Id");
            if(str != null)
                p.setId(Long.parseLong(str));

            str = attributes.getValue("PostTypeId");
            if(str != null)
                p.setType_id(Integer.parseInt(str));

            str = attributes.getValue("AnswerCount");
            if(str != null)
                p.setAns_count(Integer.parseInt(str));

            str = attributes.getValue("CommentCount");
            if(str != null)
                p.setComm_count(Integer.parseInt(str));

            str = attributes.getValue("FavoriteCount");
            if(str != null)
                p.setFav_count(Integer.parseInt(str));

            str = attributes.getValue("Score");
            if(str != null)
                p.setScore(Integer.parseInt(str));

            str = attributes.getValue("ParentId");
            if(str != null)
                p.setParent_id(Long.parseLong(str));

            str = attributes.getValue("OwnerUserId");
            if(str != null) {
                n = Long.parseLong(str);

                p.setOwner_id(n);
                count = num_posts.containsKey(n) ? num_posts.get(n) : 0;
                if (p.getType_id() == 1 || p.getType_id() == 2)
                    num_posts.put(n, count + 1);

                longs = user_posts.containsKey(n) ? user_posts.get(n) : new LinkedList<Long>();
                longs.add(p.getId());
                user_posts.put(n, longs);
            }

            str = attributes.getValue("OwnerDisplayName");
            if(str != null)
                p.setOwner_name(str);

            str = attributes.getValue("Title");
            if(str != null)
                p.setTitle(str);

            str = attributes.getValue("CreationDate");
            if(str != null)
                p.setCdate(parseDate(str));

            str = attributes.getValue("Tags");
            if(str != null)
                p.setTags(parseTags(str));

        }

    }

    /**
     * Método da interface DefaultHandler, que define o comportamento do parser no fim de cada elemento (ou tag)
     */
    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (qName.equalsIgnoreCase("row")) {
            posts.add(p);
        }
    }

    /**
     * Método utilitário para processar datas numa string com o formato "AAAA-MM-DD..."
     * @return LocalDate correspondente ao post
     */
    public LocalDate parseDate(String str){
        if(str == null)
            return null;

        StringBuilder sb = new StringBuilder();
        int year, day, month;
        sb.append(str.charAt(0));
        sb.append(str.charAt(1));
        sb.append(str.charAt(2));
        sb.append(str.charAt(3));
        year = Integer.parseInt(sb.toString());
        sb.setLength(0);

        sb.append(str.charAt(5));
        sb.append(str.charAt(6));
        month = Integer.parseInt(sb.toString());
        sb.setLength(0);

        sb.append(str.charAt(8));
        sb.append(str.charAt(9));
        day = Integer.parseInt(sb.toString());

        LocalDate d = LocalDate.of(year, month, day);
        return d;
    }

    /**
     * Método utilitário para processar o atributo Tags e devolver um conjunto de strings com as tags correspondentes
     * @return Set<String> com as tags de um post
     */
    public Set<String> parseTags(String str){
        if(str == null)
            return null;

        StringBuilder buffer = new StringBuilder();
        HashSet<String> set = new HashSet<String>();
        boolean flag = false;

        for(int i=0;i<str.length();i++) {
            if(str.charAt(i) == '<')
                buffer.setLength(0);
            else if(str.charAt(i) == '>') {
                set.add(buffer.toString());
                flag = true;
            }
            else
                buffer.append(str.charAt(i));
        }

        if(flag)
            return set;
        return null;
    }

}
