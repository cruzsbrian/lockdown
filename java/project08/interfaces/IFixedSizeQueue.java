package project08.interfaces;

import edu.caltech.cs2.project08.interfaces.IQueue;

/**
 * This interface represents a fixed-size queue - a queue with a constant and finite capacity.
 * @param <E> Element type
 */
public interface IFixedSizeQueue<E> extends IQueue<E> {

  /**
   * Checks whether the fixed-size queue is full.
   * @return True if full, false otherwise
   */
  public boolean isFull();

  /**
   * Calculates the maximum capacity of the queue.
   * @return Maximum capacity of the queue
   */
  public int capacity();
}