/*
    Kyle Free
    COSC 1436
    4/5/2023
    Stock Profit

    This program uses four objects to make a purchase and a sale of company stock,
        and demonstrates the proper use of expressions, statements, and formatting.
*/

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

namespace WeekThreeAssignment {

    template <typename T>
    string FormatComma(T &num) {

        int wholeNum = static_cast<int>(num);
        double fracNum = num - wholeNum;
        auto
            result1 = to_string(wholeNum),
            result2 = to_string(fracNum).substr(1, 3);

        int
            iterator = result1.length() - 3,
            end = 0;

        while (iterator > end) {
            result1.insert(iterator, ",");
            iterator -= 3;
        }

        return result1 + result2;
    }

    /*
        The Stock structure holds information about the stock purchased and sold.
        A stock stucture is created when the User purchases stock through the Purchase Object
    */
    struct Stock {
        // Attributes
        int shares; // The number of shares in the stock
        bool type; // The type of transaction. True for BUY; False for SELL

        double
            purchasePrice,
            salePrice,
            purchaseValue,
            saleValue;

        // Constructors
        Stock() :
            shares(0),
            purchasePrice(0.0),
            salePrice(0.0),
            purchaseValue(0.0),
            saleValue(0.0) {}

        Stock(int shares) :
            shares(shares) {}

        // This constructor is mostly for the Steve user demonstration
        Stock(int shares, double price, bool type) :
            shares(shares),
            type(type) {

                if (type) { // if bought

                    this->purchasePrice = price;
                    purchaseValue = (double) shares  * purchasePrice;
                }

                else {

                    this->salePrice = price;
                    saleValue = (double) shares * salePrice;
                }
            }
        /*
        void Print() {

            cout << fixed << setprecision(2)
                << "Shares: " << shares << endl
                << "P Price: $" << purchasePrice << endl
                << "S Price: $" << salePrice << endl
                << "Total Value: $" << totalValue << endl;
        }
        */
    };

    // This interface contains virtual utility functions for the Purchase and Sale objects
    class IUtility {
    
        public:
        virtual void SetPrice() = 0;
        virtual double Calculate() { return 0.0; }
        virtual Stock* GetStock() = 0;
        virtual void Print() = 0;
    };

    /*
        The Purchase class stores information about the purchase that is taking place.
        The Stock pointer contains the stock being purchcased, and is created when the
            object is created.
    */
    class Purchase : public IUtility {
        // Attributes
        Stock *stock{ nullptr };

        double
            commissionRate,
            commissionAmt,
            total;

        public:
        // Constructors
        Purchase(int shares) :
            stock(new Stock(shares)) {

            SetPrice();
            SetCommissionRate();
            Calculate();
        }

        // This constructor is for the Steve User Object
        Purchase(int shares, double price, double commissionRate) :
            stock(new Stock(shares, price, true)),
            commissionRate(commissionRate) 
                { Calculate(); }
        
        ~Purchase() { delete stock; }

        /*
            SetPrice prompts the user to enter the purchase price of the stock
                and then stores it in the stock struct
        */
        void SetPrice() {

            cout << "What was the purchase price of the stock?\n$";
            cin >> stock->purchasePrice;
        }

        // SetCommissionRate prompts the user to enter the commission rate of the purchase
        void SetCommissionRate() {

            cout
                << "What percent commission did you pay for this purchase? "
                << "(For example, type in 7 for 7%) ";
            cin >> commissionRate;
        }

        // Calculate runs the proper operations needed to complete the purchase
        double Calculate() {

            if (commissionRate != 0.0) {

                commissionRate /= 100; // converts the user input into a usable number
                stock->purchaseValue = stock->purchasePrice * stock->shares; // store the purchase value of the stock
                commissionAmt = stock->purchaseValue * commissionRate; // calculate the commission about of the purchase
                total = stock->purchaseValue + commissionAmt; // store the total of the purchase; stock plus commission
            } else return 0.0;

            return total; // return the total of the purchase
        }

        // Print all necessary information
        void Print() {



            cout
                << fixed << showpoint << setprecision(2) << setfill(' ')
                << "Purchase Price: \t$"
                    << setw(10) << internal << FormatComma(stock->purchaseValue)
                << "\nPurchase Commission: \t$"
                    << setw(10) << internal << FormatComma(commissionAmt)
                << "\nPurchase Total: \t$"
                    << setw(10) << internal << FormatComma(total) << endl;
        }

        // GetStock returns the stock pointer, not the structure
        Stock* GetStock()
            { return stock; }

        double GetTotal()
            { return total; }
    };

    /*
        The Sale class uses data from a Purchase object to sell the stock
            for a certain price, then calculates the profit/loss
    */
    class Sale : public IUtility {
        // Attributes
        Purchase *purchase;
        Stock *stock;

        double
            commissionRate,
            commissionAmt,
            total,
            profit;

        public:
        // Constructors
        Sale(Purchase* purchase, int shares) : 
            purchase(purchase),
            stock(this->purchase->GetStock()) {

                SetPrice();
                SetCommissionRate();
                Calculate();
            }

        // Steve Sale constructor
        Sale(Purchase *purchase, int shares, double price, double commissionRate) :
            purchase(purchase),
            stock(new Stock(shares, price, false)),
            commissionRate(commissionRate)
                { Calculate(); }

        /*
            SetPrice() prompts the user to enter the purchase price of the stock
                and then stores it in the stock struct
        */
        void SetPrice() {

            cout << "What was the sale price of the stock?\n$";
            cin >> stock->salePrice;
        }

        // SetCommissionRate() prompts the user to enter the commission rate of the purchase
        void SetCommissionRate() {

            cout
                << "What percent commission did you pay for this sale? "
                << "(For example, type in 7 for 7%) ";
            cin >> commissionRate;
        }

        // Calculate() runs the proper operations needed to complete the purchase
        double Calculate() {

            if (commissionRate != 0.0) {

                commissionRate /= 100; // converts the user input into a usable number
                stock->saleValue = (stock->salePrice * stock->shares); // store the purchase value of the stock
                commissionAmt = stock->saleValue * commissionRate; // calculate the commission about of the purchase
                total = stock->saleValue - commissionAmt; // store the total of the purchase; stock plus commission
                profit = total - purchase->GetTotal(); // Calculate profit
            } else return 0.0;

            return total;
        }

        // Return the pointer to the stock structure
        Stock* GetStock()
            { return stock; }

        // Print all necessary information
        void Print() {

            cout
                << fixed << showpoint << setprecision(2) << setfill(' ')
                << "Sale Price: \t\t$"
                    << setw(10) << internal << FormatComma(stock->saleValue)
                << "\nSale Commission: \t$"
                    << setw(10) << internal << FormatComma(commissionAmt)
                << "\nSale Total: \t\t$"
                    << setw(10) << internal << FormatComma(total) << endl;

            if (profit > 0) cout << "Profit: \t\t$";
            else cout << "Loss: \t\t\t$";

            cout << setw(10) << internal << FormatComma(profit) << endl;
        }
    };

    /* 
        The User class is where the program is born. The user object is created,
            from here the Stock, Purchase, and Sale objects are created.
        Each User has a purchase and a sale object
    */
    class User {
        // Attributes
        string *userName{ nullptr };
        Purchase *purchase{ nullptr };
        Sale *sale{ nullptr };

        // Constructors
        public:
        User()
            { Run(); }

        // This constructor will be used to build the Steve user
        User(string userName) :
            userName(new string(userName)) {

                if (userName == "Steve") {

                    purchase = new Purchase(1000, 50.00, 2);
                    Portfolio();
                }

                else cout << "PROBLEM" << endl;
        }

        // Destructor
        ~User() {
            // These blocks are used to sell the Steve Object's stock upon destruction
            if (*userName == "Steve") {

                if (purchase != nullptr) {

                    sale = new Sale(purchase, purchase->GetStock()->shares, 100.0, 2);
                    sale->Print();

                    delete purchase;
                }
            }

            // Clear memory upon destruction
            delete
                userName,
                purchase,
                sale;
        }

        private:
        // AskName() prompts the user for their name and stores it into the userName pointer
        void AskName() {

            userName = new string();

            cout << "What is your name?" << endl;
            getline(cin, *userName);
        }

        // Action() starts the program by prompting the user to buy stock if none are owned,
        //  and sell stock if some is owned
        void Action() {

            if (purchase == nullptr) {

                cout << "You do not own any stock." << endl;
                Buy();
            }

            else if (sale == nullptr) {

                cout << "You own " << purchase->GetStock()->shares << " shares." << endl;
                Sell();
            }
        }

        // Buy() prompts the user as to how many shares to buy,
        //  then passes the number to a new Purchase object
        void Buy() {

            cout << "How many shares would you like to buy?" << endl;

            int shares;
            cin >> shares;

            purchase = new Purchase(shares);
        }

        // Sell() prompts the user as to how many shares to sell,
        //  then passes the number to a new Sale object
        void Sell() {

            cout << "How many shares would you like to sell?" << endl;

            int shares;
            cin >> shares;

            while (shares > purchase->GetStock()->shares) {

                cout
                    << "You do not own that many shares. "
                    << "Please enter a number less than or equal to "
                    << purchase->GetStock()->shares << endl;

                cin >> shares;
            }

            sale = new Sale(purchase, shares);
        }

        // Portfolio() prints out information related to the User's purchase
        void Portfolio()
            { purchase->Print(); }

        // Runs the program as required
        void Run() {

            AskName();
            Action();
            Action();

            purchase->Print();
            sale->Print();
        }
    };
}

using namespace WeekThreeAssignment;

// DRIVER FUNCTION
int main() {

    cout << "Welcome to the XYZ Company Stock Portal." << endl;

    cout << "STEVE OBJECT STEVE OBJECT STEVE OBJECT" << endl;
    User* steve = new User("Steve");
    delete steve;

    cout << "USER OBJECT USER OBJECT USER OBJECT" << endl;
    User run;

    return 0;
}

