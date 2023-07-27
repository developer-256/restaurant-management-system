// username ==> qwe
// password ==> 123
#include <iostream>
#include <fstream>
using namespace std;

#define limit 100
string data[limit][4]; // Whole .txt file data will be stored in this when import_data() is called and this is global.
int rows = 0;          // rows are calculated and stored in this for later use in for loops in iterating 2D array
void import_data()
{
    data[limit][4]; // initializing array in which we will import data

    ifstream file;                   // it must be ifstream not fstream
    file.open("Data.txt", ios::app); // set file name and ios::app will not overwrite file while writing it again

    rows = 0;               // declaring rows (these are total rows in .txt file) must do otherwise every time value in global variable will be increased
    while (file.eof() == 0) // reading file till end of file
    {
        string temp_row = "";
        getline(file, temp_row); // take line 1 of file as input in temp_row
        if (temp_row == "")      // if it is empty row you will skip to next row
        {
            rows++;
            continue;
        }
        int column = 0;
        string temp_col = "";
        for (int j = 0; j < temp_row.length(); j++) // breaking row that we input into different values
        {
            if (temp_row[j] != ',') // if , does not occur keep adding character in temp_row
            {
                temp_col += temp_row[j];
            }
            else // if , occur save temp_row into 2D array
            {
                data[rows][column] = temp_col;
                temp_col = ""; // empty temp_col
                column++;
            }
        }
        rows++;
    }
    file.close(); // closing file
}
void update_txt_file()
{
    ofstream file;
    file.open("data.txt");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            file << data[i][j] << ',';
        }
        if (i < rows - 1) // if we dont do this after last row, an empty row will be added
        {
            file << endl;
        }
    }
    file.close();
}
bool admin_auth()
{
    string user, pass;

    cout << "Enter your Username: ";
    cin >> user;

    cout << "Enter password: ";
    cin >> pass;

    return (user == "qwe" && pass == "123");
}
void show_menu()
{
    import_data();
    cout << "-----------(Menu)-----------\n\n";
    for (int i = 0; i < rows; i++)
    {
        if (data[i][0] == "menu")
        {
            cout << "Food code: " << data[i][1] << endl;
            cout << "Item: " << data[i][2] << endl;
            cout << "Price: " << data[i][3] << endl
                 << endl;
        }
    }
}
void take_order()
{
    show_menu();
    cout << endl
         << endl;
    string status = "processing", order_id, customer;
    cout << "Enter order id: ";
    cin >> order_id;
    cout << "Enter customer name: ";
    getline(cin >> ws, customer);

    cout << endl;
    // checking how many dishes we have
    int total_dish = 0;
    import_data();
    for (int i = 0; i < rows; i++)
    {
        if (data[i][0] == "menu")
        {
            total_dish++;
        }
    }
    int price = 0; // price will be stored in this
    // taking order in array with size of total dishes
    string food_ordered[total_dish];
    for (int i = 0; i < total_dish; i++)
    {
        cout << "Enter the food code or @ to exit: ";
        cin >> food_ordered[i];
        string temp = food_ordered[i];
        if (food_ordered[i] != "@")
        {
            for (int i = 0; i < rows; i++) // this for loops calculates price of food
            {
                if (data[i][0] == "menu")
                {
                    if (data[i][1] == temp)
                    {
                        price = price + stoi(data[i][3]);
                    }
                }
            }
        }
        else
        {
            break;
        }
    }

    ofstream file;
    file.open("data.txt", ios::app);
    file << endl
         << status << ',' << order_id << ',' << customer << ',' << price << ',';
    file.close();
}
void order_in_kitchen()
{
    import_data();
    for (int i = 0; i < rows; i++)
    {
        if (data[i][0] == "processing")
        {
            cout << "Order id: " << data[i][1] << endl;
            cout << "Customer name: " << data[i][2] << endl
                 << endl;
        }
    }
}
void mark_processed()
{
    import_data();
    int test = 0;
    string order_id = "";
    cout << "Enter the order id: ";
    cin >> order_id;
    for (int i = 0; i < rows; i++)
    {
        if (data[i][1] == order_id)
        {
            data[i][0] = "processed";
            test++;
        }
    }
    if (test == 1)
    {
        cout << "Enjoy you meal\n";
        update_txt_file();
    }
    else
    {
        cout << "Order not found\n";
    }
}
void all_processed()
{
    import_data();
    for (int i = 0; i < rows; i++)
    {
        if (data[i][0] == "processed")
        {
            cout << "Order id: " << data[i][1] << endl;
            cout << "Customer name: " << data[i][2] << endl;
            cout << "Total price: Rs." << data[i][3] << endl
                 << endl;
        }
    }
}
void add_item()
{
    string status = "menu";
    string name, food_code, price;
    cout << "Enter the name of the dish: ";
    getline(cin >> ws, name);
    cout << "Enter dish code: ";
    cin >> food_code;
    cout << "Enter the price: ";
    cin >> price;

    ofstream file;
    file.open("data.txt", ios::app);
    file << endl // if you dont add this last line will remain empty everytime you run this function
         << status << ',' << food_code << ',' << name << ',' << price << ',';
    file.close();
}
void revenue()
{
    import_data();
    int revenue = 0;
    for (int i = 0; i < rows; i++)
    {
        if (data[i][0] == "processed")
        {
            revenue += stoi(data[i][3]);
        }
    }
    cout << "Total revenue generated: Rs." << revenue << endl;
}

int main()
{
    while (admin_auth() == false)
    {
        cout << "Invalid Username or password\n";
        system("cls");
    }

    int choice;
    do
    {
        cout << "________________________________\n";
        cout << "| Choose one of the following: |\n";
        cout << "********************************\n";
        cout << "| 1: Show menu                 |\n";
        cout << "| 2: Take order                |\n";
        cout << "| 3: All order in kitchen      |\n";
        cout << "| 4: Mark order processed      |\n";
        cout << "| 5: All order processed       |\n";
        cout << "| 6: Add item to menu          |\n";
        cout << "| 7: Total revenue generated   |\n";
        cout << "| 8: Exit                      |\n";
        cout << "********************************\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls"); // to clear console
            show_menu();
            break;

        case 2:
            system("cls");
            take_order();
            break;

        case 3:
            system("cls");
            order_in_kitchen();
            break;

        case 4:
            system("cls");
            mark_processed();
            break;

        case 5:
            system("cls");
            all_processed();
            break;

        case 6:
            system("cls");
            add_item();
            break;

        case 7:
            system("cls");
            revenue();
            break;

        case 8:
            system("cls");
            cout << "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
            cout << "%%%% (Thanks for using our software) %%%\n";
            cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n";
            break;

        default:
            system("cls");
            cout << "Invalid Input\n";
            break;
        }

    } while (choice != 8);

    return 0;
}
