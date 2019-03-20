package project08.datastructures;

import edu.caltech.cs2.project08.datastructures.LinkedDeque;
import edu.caltech.cs2.project08.interfaces.*;

import java.util.Iterator;

public class MoveToFrontDictionary<K, V> implements IDictionary<K,V> {

    private class Node {
        K key;
        V val;
        Node next;
        Node prev;
    }

    class Iter implements Iterator<K> {
        Node current;
        int size;
        int idx = 0;

        public boolean hasNext() {
            return idx < size;
        }

        public K next() {
            K tmp = current.key;
            current = current.next;
            idx++;
            return tmp;
        }
    }

    private Node start;
    private Node end;
    private int size = 0;

    public MoveToFrontDictionary() {
        this.start = new Node();
        this.end = new Node();
        this.start.next = this.end;
        this.end.prev = this.start;
    }

    // finds node with key, removes it and patches list
    protected Node grabNode(K key) {
        Node current = start.next;

        while (current != this.end) {
            if (current.key.equals(key)) {
                current.prev.next = current.next;
                current.next.prev = current.prev;

                size--;

                return current;
            }

            current = current.next;
        }

        return null;
    }

    @Override
    public V get(K key) {
        Node result = grabNode(key);

        if (result == null) {
            return null;
        }

        result.next = this.start.next;
        this.start.next.prev = result;
        this.start.next = result;
        result.prev = this.start;

        size++;

        return result.val;
    }

    @Override
    public V remove(K key) {
        Node result = grabNode(key);

        if (result == null) {
            return null;
        }

        return result.val;
    }

    @Override
    public V put(K key, V value) {
        if (containsKey(key)) {
            get(key);
            this.start.next.val = value;
        } else {
            Node newNode = new Node();
            newNode.key = key;
            newNode.val = value;

            newNode.next = this.start.next;
            this.start.next.prev = newNode;
            this.start.next = newNode;
            newNode.prev = this.start;

            size++;
        }
        return value;
    }

    @Override
    public boolean containsKey(K key) {
        Node current = this.start.next;

        while (current != this.end) {
            if (current.key.equals(key)) {
                return true;
            }

            current = current.next;
        }

        return false;
    }

    @Override
    public boolean containsValue(V value) {
        Node current = this.start.next;

        while (current != this.end) {
            if (current.val.equals(value)) {
                return true;
            }

            current = current.next;
        }

        return false;
    }

    @Override
    public int size() {
        return this.size;
    }

    @Override
    public ICollection<K> keys() {
        IDeque<K> result = new edu.caltech.cs2.project08.datastructures.LinkedDeque<>();
        Node current = this.start.next;

        while (current != this.end) {
            result.add(current.key);
            current = current.next;
        }

        return result;
    }

    @Override
    public ICollection<V> values() {
        IDeque<V> result = new LinkedDeque<>();
        Node current = this.start.next;

        while (current != this.end) {
            result.add(current.val);
            current = current.next;
        }

        return result;
    }

    @Override
    public Iterator<K> iterator() {
        Iter iter = new Iter();
        iter.current = this.start.next;
        iter.size = this.size;
        return iter;
    }
}
