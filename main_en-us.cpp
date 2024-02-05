#include <iostream>
#include <windows.h>
#include <conio.h>
#include <sstream>

using namespace std;

// Execute the command and return the result
string executeCommand(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = _popen(cmd, "r");

    if (!pipe) {
        HANDLE handle;
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, 0x0C);
        cout << "Unable to execute command!\n";
        return "";
    }

    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    _pclose(pipe);
    return result;
}

// Show all port mappings
void showPortMappings() {
    HANDLE handle;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 0x0F);
    string result = executeCommand("netsh interface portproxy show all");
    cout << result << endl;
    SetConsoleTextAttribute(handle, 0x0B);
    printf("\nPress any key to return to the main menu...");
    int ch = _getch();
}

// Add port mapping
void addPortMapping(const string& listenAddress, const string& listenPort, const string& connectAddress, const string& connectPort) {
    char command[200];
    snprintf(command, sizeof(command), "netsh interface portproxy add v4tov4 listenport=%s listenaddress=%s connectport=%s connectaddress=%s",
        listenPort.c_str(),
        listenAddress.c_str(),
        connectPort.c_str(),
        connectAddress.c_str());

    executeCommand(command);
    HANDLE handle;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 0x0A);
    cout << "\nPort mapping added:" << listenAddress << ":" << listenPort << " -> " << connectAddress << ":" << connectPort << endl;
    SetConsoleTextAttribute(handle, 0x0B);
    printf("\nPress any key to return to the main menu...");
    int ch = _getch();
}

// Delete port mapping
void removePortMapping(const string& listenAddress, const string& listenPort) {
    char command[200];
    snprintf(command, sizeof(command), "netsh interface portproxy delete v4tov4 listenport=%s listenaddress=%s", listenPort.c_str(), listenAddress.c_str());
    executeCommand(command);
    HANDLE handle;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 0x0A);
    cout << "\nPort mapping removed: " << listenAddress << ":" << listenPort << endl;
    SetConsoleTextAttribute(handle, 0x0B);
    printf("\nPress any key to return to the main menu...");
    int ch = _getch();
}

int main() {
    string listenAddress, listenPort, connectAddress, connectPort;
    int choice;

    system("title Hyper-V Port Proxy Editor");
    do {
        system("cls");
        HANDLE handle;
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, 0x0C);
        cout << "|  |                  |    |  |~~\\        |   |~~\\               |~~   |' |        \n";
        cout << "|--|\\  /|~~\\/~/|/~\\--- \\  /   |__//~\\|/~\\~|~  |__/|/~\\/~\\\\/\\  /  |--/~~||~|~/~\\|/~\\\n";
        cout << "|  | \\/ |__/\\/_|        \\/    |   \\_/|    |   |   |   \\_//\\ \\/   |__\\__|| | \\_/|   \n";
        cout << "    _/  |                                                  _/                      \n";
        SetConsoleTextAttribute(handle, 0x1F);
        cout << "  V1.0  \n";
        SetConsoleTextAttribute(handle, 0x0E);
        cout << "------------------------------------------------------\n";
        cout << "1. Show all port mappings\n";
        cout << "2. Add port mapping\n";
        cout << "3. Delete port mapping\n\n";
        cout << "0. Exit\n";
        cout << "------------------------------------------------------\n";
        SetConsoleTextAttribute(handle, 0x0F);
        cout << "Please enter(1, 2, 3, 0): ";
        cin >> choice;

        switch (choice) {
            case 1:
                showPortMappings();
                break;
            case 2: {   
                cout << "\nPlease enter the host address, host port, virtual machine address and virtual machine port separated by spaces.\n";
                SetConsoleTextAttribute(handle, 0x09);
                cout << ">";

                cin.ignore(INT_MAX, '\n'); // Clear previous newlines

                // Read entire line
                string inputLine;
                getline(cin, inputLine);

                // Parse values using stringstream
                istringstream iss(inputLine);

                // Check whether the extraction is successful and ensure that there are other characters that have not been extracted
                if (!(iss >> listenAddress >> listenPort >> connectAddress >> connectPort) || iss.rdbuf()->in_avail() != 0) {
                    SetConsoleTextAttribute(handle, 0x0C);
                    cout << "\n输入无效，请使用空格分隔主机地址、主机端口、虚拟机地址和虚拟机端口\n";
                    SetConsoleTextAttribute(handle, 0x0B);
                    printf("\nPress any key to return to the main menu...");
                    int ch = _getch();
                    
                    break;  // Exit current case
                }
                else {
                    addPortMapping(listenAddress, listenPort, connectAddress, connectPort);
                    break;
                }
            }
            case 3: {
                cout << "\nPlease enter the host address and host port to be deleted, separated by spaces\n";
                SetConsoleTextAttribute(handle, 0x09);
                cout << ">";

                cin.ignore(INT_MAX, '\n'); // Clear previous newlines

                // Read entire line
                string inputLine;
                getline(cin, inputLine);

                // Parse values using stringstream
                istringstream iss(inputLine);

                // Check whether the extraction is successful and ensure that there are other characters that have not been extracted
                if (!(iss >> listenAddress >> listenPort) || iss.rdbuf()->in_avail() != 0) {
                    SetConsoleTextAttribute(handle, 0x0C);
                    cout << "\nInvalid input, please use a space to separate the host address and host port.\n";
                    SetConsoleTextAttribute(handle, 0x0B);
                    printf("\nPress any key to return to the main menu...");
                    int ch = _getch();

                    break;  // Exit current case
                }
                else {
                    removePortMapping(listenAddress, listenPort);
                    break;
                }
            }
            case 0:
                break;
            default:
                SetConsoleTextAttribute(handle, 0x0C);
                cout << "\nInvalid choice!\n";
                SetConsoleTextAttribute(handle, 0x0B);
                printf("\nPress any key to return to the main menu...");
                int ch = _getch();
        }
    } while (choice != 0);

    return 0;
}