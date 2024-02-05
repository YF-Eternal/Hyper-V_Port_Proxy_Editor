#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <string>
#include <sstream>
#include <limits>

using namespace std;


// 执行命令并返回结果
string executeCommand(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = _popen(cmd, "r");

    if (!pipe) {
        HANDLE handle;
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, 0x0C);
        cout << "无法执行命令!\n";
        return "";
    }

    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    _pclose(pipe);
    return result;
}

// 显示所有端口映射
void showPortMappings() {
    HANDLE handle;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 0x0F);
    string result = executeCommand("netsh interface portproxy show all");
    cout << result << endl;
    SetConsoleTextAttribute(handle, 0x0B);
    printf("\n按任意键返回至主菜单...");
    int ch = _getch();
}

// 添加端口映射
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
    cout << "\n端口映射已添加：" << listenAddress << ":" << listenPort << " -> " << connectAddress << ":" << connectPort << endl;
    SetConsoleTextAttribute(handle, 0x0B);
    printf("\n按任意键返回至主菜单...");
    int ch = _getch();
}

// 删除端口映射
void removePortMapping(const string& listenAddress, const string& listenPort) {
    char command[200];
    snprintf(command, sizeof(command), "netsh interface portproxy delete v4tov4 listenport=%s listenaddress=%s", listenPort.c_str(), listenAddress.c_str());
    executeCommand(command);
    HANDLE handle;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 0x0A);
    cout << "\n端口映射已删除：" << listenAddress << ":" << listenPort << endl;
    SetConsoleTextAttribute(handle, 0x0B);
    printf("\n按任意键返回至主菜单...");
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
        cout << "1. 显示所有端口映射\n";
        cout << "2. 添加端口映射\n";
        cout << "3. 删除端口映射\n\n";
        cout << "0. 退出\n";
        cout << "------------------------------------------------------\n";
        SetConsoleTextAttribute(handle, 0x0F);
        cout << "请输入(1, 2, 3, 0): ";
        cin >> choice;

        switch (choice) {
        case 1:
            showPortMappings();
            break;
        case 2: {
            cout << "\n请输入主机地址、主机端口、虚拟机地址和虚拟机端口，以空格分隔\n";
            SetConsoleTextAttribute(handle, 0x09);
            cout << ">";

            cin.ignore(INT_MAX, '\n'); // 清除之前的换行符

            // 读取整行
            string inputLine;
            getline(cin, inputLine);

            // 使用 stringstream 解析值
            istringstream iss(inputLine);

            // 检查是否提取成功，并且确保还有其他字符未被提取
            if (!(iss >> listenAddress >> listenPort >> connectAddress >> connectPort) || iss.rdbuf()->in_avail() != 0) {
                SetConsoleTextAttribute(handle, 0x0C);
                cout << "\n输入无效，请使用空格分隔主机地址、主机端口、虚拟机地址和虚拟机端口\n";
                SetConsoleTextAttribute(handle, 0x0B);
                printf("\n按任意键返回至主菜单...");
                int ch = _getch();

                break;  // 退出当前 case
            }
            else {
                addPortMapping(listenAddress, listenPort, connectAddress, connectPort);
                break;
            }
        }
        case 3: {
            cout << "\n请输入要删除的主机地址和主机端口, 以空格分隔\n";
            SetConsoleTextAttribute(handle, 0x09);
            cout << ">";

            cin.ignore(INT_MAX, '\n'); // 清除之前的换行符

            // 读取整行
            string inputLine;
            getline(cin, inputLine);

            // 使用 stringstream 解析值
            istringstream iss(inputLine);

            // 检查是否提取成功，并且确保还有其他字符未被提取
            if (!(iss >> listenAddress >> listenPort) || iss.rdbuf()->in_avail() != 0) {
                SetConsoleTextAttribute(handle, 0x0C);
                cout << "\n输入无效，请使用空格分隔主机地址和主机端口\n";
                SetConsoleTextAttribute(handle, 0x0B);
                printf("\n按任意键返回至主菜单...");
                int ch = _getch();

                break;  // 退出当前 case
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
            cout << "\n无效的选择!\n";
            SetConsoleTextAttribute(handle, 0x0B);
            printf("\n按任意键返回至主菜单...");
            int ch = _getch();
        }
    } while (choice != 0);

    return 0;
}