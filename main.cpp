#include "sqlite3.h"
#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include <climits>

using namespace std;

/* Program name: Final Project
*  Author: Lauren Davis
*  Date last updated: 5/11/2026
* Purpose: This program connects to a plant nusery's database and allows the user to add a customer, add or delete a product, update the status of an order, update customer information, delete a product and make a purchase from the nursery. The user is also able to view a customer's order information and customer information.   
*/
//*Note:AI was used to minimize syntax errors and correct formatting *

int mainMenu();
void printMainMenu();

//2 add options
void addCustomer(sqlite3 *);
void addProduct(sqlite3 *);

//1 delete option
void deleteProduct(sqlite3 *);

//2 table update options
void updateOrderStatus(sqlite3 *);
void updateCustomerInfo(sqlite3 *);

//making a purchase
void makePurchase(sqlite3 *);

void printCustomerPage(sqlite3 *);
void printOrderInfoPage(sqlite3 *);

int main()
{

    int choice;

    sqlite3 *mydb;

    int rc;

    // Opens database
    rc = sqlite3_open("DBproject.db", &mydb);

    if (rc != SQLITE_OK)
    {
        cout << "Error opening database: "
             << sqlite3_errmsg(mydb) << endl;

        return 1;
    }
    


    do {

        cout << "\n===== STORE MENU =====\n";

        cout << "1. Add Customer\n";
        cout << "2. Add Product\n";
        cout << "3. Update Order Status\n";
        cout << "4. Update Customer Info\n";
        cout << "5. Delete Product\n";
        cout << "6. Make Purchase\n";
        cout << "7. View Customer Info\n";
        cout << "8. View Order Info\n";
        cout << "9. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {

            case 1:
                addCustomer(mydb);
                break;
        
            case 2:
                addProduct(mydb);
                break;
        
            case 3:
                updateOrderStatus(mydb);
                break;
        
            case 4:
                updateCustomerInfo(mydb);
                break;
        
            case 5:
                deleteProduct(mydb);
                break;
        
            case 6:
                makePurchase(mydb);
                break;
        
            case 7:
                printCustomerPage(mydb);
                break;
        
            case 8:
                printOrderInfoPage(mydb);
                break;
        
            case 9:
                cout << "Exiting...\n";
                break;
        
            default:
                cout << "Invalid choice.\n";
        }

    } while(choice != 9);

    sqlite3_close(mydb);
    return 0;

}

// add customer

void addCustomer(sqlite3 *DB) {

    int customerID;
    string first_name;
    string last_name;
    string phone;
    string payment_info;
    string address;
    string email;

    sqlite3_stmt* stmt;

    //finding unused customer ID
    string getID =
        "SELECT IFNULL(MAX(Customer_ID), 0) + 1 FROM Customers; ";

    sqlite3_prepare_v2(DB, getID.c_str(), -1, &stmt, NULL);

    sqlite3_step(stmt);

    customerID = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    // get user input 
    cin.ignore();

    cout << "Enter first name: ";
    getline(cin, first_name);

    cout << "Enter last name: ";
    getline(cin, last_name);

    cout << "Enter phone number: ";
    getline(cin, phone);

    cout << "Enter payment info (cash/card): ";
    getline(cin, payment_info);

    cout << "Enter address: ";
    getline(cin, address);

    cout << "Enter email: ";
    getline(cin, email);

    //insert customer

    string sql =
        "INSERT INTO customer "
        "(Customer_ID, First_name, Last_name, Phone, "
        "Payment_info, Address, Email) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, customerID);

    sqlite3_bind_text(stmt, 2,
                      first_name.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 3,
                      last_name.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 4,
                      phone.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 5,
                      payment_info.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 6,
                      address.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 7,
                      email.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    //shows message of being added successfully and displays customer ID
    cout << "\nCustomer added successfully.\n";
    cout << "Customer ID: "
         << customerID << endl;
}
// add product
void addProduct(sqlite3 *DB) {

    string name, category;
    double cost;
    int quantity, productID;

    sqlite3_stmt* stmt;

    string getID =
        "SELECT IFNULL(MAX(Product_ID), 0) + 1 FROM Product;";

    sqlite3_prepare_v2(DB, getID.c_str(), -1, &stmt, NULL);

    sqlite3_step(stmt);

    productID = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    cin.ignore();

    cout << "Enter product name: ";
    getline(cin, name);

    cout << "Enter product cost: ";
    cin >> cost;

    cout << "Enter quantity: ";
    cin >> quantity;

    cin.ignore();

    cout << "Enter category: ";
    getline(cin, category);

    string sql =
        "INSERT INTO Product "
        "(Product_ID, Product_descript, Cost, Quantity, Category) "
        "VALUES(?,?,?,?,?);";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, productID);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, cost);
    sqlite3_bind_int(stmt, 4, quantity);
    sqlite3_bind_text(stmt, 5, category.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Product added successfully.\n";
    cout << "Product ID: " << productID << endl;
}

// update order status
void updateOrderStatus(sqlite3 *DB) {

    int orderID;
    string status;

    cout << "Enter order ID: ";
    cin >> orderID;

    cin.ignore();

    cout << "Enter new status: ";
    getline(cin, status);

    string sql =
        "UPDATE Orders SET Order_status=? WHERE Order_ID=?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, orderID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Status of order "<<orderID<<" updated successfully.\n";
}

// update customer info

void updateCustomerInfo(sqlite3 *DB) {

    int customerID;

    cout << "Enter Customer ID: ";
    cin >> customerID;

    sqlite3_stmt* stmt;

    //get customer's info

    string selectSQL =
        "SELECT First_name, Last_name, Phone, "
        "Payment_info, Address, Email "
        "FROM Customers WHERE Customer_ID=?;";

    sqlite3_prepare_v2(DB,
                       selectSQL.c_str(),
                       -1,
                       &stmt,
                       NULL);

    sqlite3_bind_int(stmt, 1, customerID);

    if (sqlite3_step(stmt) != SQLITE_ROW) {

        cout << "Customer not found.\n";

        sqlite3_finalize(stmt);

        return;
    }

    // Store current values

    string first_name =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

    string last_name =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

    string phone =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

    string payment_info =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

    string address =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

    string email =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

    sqlite3_finalize(stmt);

    // Displays info before user changes it 

    cout << "\n----- CURRENT CUSTOMER INFO -----\n";

    cout << "Customer ID: " << customerID << endl;
    cout << "First Name: " << first_name << endl;
    cout << "Last Name: " << last_name << endl;
    cout << "Phone: " << phone << endl;
    cout << "Payment Info: " << payment_info << endl;
    cout << "Address: " << address << endl;
    cout << "Email: " << email << endl;

    cout << "---------------------------------\n";

    // asks user what information they would like to update

    char choice;

    cin.ignore();

    cout << "\nChange First Name? (Y/N): ";
    cin >> choice;
    cin.ignore();
    //accepts users input uppercase and lowercase
    if (toupper(choice) == 'Y') {

        cout << "Enter new first name: ";
        getline(cin, first_name);
    }

    cout << "Change Last Name? (Y/N): ";
    cin >> choice;
    cin.ignore();

    if (toupper(choice) == 'Y') {

        cout << "Enter new last name: ";
        getline(cin, last_name);
    }

    cout << "Change Phone? (Y/N): ";
    cin >> choice;
    cin.ignore();

    if (toupper(choice) == 'Y') {

        cout << "Enter new phone: ";
        getline(cin, phone);
    }

    cout << "Change Payment Info? (Y/N): ";
    cin >> choice;
    cin.ignore();

    if (toupper(choice) == 'Y') {

        cout << "Enter new payment info: ";
        getline(cin, payment_info);
    }

    cout << "Change Address? (Y/N): ";
    cin >> choice;
    cin.ignore();

    if (toupper(choice) == 'Y') {

        cout << "Enter new address: ";
        getline(cin, address);
    }

    cout << "Change Email? (Y/N): ";
    cin >> choice;
    cin.ignore();

    if (toupper(choice) == 'Y') {

        cout << "Enter new email: ";
        getline(cin, email);
    }

    // updates customer information

    string updateSQL =
        "UPDATE Customers SET "
        "First_name=?, "
        "Last_name=?, "
        "Phone=?, "
        "Payment_info=?, "
        "Address=?, "
        "Email=? "
        "WHERE Customer_ID=?;";

    sqlite3_prepare_v2(DB,
                       updateSQL.c_str(),
                       -1,
                       &stmt,
                       NULL);

    sqlite3_bind_text(stmt, 1,
                      first_name.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 2,
                      last_name.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 3,
                      phone.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 4,
                      payment_info.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 5,
                      address.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_text(stmt, 6,
                      email.c_str(),
                      -1,
                      SQLITE_STATIC);

    sqlite3_bind_int(stmt, 7, customerID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "\nCustomer information updated successfully.\n";
}

// delete product

void deleteProduct(sqlite3 *DB) {

    int Product_ID;

    cout << "Enter product ID to delete: ";
    cin >> Product_ID;

    string sql =
        "DELETE FROM Product WHERE Product_ID=?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, Product_ID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Product deleted successfully.\n";
}

// make purchase

void makePurchase(sqlite3 *DB) {

    int Customer_ID;
    int Product_ID;
    int Quantity;
    int stock = 0;
    double Cost = 0;
    double Unit_price =0;
    double Amount = 0;
   

    cout << "Enter customer ID: ";
    cin >> Customer_ID;

    cout << "Enter product ID: ";
    cin >> Product_ID;

    cout << "Enter quantity: ";
    cin >> Quantity;

    sqlite3_stmt* stmt;


    string sql =
        "SELECT Cost, Quantity FROM Product "
        "WHERE Product_ID=?;";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, Product_ID);

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        Cost = sqlite3_column_double(stmt, 0);
        stock = sqlite3_column_int(stmt, 1);
    }
    else {
        cout << "Product not found.\n";
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    if (stock < Quantity) {
        cout << "Not enough in stock.\n";
        return;
    }

    double Total_cost = Cost * Quantity;

    // Create order

    sql =
        "INSERT INTO Orders(Customer_ID,Order_date,Order_status,Total_cost) "
        "VALUES(?, datetime('now'), 'Processing', ?);";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, Customer_ID);
    sqlite3_bind_double(stmt, 2, Total_cost);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    int Order_ID = sqlite3_last_insert_rowid(DB);

    // Add order item

    sql =
        "INSERT INTO OrderItem(Order_ID,Product_ID,Quantity,Unit_price) "
        "VALUES(?,?,?,?);";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, Order_ID);
    sqlite3_bind_int(stmt, 2, Product_ID);
    sqlite3_bind_int(stmt, 3, Quantity);
    sqlite3_bind_double(stmt, 4, Unit_price);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    // Update inventory

    sql =
        "UPDATE Product SET Quantity = Quantity - ? "
        "WHERE Product_ID=?;";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, Quantity);
    sqlite3_bind_int(stmt, 2, Product_ID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    // Update customer balance

    sql =
        "UPDATE Payment SET Amount = Amount + ? "
        "WHERE Order_ID=?;";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_double(stmt, 1, Amount);
    sqlite3_bind_int(stmt, 2, Order_ID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Purchase completed successfully.\n";
}

// displays customer info

void printCustomerPage(sqlite3 *DB) {

    int customerID;

    cout << "Enter Customer ID: ";
    cin >> customerID;

    sqlite3_stmt* stmt;

    string sql =
        "SELECT First_name, Last_name, Phone, "
        "Payment_info, Address, Email "
        "FROM Customers "
        "WHERE Customer_ID=?;";

    sqlite3_prepare_v2(DB,
                       sql.c_str(),
                       -1,
                       &stmt,
                       NULL);

    sqlite3_bind_int(stmt, 1, customerID);

    int result = sqlite3_step(stmt);

    if (result == SQLITE_ROW) {

        cout << "\n----- CUSTOMER INFORMATION -----\n";

        cout << "Customer ID: "
             << customerID << endl;

        cout << "First Name: "
             << sqlite3_column_text(stmt, 0)
             << endl;

        cout << "Last Name: "
             << sqlite3_column_text(stmt, 1)
             << endl;

        cout << "Phone Number: "
             << sqlite3_column_text(stmt, 2)
             << endl;

        cout << "Payment Info: "
             << sqlite3_column_text(stmt, 3)
             << endl;

        cout << "Address: "
             << sqlite3_column_text(stmt, 4)
             << endl;

        cout << "Email: "
             << sqlite3_column_text(stmt, 5)
             << endl;

        cout << "--------------------------------\n";
    }
    else {

        cout << "Customer not found.\n";
    }

    sqlite3_finalize(stmt);
}

// shows customer order info
void printOrderInfoPage(sqlite3 *DB) {

    int orderID;

    cout << "Enter Order ID: ";
    cin >> orderID;

    string sql =
        "SELECT o.Order_ID, "
        "c.First_name, "
        "c.Last_name, "
        "p.Product_descript, "
        "oi.Quantity, "
        "o.Total_cost, "
        "o.Order_status "
        "FROM Orders o "
        "JOIN Customers c ON o.Customer_ID = c.Customer_ID "
        "JOIN OrderItem oi ON o.Order_ID = oi.Order_ID "
        "JOIN Product p ON oi.Product_ID = p.Product_ID "
        "WHERE o.Order_ID=?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB,
                       sql.c_str(),
                       -1,
                       &stmt,
                       NULL);

    sqlite3_bind_int(stmt, 1, orderID);

    cout << "\n----- ORDER INFO -----\n";

    int result;
    bool found = false;

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {

        found = true;

        cout << "Order ID: "
             << sqlite3_column_int(stmt, 0)
             << endl;

        cout << "Customer: "
             << sqlite3_column_text(stmt, 1)
             << " "
             << sqlite3_column_text(stmt, 2)
             << endl;

        cout << "Product: "
             << sqlite3_column_text(stmt, 3)
             << endl;

        cout << "Quantity: "
             << sqlite3_column_int(stmt, 4)
             << endl;

        cout << "Total: $"
             << sqlite3_column_double(stmt, 5)
             << endl;

        cout << "Status: "
             << sqlite3_column_text(stmt, 6)
             << endl;

        cout << "---------------------------\n";
    }

    if (!found) {

        cout << "Order not found.\n";
    }

    sqlite3_finalize(stmt);
}
