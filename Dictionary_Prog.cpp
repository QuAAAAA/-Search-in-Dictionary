#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<chrono>
using namespace std;


struct BSTNode {
	string key;
	BSTNode* left, * right;
	BSTNode(string x) : key(x), left(nullptr), right(nullptr) {}
};
struct SPTNode {
	string key;
	SPTNode* left, * right, * parent;
	SPTNode(string key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {}
};
struct TreapNode {
    int priority;
    string key;
    TreapNode* left, * right;

    TreapNode(string key) : key(key), priority(rand()), left(nullptr), right(nullptr) {}
};

enum Tree {
    BST_enum,
    SPT_enum,
    Treap_enum,
    Total
};
static bool IsSuccess = 0;

double avgSearchTime[3] ;

class BSTTree {
public:

    BSTTree() : root(nullptr) {}

    void insert(string key) {
        IsSuccess = 0;
        root = insert(root, key);
    }

    void deleteKey(string key) {
        IsSuccess = 0;
        root = deletE(root, key);
    }

    bool contains(string key) {
        auto start = std::chrono::high_resolution_clock::now();

        bool boolean = search(root, key);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        cout << "Running time of search is " << duration.count() << " microseconds." ;
        avgSearchTime[BST_enum] += static_cast<double>(duration.count());
        return boolean;

    }

private:
    BSTNode* root;
    BSTNode* findMin(BSTNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    BSTNode* search(BSTNode* root, string target) {

        if (root == nullptr || root->key == target) {
            return root;
        }
        if (target < root->key) {
            return search(root->left, target);
        }


        return search(root->right, target);

    }
    BSTNode* insert(BSTNode* root, string value) {
        if (root == nullptr) {
            IsSuccess = 1;
            return new BSTNode(value);          

        }

        if (value < root->key) {
            root->left = insert(root->left, value);
        }
        else if (value > root->key) {
            root->right = insert(root->right, value);
        }
        return root;
   
    }
    BSTNode* deletE(BSTNode* root, string target) {
        if (root == nullptr) {
            // 如果樹為空，直接返回
            return root;
        }

        if (target < root->key) {
            root->left = deletE(root->left, target);
        }
        else if (target > root->key) {
            root->right = deletE(root->right, target);
        }
        else {
            // 找到了要刪除的節點

            // case 1: 無子節點或只有一個子節點
            if (root->left == nullptr) {
                BSTNode* temp = root->right;
                delete root;
                IsSuccess = 1;
                return temp;
            }
            else if (root->right == nullptr) {
                BSTNode* temp = root->left;
                delete root;
                IsSuccess = 1;
                return temp;
            }

            // case 2: 有兩個子節點
            // 尋找右子樹中的最小節點（或左子樹中的最大節點）
            BSTNode* temp = findMin(root->right);

            // 複製最小節點的數據到當前節點
            root->key = temp->key;

            // 刪除右子樹中的最小節點
            root->right = deletE(root->right, temp->key); //需用return連接待刪除下的node


        }
        return root;
    };
};
class SplayTree {

public:
    SplayTree() : root(nullptr) {}
    void insert(string key) {
        IsSuccess = 0;
        SPTNode* node = new SPTNode(key);
        this->root = insertRec(this->root, node);
        splay(node);
    }
    void deleteKey(string key) {
        IsSuccess = 0;
        root = deleteNode(root, key);
    }
    SPTNode* contains(string key) {
        auto start = std::chrono::high_resolution_clock::now();

        SPTNode* result = searchRec(root, key);
        if (result != nullptr) {
            splay(result);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        cout << "Running time of search is " << duration.count() << " microseconds." ;
        avgSearchTime[SPT_enum] += static_cast<double>(duration.count());

        return result;
    }

    void inorder(SPTNode* root) {
        if (root != nullptr) {
            inorder(root->left);
            std::cout << root->key << " ";
            inorder(root->right);
        }
    }

private:
    SPTNode* root;

    void rightRotate(SPTNode* x) {
        SPTNode* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }
    void leftRotate(SPTNode* x) {
        SPTNode* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }
    void splay(SPTNode* x) {
        while (x->parent != nullptr) {
            if (x->parent->parent == nullptr) {
                if (x->parent->left == x) {
                    rightRotate(x->parent);
                }
                else {
                    leftRotate(x->parent);
                }
            }
            else if (x->parent->left == x && x->parent->parent->left == x->parent) {
                rightRotate(x->parent->parent);
                rightRotate(x->parent);
            }
            else if (x->parent->right == x && x->parent->parent->right == x->parent) {
                leftRotate(x->parent->parent);
                leftRotate(x->parent);
            }
            else if (x->parent->left == x && x->parent->parent->right == x->parent) {
                rightRotate(x->parent);
                leftRotate(x->parent);
            }
            else {
                leftRotate(x->parent);
                rightRotate(x->parent);
            }
        }
    }
    SPTNode* searchRec(SPTNode* root, string key) {
        if (root == nullptr || root->key == key) {
            return root;
        }

        if (root->key > key) {
            return searchRec(root->left, key);
        }
        else {
            return searchRec(root->right, key);
        }
    }
    SPTNode* insertRec(SPTNode* root, SPTNode* node) {
        if (root == nullptr) {
            IsSuccess = 1;

            return node;
        }
        if (node->key < root->key) {
            root->left = insertRec(root->left, node);
            root->left->parent = root;
        }
        else if (node->key > root->key) {
            root->right = insertRec(root->right, node);
            root->right->parent = root;
        }
       
        return root;
        
    }
    SPTNode* deleteNode(SPTNode* root, string key) {
        if (root == nullptr) return root;
        // 將要刪除的節點 splay 到根部
        root = searchRec(root, key);
        if (root == nullptr)  return nullptr; // 如果沒有找到，直接返回 
           
        splay(root);

        if (root->left == nullptr) {
            // 如果沒有左子樹，直接用右子樹替換
            SPTNode* temp = root;
            root = root->right;
            delete temp;
        }
        else {
            SPTNode* temp = root;
            // 將左子樹的最大節點 splay 到根部
            SPTNode* leftMax = root->left;
            while (leftMax->right != nullptr) leftMax = leftMax->right;

            splay(leftMax);
            leftMax->right = temp->right;
            if (temp->right != nullptr) temp->right->parent = leftMax;

            root = leftMax;
            delete temp;
        }

        if (root != nullptr) root->parent = nullptr;
        
        IsSuccess = 1;
        return root;
        
        
    }
};
class TreapTree {
public:
    TreapTree() : root(nullptr) {}

    void insert(string key) {
        IsSuccess = 0;
        root = insert(root, key);
    }

    void deleteKey(string key) {
        IsSuccess = 0;
        root = deleteNode(root, key);
    }

    bool contains(string key) {
        auto start = std::chrono::high_resolution_clock::now();

        bool boolean = search(root, key);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        cout << "Running time of search is " << duration.count() << " microseconds." ;
        avgSearchTime[Treap_enum] += static_cast<double>(duration.count());

        return boolean;
    }
private:
    TreapNode* root;

    TreapNode* rotateLeft(TreapNode* root) {
        TreapNode* R = root->right;
        root->right = R->left;
        R->left = root;
        return R;
    }

    TreapNode* rotateRight(TreapNode* root) {
        TreapNode* L = root->left;
        root->left = L->right;
        L->right = root;
        return L;
    }

    TreapNode* insert(TreapNode* node, string key) {
        if (node == nullptr) {
            IsSuccess = 1;
            return new TreapNode(key);
        }
           

        if (key < node->key) {
            node->left = insert(node->left, key);
            if (node->left->priority > node->priority)
                node = rotateRight(node);
        }
        else if(key > node->key) {
            node->right = insert(node->right, key);
            if (node->right->priority > node->priority)
                node = rotateLeft(node);
        }
        return node;
    }

    TreapNode* deleteNode(TreapNode* root, string key) {
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            if (root->left == nullptr) {
                TreapNode* temp = root->right;
                delete root;
                IsSuccess = 1;
                return temp;
            }
            else if (root->right == nullptr) {
                TreapNode* temp = root->left;
                delete root;
                IsSuccess = 1;
                return temp;
            }

            if (root->left->priority < root->right->priority)
                root = rotateLeft(root);
            else
                root = rotateRight(root);

            root = deleteNode(root, key);
        }
        return root;
    }

    bool search(TreapNode* root, string key) {
        if (root == nullptr)
            return false;
        if (root->key == key)
            return true;
        if (key < root->key)
            return search(root->left, key);
        return search(root->right, key);
    }
};

int main(void) {

    BSTTree BSTtree;
    SplayTree SPTtree;
    TreapTree Treaptree;
    int accuracyCount[4] = { 0 };

    int sourceText, testText;
    while (1) {
        cout << "Select No. of TestFile.txt to be tested (1: TestFile1.txt, 2: TestFile2.txt, others: leave) > ";
        cin >> sourceText;
        if (sourceText != 1 && sourceText != 2)break;

        ifstream inputFile(L"TestFile" + to_wstring(sourceText) + L".txt");
        if (!inputFile.is_open()) {
            std::cerr << "Cannot open file" << std::endl;
            return 1;
        }


        //Construct 3 trees in three type with TestFile.txt 
        string word;
        int wordCount = 0;
        while (inputFile >> word) {
            // keep alpha and lower case
            string cleanedWord;
            for (char c : word) {
                if (isalpha(c)) {
                    cleanedWord += tolower(c);
                }
                
            }
            //cout << cleanedWord<<" ";
            if (!cleanedWord.empty()) {
                BSTtree.insert(cleanedWord);
                SPTtree.insert(cleanedWord);
                Treaptree.insert(cleanedWord);
                wordCount++;
            }
        }
        inputFile.close();

        cout << "wordCount: " << wordCount << endl;

        int testMethod = 0;
        cout << "Select a method to Test (0: txt, 1: manual, others: leave) > ";
        cin >> testMethod;
        //Test by txt for TestFile.txt 
        if (testMethod == 0) {
            while (1) {
                cout << "Enter No. of TestFile.txt to test(1, 2, 3, others to leave)> ";
                cin >> testText;
                if (testText != 1 && testText != 2 && testText != 3)exit(0);
                ifstream inputFileTest(L"TestFile" + to_wstring(sourceText) + to_wstring(testText) + L".txt");
                if (!inputFileTest.is_open()) {
                    std::cerr << "Cannot open file" << std::endl;
                    return 1;
                }

                cout << "TestFile" << to_string(sourceText) << ".txt tested by TestFile" << to_string(sourceText) << to_string(testText) << ".txt :" << endl;
                while (inputFileTest >> word) {
                    string cleanedTarget;
                    for (char c : word) {
                        if (isalpha(c)) {
                            cleanedTarget += tolower(c);
                        }
                    }
                    if (!cleanedTarget.empty()) {
                        cout << "Try to find \"" << cleanedTarget << "\" in three ways" << endl;
                        if (BSTtree.contains(cleanedTarget)) { cout << " BST: successful" << endl; accuracyCount[BST_enum]++; }
                        else cout << " BST: unsuccessful" << endl;
                        if (SPTtree.contains(cleanedTarget)) { cout << " SPT: successful" << endl; accuracyCount[SPT_enum]++; }
                        else cout << " SPT: unsuccessful" << endl;
                        if (Treaptree.contains(cleanedTarget)) { cout << " Treap: successful" << endl; accuracyCount[Treap_enum]++; }
                        else cout << " Treap: unsuccessful" << endl;
                        accuracyCount[Total]++;

                        cout << "--------------------------" << endl;
                    }

                }
                inputFileTest.close();
                cout << "Accuracy of BST is " << accuracyCount[BST_enum] / (double)accuracyCount[Total] * 100 << "%" << endl;
                cout << "Accuracy of SPT is " << accuracyCount[SPT_enum] / (double)accuracyCount[Total] * 100 << "%" << endl;
                cout << "Accuracy of Treap is " << accuracyCount[Treap_enum] / (double)accuracyCount[Total] * 100 << "%" << endl;

                cout << "--------------------------" << endl;

                cout << "The Average Searching Time of BST is "<< avgSearchTime[BST_enum] / (double)accuracyCount[Total] << endl;
                cout << "The Average Searching Time of SPT is " << avgSearchTime[SPT_enum] / (double)accuracyCount[Total] << endl;
                cout << "The Average Searching Time of Treap is " << avgSearchTime[Treap_enum] / (double)accuracyCount[Total] << endl;
                cout << "searchCount: " << accuracyCount[Total] << endl;
                for (int i = 0; i < 3; ++i) avgSearchTime[i] = 0;
                for (int i = 0; i < 4; ++i) accuracyCount[i] = 0;
                cout << "--------------------------" << endl;
                
            }
            
        }
        //Test manually for TestFile.txt 
        else if (testMethod == 1) {

            while (1) {
               
                int manageMethod = 0;
                cout << "Select a method to manage the tree(0: Find, 1: Delete, 2: Insert, others: leave)> ";
                cin >> manageMethod;
                if (manageMethod != 0 && manageMethod != 1 && manageMethod != 2)exit(0);
                string target, word = "";
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除換行符之前的任何內容
                cout << "Enter a string to test> ";
                getline(cin, target);                
                /*string target = "Added to the mixture has been Terry";*/

                istringstream iss(target);
                while (iss >> word) {
                    string cleanedTarget;
                    for (char c : word) {
                        if (isalpha(c)) {
                            cleanedTarget += tolower(c);
                        }
                    }

                    if (!cleanedTarget.empty()) {
                        cout << "Try to manipulate \"" << cleanedTarget << "\"" << endl;
                        switch (manageMethod) {
                        case 0:
                            if (BSTtree.contains(cleanedTarget)) { cout << " BST: successful" << endl; accuracyCount[BST_enum]++; }
                            else cout << " BST: unsuccessful" << endl;
                            if (SPTtree.contains(cleanedTarget)) { cout << " SPT: successful" << endl; accuracyCount[SPT_enum]++; }
                            else cout << " SPT: unsuccessful" << endl;
                            if (Treaptree.contains(cleanedTarget)) { cout << " Treap: successful" << endl; accuracyCount[Treap_enum]++; }
                            else cout << " Treap: unsuccessful" << endl;


                            accuracyCount[Total]++;
                            break;

                        case 1:
                            BSTtree.deleteKey(cleanedTarget);
                            cout << " BST: Delete" << ((IsSuccess) ? " successfully" : " unsuccessfully") << endl;
                            SPTtree.deleteKey(cleanedTarget);
                            cout << " SPT: Delete" << ((IsSuccess) ? " successfully" : " unsuccessfully") << endl;
                            Treaptree.deleteKey(cleanedTarget);
                            cout << " Treap: Delete" << ((IsSuccess) ? " successfully" : " unsuccessfully") << endl;
                            break;

                        case 2:
                            BSTtree.insert(cleanedTarget);
                            cout << " BST: Insert" << ((IsSuccess) ? " successfully" : " unsuccessfully") << endl;
                            SPTtree.insert(cleanedTarget);
                            cout << " SPT: Insert" << ((IsSuccess) ? " successfully" : " unsuccessfully") << endl;
                            Treaptree.insert(cleanedTarget);
                            cout << " Treap: Insert" << ((IsSuccess) ? " successfully" : " unsuccessfully") << endl;
                            break;

                        default:
                            exit(0);
                        }

                        cout << "--------------------" << endl;

                    }
                    

                }
                if (manageMethod == 0) {
                    cout << "Accuracy of BST is " << accuracyCount[BST_enum] / (double)accuracyCount[Total] * 100 << "%" << endl;
                    cout << "Accuracy of SPT is " << accuracyCount[SPT_enum] / (double)accuracyCount[Total] * 100 << "%" << endl;
                    cout << "Accuracy of Treap is " << accuracyCount[Treap_enum] / (double)accuracyCount[Total] * 100 << "%" << endl;
                    cout << "--------------------------" << endl;

                    cout << "The Average Searching Time of BST is " << avgSearchTime[BST_enum] / (double)accuracyCount[Total] << endl;
                    cout << "The Average Searching Time of SPT is " << avgSearchTime[SPT_enum] / (double)accuracyCount[Total] << endl;
                    cout << "The Average Searching Time of Treap is " << avgSearchTime[Treap_enum] / (double)accuracyCount[Total] << endl;
                    for (int i = 0; i < 3; ++i) avgSearchTime[i] = 0;
                    for (int i = 0; i < 4; ++i) accuracyCount[i] = 0;

                }
                
                cout << "--------------------" << endl;
            }
           

        }
        //leave
        else {
            break;

        }
    }        
    system("pause");
	return 0;
}