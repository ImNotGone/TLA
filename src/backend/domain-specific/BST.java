public class BST<T extends Comparable<? super T>> implements Tree{
    private Node<T> root;

    public BST() {
       this.root = null;
    }

    public BST(Node<T> root) {
       this.root = root;
    }

    @Override
    public void insert(T element) {
       root = recursiveAddNode(root,element);
    }

    private Node<T> recursiveAddNode(Node<T> node, T data) {
            if(node == null) {
                node = new Node<T>(data);
                return node;
            }
            if(data.compareTo(node.getData()) < 0) {
                node.setLeft(recursiveAddNode(node.getLeft(),data));
            } else if(data.compareTo(node.getData()) > 0){
                node.setRight(recursiveAddNode(node.getRight(),data));
            } else {
                return node;
            }
            return node;
    }

    @Override
    public void remove(T element) {
       root = recursiveDeleteNode(root, element);
    }

    private Node<T> recursiveDeleteNode(Node<T> root, T data)  {
            if (root == null)
                return null;
            if (data.compareTo(root.getData()) < 0)
                root.setLeft(recursiveDeleteNode(root.getLeft(), data));
            else if (data.compareTo(root.getData()) > 0)
                root.setRight(recursiveDeleteNode(root.getRight(), data));
            else  {
                if (root.getLeft() == null)
                    return root.getRight();
                else if (root.getRight() == null)
                    return root.getLeft();
                root.setData(minValue(root.getRight()));
                root.setRight(recursiveDeleteNode(root.getRight(),data));
            }
            return root;
        }

    @Override
    public T min(){
        return minValue(root);
    }

    @Override
    public T max(){
        return maxValue(root);
    }

    @Override
    public T root(){
        return root.getData();
    }

    private T minValue(Node<T> root)  {
         T minval = root.getData();
         while (root.getLeft() != null)  {
           minval = root.getLeft().getData();
           root = root.getLeft();
        }
        return minval;
    }

    private T maxValue(Node<T> root)  {
             T maxval = root.getData();
             while (root.getRight() != null)  {
               maxval = root.getRight().getData();
               root = root.getRight();
            }
            return maxval;
        }


}