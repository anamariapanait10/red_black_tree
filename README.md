# Red Black Trees

Is a self-balancing binary search tree where each node
stores an extra bit representing the color (red or black),
used to ensure that the tree remains balanced during
insertions and deletions.

When the tree is modified, the new tree is rearranged and repainted to
restore the coloring properties that constrain how unbalanced the tree
can become in the worst case. The properties are designed such that this 
rearranging and recoloring can be performed efficiently.

The re-balancing is not perfect, but guarantees searching in 
O(log n) time, where n is the number of elements in the tree. 
The insert and delete operations, along with the tree 
rearrangement and recoloring, are also performed in O(log n)
time.

## Rules
1. Every node has a colour either red or black
2. The root of the tree is always black
3. There are no two adjacent red nodes
4. Every path from a node (including root) to any of its
descendants NULL nodes has the same number of black nodes (black height)
5. All leaf nodes are black nodes

## Operations implemented
- Insertion
- Removal
- Search
- Successor
- Predecessor
- Print sorted items from an interval


### Node implementation

    struct RBNode {
    int value;
    RBNode *left, *right, *parent;
    bool color;
    
        RBNode(int val = 0, RBNode *l = nullptr, RBNode *r = nullptr, RBNode *p = nullptr, bool c = BLACK){
            value = val;
            left = l;
            right = r;
            parent = p;
            color = c;
        }
    };

### Red-Black Tree implementation

    class RBTree {
        RBNode *root;
        RBNode *nil;
    public:
        RBTree();
        void leftRotate(RBNode*);
        void rightRotate(RBNode*);
        void insert(RBNode*);
        void insert_fix(RBNode*);
        RBNode* minimum(RBNode*);
        int succesor(int);
        int predecesor(int);
        void transplant(RBNode*, RBNode*);
        void rbDelete(RBNode*);
        void delete_fix(RBNode*);
        void print(RBNode*);
        RBNode* find(int);
        void interval(int, int);
        int del(int);
        RBNode* getNil();
    };

### Constructor 
    RBTree::RBTree() {
        nil = new RBNode(-1, nullptr, nullptr, nullptr, BLACK);
        root = nil;
    }

### Rotations

When you have operations that change the structure of the tree (add, delete node)
there is a risk that after such an operation the tree will not follow the rules.
Thus, after an operation, the rotation must be done.

To rebalance it, we do the left or right rotation operation. 
This operation is done in O(1) (move some pointers).

![image](https://github.com/anamariapanait10/red_black_tree/blob/master/left_rotation.gif)

#### Left-Rotate
    void RBTree::leftRotate(RBNode *x) {
        RBNode *y = x->right; // save right subtree
        x->right = y->left; // set y's left subtree as x's right subtree
        if (y->left != nil)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }
#### Right-Rotate
    void RBTree::rightRotate(RBNode *x){
        RBNode *y = x->left;
        x->left = y->right;
        if (y->right != nil)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nil)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

### Insertion

#### Simple insertion
    void RBTree::insert(RBNode *z){
        RBNode *y = nil;
        RBNode *x = root;
        while (x != nil){
            y = x;
            if (z->value < x->value)
                x = x->left;
            else
                x = x->right;
        }
        z->parent = y;
        if (y == nil) // if the root is null
            root = z;
        else if (z->value < y->value) // If the value is less then the leaf node value
                                    // Assign the new node to be its left child
            y->left = z;
        else
            y->right = z;
        z->left = nil;
        z->right = nil;
        z->color = RED;
    }

#### Insersion fix

    void RBTree::insert_fix(RBNode *z){
        RBNode *y;
        if (z->parent == z->parent->parent->left){ // z's parent left son
            y = z->parent->parent->right; // z's uncle
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else if (z == z->parent->right){ // z is right son
                z = z->parent;
                leftRotate(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rightRotate(z->parent->parent);
        } else {
            y = z->parent->parent->left; // z's uncle
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else if (z == z->parent->left){ // z is left son
                z = z->parent;
                rightRotate(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            leftRotate(z->parent->parent);
        }
        root->color = BLACK;
    }

### Succesor

    int RBTree::succesor(int val){
        int last = -1;
        RBNode *x = root;
        while (x != nil){
            if (x->value == val)
                return val;
            else if (val < x->value){
                last = x->value;
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return last;
    }
### Predecesor

    int RBTree::predecesor(int val){
        int last = -1;
        RBNode *x = root;
        while (x != nil){
            if (x->value == val)
                return val;
            else if (val < x->value){
                x = x->left;
            } else {
                last = x->value;
                x = x->right;
            }
        }
        return last;
    }

### Delete
    int RBTree::del(int val){
        int count = 0;
        RBNode* found = find(val);
        while (found != nil){
            count++;
            rbDelete(found);
            found = find(val);
        }
        return count;
    }
#### Simple delete

    void RBTree::rbDelete(RBNode *z){
    RBNode *y = z;
    RBNode *x;
    bool y_originalColor = y->color;
    
        if (z->left == nil){
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil){
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_originalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if (y_originalColor == BLACK)
            delete_fix(x);
    }
#### Fix delete
    void RBTree::delete_fix(RBNode *x){
    while (x != root && x->color == BLACK){
        if (x == x->parent->left){
            RBNode *w = x->parent->right;
            if (w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;

### Transplant 
    void RBTree::transplant(RBNode *u, RBNode *v){
        if (u->parent == nil) // u is root
            root = v;
        else if (u == u->parent->left) // u is left son
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }
### Minimum

    RBNode* RBTree::minimum(RBNode *z){
        while (z->left != nil){
            z = z->left;
        }
        return z;
    }


### Search

    RBNode* RBTree::find(int val){
        RBNode *poz = root;
        while (poz != nil){
            if (poz->value == val)
                return poz;
            else if (val < poz->value){
                poz = poz->left;
            } else {
                poz = poz->right;
            }
        }
        return nil;
    }

### Interval

    void RBTree::interval(int left, int right){
        int current = succesor(left);
        while (current <= right && current != -1){
            if (find(current) != nil){
                out << current << ' ';
            }
            current = succesor(current + 1);
        }
        out << '\n';
    }

### Main

    int main()
    {
        int n, q, x, y;
        in >> n;
        RBTree tree = RBTree();
        for (int i = 0; i < n; i++){
            in >> q >> x;
            switch (q){
                case 1:
                    tree.insert(new RBNode(x));
                    break;
                case 2:
                    tree.del(x);
                    break;
                case 3:
                    out << (tree.find(x) != tree.getNil()) << "\n";
                    break;
                case 5:
                    out << tree.succesor(x) << "\n";
                    break;
                case 4:
                    out << tree.predecesor(x) << "\n";
                    break;
                case 6:
                    in >> y;
                    tree.interval(x, y);
                    break;
            }
        }
        in.close();
        out.close();
    }
