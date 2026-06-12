#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cmath>

using namespace std;

const string ClientsFileName = "Clients.txt";

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string PhoneNumber;
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

enum enATM { QuickWithdraw = 1, Withdraw = 2, Deposit = 3, CheckBalance = 4, Logout = 5 };

stClient CurrentClient;

void ShowATMMainMenuFlow();
void ReturnToATMMainMenu();
void WithdrawScreen();
void QuickWithdrawMenuScreen();
void DepositScreen();
void PerfromQuickWithdrawOption(short QuickWithDrawOption);
short getQuickWithDrawAmount(short QuickWithDrawOption);
void Login();

string ReadAccountNumber()
{
	string AccountNumber;

	cout << "Please enter Account Number : ";
	cin >> AccountNumber;

	return AccountNumber;
}

string ReadPIN()
{
	string PINCode;

	cout << "Please enter PIN Code       : ";
	cin >> PINCode;

	return PINCode;
}

vector<string> SplitString(string Text, string delim = "#//#")
{
	vector<string> vString;

	short pos = 0;
	string word;

	while ((pos = Text.find(delim)) != string::npos)
	{
		word = Text.substr(0, pos);
		if (word != "")
		{
			vString.push_back(word);
		}
		Text.erase(0, pos + delim.size());
	}
	if (Text != "")
		vString.push_back(Text);

	return vString;
}

stClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	stClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.PhoneNumber = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);
	return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.PhoneNumber + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

vector <stClient> LoadCleintsDataFromFile(string FileName)
{
	vector <stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

void SaveCleintsDataToFile(string FileName, vector <stClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
}

short ReadQuickWithdrawOption()
{
	short Choice = 0;
	while (Choice < 1 || Choice>9)
	{
		cout << "\nChoose what to do from [1] to [9] ? ";
		cin >> Choice;
	}
	return Choice;
}

void ReturnToATMMainMenu()
{
	cout << "\n\n";
	cout << "Press any key to return Main Menu Screen...\n";
	system("pause>0");
	ShowATMMainMenuFlow();
}

void QuickWithdrawMenuScreen()
{
	system("cls");
	cout << "=============================================\n";
	cout << "             Quick Withdraw Screen           \n";
	cout << "=============================================\n";
	cout << "     [1] 20              [2] 50  \n";
	cout << "     [3] 100             [4] 200 \n";
	cout << "     [5] 400             [6] 600 \n";
	cout << "     [7] 800             [8] 1000\n";
	cout << "     [9] Exit                    \n";
	cout << "=============================================\n";
	cout << "Your Balance = " << CurrentClient.AccountBalance << endl;

	PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}

void ATMMainMenueScreen()
{
	cout << "================================================\n";
	cout << "             ATM Main Menue Screen                  \n";
	cout << "================================================\n";
	cout << "     [1] Quick Withdraw.\n";
	cout << "     [2] Normal Withdraw.\n";
	cout << "     [3] Deposit.\n";
	cout << "     [4] Check Balance.\n";
	cout << "     [5] Logout.\n";
	cout << "================================================\n";
}

void CheckBalanceScreen()
{
	system("cls");
	cout << "=============================================\n";
	cout << "            Check Balance Screen             \n";
	cout << "=============================================\n";
	cout << "Your Balance = " << CurrentClient.AccountBalance;
}

double ReadDepositAmount()
{
	double Amount = 0;
	
	while (Amount <= 0)
	{
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount;
	}
	return Amount;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount, vector <stClient>& vClients)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance = " << C.AccountBalance;
				return true;
			}
		}
		return false;
	}
	return false;
}

void PerfromDepositOption()
{
	double DepositAmount = ReadDepositAmount();

	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

	if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients))
		CurrentClient.AccountBalance += DepositAmount;
}

void DepositScreen()
{
	system("cls");
	cout << "==============================================\n";
	cout << "                Deposit Screen                \n";
	cout << "==============================================\n";

	PerfromDepositOption();
}

int ReadWithDrawAmont()
{
	int Amount;

	do
	{
		cout << "\nEnter an amount multiple of 5's : ";
		cin >> Amount;

		while (Amount > CurrentClient.AccountBalance)
		{
			cout << "\nAmount exeeds, Enter another amount : ";
			cin >> Amount;
		}

	} while (Amount % 5 != 0 || Amount > CurrentClient.AccountBalance);

	return Amount;
}


void PerfromNormalWithdrawOption()
{
	int WithDrawBalance = ReadWithDrawAmont();
	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Press Anykey to continue...";

		system("pause>0");
		WithdrawScreen();
		return;
	}

	vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients))
		CurrentClient.AccountBalance -= WithDrawBalance;
}

void WithdrawScreen()
{
	system("cls");
	cout << "==============================================\n";
	cout << "                Withdraw Screen               \n";
	cout << "==============================================\n";

	PerfromNormalWithdrawOption();
}

short getQuickWithDrawAmount(short QuickWithDrawOption)
{
	switch (QuickWithDrawOption)
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

void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
	if (QuickWithDrawOption == 9)
		return;

	short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);

	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Press Anykey to continue...";
		system("pause>0");

		QuickWithdrawMenuScreen();
		return;
	}

	vector <stClient> vClients =LoadCleintsDataFromFile(ClientsFileName);
	if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients))
		CurrentClient.AccountBalance -= WithDrawBalance;
}

void QuickWithdrawChoice( )
{
	vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
}

void LoginScreen()
{
	cout << "--------------------------------\n";
	cout << "         Login Screen         \n";
	cout << "--------------------------------\n";
}

void PerformATMMainMenuOption(enATM Option)
{
	vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

		switch (Option)
		{
		case QuickWithdraw:
			QuickWithdrawMenuScreen();
			ReturnToATMMainMenu();
			break;

		case Withdraw:
			WithdrawScreen();
			ReturnToATMMainMenu();
			break;

		case Deposit:
			DepositScreen();
			ReturnToATMMainMenu();
			break;

		case CheckBalance:
			CheckBalanceScreen();
			ReturnToATMMainMenu();
			break;

		case Logout:
			Login();
			break;
		}
}

void ShowATMMainMenuFlow()
{
	system("cls");
	ATMMainMenueScreen();

	short Choice;
	cout << "Please enter Number From [1] to [5] : ";
	cin >> Choice;

	PerformATMMainMenuOption((enATM)Choice);
}

bool CheckValidClient(string AccountNumber,string PINCode,vector<stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber&&C.PinCode == PINCode)
		{
			CurrentClient = C;
			return true;
		}
	}
	return false;
}

void Login()
{
	vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

		system("cls");
		LoginScreen();

		string AccountNumber = ReadAccountNumber();
		string PINCode = ReadPIN();

		while (!CheckValidClient(AccountNumber,PINCode, vClients))
		{
			system("cls");
			LoginScreen();
			cout << "Invalid Account Number or PIN Code!\n";
			AccountNumber = ReadAccountNumber();
			PINCode = ReadPIN();
		}

		ShowATMMainMenuFlow();
}

int main() {

	Login();

	system("pause>0");

	return 0;
}