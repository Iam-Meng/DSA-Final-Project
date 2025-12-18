#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <iomanip>
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
    cout << "ID: " << root->id << " | Name: "
         << root->name << " | Major: "
         << root->major << " | Department: "
         << root->department << " | Enrollment Date: "
         << root->date.day
         << "/" << root->date.month
         << "/" << root->date.year << endl;
    inOrder(root->right);
}

void descendingOrder(Node *root)
{
    if (!root)
        return;

    descendingOrder(root->right);
    cout << "ID: " << root->id
         << " | Name: " << root->name
         << " | Major: " << root->major
         << " | Department: " << root->department
         << " | Enrollment Date: " << root->date.day << "/"
         << root->date.month << "/"
         << root->date.year << endl;
    descendingOrder(root->left);
}

static string trim(const string &s)
{
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start])))
        start++;

    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1])))
        end--;

    return s.substr(start, end - start);
}

static vector<string> splitPipeRow(const string &line)
{
    // Supports rows like:
    //   100001 | Sok Dara | SE | CS | 12 | 09 | 2022
    //   100001|Sok Dara|SE|CS|12|09|2022
    //   | 100001 | Sok Dara | SE | CS | 12 | 09 | 2022 |
    string s = trim(line);

    if (!s.empty() && s.front() == '|')
        s.erase(s.begin());
    if (!s.empty() && s.back() == '|')
        s.pop_back();

    vector<string> out;
    string cell;
    stringstream ss(s);
    while (getline(ss, cell, '|'))
    {
        cell = trim(cell);
        if (!cell.empty())
            out.push_back(cell);
    }
    return out;
}

static bool parseDateFlexible(const string &s, enrollDate &out)
{
    // Accepts: dd/mm/yyyy, dd-mm-yyyy, or "day month year"
    string t = trim(s);
    if (t.empty())
        return false;

    // try dd/mm/yyyy or dd-mm-yyyy
    {
        char sep1 = 0, sep2 = 0;
        int d = 0, m = 0, y = 0;
        stringstream ss(t);
        if ((ss >> d >> sep1 >> m >> sep2 >> y) && (sep1 == sep2) && (sep1 == '/' || sep1 == '-'))
        {
            out.day = d;
            out.month = m;
            out.year = y;
            return true;
        }
    }

    // try "day month year"
    {
        int d = 0, m = 0, y = 0;
        stringstream ss(t);
        if (ss >> d >> m >> y)
        {
            out.day = d;
            out.month = m;
            out.year = y;
            return true;
        }
    }

    return false;
}

static bool containsId(Node *root, int id)
{
    Node *cur = root;
    while (cur)
    {
        if (id < cur->id)
            cur = cur->left;
        else if (id > cur->id)
            cur = cur->right;
        else
            return true;
    }
    return false;
}

static bool append_student_to_file(const string &filename, int id, const string &name, const string &major, const string &department, const enrollDate &date)
{
    ofstream fout(filename, ios::app);
    if (!fout)
        return false;

    // Match your student.md format:
    // id | name | major | department | day | month | year
    fout << id << " | "
         << name << " | "
         << major << " | "
         << department << " | "
         << setw(2) << setfill('0') << date.day << " | "
         << setw(2) << setfill('0') << date.month << " | "
         << date.year << "\n";

    return true;
}

static bool delete_student_from_md(const string &filename, int id)
{
    ifstream fin(filename);
    if (!fin)
        return false;

    vector<string> kept;
    kept.reserve(128);

    string line;
    bool removed = false;

    while (getline(fin, line))
    {
        string t = trim(line);
        if (t.empty())
        {
            kept.push_back(line);
            continue;
        }

        // keep any markdown separator lines, if present
        if (t.find("---") != string::npos)
        {
            kept.push_back(line);
            continue;
        }

        vector<string> cols = splitPipeRow(t);
        if (cols.empty())
        {
            kept.push_back(line);
            continue;
        }

        int rowId = 0;
        try
        {
            rowId = stoi(cols[0]);
        }
        catch (...)
        {
            // not a data row; keep it
            kept.push_back(line);
            continue;
        }

        if (rowId == id)
        {
            removed = true;
            continue; // skip this row
        }

        kept.push_back(line);
    }

    fin.close();

    if (!removed)
        return false;

    ofstream fout(filename, ios::trunc);
    if (!fout)
        return false;

    for (size_t i = 0; i < kept.size(); i++)
        fout << kept[i] << "\n";

    return true;
}

Node *load_student_data(Node *root, const string &filename)
{
    ifstream fin(filename);
    if (!fin)
    {
        cout << "Cannot open file: " << filename << endl;
        return root;
    }

    string line;
    int loaded = 0;
    int skipped = 0;

    while (getline(fin, line))
    {
        string t = trim(line);
        if (t.empty())
            continue;

        // ignore markdown separator/header rows
        if (t.find("---") != string::npos)
            continue;

        vector<string> cols = splitPipeRow(t);
        if (cols.size() < 5)
        {
            skipped++;
            continue;
        }

        // skip header row like: id | name | ...
        if (!cols[0].empty() && (cols[0] == "id" || cols[0] == "ID"))
            continue;

        int id = 0;
        try
        {
            id = stoi(cols[0]);
        }
        catch (...)
        {
            skipped++;
            continue;
        }

        string name = cols[1];
        string major = cols[2];
        string department = cols[3];

        enrollDate date{};

        // Your student.md format is:
        // id | name | major | department | day | month | year
        if (cols.size() >= 7)
        {
            try
            {
                date.day = stoi(cols[4]);
                date.month = stoi(cols[5]);
                date.year = stoi(cols[6]);
            }
            catch (...)
            {
                cout << "Skipping row (bad day/month/year): " << line << endl;
                skipped++;
                continue;
            }
        }
        else
        {
            // fallback: id|name|major|department|dd/mm/yyyy
            if (!parseDateFlexible(cols[4], date))
            {
                cout << "Skipping row (bad date): " << line << endl;
                skipped++;
                continue;
            }
        }

        if (date.month < 1 || date.month > 12 || date.day < 1 || date.day > 31 || date.year < 1900 || date.year > 2100)
        {
            cout << "Skipping row (date out of range): " << line << endl;
            skipped++;
            continue;
        }

        root = insert(root, id, &name, &major, &department, &date);
        loaded++;
    }

    cout << "Loaded " << loaded << " student(s) from " << filename;
    if (skipped > 0)
        cout << " (skipped " << skipped << " line(s))";
    cout << endl;

    return root;
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
    int choice;
    string filename;

    filename = "student.md";

    root = load_student_data(root, filename);
    while (true)
    {
        cout << "\n=== Student ID Directory Menu ===" << endl;
        cout << "1. Insert new student" << endl;
        cout << "2. Search for student" << endl;
        cout << "3. Delete student" << endl;
        cout << "4. Display in order (ascending)" << endl;
        cout << "5. Display in descending order" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        if (cin >> choice)
        {
            if (choice == 7)
                break;
            if (choice < 1 || choice > 7)
            {
                cout << "Invalid choice! Please try again." << endl;
                continue;
            }
        }
        else
        {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int id;
            cout << "Enter student ID: ";
            while (!(cin >> id) || id <= 0)
            {
                cout << "Invalid ID. Must be a positive integer. Enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            string name, major, department;
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
            while (!(cin >> date.day >> date.month >> date.year) || date.month < 1 || date.month > 12 || date.day < 1 || date.day > 31 || date.year < 1900 || date.year > 2100)
            {
                cout << "Invalid date. Month 1-12, Day 1-31, Year 1900-2100. Enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (containsId(root, id))
            {
                cout << "This student ID: " << id << " already exists!" << endl;
                break;
            }

            root = insert(root, id, &name, &major, &department, &date);
            if (!append_student_to_file("student.md", id, name, major, department, date))
            {
                cout << "Warning: could not write to student.md" << endl;
            }
            cout << "Student added successfully!" << endl;
            break;
        }
        case 2:
        {
            int id;
            cout << "Enter student ID to search: ";
            while (!(cin >> id))
            {
                cout << "Invalid ID. Please enter a number. Enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            search(root, id);
            break;
        }
        case 3:
        {
            int id;
            cout << "Enter student ID to delete: ";
            while (!(cin >> id))
            {
                cout << "Invalid ID. Please enter a number. Enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            bool existed = containsId(root, id);
            root = deletion(root, id);

            // If it existed in the BST, also remove it from student.md
            if (existed)
            {
                if (!delete_student_from_md("student.md", id))
                    cout << "Warning: could not update student.md" << endl;
            }

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
