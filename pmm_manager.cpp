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
};

buddy_node* root;
int total_size, epsilon;

void init_buddy(buddy_node* node, int l, int r){
    node->start = l;
    node->end = r;
    node->largest_free_size = r-l+1;
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
    cout << node->start << ' ' << node->end << ' ' << node->largest_free_size << endl;
    if (node->left) traverse(node->left);
    if (node->right) traverse(node->right);
}

int malloc(int size){

}

void free(){

}

int main(int argc, char const *argv[])
{
    cout << "Please enter the total size and threshold:" << endl;
    cin >> total_size >> epsilon;
    //initialize the root node
    root = new buddy_node;    
    init_buddy(root, 0, total_size-1);
    traverse(root);
    return 0;
}