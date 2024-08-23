#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct Node
{
    string student;
    int ufid;
    int balance_f;
    int height;
    Node *left;
    Node *right;
    Node() : student(""), balance_f(0), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree
{
private:
    int num = 1;
    vector<Node *> printing;
    Node *grand_child = nullptr;
    Node *parent = nullptr;

    Node *right_R(Node *node) // inspired by slide 13 in balanced tree module
    {
        grand_child = node->left->right;
        parent = node->left;
        parent->right = node;
        node->left = grand_child;
        return parent;
    }

    Node *left_R(Node *node) // inspired by slide 13 in balanced tree module
    {
        grand_child = node->right->left;
        parent = node->right;
        parent->left = node;
        node->right = grand_child;
        return parent;
    }

    int getHeight(Node *node)
    {
        if (node == nullptr)
            return 0;
        else
            return node->height;
    }

    int level_seeker(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        else
        {
            return 1 + max(level_seeker(node->left), level_seeker(node->right));
        }
    }
    string id_seeker(Node *node, int number) // check if the first node is the ufid in thre parse function
    {
        queue<Node *> q;

        int h = 1;
        if (node == nullptr)
        {
            return "unsuccessful";
        }

        if (node->ufid == number)
        {

            target = node->student;
            return target;
        }

        q.push(node);

        while (q.size() != 0) // finding central root balance factor
        {
            int size = q.size();
            for (int i = 0; i < size; i++)
            {
                Node *node = q.front();
                if (node->right != nullptr)
                {
                    if (node->right->ufid == number)
                    {
                        target = node->right->student;
                        return target;
                        cout << endl;
                    }
                    q.push(node->right);
                }
                if (node->left != nullptr)
                {
                    if (node->left->ufid == number)
                    {
                        target = node->left->student;
                        return target;
                        cout << endl;
                    }
                    q.push(node->left);
                }

                q.pop();
            }
            h++;
        }
        return "unsuccessful";
        cout << endl;
    }

    Node *InsertHelper(Node *node, const string &name, int number)
    {
        if (node == nullptr)
        {
            node = new Node();
            node->student = name;
            node->ufid = number;
            cout << "successful" << endl;
            return node;
        }
        else if (number < node->ufid)
        {
            node->left = InsertHelper(node->left, name, number);
        }
        else
        {
            node->right = InsertHelper(node->right, name, number);
        }

        node->balance_f = (level_seeker(node->left) - level_seeker(node->right));

        if (node->balance_f > 1)
        {
            if (number < node->left->ufid)
            {
                return right_R(node);
            }

            else
            {
                // Perform left rotation on the left child
                node->left = left_R(node->left);
                // Perform right rotation on the current node
                return right_R(node);
            }
        }
        if (node->balance_f < -1)
        {
            if (number > node->right->ufid)
            {
                return left_R(node);
            }

            else
            {
                // Perform right rotation on the right child
                node->right = right_R(node->right);
                // Perform left rotation on the current node
                return left_R(node);
            }
        }

        return node;
    }
    Node *RemoveHelper(Node *node, int number) // removes vectors
    {
        if (node == nullptr)
        {
            target = "unsuccessful";

            return nullptr;
        }

        if (number > node->ufid)
            node->right = RemoveHelper(node->right, number);
        else if (number < node->ufid)
            node->left = RemoveHelper(node->left, number);
        else
        {
            // zero child
            if (!node->right && !node->left)
            {

                delete node;
                target = "successful";

                return nullptr;
            }
            // 1 child
            else if (!node->right)
            {
                Node *T = node->left;
                delete node;
                return T;
            }
            else if (!node->left)
            {
                Node *T = node->right;
                delete node;
                return T;
            }
            // 2 children
            else
            {
                Node *inorder = node->right;
                while (inorder->left != nullptr)
                {
                    inorder = inorder->left;
                }
                node->ufid = inorder->ufid;
                node->student = inorder->student;

                node->right = RemoveHelper(node->right, inorder->ufid);
                return node;
            }
        }
        return node;
    }
    Node *RemoveHelper_2(Node *node, int number) // removes vectors
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (number > node->ufid)
            node->right = RemoveHelper_2(node->right, number);
        else if (number < node->ufid)
            node->left = RemoveHelper_2(node->left, number);
        else
        {
            // zero child
            if (!node->right && !node->left)
            {
                delete node;
                cout << "successful" << endl;
                return nullptr;
            }
            // 1 child
            else if (!node->right)
            {
                return node->left;
            }
            else if (!node->left)
            {
                return node->right;
            }
            // 2 children
            else
            {
                Node *inorder = node->right;
                while (inorder->left != nullptr)
                {
                    inorder = inorder->left;
                }
                node->ufid = inorder->ufid;
                node->student = inorder->student;

                node->right = RemoveHelper_2(node->right, inorder->ufid);
                return node;
            }
        }
        return node;
    }

public:
    Node *root = nullptr; /// forever changing node (always the last node that was inputed )
    string target = " ";
    bool iteration = false;
    vector<int> repeat;
    vector<Node *> repeat_n;

    void parse(istringstream &input)
    {
        string incorrect = "unsuccessful";
        bool num_true = false;
        string word = "";
        int num = 0;
        string extractedString = "";
        vector<string> command;
        int future = 1;
        char delimiter = ' ';

        while (getline(input, word, delimiter))
        {
            //checks for commands
            if (future == 2 && command[0] == "insert")
            {
                delimiter = '\"';
                for (char c : word) /// we might have to ccheck if a name has a combination of both numbers and characters
                {
                    if (!isalpha(c) && !isdigit(c))
                    {
                        if (c != '\"')
                        {
                            if (c != ' ')
                            {
                                cout << "unsuccessful" << endl;
                            }
                        }
                    }
                }
                //Checks if a number is present in the command 
                for (char x : word)
                {
                    if (isdigit(x))
                    {
                        num_true = true;
                    }
                    else if (!isdigit(x))
                    {
                        num_true = false;
                        break;
                    }
                }
                if (num_true == true)
                {

                    if (future == 1)
                    {
                        cout << "unsuccessful" << endl;
                        return;
                    }
                    else if ((command[0] == "insert") && (future == 2))
                    {
                        cout << "unsuccessful" << endl;
                        return;
                    }

                    num = stoi(word);
                }
                //checks for @
                else
                {
                    if (future > 1)
                    {
                        int startQuote = word.find_first_of('"');
                        int endQuote = word.find_last_of('"');
                        extractedString = word.substr(startQuote + 1, endQuote - startQuote - 1);
                        if (extractedString.substr(0, 1) == "@")
                        {
                            cout << "unsuccessful" << endl;
                            return;
                        }
                    }
                }
            }
            else
            {
                for (char c : word) /// we might have to ccheck if a name has a combination of both numbers and characters
                {
                    if (!isalpha(c) && !isdigit(c))
                    {
                        if (c != '\"')
                        {
                            if (c != ' ')
                            {
                                cout << "unsuccessful" << endl;
                            }
                        }
                    }
                }
                for (char x : word)
                {
                    if (isdigit(x))
                    {
                        num_true = true;
                    }
                    else if (!isdigit(x))
                    {
                        num_true = false;
                        break;
                    }
                }
                if (num_true == true)
                {

                    if (future == 1)
                    {
                        cout << "unsuccessful" << endl;
                        return;
                    }
                    else if ((command[0] == "insert") && (future == 2))
                    {
                        cout << "unsuccessful" << endl;
                        return;
                    }

                    num = stoi(word);
                }
                else
                {
                    if (future > 1)
                    {
                        int startQuote = word.find_first_of('"');
                        int endQuote = word.find_last_of('"');
                        extractedString = word.substr(startQuote + 1, endQuote - startQuote - 1);
                        if (extractedString.substr(0, 1) == "@")
                        {
                            cout << "unsuccessful" << endl;
                            return;
                        }
                    }
                }

                // seeing future commands
                // cout << word << endl;
            }

            if (future == 1)
            {
                command.push_back(word);
            }

            if ((command[0] == "insert"))
            {
                if ((future == 2) || (future == 3))
                {
                    command.push_back(extractedString);
                    if (future == 3)
                    {

                        Insert(command[1], num);
                    }
                }
            }
            //Traversal Function
            if ((command[0] == "printPreorder"))
            {
                printPreorder(root);
            }
            if ((command[0] == "printInorder"))
            {
                printinorder(root);
            }
            if ((command[0] == "printPostorder"))
            {
                printpostorder(root);
            }
            //deletes node within the Inorder traversal
            if ((command[0] == "removeInorder"))
            {
                if (root != nullptr)
                {
                    if ((future == 2) && (num_true != false))
                    {

                        removeInorder(num);
                    }
                }
                else
                {
                    cout << "unsuccessful" << endl;
                }
            }

            if ((command[0] == "search")) /// word search function
            {
                if ((future == 2) && (num_true != false))
                {
                    if (root != nullptr)
                    {
                        if (root->ufid == num)
                        {
                            cout << root->student << endl;
                        }
                        else
                        {
                            search(num);
                        }
                    }
                    else if (root == nullptr)
                    {
                        cout << "unsuccessful" << endl;
                    }
                }

                else if ((future == 2) && (num_true == false))
                {

                    if (root != nullptr)
                    {

                        Preorder_helper_2(root, repeat_n);
                        search_w(root, extractedString, repeat_n);
                        if (iteration == false)
                        {
                            cout << "unsucessful" << endl;
                        }
                    }
                    else
                    {
                        cout << "unsuccessful" << endl;
                    }
                }
            }

            else if ((command[0] == "remove"))
            {
                if (root != nullptr)
                {
                    if (future == 2)
                    {

                        command.push_back(extractedString);
                        Remove(num);
                        cout << "successful" << endl;
                    }
                }
                else
                {
                    cout << "unsuccessful" << endl;
                }
            }
            //gives height from the root 
            else if ((command[0] == "printLevelCount"))
            {
                printLevelCount(root);
            }
            future++;
        }
    }
    int height_finder(Node *node)
    {
        queue<Node *> q;
        int h = 0;
        if (node == nullptr)
        {
            return h;
        }
        q.push(node);

        while (q.size() != 0)
        {
            int size = q.size();
            for (int i = 0; i < size; i++)
            {
                Node *curr = q.front();
                if (curr->right != nullptr)
                {
                    q.push(curr->right);
                }
                if (curr->left != nullptr)
                {
                    q.push(curr->left);
                }

                q.pop();
            }
            h++;
        }
        return h;
    }

    void removeInorder(int pos)
    {
        vector<string> x;
        string s = "";
        x = printinorder_2(root);
        s = x[pos];
        RemoveHelper_2(root, search_w_in_3(root, s));
    }
    void search(int number) // searches ID and Helper function ensures that there is not duplicate in the Inserthelper above
    {
        // bool its_there;
        target = id_seeker(root, number);
        cout << target << endl;
    }

    int search_w(Node *node, const string &scholar, const vector<Node *> &n)
    {
        for (int i = 0; i < n.size(); i++)
        {
            if (n[i]->student == scholar)
            {
                iteration = true;
                cout << n[i]->ufid << endl;
            }
        }

        return -1; // Return whether a match was found or not
    }

    int search_w_in_3(Node *node, string scholar)
    {

        if (node == nullptr)
        {

            return -1;
        }

        if (node->student == scholar)
        {
            iteration = true;
            search_w_in_3(node->right, scholar);
            search_w_in_3(node->left, scholar);

            return node->ufid;
        }
        // goes to each dead end of the subtree
        int left_search = search_w_in_3(node->left, scholar);
        if (left_search != -1)
            return left_search;

        int right_search = search_w_in_3(node->right, scholar);
        if (right_search != -1)
            return right_search;

        return -1;
    }
    int search_w_in(Node *node, string scholar)
    {
        if (node == nullptr)
        {
            return -1;
        }

        if (node->student == scholar)
        {
            Remove(node->ufid);
            cout << "successful" << endl;
            return -1;
        }

        int left_search = search_w_in(node->left, scholar);
        if (left_search != -1)
            return left_search;

        int right_search = search_w_in(node->right, scholar);
        if (right_search != -1)
            return right_search;

        return -1;
    }
    void Preorder_helper(Node *node, vector<string> &print_node)
    {
        if (node == nullptr)
        {
            return;
        }

        print_node.push_back(node->student);
        Preorder_helper(node->left, print_node);
        Preorder_helper(node->right, print_node);
    }
    void Preorder_helper_2(Node *node, vector<Node *> &print_node)
    {
        iteration = false;
        if (node == nullptr)
        {
            return;
        }
        for (int i = 0; i < print_node.size(); i++)
        {
            if (print_node[i]->ufid == node->ufid)
            {
                iteration = true;
            }
        }
        if (iteration == false)
        {
            print_node.push_back(node);
        }
        Preorder_helper_2(node->left, print_node);
        Preorder_helper_2(node->right, print_node);
    }
    int search_w_in_2(Node *node, string scholar)
    {
        if (node == nullptr)
        {
            return -1;
        }

        if (node->student == scholar)
        {
            Remove(node->ufid);
            return -1;
        }

        int left_search = search_w_in_2(node->left, scholar);
        if (left_search != -1)
            return left_search;

        int right_search = search_w_in_2(node->right, scholar);
        if (right_search != -1)
            return right_search;

        return -1;
    }
    vector<string> printPreorder(Node *node)
    {
        vector<string> b;
        Preorder_helper(node, b);
        string order = "";
        int comma = 0;

        // the preorder traversal
        for (string n : b)
        {
            order += n; // print the ufid
            comma++;
            if (comma != b.size())
            {
                order += ", ";
            }
            else
            {
                order += "\n";
            }
        }
        cout << order;
        return b; // Return the vector containing preorder traversal
    }

    void inorder_helper(Node *node, vector<string> &print_node)
    {

        if (node == nullptr)
        {
            return;
        }
        inorder_helper(node->left, print_node);
        print_node.push_back(node->student);
        inorder_helper(node->right, print_node);
    }
    vector<string> printinorder_2(Node *node)
    {
        vector<string> p;
        inorder_helper(root, p);
        return p;
    }

    vector<string> printinorder(Node *node)
    {
        vector<string> w;
        inorder_helper(root, w);
        int comma = 0;
        for (string n : w)
        {
            cout << n; //  the ufid
            comma++;
            if (comma != w.size())
            {
                cout << ", ";
            }
        }
        cout << endl;
        return w;
    }
    void postorder_helper(Node *node, vector<string> &print_node)
    {

        if (node == nullptr)
        {
            return;
        }
        postorder_helper(node->left, print_node);
        postorder_helper(node->right, print_node);
        print_node.push_back(node->student);
    }

    vector<string> printpostorder(Node *node)
    {
        vector<string> e;
        postorder_helper(root, e);
        int comma = 0;
        for (string n : e)
        {
            cout << n; //  print the ufid
            comma++;
            if (comma != e.size())
            {
                cout << ", ";
            }
        }
        cout << endl;
        return e;
    }
    int printLevelCount(Node *node)
    {
        int level_c = 0;
        level_c = height_finder(root);
        cout << level_c << endl;
        return level_c;
    }
    void Insert(string name, int ufid)
    {
        target = id_seeker(root, ufid);
        if (target == "unsuccessful")
        {
            root = InsertHelper(root, name, ufid);
        }
        else
        {
            cout << "unsuccessful" << endl;
            return;
        }
    }
    void Remove(int ufid)
    {
        RemoveHelper(root, ufid);
    }

    AVLTree() : root() {}
};