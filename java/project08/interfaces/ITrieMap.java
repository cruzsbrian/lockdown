package project08.interfaces;

import edu.caltech.cs2.project08.interfaces.IDeque;
import edu.caltech.cs2.project08.interfaces.IDictionary;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;

public abstract class ITrieMap<A, K extends Iterable<A>, V> implements IDictionary<K, V> {
    protected TrieNode<A, V> root;
    protected Function<edu.caltech.cs2.project08.interfaces.IDeque<A>, K> collector;
    protected int size;

    public ITrieMap(Function<edu.caltech.cs2.project08.interfaces.IDeque<A>, K> collector) {
        this.collector = collector;
        this.size = 0;
        this.root = new TrieNode<>();
    }

    public abstract boolean isPrefix(K key);
    public abstract IDeque<V> getCompletions(K prefix);

    public void clear() {
        this.size = 0;
        this.root = new TrieNode<>();
    }

    protected static class TrieNode<A, V> {
        public final Map<A, TrieNode<A, V>> pointers;
        public V value;

        public TrieNode() {
            this(null);
        }

        public TrieNode(V value) {
            this.pointers = new HashMap<>();//new ChainingHashDictionary<>(() -> new MoveToFrontDictionary());
            this.value = value;
        }

        @Override
        public String toString() {
            StringBuilder b = new StringBuilder();
            if (this.value != null) {
                b.append("[" + this.value + "]-> {\n");
                this.toString(b, 1);
                b.append("}");
            }
            else {
                this.toString(b, 0);
            }
            return b.toString();
        }

        private String spaces(int i) {
            StringBuilder sp = new StringBuilder();
            for (int x = 0; x < i; x++) {
                sp.append(" ");
            }
            return sp.toString();
        }

        protected boolean toString(StringBuilder s, int indent) {
            Set<A> keySet = this.pointers.keySet();
            boolean isSmall = keySet.isEmpty();

            for (A idx : keySet){
                TrieNode<A, V> node = this.pointers.get(idx);

                if (node == null) {
                    continue;
                }

                V value = node.value;
                s.append(spaces(indent) + idx + (value != null ? "[" + value + "]" : ""));
                s.append("-> {\n");
                boolean bc = node.toString(s, indent + 2);
                if (!bc) {
                    s.append(spaces(indent) + "},\n");
                }
                else if (s.charAt(s.length() - 5) == '-') {
                    s.delete(s.length() - 5, s.length());
                    s.append(",\n");
                }
            }
            if (!isSmall) {
                s.deleteCharAt(s.length() - 2);
            }
            return isSmall;
        }
    }

    @Override
    public String toString() {
        return this.root.toString();
    }
}
