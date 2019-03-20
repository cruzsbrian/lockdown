package project08.interfaces;

import edu.caltech.cs2.project08.datastructures.LinkedDeque;
import edu.caltech.cs2.project08.interfaces.ICollection;
import edu.caltech.cs2.project08.interfaces.IDeque;
import edu.caltech.cs2.project08.interfaces.ISet;

public interface IDictionary<K, V> extends Iterable<K> {

    public static class Entry<K, V> {
        public final K key;
        public V value;

        public Entry(K key) {
            this(key, null);
        }

        public Entry(K key, V value) {
            this.key = key;
            this.value = value;
        }
    }

    /**
     * Returns the value to which the specified key is mapped,
     * or {@code null} if this map contains no mapping for the key.
     */
    public V get(K key);

    /**
     * Removes the mapping for the specified key from this map if present.
     *
     * @param  key key whose mapping is to be removed from the map
     * @return the previous value associated with {@code key}, or
     *         {@code null} if there was no mapping for {@code key}.
     */
    public V remove(K key);

    /**
     * Associates the specified value with the specified key in this map.
     * If the map previously contained a mapping for the key, the old
     * value is replaced.
     *
     * @param key key with which the specified value is to be associated
     * @param value value to be associated with the specified key
     * @return the previous value associated with {@code key}, or
     *         {@code null} if there was no mapping for {@code key}.
     */
    public V put(K key, V value);

    /**
     * Returns {@code true} if this map contains a mapping for the
     * specified key.
     *
     * @param   key   The key whose presence in this map is to be tested
     * @return {@code true} if this map contains a mapping for the specified
     * key.
     */
    public boolean containsKey(K key);

    /**
     * Returns {@code true} if this map maps one or more keys to the
     * specified value.
     *
     * @param value value whose presence in this map is to be tested
     * @return {@code true} if this map maps one or more keys to the
     *         specified value
     */
    public boolean containsValue(V value);

    /**
     * Returns the number of key-value mappings in this map.
     *
     * @return the number of key-value mappings in this map
     */
    public int size();


    /**
     * Returns {@code true} if this map contains no key-value mappings.
     *
     * @return {@code true} if this map contains no key-value mappings
     */
    default public boolean isEmpty() {
        return this.size() == 0;
    }

    /**
     * Returns a {@link edu.caltech.cs2.project08.interfaces.ICollection} of the keys contained in this map.
     *
     * @return a collection of the keys contained in this map
     */
    public edu.caltech.cs2.project08.interfaces.ICollection<K> keys();

    /**
     * Returns a {@link edu.caltech.cs2.project08.interfaces.ICollection} of the values contained in this map
     * which does not contain duplicates.
     *
     * @return a collection of the values contained in this map
     */
    public edu.caltech.cs2.project08.interfaces.ICollection<V> values();

    default public ICollection<Entry<K, V>> entrySet() {
        IDeque<Entry<K, V>> entries = new LinkedDeque<>();
        for (K key : keySet()) {
            entries.add(new Entry(key, this.get(key)));
        }
        return entries;
    }

    @SuppressWarnings("unchecked cast")
    default edu.caltech.cs2.project08.interfaces.ISet<K> keySet() {
        return ISet.getBackingSet((IDictionary<K, Object>)this);
    }
}
