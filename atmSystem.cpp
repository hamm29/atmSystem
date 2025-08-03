
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientDateFile = "ClientsData.txt";

struct stClient
{
    string AccountNumber;
    string PINcode;
    string Name;
    string Phone;
    float Balance;
};

enum enATMMainMenueOptions
{
    QuickWithdraw = 1, NormalWithdraw = 2, Deposit = 3,
    CheckBalance = 4, Logout = 5
};

enum enQucikWithdarq
{
    twenty = 1, fifty = 2, oneHundred = 3, twoHundred = 4, fourHundred = 5,
    sixHundred = 6, eightHundred = 7, oneThousand = 8, Exit = 9
};

stClient CurrentClient;

void Login();

void ShowATMMainMenueScreen();
void ShowQuickWithdraw();
void GoBackToMainScreen();
void ShowNormalWithdrawScreen();

vector <string> SplitString(string s, string seperator = "#//#")
{
    vector <string> Words;
    string word;
    short pos;

    while ((pos = s.find(seperator)) != std::string::npos)
    {
        word = s.substr(0, pos);

        if (word != "")
        {
            Words.push_back(word);
        }
        s.erase(0, pos + seperator.length());
    }

    if (s != "")
        Words.push_back(s);

    return Words;
}

stClient ConvertLineToRecord(string line)
{
    stClient Client;
    vector <string> sWord;
    sWord = SplitString(line);

    Client.AccountNumber = sWord[0];
    Client.PINcode = sWord[1];
    Client.Name = sWord[2];
    Client.Phone = sWord[3];
    Client.Balance = stoi(sWord[4]);

    return Client;
}

string ConvertClientRecordToLine(stClient Client)
{
    string line;
    line = Client.AccountNumber + "#//#";
    line += Client.PINcode + "#//#";
    line += Client.Name + "#//#";
    line += Client.Phone + "#//#";
    line += to_string(Client.Balance);

    return line;
}

vector <stClient> LoadClientDateFromFile(string fileName)
{
    fstream myfile;
    vector <stClient> vClients;

    myfile.open(fileName, ios::in);

    if (myfile.is_open())
    {
        string line;
        stClient Client;
        while (getline(myfile, line))
        {
            Client = ConvertLineToRecord(line);
            vClients.push_back(Client);
        }
        myfile.close();
    }
    return vClients;
}

bool FindClientByAccountNumAndPinCode(string AccountNumber, string PINCode, stClient& Client)
{
    vector <stClient> vClients = LoadClientDateFromFile(ClientDateFile);

    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PINcode == PINCode)
        {
            Client = C;
            return true;
        }
    }
    return false;

}

void SaveClientDateToFile(string fileName, vector <stClient> Clients)
{
    fstream myFile;

    myFile.open(fileName, ios::out);

    if (myFile.is_open())
    {
        string line;
        
        for (stClient C : Clients)
        {
            line = ConvertClientRecordToLine(C);
            myFile << line << endl;
        }
    }
}

void ReadAccountNumberAndPinCode(string& AN, string& PinCode)
{
    cout << "\nEnter Account Number? ";
    cin >> AN;
    cout << "\nEnter PIN Code? ";
    cin >> PinCode;
}

void ShowCheckBalance();

void ShowQuickWithdrawScreenHeader()
{
    system("cls");
    cout << "=======================================================\n";
    cout << "\t\tQuick Withdraw Screen";
    cout << "\n=======================================================\n";
    cout << "\t [1] 20 \t\t [2] 50";
    cout << "\n";
    cout << "\t [3] 100\t\t [4] 200";
    cout << "\n";
    cout << "\t [5] 400\t\t [6] 600";
    cout << "\n";
    cout << "\t [7] 800\t\t [8] 1000";
    cout << "\n";
    cout << "\t [9] Exit";
    cout << "\n=======================================================\n";
    cout << "Your Balance is " << CurrentClient.Balance;
}

void ShowBalanceAfterWithdraw(float Balance)
{
    cout << "\nBalance After Withdraw is: " << Balance;
}

bool isAmountExceedsBalance(float Balance, float Amount)
{
    if (Balance < Amount)
    {
        return true;
    }
    else
        return false;
}

double ReadDepositAmount()
{
    double Amount;
    cout << "\nEnter a positive Deposit Amount? ";
    cin >> Amount;

    while (Amount <= 0)
    {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> Amount;
    }
    return Amount;
}

bool DepositBalanceToClientByAccountNumber(string AccountNo, double Amount, vector <stClient> vClients)
{
    char Ans = 'n';

    cout << "\n\nAre you sure you want to perfrom this transaction? y/n? ";
    cin >> Ans;

    if (Ans == 'y' || Ans == 'Y')
    {
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNo)
            {
                C.Balance += Amount;
                SaveClientDateToFile(ClientDateFile, vClients); // there is a problem, which is 
                cout << "\n\nDone Successfully. New balance is: " <<
                    C.Balance;

                return true;
            }
        }
        return false;
    }
}

void PerformDepositOption()
{
    double DepositAmount = ReadDepositAmount();

    vector <stClient> vClients = LoadClientDateFromFile(ClientDateFile);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
    CurrentClient.Balance += DepositAmount;
}

void ShowNormalWithdrawScreenHeader()
{
    cout << "=======================================================\n";
    cout << "\t\tNormal Withdraw Screen";
    cout << "\n=======================================================\n";
    cout << "\nYour Balance is " << CurrentClient.Balance;
}

int ReadWithdrawAmount()
{
    int Amount;

    cout << "\nEnter an amount multiple of 5's ? ";
    cin >> Amount;

    while (Amount % 5 != 0)
    {
        cout << "\nEnter an amount multiple of 5's? ";
        cin >> Amount;
    }
    return Amount;
}

void PerformNormalWithdrawOption()
{
    int WithdrawAmount = ReadWithdrawAmount();
    
    if (WithdrawAmount > CurrentClient.Balance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
    }

    vector <stClient> vClients = LoadClientDateFromFile(ClientDateFile);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
    CurrentClient.Balance -= WithdrawAmount;

}

short getQuickWithdrawAmount(short QuickWithdrawOption)
{
    switch (QuickWithdrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice >9)
    {
        cout << "\nChoose what to do from [1] to [9]? ";
        cin >> Choice;
    }

    return Choice;
}

void QuickWithdrawOperation(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
        return;

    short WithdrawBalance = getQuickWithdrawAmount(QuickWithdrawOption);
    
    if (WithdrawBalance > CurrentClient.Balance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdraw();
        return;
    }
    vector <stClient> vClients = LoadClientDateFromFile(ClientDateFile);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);
    CurrentClient.Balance -= WithdrawBalance;
}

//  ATM Main Menue Options functions:
void ShowQuickWithdraw()
{
    ShowQuickWithdrawScreenHeader();
    QuickWithdrawOperation(ReadQuickWithdrawOption());
}

void ShowNormalWithdrawScreen()
{
    ShowNormalWithdrawScreenHeader();
    PerformNormalWithdrawOption();
}

void ShowDeposit()
{
    system("cls");
    cout << "=======================================================\n";
    cout << "\t\tDeposit Screen";
    cout << "\n=======================================================\n";
    PerformDepositOption();
}

void ShowCheckBalance()
{
    cout << "=======================================================\n";
    cout << "\t\tcheck Balance Screen";
    cout << "\n=======================================================\n";
    cout << "Your Balance is " << CurrentClient.Balance;
}
//-------------------------------------------------------------------------
void GoBackToMainScreen()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    
    ShowATMMainMenueScreen();
}

void PerformATMMainMenue(enATMMainMenueOptions ClientChoice)
{
    switch (ClientChoice)
    {
    case QuickWithdraw:
        system("cls");
        ShowQuickWithdraw();
        GoBackToMainScreen();
        break;
    case NormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainScreen();
        break;
    case Deposit:
        system("cls");
        ShowDeposit();
        GoBackToMainScreen();
        break;
    case CheckBalance:
        system("cls");
        ShowCheckBalance();
        GoBackToMainScreen();
        break;
    case Logout:
        system("cls");
        Login();
        break;
    default:
        break;
    }
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

short ReadClientChoice()
{
    short Choice;
    cout << "Choose what do you want to do? [1 to 5]? ";
    cin >> Choice;
    return Choice;
}

void ShowATMMainMenueScreen()
{
    system("cls");
    cout << "=======================================================\n";
    cout << "\t\tATM Main Menue Screen";
    cout << "\n=======================================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "=======================================================\n";
    PerformATMMainMenue(enATMMainMenueOptions(ReadClientChoice()));
}

void Login()
{
    bool LoginFaild = false;
    string AccountNum, PinCode;

    do
    {
        system("cls");
        cout << "\n--------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n--------------------------------\n";

        if (LoginFaild)
        {
            cout << "\nInvalid Account Number/PinCode!\n";
        }
        ReadAccountNumberAndPinCode(AccountNum, PinCode);

        LoginFaild = !LoadClientInfo(AccountNum, PinCode);
    } while (LoginFaild);
    ShowATMMainMenueScreen();
}

int main()
{
    Login();
    
    system("pause>0");
}
