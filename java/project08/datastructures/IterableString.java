package project08.datastructures;

import edu.caltech.cs2.project08.datastructures.LinkedDeque;
import edu.caltech.cs2.project08.interfaces.IDeque;

import java.util.Iterator;

public class IterableString implements CharSequence, Iterable<Character>, Comparable<IterableString> {
    private final String data;

    public IterableString(String data) {
        this.data = data;
    }

    @Override
    public int length() {
        return this.data.length();
    }

    @Override
    public char charAt(int index) {
        return this.data.charAt(index);
    }

    @Override
    public CharSequence subSequence(int start, int end) {
        return this.data.subSequence(start, end);
    }

    @Override
    public String toString() {
        return this.data.toString();
    }

    @Override
    public Iterator<Character> iterator() {
        IDeque<Character> d = new LinkedDeque<>();
        for (char c : this.data.toCharArray()) {
            d.addBack(c);
        }
        return d.iterator();
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof IterableString)){
            return false;
        }
        return this.data.equals(((IterableString)o).data);
    }

    @Override
    public int hashCode() {
        return this.data.hashCode();
    }

    @Override
    public int compareTo(IterableString other) {
        return this.data.compareTo(other.data);
    }

}