#include <fstream>
#include <iostream>

using namespace std;

ifstream in("abce.in");
ofstream out("abce.out");

#define RED true
#define BLACK false

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

RBTree::RBTree() {
    nil = new RBNode(-1, nullptr, nullptr, nullptr, BLACK);
    root = nil;
}
void RBTree::leftRotate(RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
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
    if (y == nil)
        root = z;
    else if (z->value < y->value)
        y->left = z;
    else
        y->right = z;
    z->left = nil;
    z->right = nil;
    z->color = RED;
}
void RBTree::insert_fix(RBNode *z){
    RBNode *y;
    if (z->parent == z->parent->parent->left){
        y = z->parent->parent->right;
        if (y->color == RED){
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
        } else if (z == z->parent->right){
            z = z->parent;
            leftRotate(z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rightRotate(z->parent->parent);
    } else {
        y = z->parent->parent->left;
        if (y->color == RED){
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
        } else if (z == z->parent->left){
            z = z->parent;
            rightRotate(z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        leftRotate(z->parent->parent);
    }
    root->color = BLACK;
}
RBNode* RBTree::minimum(RBNode *z){
    while (z->left != nil){
        z = z->left;
    }
    return z;
}
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
void RBTree::transplant(RBNode *u, RBNode *v){
    if (u->parent == nil)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}
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
}
void RBTree::print(RBNode *poz){ // inorder
    if (poz == nil)
        return;
    print(poz->left);
    out << poz->value << '\n';
    print(poz->right);
}
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
int RBTree::del(int val){
    int count = 0;
    RBNode *found = find(val);
    while (found != nil){
        count++;
        rbDelete(found);
        found = find(val);
    }
    return count;
}
RBNode* RBTree::getNil(){
    return nil;
}


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