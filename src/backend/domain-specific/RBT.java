// Based on: https://github.com/Bibeknam/algorithmtutorprograms/blob/master/data-structures/red-black-trees/RedBlackTree.java

public class RBT<T extends Comparable<? super T>> implements Tree{

    private Node<T> root;
    private Node<T> TNULL;

    public RedBlackTree() {
    		TNULL = new Node<T>();
    		root = TNULL;
    	}
private void preOrderHelper(Node<T> node) {
		if (node != TNULL) {
			System.out.print(node.getData() + " ");
			preOrderHelper(node.getLeft());
			preOrderHelper(node.getRight());
		}
	}

	private void inOrderHelper(Node<T> node) {
		if (node != TNULL) {
			inOrderHelper(node.getLeft());
			System.out.print(node.getData() + " ");
			inOrderHelper(node.getRight());
		}
	}

	private void postOrderHelper(Node<T> node) {
		if (node != TNULL) {
			postOrderHelper(node.getLeft());
			postOrderHelper(node.getRight());
			System.out.print(node.getData() + " ");
		}
	}

	private Node<T> searchTreeHelper(Node<T> node, T key) {
		if (node == TNULL || key.compareTo( node.getData()) == 0) {
			return node;
		}

		if (key.compareTo( node.getData()) < 0) {
			return searchTreeHelper(node.getLeft(), key);
		}
		return searchTreeHelper(node.getRight(), key);
	}

	// fix the rb tree modified by the delete operation
	private void fixDelete(Node<T> x) {
		Node<T> s;
		while (x != root && x.getColor() == 0) {
			if (x == x.getParent().getLeft()) {
				s = x.getParent().getRight();
				if (s.getColor() == 1) {
					// case 3.1
					s.setColor(0);
					x.getParent().setColor(1);
					leftRotate(x.getParent());
					s = x.getParent().getRight();
				}

				if (s.getLeft().getColor() == 0 && s.getRight().getColor() == 0) {
					// case 3.2
					s.setColor(1);
					x = x.getParent();
				} else {
					if (s.getRight().getColor() == 0) {
						// case 3.3
						s.getLeft().setColor(0);
						s.setColor(1);
						rightRotate(s);
						s = x.getParent().getRight();
					}

					// case 3.4
					s.setColor(x.getParent().getColor());
					x.getParent().setColor(0);
					s.getRight().setColor(0);
					leftRotate(x.getParent());
					x = root;
				}
			} else {
				s = x.getParent().getLeft();
				if (s.getColor == 1) {
					// case 3.1
					s.setColor(0);
					x.getParent().setColor(1);
					rightRotate(x.getParent());
					s = x.getParent().getLeft();
				}

				if (s.getRight().getColor() == 0) {
					// case 3.2
					s.setColor(1);
					x = x.getParent();
				} else {
					if (s.getLeft().getColor() == 0) {
						// case 3.3
						s.getRight().setColor(0);
						s.setColor(1);
						leftRotate(s);
						s = x.getParent().getLeft();
					}

					// case 3.4
					s.setColor(x.getParent().getColor());
					x.getParent().setColor(0);
					s.getLeft().setColor(0);
					rightRotate(x.getParent());
					x = root;
				}
			}
		}
		x.setColor(0);
	}


	private void rbTransplant(Node<T> u, Node<T> v){
		if (u.parent == TNULL) {
			root = v;
		} else if (u == u.getParent().getLeft()){
			u.getParent().setLeft(v);
		} else {
			u.getParent().setRight(v);
		}
		v.setParent(u.getParent());
	}

	private void deleteNodeHelper(Node<T> node, T key) {
		// find the node containing key
		Node<T> z = TNULL;
		Node<T> x, y;
		while (node != TNULL){
			if (node.getData().compareTo(key) == 0) {
				z = node;
			}

			if (node.getData().compareTo(key) <= 0) {
				node = node.getRight();
			} else {
				node = node.getLeft();
			}
		}

		if (z == TNULL) {
			System.out.println("Couldn't find key in the tree");
			return;
		}

		y = z;
		int yOriginalColor = y.getColor();
		if (z.getLeft() == TNULL) {
			x = z.getRight();
			rbTransplant(z, z.getRight());
		} else if (z.getRight() == TNULL) {
			x = z.getLeft();
			rbTransplant(z, z.getLeft());
		} else {
			y = minimum(z.getRight());
			yOriginalColor = y.getColor();
			x = y.getRight();
			if (y.getParent() == z) {
				x.setParent(y);
			} else {
				rbTransplant(y, y.getRight());
				y.setRight(z.getRight());
				y.getRight().setParent(y);
			}

			rbTransplant(z, y);
			y.setLeft(z.getLeft());
			y.getLeft().setParent(y);
			y.setColor(z.getColor());
		}
		if (yOriginalColor == 0){
			fixDelete(x);
		}
	}

	// fix the red-black tree
	private void fixInsert(Node<T> k){
		Node<T> u;
		while (k.getParent().getColor() == 1) {
			if (k.getParent() == k.getParent().getParent().getRight()) {
				u = k.getParent().getParent().getLeft(); // uncle
				if (u.getColor() == 1) {
					// case 3.1
					u.setColor(0);
					k.getParent().setColor(0);
					k.getParent().getParent().setColor(1);
					k = k.getParent().getParent();
				} else {
					if (k == k.getParent().getLeft()) {
						// case 3.2.2
						k = k.getParent();
						rightRotate(k);
					}
					// case 3.2.1
					k.getParent().setColor(0);
					k.getParent().getParent().setColor(1);
					leftRotate(k.getParent().getParent());
				}
			} else {
				u = k.getParent().getParent().getRight(); // uncle

				if (u.getColor == 1) {
					// mirror case 3.1
					u.setColor(0) = 0;
					k.getParent().setColor(0);
					k.getParent().getParent().setColor(1);
					k = k.getParent().getParent();
				} else {
					if (k == k.getParent().getRight()) {
						// mirror case 3.2.2
						k = k.getParent();
						leftRotate(k);
					}
					// mirror case 3.2.1
					k.getParent().setColor(0);
					k.getParent().getParent().setColor(1);
					rightRotate(k.getParent().getParent());
				}
			}
			if (k == root) {
				break;
			}
		}
		root.setColor(0);
	}

	// Pre-Order traversal
	// Node.Left Subtree.Right Subtree
	public void preorder() {
		preOrderHelper(this.root);
	}

	// In-Order traversal
	// Left Subtree . Node . Right Subtree
	public void inorder() {
		inOrderHelper(this.root);
	}

	// Post-Order traversal
	// Left Subtree . Right Subtree . Node
	public void postorder() {
		postOrderHelper(this.root);
	}

	// search the tree for the key k
	// and return the corresponding node
	public Node<T> searchTree(T k) {
		return searchTreeHelper(this.root, k);
	}

	@Override
	public T min(){
        return minimum(root).getData();
	}

	// find the node with the minimum key
	public Node<T> minimum(Node<T> node) {
		while (node.getLeft() != TNULL) {
			node = node.getLeft();
		}
		return node;
	}

	@Override
    	public T max(){
            return maximum(root).getData();
    	}

	// find the node with the maximum key
	public Node<T> maximum(Node<T> node) {
		while (node.getRight() != TNULL) {
			node = node.getRight();
		}
		return node;
	}

	// rotate left at node x
    	public void leftRotate(Node<T> x) {
    		Node<T> y = x.getRight();
    		x.setRight(y.getLeft());
    		if (y.getLeft() != TNULL) {
    			y.getLeft().setParent(x);
    		}
    		y.setParent(x.getParent);
    		if (x.getParent() == TNULL) {
    			this.root = y;
    		} else if (x == x.getParent().getLeft()) {
    			x.getParent().setLeft(y);
    		} else {
    			x.getParent().setRight(y);
    		}
    		y.setLeft(x);
    		x.setParent(y);
    	}

    	// rotate right at node x
    	public void rightRotate(Node<T> x) {
    		Node<T> y = x.getLeft();
    		x.setLeft(y.getRight());
    		if (y.getRight() != TNULL) {
    			y.getRight().setParent(x);
    		}
    		y.setParent(x.getParent());
    		if (x.getParent() == TNULL) {
    			this.root = y;
    		} else if (x == x.getParent().getRight()) {
    			x.getParent().setRight(y);
    		} else {
    			x.getParent().setLeft(y);
    		}
    		y.setRight(x);
    		x.setParent(y);
    	}

    	// insert the key to the tree in its appropriate position
    	// and fix the tree
    	@Override
    	public void insert(T element) {
    		// Ordinary Binary Search Insertion
    		Node<T> node = new Node<T>(element);

    		node.setParent(TNULL);
    		node.setLeft(TNULL);
    		node.setRight(TNULL);
    		node.setColor(1); // new node must be red

    		Node<T> y = TNULL;
    		Node<T> x = this.root;

    		while (x != TNULL) {
    			y = x;

    			if (node.getData().compareTo(x.getData()) < 0) {
    				x = x.getLeft();
    			} else {
    				x = x.getRight();
    			}
    		}

    		// y is parent of x
    		node.setParent(y);
    		if (y == TNULL) {
    			root = node;
    		} else if (node.getData().compareTo(y.getData()) < 0) {
    			y.setLeft(node);
    		} else {
    			y.setRight(node);
    		}

    		// if new node is a root node, simply return
    		if (node.getParent() == TNULL){
    			node.setColor(0);
    			return;
    		}

    		// if the grandparent is null, simply return
    		if (node.getParent().getParent() == TNULL) {
    			return;
    		}

    		// Fix the tree
    		fixInsert(node);
    	}

        @Override
    	public T root(){
    		return this.root.getData();
    	}

    	public void print() {
            BTreePrinter.printNode(root);
        }

    	// delete the node from the tree
    	@Override
    	public void remove(T element) {
    		deleteNodeHelper(this.root, element);
    	}




}