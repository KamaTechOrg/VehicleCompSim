#include <bits/stdc++.h>
using namespace std;

const string server = "server_exec";
const string client = "client_exec";

int main() {
   

    // Create the command to run in a new terminal
    string command = "gnome-terminal -- bash -c './" + string(server) + "; exec bash'";

    // Run the command
    cout << "\nRunning file in a new terminal..." << endl;
    system(command.c_str());
    int size_client = 0;
    std::cout << "how many files to create..." << endl;
    cin >> size_client;
    string command_client = "gnome-terminal -- bash -c './" + string(client) + "; exec bash'";

    while (size_client ){

    cout << "\nRunning file in a new terminal..." << endl;
    system(command_client.c_str());
    size_client--;
    }
    // Run the command

    return 0;
}
