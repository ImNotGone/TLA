import java.util.Iterator;
import java.util.function.Function;

public interface Tree<T extends Comparable<? super T>> extends Iterable<T>{

  void insert(T element);

  void remove(T element);

  <E extends Comparable<? super E>> Tree<E> reduce(Function<T, E> function);

  boolean isPresent(T element);

  void addTree(Tree<T> tree);

  T max();

  T min();

  Node<T> root();

  int height();

  // ===== For drawing =====

  void draw();

  void find();

  void inorder();

  void preorder();

  void postorder();

  @Override
  default Iterator<T> iterator() {
    return new BSTInOrderIterator<>(root());
  }
}
