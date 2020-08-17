package engine;

import java.util.Comparator;

/**
 * Classe comparator para comparar posts com base no score.
 */
public class compScore implements Comparator<MyPost> {

    @Override
    public int compare(MyPost o1,MyPost o2){
        if (o1.getScore() > o2.getScore())
            return -1;
        else if (o1.getScore() < o2.getScore())
            return 1;

        if (o1.getId() == o2.getId())
            return 0;
        return o1.getId() > o2.getId() ? -1 : 1;
    }
}
