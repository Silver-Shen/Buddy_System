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
        //node->total_alloc = false;
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
        cout << node->start << ' ' << node->end << ' ' << node->largest_free_size << ' ' << node->total_alloc << endl;
        if (node->left) traverse(node->left);
        if (node->right) traverse(node->right);
    }

    int malloc(int size){
    }

    int __malloc(buddy_node* node, int size){

    }

    void free(int addr){
        __free(addr, root);
    }

    int __free(int addr, buddy_node* node){
        if((node->start == addr) && (node->total_alloc == 1)){
            node->total_alloc = 0;
            node->largest_free_size = (node->end - node->start +1);
            return 1;
        }else{
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
    cout << "Please enter the total size and threshold:" << endl;
    cin >> buddy_system.total_size >> buddy_system.epsilon;
    //initialize the root node
    buddy_system.root = new buddy_node;
    buddy_system.init_buddy(buddy_system.root, 0, buddy_system.total_size-1);
    buddy_system.free(96);
    buddy_system.traverse(buddy_system.root);
    int address = buddy_system.malloc(32);
    return 0;
}
