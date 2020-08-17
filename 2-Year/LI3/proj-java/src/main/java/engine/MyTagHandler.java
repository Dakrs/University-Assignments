package engine;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.util.HashMap;
import java.util.Map;



/**
 * Classe onde está definido o comportamento que o SAXParser adota quando efetuar o parsing do Tags.xml
 */
public class MyTagHandler extends DefaultHandler {

    private Map<String, Long> tags = null;

    /**
     * Get para o HashMap de posts que o handler recolhe ao longo do ficheiro xml.
     * @return  HashMap de posts recolhidos pelo handler
     */
    public Map<String, Long> getTags() {
        return tags;
    }

    /**
     * Método da interface DefaultHandler, que define o comportamento do parser no início de cada elemento (ou tag)
     */
    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {

        if (qName.equalsIgnoreCase("row")) {

            String str1 = attributes.getValue("TagName");
            String str2 = attributes.getValue("Id");

            if (tags == null)
                tags = new HashMap<String, Long>();

            if(str1 != null && str2 != null)
                tags.put(str1, Long.parseLong(str2));
        }

    }

    /**
     * Método da interface DefaultHandler, que define o comportamento do parser no fim de cada elemento (ou tag)
     */
    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
    }

}
