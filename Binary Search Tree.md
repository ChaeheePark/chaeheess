**Binary Search Tree**

- Tree representation: A linked data structure in which each node is an object(parent node 보다 작으면 왼쪽, 크면 오른쪽)
- Support many dynamic set operations: search, minimum(제일왼쪽), maximum(제일오른쪽)
- Running time: avg-O(logn), worst-O(n)(하나로이루어진 linked list 형태일때)
- search : Inorder , Preorder, Postorder
- Successor : 해당노드보다 큰 값을 가진 노드 중 가장 작은 노드 - right의 minimum / Predecessor : 해당노드보다 작은 값을 가진 노드 중 가장 큰 노드 - left의 maximum
- Deletion
  - 1) node 'z' has no children : delete z by making the parent of z point to null
  - 2) z has one child : delete z by making the parent of z point to z's child instead of to z
  - 3) z has two children : z's successor 'y' is the minimum node in z's right subtree, y has either no or one child (no left child), so delete y from the tree(1 or 2) Replace z;s key and satellite data with y's!
- Search, Predecessor, Successor, Minimum, Maximum, Insert, Delete - > O(h)=O(logn)
- But worst case running time is O(n) (ascending or descending- > lack of balance)



**AVL TREE**

- lack of balance의 문제를 해결하기 위해 구현된 트리(AVL trees are heighted-balanced binary search trees)
- For every node, heights of left and right subtree can differ by no more than 1 (왼쪽 subtree의 높이 - 오른쪽 subtree의 높이)
- Insertion : if new balance factor ( the difference h(left)- h(right) )is 2 or -2, adjust tree by rotation!
  - 왼쪽으로 트리가 치우친 경우 - LL : 오른쪽으로 회전을 수행한다
  - 오른쪽으로 트리가 치우친 경우 - RR : 왼쪽으로 회전을 수행한다
  - 왼쪽 자식노드에 오른쪽 자식 노드만 있을 경우 - LR : 오른쪽자식노드의 부모노드를 왼쪽으로 회전시킨후, 루트노드를 오른쪽으로 회전시킨다
  - 오른쪽 자식노드에 왼쪽 자식노드만 있을 경우 - RL : 왼쪽 자식노드의 부모노드를 오른쪽으로 회전시킨 후, 루트노드를 왼쪽으로 회전시킨다
- Pros and Cons of AVL Trees 
  - Pros :  Search is O(logn) since AVL trees are always balanced, Insertion and Deletion are also O(logn)!
  - Cons: Difficult to program and debug\



 **B-TREES**

- main memory에 datasets 를 다 저장하기 어려울 때 모든 data를 같은 level에 놓고 많은 양의 데이터를 읽기 쉬운 tree모양으로 만든다 (use more branches and thus reduce the height of the tree, reduce the disk accesses time)
- Main idea: Height Reduction , instead of binary trees, use m-ary search trees (m>2) 
  - ex) 3 - way search tree (2-3 tree), 2는 key의 수, 3는 branch수
- Definition: A B-tree of order m is an m-way tree in which 
  - 1) the number of keys in each non-leaf node is one less than the number of its children
  - 2) all leaves are on the same level
  - 3) all non-leaf nodes except the root have at lest m/2 children
  - 4) the root is either a leaf node, or it has from 2 to m children
  - 5) a leaf node contains no more than m-1 keys
- Analysis of B-Trees
  - The maximum number of items in a B-tree of order m and height h : 
    - root :m-1
    - level1 : m(m-1)
    - level2 : m^2(m-1)...
    - total number of items is m^(h+1)-1
  - Generalization : Height of Tree = O(logmN)