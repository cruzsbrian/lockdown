package project08.datastructures;

import edu.caltech.cs2.project08.datastructures.ChainingHashDictionary;
import edu.caltech.cs2.project08.datastructures.MoveToFrontDictionary;
import edu.caltech.cs2.project08.interfaces.*;


public class ChainingHashSet<E> extends ISet<E> {
    public ChainingHashSet() {
        super(new ChainingHashDictionary<>(MoveToFrontDictionary::new));
    }

    public ChainingHashSet(ICollection<E> c) {
        this();
        for (E x : c) {
            this.add(x);
        }
    }
}
