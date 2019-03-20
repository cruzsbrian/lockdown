package project08.datastructures;

import edu.caltech.cs2.project08.datastructures.ArrayDeque;
import edu.caltech.cs2.project08.datastructures.MoveToFrontDictionary;
import edu.caltech.cs2.project08.interfaces.*;

import java.util.Iterator;
import java.util.function.Supplier;

public class ChainingHashDictionary<K, V> implements IDictionary<K, V> {

    private Supplier<IDictionary<K, V>> chain;

    private IDictionary<K, V>[] table;
    private int currentSizeIndex = 0;
    private int[] sizes = {2, 5, 11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437, 102877};
    private int size = 0;

    public ChainingHashDictionary(Supplier<IDictionary<K, V>> suppDude) {
        this.table = new IDictionary[sizes[currentSizeIndex]];

        for (int ii = 0; ii < table.length; ii++) {
            table[ii] = new edu.caltech.cs2.project08.datastructures.MoveToFrontDictionary<>();
        }
    }

    @Override
    public V get(K key) {
        int index = Math.abs(key.hashCode() % table.length);
        return table[index].get(key);
    }

    @Override
    public V remove(K key) {
        if (containsKey(key)) {
            size--;
        }
        int index = Math.abs(key.hashCode() % table.length);

        return table[index].remove(key);
    }

    @Override
    public V put(K key, V value) {
        if (!containsKey(key)) {
            size++;
        }

        if (this.size() == table.length) {
            currentSizeIndex++;
            IDictionary<K, V>[] newTable = new IDictionary[table.length * 2];

            for (int ii = 0; ii < newTable.length; ii++) {
                newTable[ii] = new MoveToFrontDictionary<>();
            }

            for (K aKey : this.keySet()) {
                int newIndex = Math.abs(aKey.hashCode() % newTable.length);
                newTable[newIndex].put(aKey, this.get(aKey));
            }

            this.table = newTable;
        }

        int index = Math.abs(key.hashCode() % table.length);
        table[index].put(key, value);


        return value;
    }

    @Override
    public boolean containsKey(K key) {
        int index = Math.abs(key.hashCode() % table.length);
        return table[index].containsKey(key);
    }

    @Override
    public boolean containsValue(V value) {
        for (int ii = 0; ii < table.length; ii++) {
            if (table[ii].containsValue(value)){
                return true;
            }
        }
        return false;
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public ICollection<K> keys() {
        IDeque<K> result = new edu.caltech.cs2.project08.datastructures.ArrayDeque<>();

        for (int ii = 0; ii < table.length; ii++) {
            result.addAll(table[ii].keys());
        }

        return result;
    }

    @Override
    public ICollection<V> values() {
        IDeque<V> result = new ArrayDeque<>();

        for (int ii = 0; ii < table.length; ii++) {
            result.addAll(table[ii].values());
        }

        return result;
    }

    @Override
    public Iterator<K> iterator() {
        return this.keys().iterator();
    }
}
