#include <iostream>
#include <ctime>

using namespace std;


class User {
public:
    int id;
    int balance;

    User() {
        id = -1;
        balance = 0;
    }

    User(int id, int initialBalance = 0) {
        this->id = id;
        this->balance = initialBalance;
    }

    void print() const {
        cout << "User ID: " << id << ", Balance: " << balance << endl;
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

class Block {
public:
    int index;
    time_t timestamp;
    Transaction transaction;

    Block() {
        index = 0;
        timestamp = time(0);
    }

    void print() const {
        cout << "Block #" << index << endl;
        cout << "Timestamp: " << ctime(&timestamp);
        transaction.print();
        cout << "-------------------------" << endl;
    }
};


class Blockchain {
public:
    static const int MAX_BLOCKS = 10;
    Block blocks[MAX_BLOCKS];
    int size;

    Blockchain() {
        size = 0;
    }

    void addTransaction(int from, int to, int amount) {
        if (size >= MAX_BLOCKS) {
            cout << "Blockchain is full!" << endl;
            return;
        }

        Transaction t(from, to, amount);
        Block b;
        b.index = size;
        b.transaction = t;
        b.timestamp = time(0);
        blocks[size++] = b;
    }

    void printChain() const {
        for (int i = 0; i < size; ++i) {
            blocks[i].print();
        }
    }

    void displayCustom(int blockNumber) const {
        if (blockNumber < 0 || blockNumber >= size) {
            cout << "Invalid block number!" << endl;
            return;
        }
        blocks[blockNumber].print();
    }

    void displayBalance(int userId) const {
        int balance = 0;

        for (int i = 0; i < size; ++i) {
            if (blocks[i].transaction.to == userId)
                balance += blocks[i].transaction.amount;
            if (blocks[i].transaction.from == userId)
                balance -= blocks[i].transaction.amount;
        }

        cout << "Balance of user " << userId << " is: " << balance << endl;
    }
};

int main() {
    User users[10];
    Blockchain chain;
    int userCount = 0;
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
                if (userCount >= 10) {
                    cout << "User limit reached!" << endl;
                    break;
                }
                users[userCount] = User(userCount);
                cout << "User created with ID: " << users[userCount].id << ", Balance: " << users[userCount].balance << endl;
                userCount++;
                break;

            case 2:
                cout << "Enter user ID: ";
                cin >> id;
                if (id < 0 || id >= userCount) {
                    cout << "Invalid user ID!" << endl;
                    break;
                }
                cout << "Enter amount to add: ";
                cin >> amount;
                users[id].balance += amount;
                cout << "User " << id << " new balance: " << users[id].balance << endl;
                break;

            case 3:
                cout << "Sender ID: ";
                cin >> from;
                cout << "Receiver ID: ";
                cin >> to;
                cout << "Amount: ";
                cin >> amount;
                if (from < 0 || from >= userCount || to < 0 || to >= userCount) {
                    cout << "Invalid user ID(s)!" << endl;
                    break;
                }
                if (users[from].balance < amount) {
                    cout << "Insufficient balance!" << endl;
                    break;
                }
                users[from].balance -= amount;
                users[to].balance += amount;
                chain.addTransaction(from, to, amount);
                cout << "Transaction complete.\n";
                break;

            case 4:
                for (int i = 0; i < userCount; i++) {
                    users[i].print();
                }
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
                if (id < 0 || id >= userCount) {
                    cout << "Invalid user ID!" << endl;
                    break;
                }
                cout << "Tracked balance from blockchain:\n";
                chain.displayBalance(id);
                cout << "Actual user balance:\n";
                users[id].print();
                break;

            default:
                cout << "Invalid option!\n";
                break;
        }
    }

    cout << "Exiting program.\n";
    return 0;
}

