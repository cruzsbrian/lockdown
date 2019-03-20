package project08.datastructures;

import edu.caltech.cs2.project08.interfaces.*;

import java.util.Iterator;

public class LinkedDeque<E> implements IDeque<E>, IQueue<E>, IStack<E> {

    class Node {
        E e;
        Node next;
        Node prev;
    }

    class Iter implements Iterator<E> {
        Node current;
        int size;
        int idx = 0;

        public boolean hasNext() {
            return idx < size;
        }

        public E next() {
            E tmp = current.e;
            current = current.next;
            idx++;
            return tmp;
        }
    }

    Node start;
    Node end;
    int size = 0;

    public LinkedDeque() {
        this.start = new Node();
        this.end = new Node();

        this.start.next = this.end;
        this.end.prev = this.start;
    }

    @Override
    public void addFront(E e) {
        Node newNode = new Node();
        newNode.e = e;

        newNode.prev = this.start;
        newNode.next = this.start.next;
        this.start.next.prev = newNode;
        this.start.next = newNode;

        size++;
    }

    @Override
    public void addBack(E e) {
        Node newNode = new Node();
        newNode.e = e;

        newNode.next = this.end;
        newNode.prev = this.end.prev;
        this.end.prev.next = newNode;
        this.end.prev = newNode;

        size++;
    }

    @Override
    public E removeFront() {
        if (size == 0) {
            return null;
        }
        E tmp = this.start.next.e;
        this.start = this.start.next;
        size--;
        return tmp;
    }

    @Override
    public E removeBack() {
        if (size == 0) {
            return null;
        }
        E tmp = this.end.prev.e;
        this.end = this.end.prev;
        size--;
        return tmp;
    }

    @Override
    public boolean enqueue(E e) {
        this.addFront(e);
        return true;
    }

    @Override
    public E dequeue() {
        return this.removeBack();
    }

    @Override
    public boolean push(E e) {
        this.addBack(e);
        return true;
    }

    @Override
    public E pop() {
        return this.removeBack();
    }

    @Override
    public E peek() {
        if (this.size == 0) {
            return null;
        }
        return this.end.prev.e;
    }

    @Override
    public E peekFront() {
        if (this.size == 0) {
            return null;
        }
        return this.start.next.e;
    }

    @Override
    public E peekBack() {
        if (this.size == 0) {
            return null;
        }
        return this.end.prev.e;
    }

    @Override
    public Iterator<E> iterator() {
        Iter iter = new Iter();
        iter.current = this.start.next;
        iter.size = this.size;
        return iter;
    }

    @Override
    public int size() {
        return this.size;
    }

    public String toString() {
        String result = "[";

        Node current = this.start.next;

        while (current != this.end && current.next != this.end) {
            result += current.e.toString() + ", ";
            current = current.next;
        }

        if (size != 0) {
            result += current.e.toString();
        }

        return result + "]";
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof LinkedDeque)) {
            return false;
        }

        LinkedDeque<E> other = (LinkedDeque<E>) o;

        if (other.size() != this.size()) {
            return false;
        }

        Node current = this.start.next;
        Node otherNode = other.start.next;

        while (current != this.end) {
            if (!current.e.equals(otherNode.e)) {
                return false;
            }

            current = current.next;
            otherNode = otherNode.next;
        }

        return true;
    }
}

