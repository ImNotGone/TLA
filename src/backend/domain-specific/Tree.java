import java.util.function.Function;

public interface Tree<T> {

  void insert(T element);

  void remove(T element);

  <E> Tree<E> reduce(Function<T, E> function);

  boolean isPresent(T element);

  void addTree(Tree<T> tree);

  T max();

  T min();

  T root();

  int height();

  // ===== For drawing =====

  void draw();

  void find();

  void inorder();

  void preorder();

  void postorder();
}
