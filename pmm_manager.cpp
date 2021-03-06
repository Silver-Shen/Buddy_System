#include <iostream>
#include <cstdlib>
using namespace std;

struct buddy_node
{
    buddy_node* left;
    buddy_node* right;
    int start;
    int end;
    int largest_free_size;
    bool total_alloc;
};

struct Buddy_system
{
    buddy_node* root;
    int total_size, epsilon;

    void init_buddy(buddy_node* node, int l, int r){
        node->start = l;
        node->end = r;
        node->largest_free_size = r-l+1;
        node->total_alloc = false;
        if (r-l+1>epsilon){
            node->left = new buddy_node;
            node->right = new buddy_node;
            init_buddy(node->left, l, (l+r)/2);
            init_buddy(node->right, (l+r)/2+1, r);
        }else{
            node->left = NULL;
            node->right = NULL;
        }
    }

    void traverse(buddy_node* node){
        if (node->total_alloc || node->largest_free_size == node->end-node->start+1)
            cout << node->start << ' ' << node->end << ' ' << ((node->total_alloc)?"full":"empty") << endl;
        else{
            if (node->left) traverse(node->left);
            if (node->right) traverse(node->right);
        }
    }

    int find_upper(int size){
        int s = epsilon;
        while (s < size) s *= 2;
        return s;
    }

    int malloc(int size){
        int _size = find_upper(size);
        return __malloc(root, _size);
    }

    int __malloc(buddy_node* node, int size){
        //no enough space
        if (node->largest_free_size < size) return -1;
        //2^n-1<size<=2^n
        if (size == node->end-node->start+1){
            node->largest_free_size = 0;
            node->total_alloc = true;
            return node->start;
        }
        //need split
        int left = __malloc(node->left , size);
        int right = -1;
        if (left == -1) right = __malloc(node->right, size);
        node->largest_free_size = max(node->left->largest_free_size, node->right->largest_free_size);
        return max(left, right);
    }

    void free(int addr){
        if(addr%epsilon){
            cout << "bad input!" << endl;
            return;
        }
        __free(addr, root);
    }

    int __free(int addr, buddy_node* node){
        if((node->start == addr) && (node->total_alloc == 1)){
            node->total_alloc = 0;
            node->largest_free_size = (node->end - node->start +1);
            return 1;
        }else{
            if(node->left == NULL){
                cout << "no malloc address" <<endl;
                return 0;
            }
            if(addr > node->left->end){
                if(__free(addr, node->right)){
                    if(node->left->largest_free_size == (node->left->end - node->left->start +1)){
                        node->largest_free_size = (node->end - node->start +1);
                        return 1;
                    }else{
                        node->largest_free_size = max(node->left->largest_free_size, node->right->largest_free_size);
                    }
                }
            }else{
                if(__free(addr, node->left)){
                    if(node->right->largest_free_size == (node->right->end - node->right->start +1)){
                        node->largest_free_size = (node->end - node->start +1);
                        return 1;
                    }else{
                        node->largest_free_size = max(node->left->largest_free_size, node->right->largest_free_size);
                    }
                }
            }
        }
        return 0;
    }

}buddy_system;

int main(int argc, char const *argv[])
{
    buddy_system.total_size = 1024;
    buddy_system.epsilon = 64;
    //initialize the root node
    buddy_system.root = new buddy_node;
    buddy_system.init_buddy(buddy_system.root, 0, buddy_system.total_size-1);
    cout << "this is the initial state:" << endl;
    buddy_system.traverse(buddy_system.root);

    buddy_system.malloc(100);
    buddy_system.malloc(240);
    buddy_system.malloc(64);
    buddy_system.malloc(256);
    cout <<endl << "this is the middle state:" << endl;
    buddy_system.traverse(buddy_system.root);
    buddy_system.free(256);
    buddy_system.free(0);
    buddy_system.malloc(75);
    buddy_system.free(128);
    buddy_system.free(0);
    buddy_system.free(512);
    cout <<endl << "this is the final state:" << endl;
    buddy_system.traverse(buddy_system.root);
    return 0;
}
