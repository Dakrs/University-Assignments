package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

/**
 * Classe onde está definido o comportamento que o SAXParser adota quando efetuar o parsing do Users.xml
 */
public class MyUserHandler extends DefaultHandler {

    private List<MyUser> users = null;
    private Map<Long, Integer> users_rep = null;
    private MyUser u = null;

    /**
     * Get para a lista de users que o handler recolher ao longo do ficheiro xml.
     * @return  Lista de users recolhidos pelo handler
     */
    public List<MyUser> getUsers() {
        return users;
    }

    /**
     * Get para o map que ao longo do ficheiro associa a cada id a reputação do respetivo user
     * @return  Mapa com (Id do user -> Reputação)
     */
    public Map<Long, Integer> getUsers_rep() {
        return users_rep;
    }

    /**
     * Método da interface DefaultHandler, que define o comportamento do parser no início de cada elemento (ou tag)
     */
    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {

        if (users == null)
            users = new LinkedList<MyUser>();

        if (users_rep == null)
            users_rep = new HashMap<Long, Integer>();

        int n;

        if (qName.equalsIgnoreCase("row")) {
            u = new MyUser();

            String str = attributes.getValue("Id");
            if(str != null)
                u.setId(Long.parseLong(str));

            str = attributes.getValue("DisplayName");
            if(str != null)
                u.setUsername(str);

            str = attributes.getValue("Reputation");
            if(str != null) {
                n = Integer.parseInt(str);
                u.setRep(n);
                users_rep.put(u.getId(), n);
            }

            str = attributes.getValue("AboutMe");
            if(str != null)
                u.setBio(str);

        }

    }

    /**
     * Método da interface DefaultHandler, que define o comportamento do parser no fim de cada elemento (ou tag)
     */
    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (qName.equalsIgnoreCase("row")) {
            users.add(u);
        }
    }

}
