package project08.interfaces;

/**
 * This interface represents a stack - a data structure that can add elements remove elements from a single end.
 * @param <E> Element type
 */
public interface IStack<E> extends Iterable<E> {

  /**
   * Adds an element to the top of the stack.
   * @param e Element to add
   * @return True if successful, false otherwise (i.e. fixed data structure is full)
   */
  public boolean push(E e);

  /**
   * Removes and returns the element at the top of the stack. Returns null if stack is empty.
   * @return Element at top of the stack, if one exists; null otherwise
   */
  public E pop();

  /**
   * Returns (but does not remove) the element at the top of the stack. Returns null if stack is empty.
   * @return Element at top of the stack, if one exists; null otherwise
   */
  public E peek();

  /**
   * Calculates the size of the stack.
   * @return Number of elements in the stack
   */
  public int size();
}
