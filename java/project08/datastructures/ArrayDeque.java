package project08.datastructures;

import edu.caltech.cs2.project08.interfaces.*;

import java.util.Iterator;

public class ArrayDeque<E> implements IDeque<E>, IQueue<E>, IStack<E> {

    private E[] data;
    private static final int defaultCapacity = 10;
    private static final int grow = 2;
    private int size = 0;

    private int front;
    private int back;

    public ArrayDeque(int initialCapacity) {
        // Initializes the ArrayDeque with the initial capacity of initialCapacity plus two.
        this.data = (E[])new Object[initialCapacity + grow];
        front = this.data.length/2 + 1;
        back = this.data.length/2;
    }

    public ArrayDeque() {
        this(defaultCapacity);
    }

    @Override
    public String toString() {
        if (size == 0) return "[]";

        StringBuilder b = new StringBuilder().append("[");
        for (int i = front; i <= back; i++) b.append(data[i]).append(", ");
        String str = b.toString();
        return str.substring(0,str.length()-2) + "]";
    }

    private void grow() {
        int newCapacity = data.length * grow;
        E[] newData = (E[]) new Object[newCapacity];
        System.arraycopy(data, front, newData, data.length/2, size);
        front = data.length/2;
        back = front + size - 1;
        data = newData;
    }

    @Override
    public void addFront(E e) {
        if (front - 1 < 0) grow();

        front--;
        size++;
        data[front] = e;
    }

    @Override
    public void addBack(E e) {
        if (back + 1 == data.length) grow();

        back++;
        size++;
        data[back] = e;
    }

    @Override
    public E removeFront() {
        if (size == 0) return null;

        E val = data[front];
        data[front] = null;
        front++;
        size--;

        return val;
    }

    @Override
    public E removeBack() {
        if (size == 0) return null;

        E val = data[back];
        data[back] = null;
        back--;
        size--;

        return val;
    }

    @Override
    public boolean enqueue(E e) {
        addBack(e);
        return true;
    }

    @Override
    public E dequeue() {
        return removeFront();
    }

    @Override
    public boolean push(E e) {
        addFront(e);
        return true;
    }

    @Override
    public E pop() {
        return removeFront();
    }

    @Override
    public E peek() {
        return peekFront();
    }

    @Override
    public E peekFront() {
        return data[front];
    }

    @Override
    public E peekBack() {
        return data[back];
    }

    @Override
    public Iterator<E> iterator() {
        return new Iterator<>() {

            int position = front;
            @Override
            public boolean hasNext() {
                return position <= back;
            }

            @Override
            public E next() {
                E d = data[position];
                position ++;
                return d;
            }
        };
    }

    @Override
    public int size() {
        return size;
    }
}
