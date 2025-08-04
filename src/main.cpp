#include <iostream>
#include <vector>
#include <limits>
#include <algorithm> // for sort, transform
#include <cctype>    // for tolower
#include <string>

#include "../include/User.hpp"
#include "../include/Stock.hpp"
#include "../include/ExcelUtil.hpp"
#include "../include/DisplayUtil.hpp"
#include "../include/Receipt.hpp"

using namespace std;

// ─── Function Prototypes ────────────────────────────────────────
void displayMainMenu();
void adminLogin();
void userRegister();
void userLogin();
void adminDashboard();
void staffDashboard();
void searchStock();
void addStock();
void updateStock();
void deleteStock();
void displayAllStocks();

// Global data storage
vector<User> users;
vector<Stock> stocks;

// ─── Utility: Lowercase String ──────────────────────────────────
std::string toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return result;
}

// ─── Main Function ──────────────────────────────────────────────
int main() {
    cout << R"(
______   _______  _______  _______  _______  _______  _______  _       
(  __  \ (  ___  )(  ____ )(  ___  )(  ____ \(       )(  ___  )( (    /|
| (  \  )| (   ) || (    )|| (   ) || (    \/| () () || (   ) ||  \  ( |
| |   ) || |   | || (____)|| (___) || (__    | || || || |   | ||   \ | |
| |   | || |   | ||     __)|  ___  ||  __)   | |(_)| || |   | || (\ \) |
| |   ) || |   | || (\ (   | (   ) || (      | |   | || |   | || | \   |
| (__/  )| (___) || ) \ \__| )   ( || (____/\| )   ( || (___) || )  \  |
(______/ (_______)|/   \__/|/     \|(_______/|/     \|(_______)|/    )_)
    )";

    try {
        DisplayUtil::displayWelcome();

        users = ExcelUtil::readUsersFromFile("data/users.xlsx");
        if (users.empty()) {
            cout << "No users found. Creating default admin account...\n";
            users.emplace_back("admin", "adminpass", true);
            ExcelUtil::writeUsersToFile("data/users.xlsx", users);
            cout << "Default admin created: admin / adminpass\n";
        }

        stocks = ExcelUtil::readStockFromFile("data/stock.xlsx");
        if (stocks.empty()) {
            cout << "No stock found. Creating sample stock data...\n";
            stocks.emplace_back(1, "Laptop", 10, 599.99);
            stocks.emplace_back(2, "Mouse", 30, 12.5);
            stocks.emplace_back(3, "Keyboard", 20, 25.0);
            ExcelUtil::writeStockToFile("data/stock.xlsx", stocks);
        }

        cout << "\nInitialization complete. Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        displayMainMenu();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

// ─── Main Menu ──────────────────────────────────────────────────
void displayMainMenu() {
    int choice;
    do {
        cout << "\n--- Main Menu ---\n";
        cout << "[1] Admin\n[2] User\n[3] Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: adminLogin(); break;
            case 2: cout << "User menu not implemented.\n"; break;
            case 3: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

// ─── Admin Login ────────────────────────────────────────────────
void adminLogin() {
    string username, password;
    cout << "\n--- Admin Login ---\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    for (const auto& user : users) {
        if (user.getUsername() == username && user.getPassword() == password && user.isAdmin()) {
            cout << "Login successful!\n";
            adminDashboard();
            return;
        }
    }
    cout << "Invalid credentials or not an admin.\n";
}

// ─── Admin Dashboard ────────────────────────────────────────────
void adminDashboard() {
    int choice;
    do {
        cout << "\n--- Admin Dashboard ---\n";
        cout << "[1] Add Stock\n";
        cout << "[2] Update Stock\n";
        cout << "[3] Delete Stock\n";
        cout << "[4] Search Stock\n[5] Display All Stocks\n";
        cout << "[6] Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStock(); break;
            case 2: updateStock(); break;
            case 3: deleteStock(); break;
            case 4: searchStock(); break;
            case 5: displayAllStocks(); break;
            case 6: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

// ─── Add Stock ──────────────────────────────────────────────────
void addStock() {
    string name;
    int quantity;
    double price;

    cout << "\n--- Add Stock ---\n";
    cout << "Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    cout << "Quantity: ";
    cin >> quantity;
    cout << "Price: ";
    cin >> price;

    int newId = ExcelUtil::getNextStockId(stocks);
    stocks.emplace_back(newId, name, quantity, price);
    ExcelUtil::writeStockToFile("data/stock.xlsx", stocks);
    cout << "Stock added! ID: " << newId << endl;
}

// ─── Update Stock ───────────────────────────────────────────────
void updateStock() {
    int id, choice;
    cout << "\n--- Update Stock ---\n";
    cout << "Enter stock ID: ";
    cin >> id;

    auto it = find_if(stocks.begin(), stocks.end(), [id](const Stock& s) {
        return s.getId() == id;
    });

    if (it == stocks.end()) {
        cout << "Stock not found.\n";
        return;
    }

    cout << "Editing: " << it->getName() << endl;
    cout << "[1] Name\n[2] Quantity\n[3] Price\n";
    cout << "Select field: ";
    cin >> choice;

    if (choice == 1) {
        string name;
        cout << "New name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        it->setName(name);
    } else if (choice == 2) {
        int qty;
        cout << "New quantity: ";
        cin >> qty;
        it->setQuantity(qty);
    } else if (choice == 3) {
        double price;
        cout << "New price: ";
        cin >> price;
        it->setPrice(price);
    } else {
        cout << "Invalid option.\n";
        return;
    }

    ExcelUtil::writeStockToFile("data/stock.xlsx", stocks);
    cout << "Stock updated.\n";
}

// ─── Delete Stock ───────────────────────────────────────────────
void deleteStock() {
    int id;
    cout << "\n--- Delete Stock ---\n";
    cout << "Enter stock ID: ";
    cin >> id;

    auto it = find_if(stocks.begin(), stocks.end(), [id](const Stock& s) {
        return s.getId() == id;
    });

    if (it != stocks.end()) {
        stocks.erase(it);
        ExcelUtil::writeStockToFile("data/stock.xlsx", stocks);
        cout << "Deleted stock with ID " << id << endl;
    } else {
        cout << " Stock not found.\n";
    }
}

// ─── Search Stock ───────────────────────────────────────────────
void searchStock() {
    cout << "\n--- Search Stock by ID ---\n";
    
    int id;
    cout << "Enter ID: ";
    cin >> id;

    bool found = false;
    for (const auto& s : stocks) {
        if (s.getId() == id) {
            cout << "Stock Found:\n";
            cout << "ID: " << s.getId()
                 << " | Name: " << s.getName()
                 << " | Qty: " << s.getQuantity()
                 << " | Price: $" << s.getPrice() << endl;
            found = true;
            break; // Since ID is unique, break early
        }
    }

    if (!found) {
        cout << "No stock item found with ID: " << id << endl;
    }
}


// ─── Display All Stocks ─────────────────────────────────────────
void displayAllStocks() {
    DisplayUtil::displayStocks(stocks);
}

// ─── Stub Functions ─────────────────────────────────────────────
void userRegister() { cout << "User registration not yet implemented.\n"; }
void userLogin()    { cout << "User login not yet implemented.\n"; }
void staffDashboard(){ cout << "Staff dashboard not yet implemented.\n"; }
