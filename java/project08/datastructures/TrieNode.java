package project08.datastructures;

import java.util.HashMap;

public class TrieNode<A,V> {
    public HashMap<A, TrieNode> pointers = new HashMap<>();
    public V value;
}