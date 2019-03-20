package project08.datastructures;

import edu.caltech.cs2.project08.datastructures.ChainingHashDictionary;
import edu.caltech.cs2.project08.datastructures.MoveToFrontDictionary;
import edu.caltech.cs2.project08.interfaces.*;


import java.util.Iterator;

public class MinFourHeap<E> implements IPriorityQueue<E> {

    private static final int DEFAULT_CAPACITY = 5;
    private static final int GROWTH_FACTOR = 20;

    private int size;
    private PQElement<E>[] data;
    private IDictionary<E, Integer> keyToIndexMap;

    /**
     * Creates a new empty heap with DEFAULT_CAPACITY.
     */
    public MinFourHeap() {
        this.size = 0;
        this.data = new PQElement[DEFAULT_CAPACITY];
        this.keyToIndexMap = new ChainingHashDictionary<>(MoveToFrontDictionary::new);
    }

    private void growArray(int growthSize){
        PQElement<E>[] newArray = new PQElement[this.data.length + growthSize];
        for(int i=0; i<this.data.length; i++){
            newArray[i] = this.data[i];
        }
        this.data = newArray;
    }

    private int getMinChild(int index){
        if(index*4 + 1 >= this.size)
            return -1;
        int minIndex = index*4 + 1;
        double minPriority = this.data[minIndex].priority;
        for(int childIndex=index*4 + 2; childIndex<=(index+1)*4 && childIndex < this.size; childIndex++){
            if(this.data[childIndex].priority < minPriority){
                minIndex = childIndex;
                minPriority = this.data[minIndex].priority;
            }
        }

        return minIndex;
    }

    private void swap(int index1, int index2){
        PQElement<E> old = data[index1];
        data[index1] = data[index2];
        data[index2] = old;

        keyToIndexMap.put(data[index1].data, index1);
        keyToIndexMap.put(data[index2].data, index2);
    }

    private void percolateUp(int index){
        int parentIndex = (index-1)/4;

        while (index > 0 && data[index].priority <= data[parentIndex].priority){
            swap(index, parentIndex);

            if(data[index].priority == data[parentIndex].priority && data[index].data == data[parentIndex].data)
                throw new IllegalArgumentException();

            index = parentIndex;
            parentIndex = (index-1)/4;
        }

    }

    private void percolateDown(int index){
        int childIndex = this.getMinChild(index);

        while (childIndex >= 0 && this.data[childIndex].priority < data[index].priority) {
            swap(index, childIndex);
            index = childIndex;
            childIndex = this.getMinChild(index);
        }
    }

    @Override
    public void increaseKey(PQElement<E> key) {
        Integer index = keyToIndexMap.get(key.data);
        if (index == null)
            throw new IllegalArgumentException();

        if(key.priority > data[index].priority){
            data[index] = key;
            this.percolateDown(index);
        }
    }

    @Override
    public void decreaseKey(PQElement<E> key) {
        Integer index = keyToIndexMap.get(key.data);
        if (index == null)
            throw new IllegalArgumentException();

        if(key.priority < data[index].priority){
            data[index] = key;
            this.percolateUp(index);
        }
    }

    @Override
    public boolean enqueue(PQElement<E> epqElement) {
        if (this.size == data.length - 1)
            this.growArray(GROWTH_FACTOR);

        data[this.size] = epqElement;
        keyToIndexMap.put(epqElement.data, this.size);
        this.percolateUp(this.size++);

        return true;
    }

    @Override
    public PQElement<E> dequeue() {
        if(this.size == 0)
            return null;

        PQElement<E> min = data[0];
        data[0] = data[--this.size];
        data[this.size] = null;
        keyToIndexMap.remove(min.data);
        this.percolateDown(0);

        return min;
    }

    @Override
    public PQElement<E> peek() {
        if (this.size == 0)
            return null;
        return data[0];
    }

    @Override
    public int size() {
        return this.size;
    }

    @Override
    public Iterator<PQElement<E>> iterator() {
        return null;
    }
}