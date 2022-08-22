#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// define a structure to hold Course information
struct Course {
    string courseID = "";
    string courseDes = "";
    vector<string> coursePre;
    bool Print = false;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a Course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

// Displays a course with Prequisites 
void displayCourse(Course course) {
    cout << course.courseID << ": " << course.courseDes << " | ";
    if (course.coursePre.size() != 0) {
        cout << "Prerequisites: ";
        for (int i = 0; i < course.coursePre.size(); i++) {
            if (course.coursePre.size() - i > 1) {
                cout << course.coursePre.at(i) << ", ";
            }
            else {
                cout << course.coursePre.at(i);
            }
        }
    }
    cout << endl;
    return;
}

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void preOrder(Node* node);
    void deconstruct(Node* node);
    Node* removeNode(Node* node, string courseID);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    Node* IsEmpty();
    void PreOrder();
    void Insert(Course course);
    void Remove(string courseID);
    Course Search(string courseID);
    Node* SearchNode(string courseID);
    void preCheck(Node* node);
    void Printer(Node* node);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/**
 * Default deconstructor
 */
void BinarySearchTree::deconstruct(Node* node) {
    if (node != nullptr) {
        if (node->left != nullptr) {
            deconstruct(node->left);
        }
        if (node->right != nullptr) {
            deconstruct(node->right);
        }
        delete node;
    }
}
/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    deconstruct(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

Node* BinarySearchTree::IsEmpty() {
    if (root == nullptr) {
        return nullptr;
    }
}
/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    preOrder(root);
}

//Inserts Course into tree by calling addNode and passing course.
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

//Calls removeNode and passes a courseID
void BinarySearchTree::Remove(string courseID) {
    this->removeNode(root, courseID);
}

//Allows For searching of tree
Course BinarySearchTree::Search(string courseID) {
    Node* cur = root;
    while (cur != nullptr) {
        if (cur->course.courseID.compare(courseID) == 0) {
            return cur->course;
        }
        else if (courseID.compare(cur->course.courseID) < 0) {
            cur = cur->left;
        }
        else {
            cur = cur->right;
        }
    }
    Course course;
    return course;
}

// Adds nodes in the proper order
void BinarySearchTree::addNode(Node* node, Course course) {
    if (node != nullptr && node->course.courseID.compare(course.courseID) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
            return;
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
            return;
        }
        else {
            this->addNode(node->right, course);
        }
    }
}

//Displays nodes in sorted order
void BinarySearchTree::inOrder(Node* node) {
    
    if (node != nullptr) {
        inOrder(node->left);
        displayCourse(node->course);
        inOrder(node->right);
    }
}

// Searches and returns a specfic node
Node* BinarySearchTree::SearchNode(string courseID) {
    Node* cur = root;
    while (cur != nullptr) {
        if (cur->course.courseID.compare(courseID) == 0) {
            return cur;
        }
        else if (courseID.compare(cur->course.courseID) < 0) {
            cur = cur->left;
        }
        else {
            cur = cur->right;
        }
    }
    return nullptr;
}

// Checks if needs to be printed and prints if it does
void BinarySearchTree::Printer(Node* node) {
    if (node->course.Print == false) {
        cout << node->course.courseID << ", " << node->course.courseDes << endl;
        node->course.Print = true;
    }
}

// Checks the node for preqs and if whether it needs to be printed or can be printed yet
void BinarySearchTree::preCheck(Node* node) {
    if (node->course.Print == false && node->course.coursePre.size() == 0) {
        Printer(node);
    }
    else if (node->course.Print == false && node->course.coursePre.size() == 1) {
        Node* temp = SearchNode(node->course.coursePre.at(0));
        if (temp->course.Print == false && temp->course.coursePre.size() == 0) {
            Printer(temp);
        }
        else if (temp->course.coursePre.size() > 0) {
            preCheck(temp);
        }
        Printer(node);
    }
    else if (node->course.Print == false && node->course.coursePre.size() >= 2) {
        for (string item : node->course.coursePre) {
            Node* temp = SearchNode(item);
            if (temp->course.Print == false && temp->course.coursePre.size() == 0) {
                Printer(temp);
            }
            else if (temp->course.coursePre.size() > 0) {
                preCheck(temp);
            }
            Printer(node);
        }
    }
}

// Traverses each node one at a time and checks the node for prequisites
void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        preCheck(node);
        preOrder(node->left);
        preOrder(node->right);
    }
}

// Used for removal of a specific node
Node* BinarySearchTree::removeNode(Node* node, string courseID) {
    if (node == nullptr) {
        return node;
    }
    else if (courseID.compare(node->course.courseID) < 0) {
        node->left = removeNode(node->left, courseID);
    }
    else if (courseID.compare(node->course.courseID) > 0) {
        node->right = removeNode(node->right, courseID);
    }
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
            temp = nullptr;
        }
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
            temp = nullptr;
        }
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseID);
        }
    }
    return node;
}

// Reads the input file and stores the information in a tree
void readFile(BinarySearchTree* bst) {
    string fileName = "ABCU_Advising_Program_Input.txt";
    ifstream file;
    file.open(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss;
        string temp;
        Course course;
        ss.str(line);
        getline(ss, temp, ',');
        course.courseID = temp;
        getline(ss, temp, ',');
        course.courseDes = temp;
        while (getline(ss, temp, ',')) {
            course.coursePre.push_back(temp);
        }
        bst->Insert(course);
    }
    file.close();
}

//Displays header for Course Schedule and prints keeping the Prerequisites in mind
void CourseSchedule(BinarySearchTree* bst) {
    cout << " _" << "_______________________________________" << endl
        << "|" << "                                        " << "|" << endl
        << "|" << "        ~ ABCU Courses Schedule~        " << "|" << endl
        << "|" << "________________________________________" << "|" << endl;
    bst->PreOrder();
}

//Displays header for Course List and prints in AlphaNumeric Order
void courseList(BinarySearchTree* bst) {
    cout << " _" << "_______________________________________" << endl
        << "|" << "                                        " << "|" << endl
        << "|" << "  ~Here is the full ABCU Courses List~  " << "|" << endl
        << "|" << "________________________________________" << "|" << endl;
    bst->InOrder();
}

//For displaying menu without the welcome header
void slimMenu() {
    cout << " _" << "_______________________________________" << endl
         << "|" << "                                        " << "|" << endl
         << "|" << "1. Load Data                            " << "|" << endl
         << "|" << "2. Remove Data                          " << "|" << endl
         << "|" << "3. Print Courses List (Alphanumeric)    " << "|" << endl
         << "|" << "4. Print Course Schedule (Prerequisites)" << "|" << endl
         << "|" << "5. Search for Course                    " << "|" << endl
         << "|" << "6. Exit                                 " << "|" << endl
         << "|_" << "_______________________________________" << "|" << endl;
}

//Initializes the welcome menu and the user selections
void menu() { 
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    cout << " _" << "_______________________________________" << endl
         << "|" << "                                        " << "|" << endl
         << "|" << "  ~Welcome to the ABCU Course Planner~  " << "|" << endl
         << "|" << "________________________________________" << "|" << endl
         << "|" << "                                        " << "|" << endl
         << "|" << "1. Load Data                            " << "|" << endl
         << "|" << "2. Remove Data                          " << "|" << endl
         << "|" << "3. Print Courses List (Alphanumeric)    " << "|" << endl
         << "|" << "4. Print Course Schedule (Prerequisites)" << "|" << endl
         << "|" << "5. Search for Course                    " << "|" << endl
         << "|" << "6. Exit                                 " << "|" << endl
         << "|_" << "_______________________________________" << "|" << endl;
    int selection = 0;
    while (selection != 6) {
        cout << "\nWhat would you like to do?" << endl;
        while (!(cin >> selection) && selection < 7) {
            cout << "Selection was not valid please try again." << endl;
            cin.clear();
            cin.ignore(123, '\n');
            slimMenu();
            cout << "\nWhat would you like to do?" << endl;
        }
        string search;
        switch (selection) {
            case 1:
                readFile(bst);
                cout << "File Loaded." << endl;
                slimMenu();
                break;
            case 2:
                bst = new BinarySearchTree();
                cout << "Removed Data." << endl;
                slimMenu();
                break;
            case 3:
                if (bst->IsEmpty() == nullptr) {
                    cout << "Please load data." << endl;
                    break;
                }
                courseList(bst);
                slimMenu();
                break;
            case 4:
                if (bst->IsEmpty() == nullptr) {
                    cout << "Please load data." << endl;
                    break;
                }
                CourseSchedule(bst);
                bst = new BinarySearchTree();
                readFile(bst);
                slimMenu();
                break;
            case 5:
                if (bst->IsEmpty() == nullptr) {
                    cout << "Please load data." << endl;
                    break;
                }
                cout << "What course would you like to search for?" << endl;
                cin >> search;
                for (int i = 0; i < search.length(); i++) {
                    if (!isdigit(search[i])) {
                        search[i] = toupper(search[i]);
                    }
                }
                if (bst->Search(search).courseID != "") {
                    cout << "\nResult Found:" << endl;
                    displayCourse(bst->Search(search));
                }
                else {
                    cout << "\n" << search << " is not an available course." << endl;
                }
                slimMenu();
                break;
            case 6:
                break;
            default:
                cout << selection << " was not valid please try again." << endl;
                slimMenu();
        }
    }
    cout << "\nGoodbye!" << endl;
}

// The main function
int main()
{   
    menu();
    return 0;
}

