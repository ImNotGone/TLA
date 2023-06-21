import java.awt.*;

public class Node<T extends Comparable<? super T>>{

        private T data;
    	private Node<T> left;
    	private Node<T> right;
    	private int h; //altura en la que se encuentra el nodo
    	//Para el RBT
    	private Node<T> parent;
    	private Color color;

        public Node(T data){
            this.data=data;
            this.color=Color.BLACK;
            this.left=null;
            this.right=null;
            this.h=0;
        }

        public Node() {
                this.data = null;
                this.h = 0;
                this.color = Color.BLACK;
                this.left = null;
                this.right = null;
            }

    	public T getData() {
            return data;
        }

        public void setData(T data){
            this.data=data;
        }

        public Node<T> getLeft() {
        	return left;
        }


        public Node<T> getRight() {
        	return right;
        }

        public Node<T> getParent() {
                	return parent;
                }

        public void setLeft(Node<T> left) {
            this.left = left;
        }

        public void setRight(Node<T> right) {
            this.right = right;
        }

        public void setParent(Node<T> parent) {
                    this.parent = parent;
                }

        public int getH(){
            return h;
        }

        public void setH(int h){
            this.h = h;
        }

        private boolean isLeaf() {
        	return left == null && right == null;
        }

        public Color getColor(){
            return color;
        }

        public void setColor(Color color){
            this.color= color;
        }
}