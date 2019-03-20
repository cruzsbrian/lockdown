package project08.bots;

import edu.caltech.cs2.project08.bots.BestMove;
import edu.caltech.cs2.project08.game.*;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

public abstract class AbstractSearcher<B extends Board> implements Searcher<B> {
    protected Evaluator<B> evaluator;
    protected Timer timer;
    protected int ply;
    protected int cutoff;
    private edu.caltech.cs2.project08.bots.BestMove bestMove;
    private PropertyChangeSupport bestMovePropertyChange;

    public AbstractSearcher() {
        setTimer(new SimpleTimer(180000));
    }

    public void setEvaluator(Evaluator<B> e) {
        evaluator = e;
    }

    public void setDepth(int depth) {
        this.ply = depth;
    }

    public void setCutoff(int cutoff) {
        this.cutoff = cutoff;
    }

    public void setTimer(Timer t) {
        this.timer = t;
    }

    protected void reportNewBestMove(BestMove move) {
        this.bestMovePropertyChange.firePropertyChange("bestMove", this.bestMove, move);
        this.bestMove = move;
    }

    public void addOnBestMoveListener(PropertyChangeListener listener) {
        this.bestMovePropertyChange.addPropertyChangeListener(listener);
    }

    public void removeOnBestMoveListener(PropertyChangeListener listener) {
        this.bestMovePropertyChange.removePropertyChangeListener(listener);
    }
}