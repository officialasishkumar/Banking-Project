#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#define MIN_BALANCE 500
using namespace std;

class InsufficientFunds; 

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;

    static long NextAccountNumber;   // Attribute of an Entire class
public:
    // No-Arg Constructor..
    Account() {};
    // Parameterized Constructor...
    Account(string fname ,string lname ,float balance);

    // Getters....
    long getAccNo() {return accountNumber;}
    string getFirstName() {return firstName;}
    string getLastName() {return lastName;}
    float getBalance() {return balance;}

    // Member Funcitons....
    void Deposit(float amount);
    void Withdraw(float amount);
    static void SetNextAccountNumber(long accountNumber);

    // Friend Functions.....
    friend ofstream& operator<<(ofstream &fout ,Account &acc);
    friend ifstream& operator>>(ifstream &fin , Account &acc);
    friend ostream& operator<<(ostream& cout , Account &acc);
};
// Intializing Static Member...
long Account::NextAccountNumber = 0;


class Bank
{
private:
    map<long , Account> accounts;   // Act as a Container..in which each account(obj.) created is stored
public:
    // No Arg Constructor..
    Bank();
    // Member Function....
    Account OpenAccount(string fname , string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber , float amount);
    Account Withdraw(long accountNumber , float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();

    // Destructor...
    ~Bank();
};

int main()
{
    // Objects Instantiated....of each Class....
    Bank b;
    Account acc;

    string fname, lname;
    long accountNumber;
    float balance;
    float amount;

    int choice;

    cout <<"***Banking System***" << endl;

    do
    {
        cout << "\nSelect one option below--- ";
        cout << "\n1.) Open an Account: ";
        cout << "\n2.) Balance Enquiry: ";
        cout << "\n3.) Deposit: ";
        cout << "\n4.) Withdrawl: ";
        cout << "\n5.) Close an Account: ";
        cout << "\n6.) Show All Account: ";
        cout << "\n7.) Quit..";
        cout << "\nEnter your Choice: ";
        cin >> choice;
        switch(choice)
        {
            case 1:
                cout << "Enter first Name: ";
                cin >> fname;
                cout << "Enter last Name: ";
                cin >> lname;
                cout << "Enter Initial Balance: ";
                cin >> balance;
                acc = b.OpenAccount(fname, lname, balance);
                cout << endl << "Congratulations Account is Created"<< endl;
                cout << acc;
                break;
            
            case 2:
                cout << "Enter accountNumber: ";
                cin >> accountNumber;
                acc = b.BalanceEnquiry(accountNumber);
                cout << endl << "Your Account Details"<<endl;
                cout << acc;
                break;

            case 3:
                cout << "Enter accountNumber: ";
                cin >> accountNumber;
                cout << "Enter amount to be deposited: ";
                cin >> amount;
                acc = b.Deposit(accountNumber ,amount);
                cout << endl<<"Amount Deposited"<< endl;
                cout << acc;
                break;
            
            case 4:
                cout << "Enter accountNumber: ";
                cin >> accountNumber;
                cout << "Enter amount to be withdraw: ";
                cin >> amount;
                acc = b.Withdraw(accountNumber , amount);
                cout << endl << "Amount Withdrawn" << endl;
                cout << acc;
                break;
            
            case 5:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                b.CloseAccount(accountNumber);
                cout<< endl << "Your Account is Closed" << endl;
                break;

            case 6:
                b.ShowAllAccounts();
                break;
            
            case 7 :
                break;
            
            // Incase Some Other Input is passed in place of (int) in choice.
            default:
                cout << "\nEnter correct choice";
                exit(0);
        }
    }while(choice != 7);

    return 0;
}

// Our own(User) Defined class inherting from In-built class--(exception) ..
// It helps us to return a C-style literal string...
class InsufficientFunds : public exception
{
    virtual const char* what() const noexcept {
        return "Withdrawl not possibel bcz of InsufficientFunds ";
    }
};
// ======================================================================================

// Parameterized Constructor...
Account::Account(string fname, string lname, float balance)
    :firstName(fname), lastName(lname) ,balance(balance)
{
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
}

// Deposit funciton...
void Account::Deposit(float amount)
{
    if (amount < 0) 
    {
        cerr << " Enter valid amount !!!!!!!";
        return ;
    }
    else{
        balance += amount;
    }
}

// Withdraw Function...
void Account::Withdraw(float amount)
{
    if(balance - amount < MIN_BALANCE ){
        throw InsufficientFunds();
    }
    else
    {
        balance -= amount;
    }
}

// Operator Overloading......
ofstream& operator<<(ofstream &fout, Account &acc)
{
    fout << acc.accountNumber <<endl;
    fout << acc.firstName <<endl;
    fout << acc.lastName <<endl;
    fout << acc.balance <<endl;
    return fout;
}
ifstream& operator>>(ifstream &fin ,Account &acc)
{
    fin >> acc.accountNumber;
    fin >> acc.firstName;
    fin >> acc.lastName;
    fin >> acc.balance;
    return fin;
}
ostream & operator<<(ostream &os,Account &acc)
{
    os <<"First Name:" << acc.getFirstName() << endl;
    os <<"Last Name:" << acc.getLastName() << endl;
    os <<"Account Number:" << acc.getAccNo() << endl;
    os <<"Balance:" << acc.getBalance() << endl;
    return os;
}

// This func. is req.- when we exit our prog. and again runs it to watch the previously accounts created 
// and we our in bank constructor and extracting the previous date in the container of bank.(accounts) then it helps the bank to maintain serial order of account_number 
void Account::SetNextAccountNumber( long accountNumber )
{
    NextAccountNumber = accountNumber;
}


// =================================**************************====================================================


// Bank Constructor....Most Imp....
// bcz. when we exit our program and agian run it firstly bank obj. is instanstiated and NextNumberindex gets assigned by extracting data from file 
Bank::Bank() 
{
    Account account;
    ifstream fin;

    fin.open("Bank.dat");
    if(!fin.is_open())
    {
        cerr << "Error in Opening! File not found! " << endl;
        return;
    }
    
    while (!fin.eof())
    {
        fin >> account;
        accounts.insert(pair<long, Account> (account.getAccNo() ,account));
    }
    Account::SetNextAccountNumber(account.getAccNo());
    fin.close();
}

Account Bank::OpenAccount(string fname,string lname,float balance)
{
    ofstream fout;
    Account account(fname, lname, balance);
    accounts.insert(pair<long ,Account> (account.getAccNo() , account));

    fout.open("Bank.dat");

    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++) {
    fout<<itr->second;
    }
    fout.close();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
    map<long , Account>::iterator itr= accounts.find(accountNumber);
    return itr->second;
}

Account Bank::Deposit(long accountNumber , float amount)
{
    map< long, Account >::iterator itr = accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::Withdraw(long accountNumber , float amount)
{
    map<long, Account> ::iterator itr = accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
    map<long,Account>::iterator itr = accounts.find(accountNumber);
    cout<<"Account Deleted"<<itr->second;
    accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
    }
}

// Destructor...of bank
Bank::~Bank() {
}
