package project08.interfaces;


import edu.caltech.cs2.project08.interfaces.IDictionary;

import java.util.Iterator;

public class ISet<E> implements Iterable<E> {

    private edu.caltech.cs2.project08.interfaces.IDictionary<E, Object> internalDictionary;

    protected ISet(edu.caltech.cs2.project08.interfaces.IDictionary<E, Object> dict) {
        this.internalDictionary = dict;
    }

    public static <E> ISet<E> getBackingSet(IDictionary<E, Object> dict) {
        return new ISet<E>(dict);
    }

    /**
     * Returns the number of elements in this set (its cardinality).  If this
     * set contains more than {@code Integer.MAX_VALUE} elements, returns
     * {@code Integer.MAX_VALUE}.
     *
     * @return the number of elements in this set (its cardinality)
     */
    public int size() {
        return this.internalDictionary.size();
    }

    /**
     * Returns {@code true} if this set contains no elements.
     *
     * @return {@code true} if this set contains no elements
     */
    public boolean isEmpty() {
        return this.internalDictionary.isEmpty();
    }

    /**
     * Returns {@code true} if this set contains the specified element.
     * More formally, returns {@code true} if and only if this set
     * contains an element {@code e} such that
     * {@code Objects.equals(o, e)}.
     *
     * @param e element whose presence in this set is to be tested
     * @return {@code true} if this set contains the specified element
     */
    public boolean contains(E e) {
        return this.internalDictionary.containsKey(e);
    }

    /**
     * Returns an iterator over the elements in this set.  The elements are
     * returned in no particular order (unless this set is an instance of some
     * class that provides a guarantee).
     *
     * @return an iterator over the elements in this set
     */
    @Override
    public Iterator<E> iterator() {
        return this.internalDictionary.iterator();
    }


    // Modification Operations

    /**
     * Adds the specified element to this set if it is not already present
     * (optional operation).  More formally, adds the specified element
     * {@code e} to this set if the set contains no element {@code e2}
     * such that
     * {@code Objects.equals(e, e2)}.
     * If this set already contains the element, the call leaves the set
     * unchanged and returns {@code false}.  In combination with the
     * restriction on constructors, this ensures that sets never contain
     * duplicate elements.
     *
     * @param e element to be added to this set
     * @return {@code true} if this set did not already contain the specified
     *         element
     */
    public boolean add(E e) {
        return this.internalDictionary.put(e, new Object()) != null;
    }


    /**
     * Removes the specified element from this set if it is present
     * (optional operation).  More formally, removes an element {@code e}
     * such that
     * {@code Objects.equals(o, e)}, if
     * this set contains such an element.  Returns {@code true} if this set
     * contained the element (or equivalently, if this set changed as a
     * result of the call).  (This set will not contain the element once the
     * call returns.)
     *
     * @param e object to be removed from this set, if present
     * @return {@code true} if this set contained the specified element
     */
    public boolean remove(E e) {
        return this.internalDictionary.remove(e) != null;
    }

    /**
     * Removes all of the elements from this set (optional operation).
     * The set will be empty after this call returns.
     *
     * @throws UnsupportedOperationException if the {@code clear} method
     *         is not supported by this set
     */
    public void clear() {
        throw new UnsupportedOperationException();
    }

    @Override
    public String toString() {
        StringBuilder b = new StringBuilder();
        for (E e : this) {
            b.append(e + ", ");
        }
        if  (b.length() > 0) {
            b.delete(b.length() - 2,  b.length());
        }
        return "[" + b.toString() + "]";
    }
}
