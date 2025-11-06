#include <iostream>
#include <ctime>

using namespace std;

// Node for User doubly linked list
class UserNode {
public:
    int id;
    int balance;
    UserNode* next;
    UserNode* prev;

    UserNode() {
        id = -1;
        balance = 0;
        next = nullptr;
        prev = nullptr;
    }

    UserNode(int id, int initialBalance = 0) {
        this->id = id;
        this->balance = initialBalance;
        this->next = nullptr;
        this->prev = nullptr;
    }

    void print() const {
        cout << "User ID: " << id << ", Balance: " << balance << endl;
    }
};

// Doubly linked list for Users
class UserList {
public:
    UserNode* head;
    UserNode* tail;
    int size;

    UserList() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~UserList() {
        UserNode* current = head;
        while (current != nullptr) {
            UserNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addUser(int id, int initialBalance = 0) {
        UserNode* newUser = new UserNode(id, initialBalance);

        if (head == nullptr) {
            head = tail = newUser;
        } else {
            tail->next = newUser;
            newUser->prev = tail;
            tail = newUser;
        }
        size++;
    }

    UserNode* findUser(int id) {
        UserNode* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void printAll() const {
        UserNode* current = head;
        while (current != nullptr) {
            current->print();
            current = current->next;
        }
    }
};

class Transaction {
public:
    int from;
    int to;
    int amount;

    Transaction() {
        from = to = amount = 0;
    }

    Transaction(int f, int t, int amt) {
        from = f;
        to = t;
        amount = amt;
    }

    void print() const {
        cout << "From: " << from << ", To: " << to << ", Amount: " << amount << endl;
    }
};

// Node for Block doubly linked list
class BlockNode {
public:
    int index;
    time_t timestamp;
    Transaction transaction;
    BlockNode* next;
    BlockNode* prev;

    BlockNode() {
        index = 0;
        timestamp = time(0);
        next = nullptr;
        prev = nullptr;
    }

    void print() const {
        cout << "Block #" << index << endl;
        cout << "Timestamp: " << ctime(&timestamp);
        transaction.print();
        cout << "-------------------------" << endl;
    }
};

// Doubly linked list for Blockchain
class Blockchain {
public:
    BlockNode* head;
    BlockNode* tail;
    int size;

    Blockchain() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~Blockchain() {
        BlockNode* current = head;
        while (current != nullptr) {
            BlockNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addTransaction(int from, int to, int amount) {
        BlockNode* newBlock = new BlockNode();
        newBlock->index = size;
        newBlock->transaction = Transaction(from, to, amount);
        newBlock->timestamp = time(0);

        if (head == nullptr) {
            head = tail = newBlock;
        } else {
            tail->next = newBlock;
            newBlock->prev = tail;
            tail = newBlock;
        }
        size++;
    }

    void printChain() const {
        BlockNode* current = head;
        while (current != nullptr) {
            current->print();
            current = current->next;
        }
    }

    void displayCustom(int blockNumber) const {
        if (blockNumber < 0 || blockNumber >= size) {
            cout << "Invalid block number!" << endl;
            return;
        }

        BlockNode* current = head;
        int count = 0;
        while (current != nullptr && count < blockNumber) {
            current = current->next;
            count++;
        }

        if (current != nullptr) {
            current->print();
        }
    }

    void displayBalance(int userId) const {
        int balance = 0;

        BlockNode* current = head;
        while (current != nullptr) {
            if (current->transaction.to == userId)
                balance += current->transaction.amount;
            if (current->transaction.from == userId)
                balance -= current->transaction.amount;
            current = current->next;
        }

        cout << "Balance of user " << userId << " is: " << balance << endl;
    }
};

int main() {
    UserList users;
    Blockchain chain;
    int option;

    while (true) {
        cout << "\n--- MENU ---\n";
        cout << "1. Add user\n";
        cout << "2. Add balance\n";
        cout << "3. Make transaction\n";
        cout << "4. Show all users\n";
        cout << "5. Show blockchain\n";
        cout << "6. Show specific block\n";
        cout << "7. Show user balance\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> option;

        if (option == 0) break;

        int id, amount, from, to, blockNum;

        switch (option) {
            case 1:
                users.addUser(users.size);
                cout << "User created with ID: " << users.size - 1 << ", Balance: 0" << endl;
                break;

            case 2:
                cout << "Enter user ID: ";
                cin >> id;
                {
                    UserNode* user = users.findUser(id);
                    if (user == nullptr) {
                        cout << "Invalid user ID!" << endl;
                        break;
                    }
                    cout << "Enter amount to add: ";
                    cin >> amount;
                    user->balance += amount;
                    cout << "User " << id << " new balance: " << user->balance << endl;
                }
                break;

            case 3:
                cout << "Sender ID: ";
                cin >> from;
                cout << "Receiver ID: ";
                cin >> to;
                cout << "Amount: ";
                cin >> amount;
                {
                    UserNode* sender = users.findUser(from);
                    UserNode* receiver = users.findUser(to);

                    if (sender == nullptr || receiver == nullptr) {
                        cout << "Invalid user ID(s)!" << endl;
                        break;
                    }
                    if (sender->balance < amount) {
                        cout << "Insufficient balance!" << endl;
                        break;
                    }
                    sender->balance -= amount;
                    receiver->balance += amount;
                    chain.addTransaction(from, to, amount);
                    cout << "Transaction complete.\n";
                }
                break;

            case 4:
                users.printAll();
                break;

            case 5:
                chain.printChain();
                break;

            case 6:
                cout << "Enter block number: ";
                cin >> blockNum;
                chain.displayCustom(blockNum);
                break;

            case 7:
                cout << "Enter user ID: ";
                cin >> id;
                {
                    UserNode* user = users.findUser(id);
                    if (user == nullptr) {
                        cout << "Invalid user ID!" << endl;
                        break;
                    }
                    cout << "Tracked balance from blockchain:\n";
                    chain.displayBalance(id);
                    cout << "Actual user balance:\n";
                    user->print();
                }
                break;

            default:
                cout << "Invalid option!\n";
                break;
        }
    }

    cout << "Exiting program.\n";
    return 0;
}