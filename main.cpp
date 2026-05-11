#include "sqlite3.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <climits>

using namespace std;

/* Program name: Final Project
*  Author: Lauren Davis
*  Date last updated: 5/11/2026
* Purpose: This program connects to a plant nusery's database and allows the user to add a customer, add or delete a product, update the status of an order, update customer information, delete a product and make a purchase from the nursery. The user is also able to view a customer's order information and customer information.   
*/
//*Note:AI was used to *

int mainMenu();
void printMainMenu();

//2 add options
void addCustomer(sqlite3 *);
void addProduct(sqlite3 *);

//1 delete option
void deleteProduct(sqlite3 *);

//2 table update options
void updateOrder_status(sqlite3 *);
void updateCustomerInfo(sqlite3 *);

//making a purchase
void purchase(sqlite3 *);

//display uses join
void printCustomerPage(sqlite3_stmt *, int, int);//check if this one qualifies
void printOrderinfoPage(sqlite3_stmt *, int, int);


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
                addCustomer();
                break;

            case 2:
                addProduct();
                break;

            case 3:
                updateOrderStatus();
                break;

            case 4:
                updateCustomerInfo();
                break;

            case 5:
                deleteProduct();
                break;

            case 6:
                makePurchase();
                break;

            case 7:
                printCustomerPage();
                break;

            case 8:
                printOrderInfoPage();
                break;

            case 9:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while(choice != 9);

    sqlite3_close(mydb);

}

// add customer

void addCustomer() {

    string name, email, phone;
    double balance;

    cin.ignore();

    cout << "Enter customer name: ";
    getline(cin, name);

    cout << "Enter email: ";
    getline(cin, email);

    cout << "Enter phone: ";
    getline(cin, phone);

    cout << "Enter balance: ";
    cin >> balance;

    string sql =
        "INSERT INTO customer(name,email,phone,balance) "
        "VALUES(?,?,?,?);";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, balance);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Customer added successfully.\n";
}

// add product
void addProduct() {

    string name;
    double price;
    int quantity;

    cin.ignore();

    cout << "Enter product name: ";
    getline(cin, name);

    cout << "Enter price: ";
    cin >> price;

    cout << "Enter quantity: ";
    cin >> quantity;

    string sql =
        "INSERT INTO product(productName,price,quantity) "
        "VALUES(?,?,?);";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, price);
    sqlite3_bind_int(stmt, 3, quantity);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Product added successfully.\n";
}

// update order status
void updateOrderStatus() {

    int orderID;
    string status;

    cout << "Enter order ID: ";
    cin >> orderID;

    cin.ignore();

    cout << "Enter new status: ";
    getline(cin, status);

    string sql =
        "UPDATE orders SET status=? WHERE orderID=?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, orderID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Order updated successfully.\n";
}

// update customer info

void updateCustomerInfo() {

    int customerID;
    string email, phone;

    cout << "Enter customer ID: ";
    cin >> customerID;

    cin.ignore();

    cout << "Enter new email: ";
    getline(cin, email);

    cout << "Enter new phone: ";
    getline(cin, phone);

    string sql =
        "UPDATE customer SET email=?, phone=? "
        "WHERE customerID=?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, customerID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Customer updated successfully.\n";
}

// delete product

void deleteProduct() {

    int productID;

    cout << "Enter product ID to delete: ";
    cin >> productID;

    string sql =
        "DELETE FROM product WHERE productID=?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, productID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Product deleted successfully.\n";
}

// make purchase

void makePurchase() {

    int customerID;
    int productID;
    int quantity;

    cout << "Enter customer ID: ";
    cin >> customerID;

    cout << "Enter product ID: ";
    cin >> productID;

    cout << "Enter quantity: ";
    cin >> quantity;

    sqlite3_stmt* stmt;

    double price = 0;
    int stock = 0;

    string sql =
        "SELECT price, quantity FROM product "
        "WHERE productID=?;";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, productID);

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        price = sqlite3_column_double(stmt, 0);
        stock = sqlite3_column_int(stmt, 1);
    }
    else {
        cout << "Product not found.\n";
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    if (stock < quantity) {
        cout << "Not enough stock.\n";
        return;
    }

    double total = price * quantity;

    // Create order

    sql =
        "INSERT INTO orders(customerID,orderDate,status,totalAmount) "
        "VALUES(?, datetime('now'), 'Processing', ?);";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, customerID);
    sqlite3_bind_double(stmt, 2, total);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    int orderID = sqlite3_last_insert_rowid(DB);

    // Add order item

    sql =
        "INSERT INTO orderItem(orderID,productID,quantity,itemPrice) "
        "VALUES(?,?,?,?);";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, orderID);
    sqlite3_bind_int(stmt, 2, productID);
    sqlite3_bind_int(stmt, 3, quantity);
    sqlite3_bind_double(stmt, 4, price);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    // Update inventory

    sql =
        "UPDATE product SET quantity = quantity - ? "
        "WHERE productID=?;";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, quantity);
    sqlite3_bind_int(stmt, 2, productID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    // Update customer balance

    sql =
        "UPDATE customer SET balance = balance + ? "
        "WHERE customerID=?;";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_double(stmt, 1, total);
    sqlite3_bind_int(stmt, 2, customerID);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    cout << "Purchase completed successfully.\n";
}

// displays customer info

void printCustomerPage() {

    string sql = "SELECT * FROM customer;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    cout << "\n----- CUSTOMER PAGE -----\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        cout << "ID: "
             << sqlite3_column_int(stmt, 0) << endl;

        cout << "Name: "
             << sqlite3_column_text(stmt, 1) << endl;

        cout << "Email: "
             << sqlite3_column_text(stmt, 2) << endl;

        cout << "Phone: "
             << sqlite3_column_text(stmt, 3) << endl;

        cout << "Balance: $"
             << sqlite3_column_double(stmt, 4) << endl;

        cout << "------------------------\n";
    }

    sqlite3_finalize(stmt);
}

// shows customer order info
void printOrderInfoPage() {

    string sql =
        "SELECT o.orderID, c.name, p.productName, "
        "oi.quantity, o.totalAmount, o.status "
        "FROM orders o "
        "JOIN customer c ON o.customerID = c.customerID "
        "JOIN orderItem oi ON o.orderID = oi.orderID "
        "JOIN product p ON oi.productID = p.productID;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    cout << "\n----- ORDER INFO PAGE -----\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        cout << "Order ID: "
             << sqlite3_column_int(stmt, 0) << endl;

        cout << "Customer: "
             << sqlite3_column_text(stmt, 1) << endl;

        cout << "Product: "
             << sqlite3_column_text(stmt, 2) << endl;

        cout << "Quantity: "
             << sqlite3_column_int(stmt, 3) << endl;

        cout << "Total: $"
             << sqlite3_column_double(stmt, 4) << endl;

        cout << "Status: "
             << sqlite3_column_text(stmt, 5) << endl;

        cout << "---------------------------\n";
    }

    sqlite3_finalize(stmt);
}
