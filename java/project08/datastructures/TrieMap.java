package project08.datastructures;

import edu.caltech.cs2.project08.datastructures.LinkedDeque;
import edu.caltech.cs2.project08.interfaces.*;

import java.util.Iterator;
import java.util.function.Function;

public class TrieMap<A, K extends Iterable<A>, V> extends ITrieMap<A, K, V> {

    protected TrieNode<A, V> root;
    private int size;

    public TrieMap(Function<IDeque<A>, K> collector) {
        super(collector);
        this.root = new TrieNode();
        this.size = 0;
    }

    @Override
    public boolean isPrefix(K key) {
        TrieNode<A, V> current = this.root;

        for (A letter : key) {
            if (current.pointers.containsKey(letter)) {
                current = current.pointers.get(letter);
            } else {
                return false;
            }
        }
        return current.value != null || current.pointers.size() > 0;
    }

    @Override
    public IDeque<V> getCompletions(K prefix) {
        IDeque<V> result = new edu.caltech.cs2.project08.datastructures.LinkedDeque<>();

        TrieNode<A, V> current = this.root;

        for (A letter : prefix) {
            if (current.pointers.containsKey(letter)) {
                current = current.pointers.get(letter);
            } else {
                return result;
            }
        }

        return getDescendantsValues(current);
    }

    protected IDeque<V> getDescendantsValues(TrieNode<A, V> node) {
        IDeque<V> result = new edu.caltech.cs2.project08.datastructures.LinkedDeque<>();

        if (node.value != null) {
            result.add(node.value);
        }

        for (TrieNode child : node.pointers.values()) {
            result.addAll(getDescendantsValues(child));
        }

        return result;
    }

    @Override
    public V get(K key) {
        TrieNode<A, V> current = this.root;

        for (A letter : key) {
            if (current.pointers.containsKey(letter)) {
                current = current.pointers.get(letter);
            } else {
                return null;
            }
        }
        return current.value;
    }

    @Override
    public V remove(K key) {
        if (!this.containsKey(key)) {
            return null;
        }

        V tmp = this.get(key);

        if (remove(key.iterator(), this.root)) {
            this.root = new TrieNode<>();
        }

        size--;

        return tmp;
    }

    protected boolean remove(Iterator<A> key, TrieNode<A, V> node) {
       if (!key.hasNext()) { //we've reached the end of the key
            node.value = null; //we set the value of the node to null
            return node.pointers.size() == 0;
        }

        A letter = key.next();                          //go deeper
        if (remove(key, node.pointers.get(letter))) {   //remove if safe
            node.pointers.remove(letter);               //but only that pointer
        }
        return node.pointers.size() == 0 && node.value == null;                                   //not safe to remove
    }

    @Override
    public V put(K key, V value) {
        TrieNode<A, V> current = this.root;


        if (!this.containsKey(key)) {
            size++;
        }

        for (A letter : key) {
            if (current.pointers.containsKey(letter)) {
                current = current.pointers.get(letter);
            } else {
                TrieNode<A, V> tn = new TrieNode();
                current.pointers.put(letter, tn);
                current = tn;
            }
        }

        current.value = value;
        return current.value;
    }

    @Override
    public boolean containsKey(K key) {
        TrieNode<A, V> current = this.root;

        for (A letter : key) {
            if (current.pointers.containsKey(letter)) {
                current = current.pointers.get(letter);
            } else {
                return false;
            }
        }

        return current.value != null;
    }

    @Override
    public boolean containsValue(V value) {
        return getDescendantsValues(this.root).contains(value);
    }

    @Override
    public int size() {
        return this.size;
    }

    @Override
    public ICollection<K> keys() {
        IDeque<K> result = new edu.caltech.cs2.project08.datastructures.LinkedDeque<>();
        getKeys(this.root, new LinkedDeque<A>(), result);

        return result;
    }

    protected void getKeys(TrieNode<A, V> node, IDeque<A> partial, IDeque<K> keys) {
        if (node.value != null) {
            keys.add(collector.apply(partial));
        }
        for (A letter : node.pointers.keySet()) {
            partial.addBack(letter);
            getKeys(node.pointers.get(letter), partial, keys);
            partial.removeBack();
        }
    }

    @Override
    public ICollection<V> values() {
        return getDescendantsValues(this.root);
    }

    @Override
    public Iterator<K> iterator() {
        return keySet().iterator();
    }
}