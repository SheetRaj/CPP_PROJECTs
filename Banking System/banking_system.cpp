#include <bits/stdc++.h>
using namespace std;

class Account
{
protected:
    string name;
    int id;
    double balance;

public:
    Account(string name, int id, double balance) : name(name), id(id), balance(balance) {}

    virtual void deposit(double amount)
    {
        if (amount <= 0)
        {
            throw invalid_argument("Deposit amount must be positive.");
        }

        balance += amount;
    }

    virtual void withdraw(double amount)
    {
        if (amount <= 0)
        {
            throw invalid_argument("Withdraw amount must be positive");
        }

        if (amount > balance)
        {
            throw invalid_argument("Insufficient amount");
        }

        balance -= amount;
    }

    int getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }

    double getBalance() const
    {
        return balance;
    }

    virtual void display() const
    {
        cout << "Name: " << name << ", ID: " << id << ", Balance: " << fixed << setprecision(2) << balance << endl;
    }
    virtual string getAccountType() const = 0;

    virtual void save(ofstream &ofs) const
    {
        ofs << getAccountType() << endl
            << name << endl
            << id << endl
            << balance << endl;
    }
};

class SavingsAccount : public Account
{
private:
    double interestRate;

public:
    SavingsAccount(string name, int id, double balance, double interestRate) : Account(name, id, balance), interestRate(interestRate) {}

    void applyInterest()
    {
        balance += balance * interestRate;
    }

    void display() const override
    {
        Account::display();
        cout << "Account Type: Saving, Interest Rate: " << (interestRate * 100) << '%\n';
    }

    string getAccountType() const override
    {
        return "Savings";
    }

    void save(ofstream &ofs) const override
    {
        Account::save(ofs);
        ofs << interestRate << endl;
    }

    static SavingsAccount *load(ifstream &ifs)
    {
        string name;
        int id;
        double balance, interestRate;

        ifs >> name >> id >> balance >> interestRate;

        return new SavingsAccount(name, id, balance, interestRate);
    }
};
class CheckAccount : public Account
{
private:
    double overDriftLimit;

public:
    CheckAccount(string name, int id, double balance, double overDriftLimit) : Account(name, id, balance), overDriftLimit(overDriftLimit) {}

    void withdraw(double amount) override
    {
        if (amount <= 0)
        {
            throw invalid_argument("Withdraw amount must be positive.");
        }

        if (amount > balance + overDriftLimit)
        {
            throw invalid_argument("Overdraft limit exceeded");
        }

        balance -= amount;
    }

    void display() const override
    {
        Account::display();

        cout << "Account Type: Checking, overdraft Limit: " << overDriftLimit << endl;
    }

    string getAccountType() const override
    {
        return "Checking";
    }

    void save(ofstream &ofs) const override
    {
        Account::save(ofs);
        ofs << overDriftLimit << endl;
    }

    static CheckAccount *load(ifstream &ifs)
    {
        string name;
        int id;
        double balance, overDriftLimit;

        ifs >> name >> id >> balance >> overDriftLimit;

        return new CheckAccount(name, id, balance, overDriftLimit);
    }
};

class Bank
{
private:
    vector<Account *> accounts;
    int generateAccountID()
    {
        static int nextID = 1;
        return nextID++;
    }

public:
    ~Bank()
    {
        for (Account *account : accounts)
        {
            delete account;
        }
    }

    void openSavingAccount(const string &name, double initialBalance, double interestRate)
    {
        int newID = generateAccountID();
        accounts.push_back(new SavingsAccount(name, newID, initialBalance, interestRate));

        cout << "Saving account opened with ID: " << newID << endl;
    }

    void openCheckingAccount(const string &name, double initialBalance, double overdraftLimit)
    {
        int newID = generateAccountID();

        accounts.push_back(new CheckAccount(name, newID, initialBalance, overdraftLimit));

        cout << "Checking account opened with ID: " << newID << endl;
    }

    Account *getAccount(int id)
    {
        for (Account *account : accounts)
        {
            if (account->getId() == id)
            {
                return account;
            }
        }

        throw invalid_argument("Account not found.");
    }

    void displayAllAccounts() const
    {
        for (const Account *account : accounts)
        {
            account->display();
        }
        cout << endl;
    }

    void saveAccounts(const string &filename) const
    {
        ofstream ofs(filename);

        if (!ofs)
        {
            throw runtime_error("Could not open file for saving");
        }

        ofs << accounts.size() << endl;

        for (const Account *account : accounts)
        {
            account->save(ofs);
        }
    }

    void loadAccounts(const string &filename)
    {
        ifstream ifs(filename);

        if (!ifs)
        {
            throw runtime_error("Could not open file for loading.");
        }

        size_t numAccounts;

        ifs >> numAccounts;

        for (size_t i = 0; i < numAccounts; i++)
        {
            string accountType;

            ifs >> accountType;

            if (accountType == "Savings")
            {
                accounts.push_back(SavingsAccount::load(ifs));
            }
            else if (accountType == "Checking")
            {
                accounts.push_back(CheckAccount::load(ifs));
            }
            else
            {
                throw runtime_error("Unknown account type in file.");
            }
        }
    }
};

int main()
{
    Bank bank;
    try
    {
        bank.loadAccounts("accounts.txt");
    }
    catch (const exception &error)
    {
        cerr << error.what() << endl;
    }

    bool running = true;

    while (running)
    {
        cout << "Banking system Menu: " << endl;
        cout << "1. Open Saving Account" << endl;
        cout << "2. Open Checking Account" << endl;
        cout << "3. Deposit" << endl;
        cout << "4. Withdraw" << endl;
        cout << "5. Check Balance" << endl;
        cout << "6. Display all Accounts" << endl;
        cout << "7. Save and exit" << endl;
        cout << "Choose  an option: ";

        int choice;
        cin >> choice;
        try
        {

            if (choice == 1)
            {
                string name;
                double initialBalance, interestRate;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter initial balance: ";
                cin >> initialBalance;
                cout << "Enter interest rate: ";
                cin >> interestRate;

                bank.openSavingAccount(name, initialBalance, interestRate);
            }

            else if (choice == 2)
            {
                string name;
                double initialBalance, overdraftLimit;

                cout << "Enter name: ";
                cin >> name;

                cout << "Enter initial Balance: ";
                cin >> initialBalance;

                cout << "Enter overdraft limit: ";
                cin >> overdraftLimit;

                bank.openCheckingAccount(name, initialBalance, overdraftLimit);
            }

            else if (choice == 3)
            {
                int id;
                double amount;
                cout << "Enter account ID: ";
                cin >> id;
                cout << "Enter deposit amount: ";
                cin >> amount;

                bank.getAccount(id)->deposit(amount);
                cout << "Deposit Successful." << endl;
            }

            else if (choice == 4)
            {
                int id;
                double amount;
                cout << "Enter account ID: ";
                cin >> id;

                cout << "Enter withdrawal amount: ";
                cin >> amount;
                bank.getAccount(id)->withdraw(amount);
                cout << "Withdrawal successful." << endl;
            }
            else if (choice == 5)
            {
                int id;
                cout << "Enter account ID: ";
                cin >> id;
                Account *account = bank.getAccount(id);
                cout << "Balance: " << fixed << setprecision(2) << account->getBalance() << endl;
            }
            else if (choice == 6)
            {
                bank.displayAllAccounts();
            }
            else if (choice == 7)
            {
                bank.saveAccounts("accounts.txt");
                running = false;
            }
            else
            {
                cerr << "Invalid choice. Please try again";
            }
        }
        catch (const exception &e)
        {
            cerr << e.what() << endl;
        }
    }

    return 0;
}