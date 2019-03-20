package project08.interfaces;

import edu.caltech.cs2.project08.interfaces.IQueue;

/**
 * This interface represents a Priority Queue - a data structure that is very similar to a queue but
 * stores values in ascending order.
 * @param <E> Element type
 */
public interface IPriorityQueue<E> extends IQueue<IPriorityQueue.PQElement<E>> {
    public static class PQElement<E> {
        public final E data;
        public final double priority;

        public PQElement(E data, double priority) {
            this.data = data;
            this.priority = priority;
        }

        @Override
        public int hashCode() {
            return this.data.hashCode();
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof PQElement)) {
                return false;
            }
            return this.data.equals(((PQElement)o).data);
        }

        @Override
        public String toString() {
            return "(" + this.data + ", " + this.priority + ")";
        }
    }


    /**
     * Increase the priority of the key at idx
     * @param key - the new key with the new priority
     */
    public void increaseKey(PQElement<E> key);

    /**
     * Decrease the priority of the key at idx
     * @param key - the new key with the new priority
     */
    public void decreaseKey(PQElement<E> key);

}