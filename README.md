🏦 Banking System Management Application (C Language)
📌 Project Overview

This project is a console-based Banking System Management Application developed in C language.
It simulates real-world banking operations with role-based access control, persistent storage, and transaction logging.

The system supports two user roles:

Admin

Customer

Each role has clearly defined responsibilities and permissions, similar to a real banking environment.

🎯 Key Objectives

Implement a menu-driven banking system

Apply modular programming using multiple source files

Use file handling for permanent data storage

Maintain data integrity during financial transactions

Demonstrate real-world software design principles

🧑‍💼 User Roles & Features
🔐 Admin Features

Create new customer bank accounts

Display all active accounts

Search account by account number

Delete accounts (logical deletion with balance check)

View all transactions (audit trail)

Secure logout

👤 Customer Features

View own account details

Deposit money

Withdraw money (with balance validation)

Transfer money to another account (atomic operation)

View personal transaction history

Secure logout

🗂️ File Structure
banking_system/
│
├── main.c            → Program entry point & main menu
├── banking.h         → Common macros, structures, includes
│
├── auth.c / auth.h   → Login & authentication logic
├── admin.c / admin.h → Admin-related operations
├── customer.c / customer.h → Customer-related operations
├── file_utils.c / file_utils.h → File initialization utilities
│
├── accounts.dat      → Stores bank account records
├── users.dat         → Stores user login credentials
├── transactions.dat → Stores transaction logs
│
└── README.md

🧱 Data Structures Used
struct BankAccount

Stores bank account details such as account number, holder name, account type, balance, and active status.

struct Transaction

Stores transaction details including transaction ID, account numbers involved, transaction type, amount, date, and time.

struct User

Stores login credentials and role information.

struct Session

Maintains runtime session details of the logged-in user.

💾 Data Persistence

All data is stored using binary files:

accounts.dat → Account records

users.dat → Login credentials

transactions.dat → Transaction history

The system ensures:

No data loss after program exit

Unique account numbers across executions

Safe file updates using temporary files

🔐 Security & Validation

Role-based access (Admin vs Customer)

Customers can access only their own accounts

Validation for:

Positive transaction amounts

Sufficient balance before withdrawal/transfer

Valid receiver account for transfers

Logical deletion instead of physical deletion

⚙️ Compilation & Execution
Compile (Linux / macOS / Windows – GCC):
gcc main.c auth.c admin.c customer.c file_utils.c -o bank

Run:
./bank


(or bank.exe on Windows)

🧪 Default Admin Credentials

On first run, the system automatically creates a default admin:

Username: admin
Password: admin123


(These can be changed later from code if required.)

🧠 Design Highlights

Modular & scalable architecture

Clean separation of concerns

Atomic transaction handling

Audit-friendly transaction logging

Suitable for viva, exams, and interviews

📚 Concepts Demonstrated

File handling (fopen, fread, fwrite, rename)

Structures and modular programming

Menu-driven applications

Role-based authentication

Real-world banking logic

✅ Conclusion

This project demonstrates a complete and realistic banking system implemented in C, following good software engineering practices.
It is designed to be extendable, secure, and easy to maintain, making it ideal for academic evaluation and learning purposes.
