
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

void GoBackToMainScreen();

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

void UpdateClientBalanceInFileByAccountNo(string fileName, string AccountNo, float newBalance)
{
    vector <stClient> Clients;
    Clients = LoadClientDateFromFile(fileName);
    for (stClient& C : Clients)
    {
        if (C.AccountNumber == AccountNo)
        {
            C.Balance = newBalance;
            SaveClientDateToFile(fileName, Clients);
            Clients = LoadClientDateFromFile(fileName);
            break;
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

void ShowQuickWithdrawOptionsScreenHeader()
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
    cout << "\nChoose what to withdraw from [1] to [8] ? ";
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

void ShowDepositHeader()
{
    cout << "=======================================================\n";
    cout << "\t\tDeposit Screen";
    cout << "\n=======================================================\n";

}

bool DepositOperation()
{
    float WithdrawAmount;
    ShowDepositHeader();

    cout << "Enter a positive Deposit Amount? ";
    cin >> WithdrawAmount;

    CurrentClient.Balance += WithdrawAmount;
    UpdateClientBalanceInFileByAccountNo(ClientDateFile, CurrentClient.AccountNumber, CurrentClient.Balance);

    cout << "\n\nDone Successfully. New balance is: " << CurrentClient.Balance;

    GoBackToMainScreen();

    return true;
}

bool WithdrawOperation(float WithdrawAmount = 0)
{
    bool AmountExeeds = false;
    AmountExeeds = isAmountExceedsBalance(CurrentClient.Balance, WithdrawAmount);
    if (AmountExeeds == false)
    {
        CurrentClient.Balance -= WithdrawAmount;
        UpdateClientBalanceInFileByAccountNo(ClientDateFile, CurrentClient.AccountNumber, CurrentClient.Balance);
        return false;
    }
    else
    {
        return true;
    }
}

void ShowNormalWithdrawScreenHeader()
{
    cout << "=======================================================\n";
    cout << "\t\tNormal Withdraw Screen";
    cout << "\n=======================================================\n";
    cout << "\nYour Balance is " << CurrentClient.Balance;
}

bool NormalWithdrawOperation()
{
    int WithdrawAmount = 0;
    bool AmountExeeds = false;

    do
    {
        system("cls");
        ShowNormalWithdrawScreenHeader();
        cout << "\nPlease enter an amount multiple of 5's ? ";        
        cin >> WithdrawAmount;
        while(WithdrawAmount % 5 != 0 )
        {
            cout << "\nPlease enter an amount multiple of 5's ? ";
            cin >> WithdrawAmount;
        }

        if (!WithdrawOperation(WithdrawAmount))
        {
            ShowBalanceAfterWithdraw(CurrentClient.Balance);
            return true;
        }
        else
        {
            AmountExeeds = isAmountExceedsBalance(CurrentClient.Balance, WithdrawAmount);
            cout << "\nThe amount exceeds your Balance, make another choice.";
            cout << "Press Any key to continue...";
            system("pause>0");
            //return false;
        }
    } while (AmountExeeds);
}

void QuickWithdrawOperation()
{
    short userChoice;
    float WithdrawAmount = 0;
    bool AmountExeeds = false;

    do
    {
        cin >> userChoice;
        switch (enQucikWithdarq(userChoice))
        {
        case twenty:
            AmountExeeds = WithdrawOperation(20);
            break;
        case fifty:
            AmountExeeds = WithdrawOperation(50);
            break;
        case oneHundred:
            AmountExeeds = WithdrawOperation(100);
            break;
        case twoHundred:
            AmountExeeds = WithdrawOperation(200);
            break;
        case fourHundred:
            AmountExeeds = WithdrawOperation(400);
            break;
        case sixHundred:
            AmountExeeds = WithdrawOperation(600);
            break;
        case eightHundred:
            AmountExeeds = WithdrawOperation(800);
            break;
        case oneThousand:
            AmountExeeds = WithdrawOperation(1000);
            break;
        case Exit:
            ShowATMMainMenueScreen();
            break;
        default:
            break;
        }

        if (AmountExeeds == false)
        {
            ShowBalanceAfterWithdraw(CurrentClient.Balance);
        }
        else
        {
            cout << "\nThe amount exceeds your balance, make another choice.";
            cout << "\nPress Anykey to continue...";
            system("pause>0");
            ShowQuickWithdrawOptionsScreenHeader();
        }

    } while (AmountExeeds);
}

void ShowQuickWithdrawOptionsScreen()
{
    ShowQuickWithdrawOptionsScreenHeader();
    QuickWithdrawOperation();
}

void ShowNormalWithdrawScreen()
{
    NormalWithdrawOperation();
}

//  ATM Main Menue Options functions:
void ShowQuickWithdraw()
{
    ShowQuickWithdrawOptionsScreen();
}

 void ShowNormalWithdraw()
{
     ShowNormalWithdrawScreen();
}

void ShowDeposit()
{
    DepositOperation();
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
        ShowNormalWithdraw();
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
        Login();
        break;
    default:
        break;
    }
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

void ShowLoginScreen()
{
    bool LoginFaild = false;

    string AccountNum, PinCode;
    vector <stClient> vClients = LoadClientDateFromFile(ClientDateFile);

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

        if (!FindClientByAccountNumAndPinCode(AccountNum, PinCode, CurrentClient))
        {
            LoginFaild = true;
        }
        else
        {
            ShowATMMainMenueScreen();
            LoginFaild = false;
        }

    } while (LoginFaild);

}

void Login()
{
    ShowLoginScreen();
}

int main()
{
    Login();
    
    system("pause>0");
}
