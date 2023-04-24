#include "RedBlack.h"

RedBlack::RedBlack() {
    root = nullptr;
}

void RedBlack::Insert(std::string fName, std::string lName, std::string address, std::string city, std::string state,
                      std::string zip, std::string phoneNum) {
    const Person* newPerson =  new Person(fName, lName, address, city, state, zip, phoneNum);
    root = Insert(root, newPerson);
    HandleInsertion(root);
}

RedBlack::Node *RedBlack::Insert(RedBlack::Node* root, const Person*& newPerson) {
    if (root == nullptr)
        return new Node(newPerson);

    if (newPerson < root->data) {
        root->left = Insert(root->left, newPerson);
        root->left->parent = root;
    }
    else if (newPerson > root->data) {
        root->right = Insert(root->right, newPerson);
        root->right->parent = root;
    }
    else if (newPerson == root->data) {
        if (newPerson->phoneNum < root->data->phoneNum) {
            root->left = Insert(root->left, newPerson);
            root->left->parent = root;
        }
        else {
            root->right = Insert(root->right, newPerson);
            root->right->parent = root;
        }
    }

    return root;
}

void RedBlack::HandleInsertion(RedBlack::Node* newNode) {
    // https://www.andrew.cmu.edu/user/mm6/95-771/examples/RedBlackTreeProject/dist/javadoc/redblacktreeproject/RedBlackTree.html

    if (newNode == root) {
        newNode->color = BLACK;
        return;
    }

    if (newNode->parent->color == BLACK)
        return;


    while (newNode->parent->color == RED) {
        if (newNode->parent == newNode->parent->parent->left) {
            Node* uncle = newNode->parent->parent->right;
            if (uncle->color == RED) {
                newNode->parent->color = BLACK;
                uncle->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            }
            else {
                if (newNode == newNode->parent->right) {
                    newNode = newNode->parent;
                    RotateLeft(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                RotateRight(newNode->parent->parent);
            }
        }
        else {
            Node* uncle = newNode->parent->parent->left;
            if (uncle->color == RED) {
                newNode->parent->color = BLACK;
                uncle->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            }
            else {
                if (uncle == newNode->parent->left) {
                    newNode = newNode->parent;
                    RotateRight(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                RotateLeft(newNode->parent->parent);
            }
        }
        root->color = BLACK;
    }
}

void RedBlack::RotateLeft(RedBlack::Node* toRotate) {
    Node* right = toRotate->right;
    toRotate->right = right->left;
    right->left->parent = toRotate;
    right->parent = toRotate->parent;

    if (toRotate->parent == nullptr)
        root = right;
    else {
        if (toRotate == toRotate->parent->left)
            toRotate->parent->left = right;
        else
            toRotate->parent->right = right;
    }

    right->left = toRotate;
    toRotate->parent = right;
}

void RedBlack::RotateRight(RedBlack::Node* toRotate) {
    Node* left = toRotate->left;
    toRotate->left = left->right;
    left->right->parent = toRotate;
    left->parent = toRotate->parent;

    if (toRotate->parent == nullptr)
        root = left;
    else {
        if (toRotate == toRotate->parent->left)
            toRotate->parent->left = left;
        else
            toRotate->parent->right = left;
    }

    left->right = toRotate;
    toRotate->parent = left;
}

void RedBlack::Search(std::string first, std::string last) {
    Node* res = Search(root, first, last);
    if (res != nullptr) {
        std::cout << "Match Found. Displaying." << std::endl;
        res->data->Print();
    }
    else
        std::cout << "No Matches Found." << std::endl;
}

// TODO: fix search so that it proceeds if encountering two nodes that have same fname + lname
RedBlack::Node* RedBlack::Search(Node* current, std::string first, std::string last) {
    if (current == nullptr)
        return nullptr;
    else if ((first + last) < (current->data->fName + current->data->lName))
        return Search(current->left, first, last);
    else if ((first + last) > (current->data->fName + current->data->lName))
        return Search(current->right, first, last);
    else
        return current;
}

void RedBlack::TimeTrial(int n) {
    std::queue<RedBlack::Node*> q;
    q.push(root);
    Node* current;
    int i = 0;

    auto start = std::chrono::high_resolution_clock::now();
    while (i < n) {
        current = q.front();
        if (q.empty())
            break;

        q.pop();

        if (current->left != nullptr) {
            q.push(current->left);
            i++;
        }
        if (current->right != nullptr) {
            q.push(current->right);
            i++;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Accessed the first " << n << " Red-Black Tree elements sequentially in " << duration.count() << " microseconds" << std::endl;
}
