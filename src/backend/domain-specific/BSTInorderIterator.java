import java.util.Iterator;
import java.util.Stack;

class BSTInOrderIterator<T extends Comparable<? super T>> implements Iterator<T> {
    Stack<Node<T>> stack;
    Node<T> current;

    public BSTInOrderIterator(Node<T> root) {
        stack= new Stack<>();
        current= root;

    }

    @Override
    public boolean hasNext() {
        return ! stack.isEmpty() || current != null;
    }

    @Override
    public T next() {
        while(current != null) {
            stack.push(current);
            current= current.getLeft();
        }

        Node<T> elementToProcess= stack.pop();
        current= elementToProcess.getRight();
        return elementToProcess.getData();
    }
}
