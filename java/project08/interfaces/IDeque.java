package project08.interfaces;

import edu.caltech.cs2.project08.interfaces.ICollection;

/**
 * This interface represents a deque - a data structure that can add and remove elements from either end of a list.
 * @param <E> Element type
 */
public interface IDeque<E> extends edu.caltech.cs2.project08.interfaces.ICollection<E> {

  /**
   * Add an element to the front end of the deque.
   * @param e Element to add
   */
  public void addFront(E e);

  /**
   * Add an element to the back end of the deque.
   * @param e Element to add
   */
  public void addBack(E e);

  /**
   * Adds an element to the collection.
   * @param e Element to add
   */
  @Override
  default public void add(E e){
    this.addFront(e);
  }

  /**
   * Adds a collection of elements to this collection.
   * @param c Collection of elements to add
   */
  default public void addAll(ICollection<E> c) {
    for (E e : c) {
      this.add(e);
    }
  }

  /**
   * Removes and returns the element at the front end of the deque. Returns null if deque is empty.
   * @return Element at front, if one exists; null otherwise
   */
  public E removeFront();

  /**
   * Removes and returns the element at the back end of the deque. Returns null if deque is empty.
   * @return Element at back, if one exists; null otherwise
   */
  public E removeBack();

  /**
   * Removes all elements in the deque.
   */
  @Override
  default public void clear() {
    while (!this.isEmpty()) {
      this.removeBack();
    }
  }

  /**
   * Returns (but does not remove) the element at one end of the deque. Returns null if deque is empty.
   * Note: The side you peek from should be chosen such that both the IQueue and IStack interfaces are implemented correctly.
   * @return Element at one end, if one exists; null otherwise
   */
  public E peek();

  /**
   * Returns (but does not remove) the element at the front end of the deque. Returns null if deque is empty.
   * @return Element at front, if one exists; null otherwise
   */
  public E peekFront();

  /**
   * Returns (but does not remove) the element at the back end of the deque. Returns null if deque is empty.
   * @return Element at back, if one exists; null otherwise
   */
  public E peekBack();
}
