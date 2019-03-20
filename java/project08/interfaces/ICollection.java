package project08.interfaces;

import java.util.Iterator;

/**
 * This interface represents some group or collection of elements.
 * @param <E> Element type
 */
public interface ICollection<E> extends Iterable<E> {

  /**
   * Add an element to the collection.
   * @param e Element to add
   */
  public void add(E e);

  /**
   * Removes all elements from the collection.
   */
  public void clear();

  /**
   * Generates an iterator over the collection.
   * @return Iterator over the collection
   */
  @Override
  public Iterator<E> iterator();

  /**
   * Checks if an element is contained in the collection.
   * @param e Element to check for
   * @return True if element is contained in collection, false otherwise
   */
  default public boolean contains(E e) {
    for (E item : this) {
      if (e == null ? item == null : e.equals(item)) {
        return true;
      }
    }
    return false;
  }

  /**
   * Calculates the size of the collection.
   * @return Number of elements in the collection
   */
  public int size();

  /**
   * Checks whether the collection contains no elements.
   * @return True if no elements are contained, false otherwise
   */
  default public boolean isEmpty() {
    return this.size() == 0;
  }
}
