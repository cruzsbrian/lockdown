package project08.interfaces;

/**
 * This interface represents a queue - a data structure that can add elements at one end and remove them from the other.
 * @param <E> Element type
 */
public interface IQueue<E> extends Iterable<E> {
  /**
   * Adds an element to the back of the queue.
   * @param e Element to add
   * @return True if successful, false otherwise (i.e. fixed data structure is full)
   */
  public boolean enqueue(E e);

  /**
   * Removes and returns the element at the front of the queue. Returns null if queue is empty.
   * @return Element at front of queue, if one exists; null otherwise
   */
  public E dequeue();

  /**
   * Returns (but does not remove) the element at the front of the queue. Returns null if queue is empty.
   * @return Element at front of queue, if one exists; null otherwise
   */
  public E peek();

  /**
   * Calculates the size of the queue.
   * @return Number of elements in the queue
   */
  public int size();
}
