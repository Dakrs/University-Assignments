package engine;

import java.util.Comparator;


/**
 * Classe comparator para comparar posts com base no answer count.
 */
public class compAnswer implements Comparator<MyPost> {

    @Override
    public int compare(MyPost o1,MyPost o2){
        if (o1.getAns_count() > o2.getAns_count())
            return -1;
        else if (o1.getAns_count() < o2.getAns_count())
            return 1;

        if (o1.getId() == o2.getId())
            return 0;
        return o1.getId() > o2.getId() ? -1 : 1;
    }
}
