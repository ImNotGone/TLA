import java.util.Iterator;
import java.util.function.Function;

public abstract class Tree<T extends Comparable<? super T>> implements Iterable<T>{

  protected Node<T> root;

  abstract void insert(T element);

  abstract void remove(T element);

  abstract <E extends Comparable<? super E>> Tree<E> reduce(Function<T, E> function);

  public void addTree(Tree<T> tree) {
    for (T element : tree) {
      this.insert(element);
    }
  }

  abstract T max();

  abstract T min();

  public Node<T> root(){
    return root;
  }

  public int height() {
    return heightFromNode(root);
  }

  // ===== For drawing =====

  abstract void draw();

  abstract void find();

  public void inorder() {
    getInorderFromNode(root);
    System.out.println();
  }

  public void preorder() {
    getPreorderFromNode(root);
    System.out.println();
  }

  public void postorder() {
    getPostorderFromNode(root);
    System.out.println();
  }

  @Override
  public Iterator<T> iterator() {
    return new BSTInorderIterator<>(root());
  }

  public boolean isPresent(T element) {
    if(this.root == null)
      return false;
    else
      return this.contains(this.root, element);
  }

  private boolean contains(Node<T> node, T element) {
    boolean found = false;
    while (node != null && !found) {
      if(node.getData().equals(element))
        found = true;
      else if(node.getData().compareTo(element) > 0)
        node = node.getLeft();
      else
        node = node.getRight();
    }
    return found;
  }

  // A utility function to print preorder traversal of
  // the tree. The function also prints height of every
  // node
  private void getPreorderFromNode(Node<T> node) {
    if (node != null) {
      System.out.print(node.getData() + " ");
      getPreorderFromNode(node.getLeft());
      getPreorderFromNode(node.getRight());
    }
  }

  private void getPostorderFromNode(Node<T> currentNode) {
    if (currentNode != null) {
      getPostorderFromNode(currentNode.getLeft());
      getPostorderFromNode(currentNode.getRight());
      System.out.print(currentNode.getData() + " ");
    }
  }

  private void getInorderFromNode(Node<T> currentNode) {
    if (currentNode != null) {
      getInorderFromNode(currentNode.getLeft());
      System.out.print(currentNode.getData()  + " ");
      getInorderFromNode(currentNode.getRight());
    }
  }

  // A utility function to get height of the tree
  protected int heightFromNode(Node<T> N) {
    if (N == null)
      return 0;
    return N.getH();
  }
}
