#include <iostream>
#include <string>
using namespace std;

struct enrollDate
{
    int day, month, year;
};

struct Node
{
    int id;
    string name;
    string major;
    string department;
    enrollDate date;
    Node *left;
    Node *right;
};

Node *newNode(int id, const string *name, const string *major, const string *department)
{
    Node *n = new Node();
    if (!n)
    {
        cout << "Memory Allocation Failed!" << endl;
    }
    n->id = id;
    n->name = *name;
    n->major = *major;
    n->department = *department;
    n->right = n->left = NULL;
    return n;
}

Node *insert(Node *root, int id, const string *name, const string *major, const string *department, enrollDate *date)
{
    if(!root){
        Node *nNode = new Node();
        nNode->id = id;
        nNode->name = *name;
        nNode->major = *major;
        nNode->department = *department;
        nNode->date = *date;
        nNode->left = nNode->right = NULL;
        return nNode;
    }
    if(id < root->id){
        root->left = insert(root->left, id, name, major, department, date);
    } else if(id > root->id){
        root->right = insert(root->right, id, name, major, department, date);
    }else {
        cout << "This student ID:"<< id << " is already exists!" << endl;
    }
    return root;
}

Node *deletion(Node *root, int id)
{
    if(!root){
        cout << "This student ID:" << id << "is not founded!" << endl;
        return NULL;
    }
    if(id < root->id){
        root->left = deletion(root->left, id);
    }else if(id > root->id){
        root->right = deletion(root->right, id);
    }else {
        if(!root->left){
            Node *temp = root->right;
            delete root;
            cout << "Student ID:" << id << "has deleted successfully!" << endl;
            return temp;
        }else if(!root->right){
            Node *temp = root->left;
            delete root;
            cout << "Student ID:" << id << "has deleted successfully!" << endl;
            return temp;
        }
    }
}

Node *search()
{
    
}

void inOrder()
{
}

void descendingOrder()
{
}

int main()
{
}