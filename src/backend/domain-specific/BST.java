public class BST<T extends Comparable<? super T>> {
    private Node<T> root;

    public BST() {
       this.root = null;
    }

    public BST(Node<T> root) {
       this.root = root;
    }

    public void addNode(T data) {
       root = recursiveAddNode(root,data);
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

    public void deleteNode(T data) {
       root = recursiveDeleteNode(root, data);
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
    private T minValue(Node<T> root)  {
         T minval = root.getData();
         while (root.getLeft() != null)  {
           minval = root.getLeft().getData();
           root = root.getLeft();
        }
        return minval;
    }


}