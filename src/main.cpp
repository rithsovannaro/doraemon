#include <iostream>
#include <vector>
#include <limits>
#include "../include/User.hpp"
#include "../include/Stock.hpp"
#include "../include/ExcelUtil.hpp"
#include "../include/DisplayUtil.hpp"
#include "../include/Receipt.hpp"

using namespace std;
// Function prototypes
void displayMainMenu();
void adminLogin();
void userRegister();
void userLogin();
void adminDashboard();
void staffDashboard();
void addStock();
void updateStock();
void deleteStock();
void displayAllStocks();

// Global data storage
vector<User> users;
vector<Stock> stocks;

int main() {
    try {
        DisplayUtil::displayWelcome();

        cout << "Loading data from users.xlsx..." << endl;
        users = ExcelUtil::readUsersFromFile("data/users.xlsx");
        if (users.empty()) {
            cout << "No users found in file. Creating a default admin account..." << endl;
            users.emplace_back("admin", "adminpass", true);
            ExcelUtil::writeUsersToFile("data/users.xlsx", users);
            cout << "Default admin account created: admin/adminpass" << endl;
        }

        cout << "\nLoading data from stock.xlsx..." << endl;
        stocks = ExcelUtil::readStockFromFile("data/stock.xlsx");
        if (stocks.empty()) {
            cout << "No stock found. A new stock file with headers has been created." << endl;
        }

        cout << "\nData loading and initialization complete. Press Enter to continue..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        displayMainMenu();
    } catch (const exception& e) {
        cerr << "An unhandled error occurred: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}

// Function to display the main menu
void displayMainMenu() {
    int choice;
    do {
        cout << "\n--- Stock Management System Main Menu ---" << endl;
        cout << "[1] Admin" << endl;
        cout << "[2] User" << endl;
        cout << "[3] Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                cout << "User menu not yet implemented." << endl;
                break;
            case 3:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}

// Admin login function
void adminLogin() {
    string username, password;
    cout << "\n--- Admin Login ---" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    bool loginSuccess = false;
    for (const auto& user : users) {
        if (user.getUsername() == username && user.getPassword() == password && user.isAdmin()) {
            loginSuccess = true;
            break;
        }
    }

    if (loginSuccess) {
        cout << "Admin login successful!" << endl;
        adminDashboard();
    } else {
        cout << "Invalid username or password, or not an admin." << endl;
    }
}

// Admin dashboard function
void adminDashboard() {
    int choice;
    do {
        cout << "\n--- Admin Dashboard ---" << endl;
        cout << "Welcome, Admin!" << endl;
        cout << "[1] Add Stock" << endl;
        cout << "[2] Update Stock" << endl;
        cout << "[3] Delete Stock" << endl;
        cout << "[4] Search Stock" << endl;
        cout << "[5] Display All Stocks" << endl;
        cout << "[6] Track Inventory" << endl;
        cout << "[7] Generate Low-Stock Alerts" << endl;
        cout << "[8] Print Reports" << endl;
        cout << "[9] Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                addStock();
                break;
            case 2:
                updateStock();
                break;
            case 3:
                deleteStock();
                break;
            case 5:
                displayAllStocks();
                break;
            case 9:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Feature not yet implemented." << endl;
                break;
        }
    } while (choice != 9);
}

// Add Stock Function
void addStock() {
    string name;
    int quantity;
    double price;

    cout << "\n--- Add New Stock ---" << endl;
    cout << "Enter stock name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before getting string
    getline(cin, name);

    cout << "Enter quantity: ";
    cin >> quantity;
    cout << "Enter price: ";
    cin >> price;

    int newId = ExcelUtil::getNextStockId(stocks);
    stocks.emplace_back(newId, name, quantity, price);
    ExcelUtil::writeStockToFile("data/stock.xlsx", stocks);

    cout << "\nStock added successfully! (ID: " << newId << ")" << endl;
}

// Update Stock Function
void updateStock() {
    int id;
    int choice;
    cout << "\n--- Update Stock ---" << endl;
    cout << "Enter stock ID to update: ";
    cin >> id;

    // Find the stock item by ID
    auto it = find_if(stocks.begin(), stocks.end(), [id](const Stock& s) {
        return s.getId() == id;
    });

    if (it != stocks.end()) {
        cout << "Stock found: " << it->getName() << endl;
        cout << "[1] Update Name" << endl;
        cout << "[2] Update Quantity" << endl;
        cout << "[3] Update Price" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        string newName;
        int newQuantity;
        double newPrice;

        switch (choice) {
            case 1:
                cout << "Enter new name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, newName);
                it->setName(newName);
                break;
            case 2:
                cout << "Enter new quantity: ";
                cin >> newQuantity;
                it->setQuantity(newQuantity);
                break;
            case 3:
                cout << "Enter new price: ";
                cin >> newPrice;
                it->setPrice(newPrice);
                break;
            default:
                cout << "Invalid choice. No changes made." << endl;
                return;
        }

        ExcelUtil::writeStockToFile("data/stock.xlsx", stocks);
        cout << "Stock updated successfully!" << endl;
    } else {
        cout << "Stock with ID " << id << " not found." << endl;
    }
}

// Delete Stock Function
void deleteStock() {
    int id;
    cout << "\n--- Delete Stock ---" << endl;
    cout << "Enter stock ID to delete: ";
    cin >> id;

    // Find the stock item by ID and remove it
    auto it = find_if(stocks.begin(), stocks.end(), [id](const Stock& s) {
        return s.getId() == id;
    });

    if (it != stocks.end()) {
        stocks.erase(it);
        ExcelUtil::writeStockToFile("data/stock.xlsx", stocks);
        cout << "Stock with ID " << id << " deleted successfully!" << endl;
    } else {
        cout << "Stock with ID " << id << " not found." << endl;
    }
}

// Display All Stocks Function
void displayAllStocks() {
    DisplayUtil::displayStocks(stocks);
}

// Placeholder functions for other features
void userRegister() {
    cout << "User registration not yet implemented." << endl;
}
void userLogin() {
    cout << "User login not yet implemented." << endl;
}
void staffDashboard() {
    cout << "Staff dashboard not yet implemented." << endl;
}
