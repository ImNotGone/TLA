import java.io.IOException;
public class Main {
public static void main(String[] args) throws IOException {
Tree<Integer> t1 = new AVL<>();
Tree<Integer> t2 = new AVL<>();
t2.addTree(t1);
t1.insert((1));
int a;
a = t1.min();
int b = t1.max();
}
}
