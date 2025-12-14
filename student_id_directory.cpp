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

Node *insert(Node *root, int id, const string *name, const string *major, const string *department, enrollDate *date)
{
    if (!root)
    {
        Node *nNode = new Node();
        nNode->id = id;
        nNode->name = *name;
        nNode->major = *major;
        nNode->department = *department;
        nNode->date = *date;
        nNode->left = nNode->right = NULL;
        return nNode;
    }
    if (id < root->id)
    {
        root->left = insert(root->left, id, name, major, department, date);
    }
    else if (id > root->id)
    {
        root->right = insert(root->right, id, name, major, department, date);
    }
    else
    {
        cout << "This student ID: " << id << " already exists!" << endl;
    }
    return root;
}

Node *deletion(Node *root, int id)
{
    if (!root)
    {
        cout << "This student ID: " << id << " is not found!" << endl;
        return NULL;
    }
    if (id < root->id)
    {
        root->left = deletion(root->left, id);
    }
    else if (id > root->id)
    {
        root->right = deletion(root->right, id);
    }
    else
    {
        if (!root->left)
        {
            Node *temp = root->right;
            delete root;
            cout << "Student ID: " << id << " has been deleted successfully!" << endl;
            return temp;
        }
        else if (!root->right)
        {
            Node *temp = root->left;
            delete root;
            cout << "Student ID: " << id << " has been deleted successfully!" << endl;
            return temp;
        }
        else
        {
            Node *success = root->right;
            while (success->left)
            {
                success = success->left;
            }
            root->id = success->id;
            root->name = success->name;
            root->major = success->major;
            root->department = success->department;
            root->date = success->date;
            root->right = deletion(root->right, success->id);
        }
    }
    return root;
}

Node *search(Node *root, int id)
{
    if (!root)
    {
        cout << "This student ID: " << id << " is not found!" << endl;
        return NULL;
    }

    if (id < root->id)
    {
        return search(root->left, id);
    }
    else if (id > root->id)
    {
        return search(root->right, id);
    }
    else
    {
        cout << "\n=== Student Found ===" << endl;
        cout << "ID: " << root->id << endl;
        cout << "Name: " << root->name << endl;
        cout << "Major: " << root->major << endl;
        cout << "Department: " << root->department << endl;
        cout << "Enrollment Date: " << root->date.day << "/" << root->date.month << "/" << root->date.year << endl;
        return root;
    }
}

void inOrder(Node *root)
{
    if (!root)
        return;

    inOrder(root->left);
    cout << "ID: " << root->id << " | Name: " << root->name << " | Major: " << root->major << " | Department: " << root->department << " | Enrollment Date: " << root->date.day << "/" << root->date.month << "/" << root->date.year << endl;
    inOrder(root->right);
}

void descendingOrder(Node *root)
{
    if (!root)
        return;

    descendingOrder(root->right);
    cout << "ID: " << root->id << " | Name: " << root->name << " | Major: " << root->major << " | Department: " << root->department << " | Enrollment Date: " << root->date.day << "/" << root->date.month << "/" << root->date.year << endl;
    descendingOrder(root->left);
}

void deleteTree(Node *root)
{
    if (!root)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main()
{
    Node *root = NULL;
    int choice = 0;

    while (choice != 6)
    {
        cout << "\n=== Student ID Directory Menu ===" << endl;
        cout << "1. Insert new student" << endl;
        cout << "2. Search for student" << endl;
        cout << "3. Delete student" << endl;
        cout << "4. Display in order (ascending)" << endl;
        cout << "5. Display in descending order" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int id;
            string name, major, department;
            cout << "Enter student ID: ";
            cin >> id;
            if (id <= 0)
            {
                cout << "Invalid ID. Must be a positive integer." << endl;
                break;
            }
            cout << "Enter student name: ";
            cin.ignore();
            getline(cin, name);
            if (name.empty())
            {
                cout << "Name cannot be empty." << endl;
                break;
            }
            cout << "Enter major: ";
            getline(cin, major);
            if (major.empty())
            {
                cout << "Major cannot be empty." << endl;
                break;
            }
            cout << "Enter department: ";
            getline(cin, department);
            if (department.empty())
            {
                cout << "Department cannot be empty." << endl;
                break;
            }
            enrollDate date;
            cout << "Enter enrollment date (day month year): ";
            cin >> date.day >> date.month >> date.year;
            if (date.month < 1 || date.month > 12 || date.day < 1 || date.day > 31 || date.year < 1900 || date.year > 2100)
            {
                cout << "Invalid date. Month 1-12, Day 1-31, Year 1900-2100." << endl;
                break;
            }
            root = insert(root, id, &name, &major, &department, &date);
            cout << "Student added successfully!" << endl;
            break;
        }
        case 2:
        {
            int id;
            cout << "Enter student ID to search: ";
            cin >> id;
            search(root, id);
            break;
        }
        case 3:
        {
            int id;
            cout << "Enter student ID to delete: ";
            cin >> id;
            root = deletion(root, id);
            break;
        }
        case 4:
        {
            if (!root)
            {
                cout << "Tree is empty!" << endl;
            }
            else
            {
                cout << "\n===== Students (Ascending Order) =====" << endl;
                inOrder(root);
            }
            break;
        }
        case 5:
        {
            if (!root)
            {
                cout << "Tree is empty!" << endl;
            }
            else
            {
                cout << "\n===== Students (Descending Order) =====" << endl;
                descendingOrder(root);
            }
            break;
        }
        case 6:
        {
            cout << "Exiting program..." << endl;
            break;
        }
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    }
    deleteTree(root);
    return 0;
}