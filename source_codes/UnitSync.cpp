#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;
// UnitSync

const int MAX_USER = 500;
const int NUM_TRAINING_DAY = 15;

// User files
const string FILE_BASIC = "basicCadet.txt";
const string FILE_ADVANCE = "advanceCadet.txt";
const string FILE_OFFICER = "staffOfficer.txt";
const string FILE_BRIGADE = "brigadeStaff.txt";

const string FILE_ANNOUNCE = "announcement.txt";

// 2D array for all conversions
const string CONVERT_TABLE[5][4] = {
    {"Basic Cadet", "Advance Cadet", "Staff Officer", "Brigade Staff"}, // Role (row 0)
    {"Alpha", "Bravo", "Charlie", "Delta"},                             // Company (row 1)
    {"1st Platoon", "2nd Platoon", "3rd Platoon", "4th Platoon"},       // Platoon (row 2)
    {"S1", "S3", "", ""},                                               // Staff (row 3)
    {"Present", "Late", "Excuse", "Absent"}                             // Status (row 4)
};

// Struct for User's data
struct User
{
    string userName = "";
    string lastName = "";
    string sureName = "";
    string middleName = "";
    string studentID = "";
    string program = "";
    string company = "";
    string platoon = "";
    string status[NUM_TRAINING_DAY] = {""};
    string role = "";
    string rank = "";     // For advance cadets, staff officers and brigade staff
    string staff = "";    // For staff officers and brigade staff
    string password = ""; // User Password
    int numPresent = 0;
    int numLate = 0;
    int numAbsent = 0;
    int numExcuse = 0;
};

struct Announcement
{
    string what = "";
    string who = "";
    string where = "";
    string when = "";
    string attire = "";
    string toBring = "";
    string note = "";
};

void clearScreen();
void displaySubHeader(string title, int width);
void displayHeader(string title, int borderWidth);
void pressEnter();
void displayUserMenu(string typeMenu, string userName = "", int *size = 0);
bool verifyInput(int size, int choice);
void verifyInput(int size, int *choice, bool *verify);
int userMenu(string typeMenu, string userName = "");
void displayLogout(string userName);
string companyConverter(int company);
string platoonConverter(int platoon);
string staffConverter(int staff);
string statusConverter(int status);
void roleConverter(User *user, int role, int index);
void selectUnit(User *basicCadets, int index);
void companyPlatoonSelection(string *company, string *platoon);
void registerUser(User *user, int index);
void registerRank(User *user, int index);
void registerStaff(User *user, int index);
string spaceRemover(string text);
string generateUserName(User *user, int index);
string generatePassword(User *user, int index);

template <typename T>
void swapUser(T &a, T &b);

void sortUsersAlphabetically(User *user, int numUser);
void writeFile(fstream &file, User *user, int index);
void saveUser(User *user, int index, string fileName);
int loadUser(User *user, string fileName);
void updateFile(User *user, int numUser, string fileName);
void registerUser(User *user, int *index, int *numberOfUser, string fileName);
bool verifyUserName(User *user, string userName, int numUser, int *index);
bool verifyPassword(User *user, string password, int index);
bool errorTracker(int *numError);
bool userLogIn(User *user, int numUser, int *index);
void viewProfile(User *user, int index);
void printColoredStatus(string status, string text);
void statusCounter(User *user, int index, int statusIndex);
void userTableHeader(string role);
void displayUser(User *user, int index, int number);
void trainingDayHeader();
void displayStatus(User *user, int index);
void viewAttendance(User *user, int index);
void takeAttendance(User *user, int numUser, string company, string platoon);
void takeAttendance(User *user, int numUser, string staff);
void loadAnnouncement(Announcement *announcement);
void displayAnnouncement(Announcement *announcement);
void saveAnnouncement(Announcement *announcement);
void list(User *user, int numUser);
void listOfUsers();
void attendanceSmartSearch(User *user, int numUser, string target, string type);
void attendanceSmartSearch(User *user, int numUser);
void attendanceSmartSearch();
void createAnnouncement();
void usersFeature(User *user, int index);

int main()
{
    int option = 0;            // Store the user's option
    int role = 0;              // Store the user's role
    int userIndex = 0;         // Store the user's index in logging in
    bool running = true;       // Condition to keep the system running
    bool loginSuccess = false; // Track if login was successful

    // Basic Cadets Main Variables
    int numCadets = 0;                      // total number of basic cadets registered
    int lastCadet = 0;                      // track the last index of basic cadets
    User *basicCadets = new User[MAX_USER]; // dynamic array to store basic cadets

    // Advance Cadets Main Variables
    int numAdvanceCadets = 0;                 // total number of advance cadets registered
    int lastAdvanceCadet = 0;                 // track the last index of advance cadets
    User *advanceCadets = new User[MAX_USER]; // dynamic array to store advance cadets

    // Staff Officers Main Variables
    int numOfficers = 0;                      // total number of staff officer registered
    int lastOfficer = 0;                      // track the last index of staff officer
    User *staffOfficers = new User[MAX_USER]; // dynamic array to store staff officers

    // Brigade Staffs Main Variables
    int numBrigades = 0;                      // total number of brigade staff registered
    int lastBrigade = 0;                      // track the last index of brigade staff
    User *brigadeStaffs = new User[MAX_USER]; // dynamic array to store brigade staffs

    while (running)
    {
        // Load all users from files
        lastCadet = loadUser(basicCadets, FILE_BASIC);
        lastAdvanceCadet = loadUser(advanceCadets, FILE_ADVANCE);
        lastOfficer = loadUser(staffOfficers, FILE_OFFICER);
        lastBrigade = loadUser(brigadeStaffs, FILE_BRIGADE);

        // Store the number of user in different role
        numCadets = lastCadet;
        numAdvanceCadets = lastAdvanceCadet;
        numOfficers = lastOfficer;
        numBrigades = lastBrigade;

        option = userMenu("Main", ""); // Main Menu

        switch (option)
        {
        case 1: // Registration Section
            // Role Based Registration
            role = userMenu("Role", ""); // Role Menu

            switch (role)
            {
            case 1: // Basic Cadets
                roleConverter(basicCadets, role, lastCadet);
                registerUser(basicCadets, &lastCadet, &numCadets, FILE_BASIC);
                break;
            case 2: // Advance Cadets
                roleConverter(advanceCadets, role, lastAdvanceCadet);
                registerUser(advanceCadets, &lastAdvanceCadet, &numAdvanceCadets, FILE_ADVANCE);
                break;
            case 3: // Staff Officers
                roleConverter(staffOfficers, role, lastOfficer);
                registerUser(staffOfficers, &lastOfficer, &numOfficers, FILE_OFFICER);
                break;
            case 4: // Brigade Staff
                roleConverter(brigadeStaffs, role, lastBrigade);
                registerUser(brigadeStaffs, &lastBrigade, &numBrigades, FILE_BRIGADE);
                break;
            }
            break;

        case 2: // Log In Section
            // Role Based Log In
            role = userMenu("Role", ""); // Role Menu

            switch (role)
            {
            case 1: // Basic Cadets
                loginSuccess = userLogIn(basicCadets, numCadets, &userIndex);
                if (loginSuccess)
                {
                    usersFeature(basicCadets, userIndex);
                }
                break;
            case 2: // Advance Cadets
                loginSuccess = userLogIn(advanceCadets, numAdvanceCadets, &userIndex);
                if (loginSuccess)
                {
                    usersFeature(advanceCadets, userIndex);
                }
                break;
            case 3: // Staff Officers
                loginSuccess = userLogIn(staffOfficers, numOfficers, &userIndex);
                if (loginSuccess)
                {
                    usersFeature(staffOfficers, userIndex);
                }
                break;
            case 4: // Brigade Staff
                loginSuccess = userLogIn(brigadeStaffs, numBrigades, &userIndex);
                if (loginSuccess)
                {
                    usersFeature(brigadeStaffs, userIndex);
                }
                break;
            }
            break;

        case 3:              // Exit
            running = false; // Stop the system from looping
            cout << "\nThank you for using UnitSync!\n";
            break;
        }
    }

    // Free Memory
    delete[] basicCadets;
    delete[] advanceCadets;
    delete[] staffOfficers;
    delete[] brigadeStaffs;
    return 0;
}

// Function that clear the screen
void clearScreen()
{
    cout << "\033[2J\033[1;1H";
}

void displaySubHeader(string title, int width)
{
    string line = string(width, '-');

    cout << line << "\n";
    cout << " " << title << "\n";
    cout << line << "\n";
}

// Displays a centered title with border
void displayHeader(string title, int borderWidth)
{
    clearScreen();
    int titleLength = title.length();             // Length of the title text
    int spaces = (borderWidth - titleLength) / 2; // Spaces needed to center title

    cout << string(borderWidth, '=') << "\n";
    cout << string(spaces, ' ') << title << "\n";
    cout << string(borderWidth, '=') << "\n";
}

void pressEnter()
{
    cout << "\nPress Enter to Continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function that display the user menu
void displayUserMenu(string typeMenu, string userName, int *size)
{
    int width = 40;
    string line1 = string(width, '=');
    string line2 = string(width, '-');

    if (typeMenu == "Main")
    {
        displayHeader("UnitSync", width);     // Display header and clear the screen
        displaySubHeader("MAIN MENU", width); // Display sub heaader
        // Display the menu choices
        cout << " 1. Register\n";
        cout << " 2. Log In\n";
        cout << " 3. Exit\n";
        // Display a line for cleaner UI
        cout << line1 << "\n";

        *size = 3; // Modify the size for error handling
    }
    else if (typeMenu == "Role")
    {
        displayHeader("UnitSync", width);
        displaySubHeader("ROLE SELECTION", width);
        cout << " 1. Basic Cadet\n";
        cout << " 2. Advance Cadet\n";
        cout << " 3. Staff Officer\n";
        cout << " 4. Brigade Staff\n";
        cout << line1 << "\n";

        *size = 4;
    }
    else if (typeMenu == "Cadets Menu")
    {
        displayHeader("UnitSync", width);
        displaySubHeader("User: " + userName, width);
        cout << " 1. View My Profile\n";
        cout << " 2. Announcements\n";
        cout << " 3. View My Attendance\n";
        cout << " 4. Log Out\n";
        cout << line1 << "\n";

        *size = 4;
    }
    else if (typeMenu == "Staff Officer Menu")
    {
        displayHeader("UnitSync", width);
        displaySubHeader("User: " + userName, width);
        cout << " 1. View My Profile\n";
        cout << " 2. Take Attendance\n";
        cout << " 3. Announcements\n";
        cout << " 4. Log Out\n";
        cout << line1 << "\n";

        *size = 4;
    }
    else if (typeMenu == "Brigade Staff Menu")
    {
        displayHeader("UnitSync", width);
        displaySubHeader("User: " + userName, width);
        cout << " 1. View My Profile\n";
        cout << " 2. Create an Announcement\n";
        cout << " 3. List of Users\n";
        cout << " 4. Attendance Smart Search\n";
        cout << " 5. Log Out\n";
        cout << line1 << "\n";

        *size = 5;
    }
    else if (typeMenu == "Company Menu")
    {
        displayHeader("Company Selection", width);
        cout << line2 << "\n";
        cout << " 1. Alpha\n";
        cout << " 2. Bravo\n";
        cout << " 3. Charlie\n";
        cout << " 4. Delta\n";
        cout << line1 << "\n";

        *size = 4;
    }
    else if (typeMenu == "Platoon Menu")
    {
        displayHeader("Platoon Selection", width);
        cout << line2 << "\n";
        cout << " 1. 1st Platoon\n";
        cout << " 2. 2nd Platoon\n";
        cout << " 3. 3rd Platoon\n";
        cout << " 4. 4th Platoon\n";
        cout << line1 << "\n";

        *size = 4;
    }
    else if (typeMenu == "Staff Menu")
    {
        displayHeader("Staff Selection", width);
        cout << line2 << "\n";
        cout << " 1. S1\n";
        cout << " 2. S3\n";
        cout << line1 << "\n";

        *size = 2;
    }
    else if (typeMenu == "Status Menu")
    {
        cout << line2 << line2 << "\n";
        cout << " STATUS SELECTION\n";
        cout << line2 << line2 << "\n";
        cout << " [1] Present            ";
        cout << "[2] Late             ";
        cout << "[3] Excuse             ";
        cout << "[4] Absent\n";
        cout << line1 << line1 << "\n";
    }
    else if (typeMenu == "Role Smart Search")
    {
        displayHeader("Search Selection", width);
        cout << line2 << "\n";
        cout << " 1. Basic Cadets\n";
        cout << " 2. Advance Cadets\n";
        cout << line1 << "\n";

        *size = 2;
    }
    else if (typeMenu == "Search By Name Menu")
    {
        displayHeader("Search Selection", width);
        cout << line2 << "\n";
        cout << " 1. Last Name\n";
        cout << " 2. Sure Name\n";
        cout << line1 << "\n";

        *size = 2;
    }
    else if (typeMenu == "Basic Cadet Search Menu")
    {
        displayHeader("Search Selection", width);
        cout << line2 << "\n";
        cout << " 1. Company\n";
        cout << " 2. Platoon\n";
        cout << " 3. Program\n";
        cout << " 4. Name\n";
        cout << line1 << "\n";

        *size = 4;
    }
    else if (typeMenu == "Advance Cadet Search Menu")
    {
        displayHeader("Search Selection", width);
        cout << line2 << "\n";
        cout << " 1. Platoon\n";
        cout << " 2. Program\n";
        cout << " 3. Name\n";
        cout << line1 << "\n";

        *size = 3;
    }
}

// OVerload 1: function to verify the user input
bool verifyInput(int size, int choice)
{
    return (choice >= 1 && choice <= size);
}

// Overload 2: function to handle non interger input
void verifyInput(int size, int *choice, bool *verify)
{
    // Vefiry if the choice is an interger
    if (cin >> *choice)
    {
        // Handle range of integer input
        *verify = verifyInput(size, *choice);

        if (!*verify)
        {
            cout << "\n[!] Invalid Input. Enter 1-" << size << " Only\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    else
    {
        // Handle non-integer input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n[!] Please enter a number.\n";

        *verify = false;
    }
}

// Function that verify the choice before returning the value
int userMenu(string typeMenu, string userName)
{
    int choice = 0;
    int size = 0;
    bool verify = false;

    do
    {
        displayUserMenu(typeMenu, userName, &size);
        cout << "\n Enter Choice: ";

        verifyInput(size, &choice, &verify);

        pressEnter();

    } while (!verify);

    return choice;
}

// Function to display logout confirmation
void displayLogout(string userName)
{
    const int width = 40;
    string line1 = string(width, '=');
    string line2 = string(width, '-');

    displayHeader("UnitSync", width);
    displaySubHeader("LOGOUT", width);

    cout << " User: " << userName << endl;
    cout << line2 << endl;
    cout << " [/] You have successfully logged out.\n";
    cout << line1 << endl;
}

// Convert company choice to string
string companyConverter(int company)
{
    return CONVERT_TABLE[1][company - 1];
}

// Convert platoon choice to string
string platoonConverter(int platoon)
{
    return CONVERT_TABLE[2][platoon - 1];
}

// Convert staff choice to string
string staffConverter(int staff)
{
    return CONVERT_TABLE[3][staff - 1];
}

// Convert status choice to string
string statusConverter(int status)
{
    return CONVERT_TABLE[4][status - 1];
}

// Convert role
void roleConverter(User *user, int role, int index)
{
    (user + index)->role = CONVERT_TABLE[0][role - 1];
}

// Function to select company or platoon
void selectUnit(User *basicCadets, int index)
{
    int choice = 0; // Store the users choice

    choice = userMenu("Company Menu", "");
    (basicCadets + index)->company = companyConverter(choice);

    choice = userMenu("Platoon Menu", "");
    (basicCadets + index)->platoon = platoonConverter(choice);
}

// Function for selecting company and platoon
void companyPlatoonSelection(string *company, string *platoon)
{
    int choice = 0;

    choice = userMenu("Company Menu", "");
    *company = companyConverter(choice);

    choice = userMenu("Platoon Menu", "");
    *platoon = platoonConverter(choice);
}

// Function to register user student information
// This is a required for every type of user
void registerUser(User *user, int index)
{
    cout << "Last Name(ex. DELA CRUZ)     : ";
    getline(cin, (user + index)->lastName);
    cout << "Sure Name(ex. JUAN)          : ";
    getline(cin, (user + index)->sureName);
    cout << "Middle Name(ex. REYES)       : ";
    getline(cin, (user + index)->middleName);
    cout << "Student ID(ex. LQ-00093-2025): ";
    getline(cin, (user + index)->studentID);
    cout << "Program(ex. BSCE-1-1)        : ";
    getline(cin, (user + index)->program);
}

// Function to register advance cadets, staff officer and brigade staff ranks
void registerRank(User *user, int index)
{
    cout << "Rank(ex. C/1LT): ";
    getline(cin, (user + index)->rank);
}

// Function to register the staff of advance cadets, staff officer and brigade staff
void registerStaff(User *user, int index)
{
    int staff = 0;
    staff = userMenu("Staff Menu");
    (user + index)->staff = staffConverter(staff);
}

// Function that removes the space in text
string spaceRemover(string text)
{
    string result = "";
    for (char c : text)
    {
        if (c != ' ')
        {
            result += c;
        }
    }
    return result;
}

// Function that auto generate the user's name in a format
string generateUserName(User *user, int index)
{
    string noSpaceLastName = "",
           noSpaceSureName = "",
           userName = "";

    noSpaceLastName = spaceRemover((user + index)->lastName);
    noSpaceSureName = spaceRemover((user + index)->sureName);

    userName = noSpaceLastName + "." + noSpaceSureName;

    return userName;
}

// Function that auto generate the user's password
string generatePassword(User *user, int index)
{
    string password = ""; // Store the user password
    string userIndex = to_string(index);

    string lastName = spaceRemover((user + index)->lastName);

    string studentID = (user + index)->studentID;

    // Find positions of the dashes
    int firstDash = studentID.find("-");
    int secondDash = studentID.find("-", firstDash + 1);

    // Extract between first and second dash
    string idNumber = studentID.substr(firstDash + 1, secondDash - firstDash - 1);
    password = lastName + "-" + idNumber + "-" + userIndex;

    return password;
}

template <typename T>
void swapUser(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

// Sort users alphabetically by last name (A-Z)
void sortUsersAlphabetically(User *user, int numUser)
{
    for (int i = 0; i < numUser - 1; i++)
    {
        for (int j = 0; j < numUser - i - 1; j++)
        {
            // Direct comparison (case sensitive)
            if ((user + j)->lastName > (user + j + 1)->lastName)
            {
                // Dereference the pointers to pass by reference
                swapUser(*(user + j), *(user + j + 1));
            }
        }
    }
}

// Function to write data in files, serve as the helper function for save user and update file function
void writeFile(fstream &file, User *user, int index)
{

    file << (user + index)->userName << ",";
    file << (user + index)->lastName << ",";
    file << (user + index)->sureName << ",";
    file << (user + index)->middleName << ",";
    file << (user + index)->studentID << ",";
    file << (user + index)->program << ",";
    file << (user + index)->company << ",";
    file << (user + index)->platoon << ",";
    file << (user + index)->role << ",";
    file << (user + index)->rank << ",";
    file << (user + index)->staff << ",";
    file << (user + index)->password << ",";
    file << (user + index)->numPresent << ",";
    file << (user + index)->numLate << ",";
    file << (user + index)->numAbsent << ",";
    file << (user + index)->numExcuse << ",";

    // Save status array
    for (int i = 0; i < NUM_TRAINING_DAY; i++)
    {
        if ((user + index)->status[i].empty())
            file << "--";
        else
            file << (user + index)->status[i];

        if (i < NUM_TRAINING_DAY - 1)
        {
            file << ",";
        }
    }

    file << "\n";
}

// Function for saving the user in a file based on their role
void saveUser(User *user, int index, string fileName)
{

    fstream file;

    file.open(fileName, ios::app);

    // Save structure members
    if (file.is_open())
    {
        writeFile(file, user, index);
        file.close();
    }
}

// Function that load users from file
int loadUser(User *user, string fileName)
{
    fstream file;

    file.open(fileName, ios::in);

    if (!file.is_open())
    {
        cout << "File could not be opened.\n";
        return 0;
    }

    string line;
    int index = 0;

    while (getline(file, line))
    {
        stringstream ss(line);
        string temp;

        getline(ss, (user + index)->userName, ',');
        getline(ss, (user + index)->lastName, ',');
        getline(ss, (user + index)->sureName, ',');
        getline(ss, (user + index)->middleName, ',');
        getline(ss, (user + index)->studentID, ',');
        getline(ss, (user + index)->program, ',');
        getline(ss, (user + index)->company, ',');
        getline(ss, (user + index)->platoon, ',');
        getline(ss, (user + index)->role, ',');
        getline(ss, (user + index)->rank, ',');
        getline(ss, (user + index)->staff, ',');
        getline(ss, (user + index)->password, ',');

        // Convert string to int
        getline(ss, temp, ',');
        (user + index)->numPresent = stoi(temp);

        getline(ss, temp, ',');
        (user + index)->numLate = stoi(temp);

        getline(ss, temp, ',');
        (user + index)->numAbsent = stoi(temp);

        getline(ss, temp, ',');
        (user + index)->numExcuse = stoi(temp);

        // Load status array
        for (int i = 0; i < NUM_TRAINING_DAY; i++)
        {
            getline(ss, (user + index)->status[i], ',');

            // Convert "--" back to empty string
            if ((user + index)->status[i] == "--")
            {
                (user + index)->status[i] = "";
            }
        }

        index++;
    }

    file.close();

    return index; // returns number of users loaded
}

// For attendance update — rewrites entire file
void updateFile(User *user, int numUser, string fileName)
{
    fstream file;
    file.open(fileName, ios::out);

    if (file.is_open())
    {
        for (int i = 0; i < numUser; i++)
        {
            writeFile(file, user, i);
        }
        file.close();
    }
}

// Function to register a user
void registerUser(User *user, int *index, int *numberOfUser, string fileName)
{
    // Store the role
    string role = (user + *index)->role;
    int platoon = 0;
    const int width = 40;

    string line1 = string(width, '=');
    string line2 = string(width, '-');

    displayHeader("UnitSync", width);
    displaySubHeader("REGISTRATION", width);

    registerUser(user, *index); // register student information

    // Registration of Basic Cadets
    if (role == "Basic Cadet")
    {
        // Function for selecting the unit
        // Select Company and Platoon
        selectUnit(user, *index);
    }
    // Registration of Advance Cadets
    else if (role == "Advance Cadet")
    {
        registerRank(user, *index);
        // Select Platoon
        platoon = userMenu("Platoon Menu", "");
        (user + *index)->platoon = platoonConverter(platoon);
    }
    // Registration of Staff Officers and Brigade Staff
    else if (role == "Staff Officer" || role == "Brigade Staff")
    {
        registerRank(user, *index);
        registerStaff(user, *index);
    }

    (user + *index)->userName = generateUserName(user, *index);
    (user + *index)->password = generatePassword(user, *index);

    displayHeader("UnitSync", width);
    displaySubHeader("REGISTRATION", width);
    cout << " User Name: " << (user + *index)->userName << endl;
    cout << " Your Password: " << (user + *index)->password << endl;
    cout << " Please Save your password!\n";
    cout << line2 << endl;

    saveUser(user, *index, fileName);

    (*index)++;
    (*numberOfUser)++;

    sortUsersAlphabetically(user, *numberOfUser);
    updateFile(user, *numberOfUser, fileName);

    cout << line1 << endl;
    cout << " [/] Registration Complete!\n";
    cout << line1 << endl;

    pressEnter();
}

// Function that verify the user name
bool verifyUserName(User *user, string userName, int numUser, int *index)
{
    // Linear Search to search if the user name exist
    for (int i = 0; i < numUser; i++)
    {
        if (userName == (user + i)->userName) // Condition to verify the user name
        {
            *index = i;  // Store the index of the user
            return true; // return if the user name exist
        }
    }

    cout << " [!] No Name Found\n";
    cout << string(40, '=') << endl;
    pressEnter();
    return false; // return if the user name is not found
}

// Function that verify the user's password
bool verifyPassword(User *user, string password, int index)
{

    if (password == (user + index)->password) // Condition to verify the password
    {
        return true; // return if the password is correct
    }
    else
    {
        cout << " [!] Password Incorrect\n";
        cout << string(40, '=') << endl;
        pressEnter();
        return false; // return if the password is incorrect
    }
}

bool errorTracker(int *numError)
{
    const int width = 40;
    (*numError)++;

    if (*numError == 3)
    {
        displayHeader("UnitSync", width);
        displaySubHeader("LOG IN", width);
        cout << " [!] Too many failed attempts.\n Please try again later.\n";
        cout << string(width, '=') << endl;
        pressEnter();
        *numError = 0;
        return true; // signal to stop trying
    }

    return false; // signal to keep trying
}

// Function for user log in - returns true if successful, false if too many attempts
bool userLogIn(User *user, int numUser, int *index)
{
    int numError = 0;
    string userName = "";
    string password = "";
    bool verification = false;
    bool tryAgain = false;
    const int width = 40;

    // Username validation
    do
    {
        displayHeader("UnitSync", width);
        displaySubHeader("LOG IN", width);
        cout << "User Name: ";
        getline(cin, userName);
        verification = verifyUserName(user, userName, numUser, index);

        if (!verification)
        {
            tryAgain = errorTracker(&numError);
            if (tryAgain)
            {
                return false; // Too many username attempts - exit
            }
        }
    } while (!verification);

    numError = 0; // reset counter for password attempts

    // Password validation
    do
    {
        displayHeader("UnitSync", width);
        displaySubHeader("LOG IN", width);
        cout << "User Name: " << (user + *index)->userName;
        cout << "\nPassword : ";
        cin >> password;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        verification = verifyPassword(user, password, *index);

        if (!verification)
        {
            tryAgain = errorTracker(&numError);
            if (tryAgain)
            {
                return false; // Too many password attempts - exit
            }
        }
    } while (!verification);

    cout << " [/] Log In Successfully\n";
    cout << string(width, '=') << endl;
    pressEnter();

    return true; // Login successful
}

// Function for the user's to view their profile
void viewProfile(User *user, int index)
{
    string role = (user + index)->role;
    const int width = 70;
    string line1 = string(width, '=');
    string line2 = string(width, '-');

    // Display the user's profile using setw
    displayHeader("USER PROFILE", width);
    cout << left << setw(15) << "User Name" << ": " << (user + index)->userName << "\n";
    cout << line2 << "\n";
    cout << left << setw(15) << "Last Name" << ": " << setw(25) << (user + index)->lastName
         << left << setw(10) << "Sure Name" << ": " << (user + index)->sureName << "\n";
    cout << left << setw(15) << "Middle Name" << ": " << setw(25) << (user + index)->middleName
         << left << setw(10) << "Student ID" << ": " << (user + index)->studentID << "\n";
    cout << left << setw(15) << "Program" << ": " << setw(25) << (user + index)->program
         << left << setw(10) << "Role" << ": " << (user + index)->role << "\n";

    // Additional information based on the user's role
    if (role == "Basic Cadet")
    {
        cout << left << setw(15) << "Company" << ": " << setw(25) << (user + index)->company
             << left << setw(10) << "Platoon" << ": " << (user + index)->platoon << "\n";
    }
    else if (role == "Advance Cadet")
    {
        cout << left << setw(15) << "Platoon" << ": " << setw(25) << (user + index)->platoon
             << left << setw(10) << "Rank" << ": " << (user + index)->rank << "\n";
    }
    else if (role == "Staff Officer" || role == "Brigade Staff")
    {
        cout << left << setw(15) << "Rank" << ": " << setw(25) << (user + index)->rank
             << left << setw(10) << "Staff" << ": " << (user + index)->staff << "\n";
    }

    cout << line1 << "\n";
}

// Function for displaying colored text
void printColoredStatus(string status, string text)
{

    if (status == "Present")
    {
        cout << "\033[32m" << text << "\033[0m"; // Green
    }
    else if (status == "Late")
    {
        cout << "\033[34m" << text << "\033[0m"; // Blue
    }
    else if (status == "Excuse")
    {
        cout << "\033[33m" << text << "\033[0m"; // Yellow
    }
    else if (status == "Absent")
    {
        cout << "\033[31m" << text << "\033[0m"; // Red
    }
}

// Function to count the status of users
void statusCounter(User *user, int index, int statusIndex)
{
    string status = (user + index)->status[statusIndex];

    if (status == "Present")
    {
        ((user + index)->numPresent)++;
    }
    else if (status == "Late")
    {
        ((user + index)->numLate)++;
    }
    else if (status == "Excuse")
    {
        ((user + index)->numExcuse)++;
    }
    else if (status == "Absent")
    {
        ((user + index)->numAbsent)++;
    }
}

// Function that display the header for listing based on role
void userTableHeader(string role)
{
    // User information header
    cout << setw(5) << left << "No.";

    if (role == "Advance Cadet" || role == "Staff Officer" || role == "Brigade Staff")
    {
        cout << setw(10) << left << "Rank";
        if (role == "Staff Officer" || role == "Brigade Staff")
        {
            cout << setw(10) << left << "Staff";
        }
    }

    cout << setw(14) << left << "Last Name" << setw(15) << left << "Sure Name"
         << setw(16) << left << "Middle Name" << setw(12) << left << "Program";
}

void displayUser(User *user, int index, int number)
{
    // display user information

    string role = (user + index)->role;

    cout << setw(5) << left << to_string(number);

    if (role == "Advance Cadet" || role == "Staff Officer" || role == "Brigade Staff")
    {
        cout << setw(10) << left << (user + index)->rank;
        if (role == "Staff Officer" || role == "Brigade Staff")
        {
            cout << setw(10) << left << (user + index)->staff;
        }
    }

    cout << setw(14) << left << (user + index)->lastName << setw(15) << left << (user + index)->sureName
         << setw(16) << left << (user + index)->middleName << setw(12) << left << (user + index)->program;
}

void trainingDayHeader()
{
    cout << setw(15) << left << "TRAINING DAY";
    for (int i = 0; i < NUM_TRAINING_DAY; i++)
    {
        if (i < 9)
        {
            cout << "0";
        }
        cout << to_string(i + 1) << " ";
    }
}

void displayStatus(User *user, int index)
{
    cout << setw(14) << left << "STATUS";
    cout << "|";
    for (int i = 0; i < NUM_TRAINING_DAY; i++)
    {
        // Display for empty status
        if ((user + index)->status[i].empty())
        {
            cout << "--";
        }
        // Display colored text based on the user status
        else
        {
            printColoredStatus((user + index)->status[i], "=="); // Display
            statusCounter(user, index, i);                       // Count the status
        }
        cout << "|";
    }
}
// Function the display the user's attendance and status
void viewAttendance(User *user, int index)
{
    const int width = 60;
    string line1 = string(width, '=');
    string line2 = string(width, '-');

    // Reset counts before recalculating
    (user + index)->numPresent = 0;
    (user + index)->numLate = 0;
    (user + index)->numExcuse = 0;
    (user + index)->numAbsent = 0;

    // Display feature title

    displayHeader("USER ATTENDANCE", width);

    // Color representation legend
    cout << "Legend:\n";
    printColoredStatus("Present", "== Present  ");
    printColoredStatus("Late", "== Late  ");
    printColoredStatus("Excuse", "== Excuse  ");
    printColoredStatus("Absent", "== Absent  ");
    cout << "\033[0m-- Not Yet Taken\n";
    cout << line2 << "\n";

    // Display the number of training days
    trainingDayHeader();
    cout << endl;

    // Display the user's status
    displayStatus(user, index);
    cout << endl;

    cout << line2 << "\n";
    cout << endl;

    // Display attendance summary with colors
    cout << string(21, ' ') << "ATTENDANCE SUMMARY" << "\n";
    cout << line2 << "\n";

    // Row 1
    printColoredStatus("Present", "Present");
    cout << left << setw(10) << "" << ": " << setw(20) << (user + index)->numPresent;
    printColoredStatus("Late", "Late");
    cout << left << setw(13) << "" << ": " << (user + index)->numLate << "\n";

    // Row 2
    printColoredStatus("Excuse", "Excuse");
    cout << left << setw(11) << "" << ": " << setw(20) << (user + index)->numExcuse;
    printColoredStatus("Absent", "Absent");
    cout << left << setw(11) << "" << ": " << (user + index)->numAbsent << "\n";

    cout << line1 << "\n";
}

// Function for the staff officer to take cadets attendance
void takeAttendance(User *user, int numUser, string company, string platoon)
{
    int trainingDay = 0;
    int status = 0;
    int counter = 0;
    const int width = 40;
    int doubleWidth = width * 2;
    string fileName = "";
    string role = (user + 0)->role;
    string line1 = string(doubleWidth, '=');
    string line2 = string(doubleWidth, '-');
    bool verify = false;
    bool statusValid = false;

    // Training Day Input with Validation
    do
    {
        displayHeader("TAKE ATTENDANCE", width);
        displaySubHeader("Enter Training Day (1-15)", width);
        cout << " Training Day: ";
        verifyInput(15, &trainingDay, &verify);

        if (!verify)
        {
            pressEnter();
        }

    } while (!verify);

    if (role == "Basic Cadet")
    {
        fileName = FILE_BASIC; // Basic Cadet File
    }
    else
    {
        fileName = FILE_ADVANCE; // Advance Cadet File
    }

    displayHeader("TAKE ATTENDANCE", doubleWidth);
    displaySubHeader("TRAINING DAY: " + to_string(trainingDay), doubleWidth);
    displayUserMenu("Status Menu");
    cout << endl;

    cout << "DESIGNATION: " << company << " " << platoon << endl;
    cout << string(doubleWidth, '-') << endl;

    // Cadets information header
    userTableHeader(role);
    cout << setw(5) << left << "Status" << endl;

    for (int i = 0; i < numUser; i++)
    {
        if ((user + i)->company == company && (user + i)->platoon == platoon)
        {

            do
            {
                // Display the users basic information
                counter++;
                displayUser(user, i, counter);
                cout << setw(5) << left;

                // Use verifyInput for status validation (1-4 range)
                verifyInput(4, &status, &statusValid);

                if (!statusValid)
                {
                    // Decrement counter to re-display the same user
                    counter--;
                    cout << line2 << endl;
                    cout << line1 << endl;
                    pressEnter();

                    // Redisplay the header and user list from the start
                    displayHeader("TAKE ATTENDANCE", doubleWidth);
                    displaySubHeader("TRAINING DAY: " + to_string(trainingDay), doubleWidth);
                    displayUserMenu("Status Menu");
                    cout << endl;
                    cout << "DESIGNATION: " << company << " " << platoon << endl;
                    cout << line2 << endl;

                    userTableHeader(role);
                    cout << setw(5) << left << "Status" << endl;
                }
                else
                {
                    // Valid status - save it
                    (user + i)->status[trainingDay - 1] = statusConverter(status);
                }

            } while (!statusValid);
        }
    }

    cout << line2 << endl;
    cout << line1 << endl;

    updateFile(user, numUser, fileName);
}

// Function overloading to select the company and platoon of cadets
void takeAttendance(User *user, int numUser, string staff)
{
    int choice = 0;

    string company = "",
           platoon = "";

    if (staff == "S1")
    {
        choice = userMenu("Company Menu", "");
        company = companyConverter(choice);
    }

    choice = userMenu("Platoon Menu", "");
    platoon = platoonConverter(choice);

    takeAttendance(user, numUser, company, platoon);
}

// Function that load the announcement
void loadAnnouncement(Announcement *announcement)
{

    fstream file;
    file.open(FILE_ANNOUNCE, ios::in);

    if (!file.is_open())
    {
        cout << "No announcement found.\n";
        return;
    }

    string temp = "";

    getline(file, announcement->what, ',');
    getline(file, announcement->who, ',');
    getline(file, announcement->where, ',');
    getline(file, announcement->when, ',');
    getline(file, announcement->attire, ',');
    getline(file, announcement->toBring, ',');
    getline(file, announcement->note, ',');

    file.close();
}

// Function the display the announcement
void displayAnnouncement(Announcement *announcement)
{
    const int width = 50;
    string line1 = string(width, '=');
    string line2 = string(width, '-');

    // Display the announcements
    displayHeader("!! ANNOUNCEMENT !!", width);
    displaySubHeader("ANNOUNCEMENT DETAILS", width);

    cout << left << setw(10) << "WHAT" << ": " << announcement->what << "\n";
    cout << left << setw(10) << "WHO" << ": " << announcement->who << "\n";
    cout << left << setw(10) << "WHERE" << ": " << announcement->where << "\n";
    cout << left << setw(10) << "WHEN" << ": " << announcement->when << "\n";
    cout << left << setw(10) << "ATTIRE" << ": " << announcement->attire << "\n";
    cout << line2 << "\n";
    cout << "TO BRING:\n"
         << announcement->toBring;
    cout << line2 << "\n";
    cout << "NOTE:\n"
         << announcement->note;
    cout << line1 << "\n";
}

// Function that can save the announcement
void saveAnnouncement(Announcement *announcement)
{
    fstream file;

    file.open(FILE_ANNOUNCE, ios::out);

    if (file.is_open())
    {
        file << announcement->what << ",";
        file << announcement->who << ",";
        file << announcement->where << ",";
        file << announcement->when << ",";
        file << announcement->attire << ",";
        file << announcement->toBring << ",";
        file << announcement->note << ",";
    }
    else
    {
        cout << "File not open\n";
    }

    file.close();
}

// Helper function to list the users
void list(User *user, int numUser)
{
    for (int i = 0; i < numUser; i++)
    {
        displayUser(user, i, i + 1);
        cout << endl;
    }
}

// Function that displat every user in the programs file
// WIP: UI will be improve later
void listOfUsers()
{
    const int width = 80;
    string line1 = string(width, '=');
    string line2 = string(width, '-');

    // Load all users
    User *basicCadets = new User[MAX_USER];
    User *advanceCadets = new User[MAX_USER];
    User *staffOfficers = new User[MAX_USER];
    User *brigadeStaffs = new User[MAX_USER];

    // Store the number of user based on type
    int numCadets = loadUser(basicCadets, FILE_BASIC);
    int numAdvanceCadets = loadUser(advanceCadets, FILE_ADVANCE);
    int numOfficers = loadUser(staffOfficers, FILE_OFFICER);
    int numBrigade = loadUser(brigadeStaffs, FILE_BRIGADE);

    displayHeader("USER'S INFORMATION CHART", width);

    displaySubHeader("BASIC CADETS", width);
    userTableHeader("Basic Cadet");
    cout << endl;
    list(basicCadets, numCadets);

    displaySubHeader("ADVANCE CADETS", width);
    userTableHeader("Advance Cadet");
    cout << endl;
    list(advanceCadets, numAdvanceCadets);

    displaySubHeader("STAFF OFFICERS", width);
    userTableHeader("Staff Officer");
    cout << endl;
    list(staffOfficers, numOfficers);

    displaySubHeader("BRIGADE STAFFS", width);
    userTableHeader("Brigade Staff");
    cout << endl;
    list(brigadeStaffs, numBrigade);
    cout << line2 << endl;
    cout << line1 << endl;

    cout << line2 << endl;
    cout << left << setw(25) << " Total Basic Cadets" << ": " << numCadets << "\n";
    cout << left << setw(25) << " Total Advance Cadets" << ": " << numAdvanceCadets << "\n";
    cout << left << setw(25) << " Total Staff Officers" << ": " << numOfficers << "\n";
    cout << left << setw(25) << " Total Brigade Staffs" << ": " << numBrigade << "\n";
    cout << line1 << endl;

    // Free Memory
    delete[] basicCadets;
    delete[] advanceCadets;
    delete[] staffOfficers;
    delete[] brigadeStaffs;
}

// Overloading function for displaying user attendance based on brigade choice
void attendanceSmartSearch(User *user, int numUser, string target, string type)
{
    int counter = 0;
    int width = 122;
    bool find = false;
    string secondTarget = "";
    string role = (user + 0)->role;

    if (role == "Advance Cadet")
    {
        width += 10;
    }

    string line1 = string(width, '=');
    string line2 = string(width, '-');

    if (type == "Company Platoon")
    {
        companyPlatoonSelection(&target, &secondTarget);
    }

    displayHeader("CADETS ATTENDANCE CHART", width);
    cout << line2 << endl;
    userTableHeader(role);
    trainingDayHeader();
    cout << endl;

    for (int i = 0; i < numUser; i++)
    {

        if (type == "Company")
        {
            if ((user + i)->company == target)
            {
                counter++;
                displayUser(user, i, counter);
                displayStatus(user, i);
                cout << endl;
                find = true;
            }
        }
        else if (type == "Company Platoon")
        {
            if ((user + i)->company == target && (user + i)->platoon == secondTarget)
            {
                counter++;
                displayUser(user, i, counter);
                displayStatus(user, i);
                cout << endl;
                find = true;
            }
        }
        else if (type == "Platoon")
        {
            if ((user + i)->platoon == target)
            {
                counter++;
                displayUser(user, i, counter);
                displayStatus(user, i);
                cout << endl;
                find = true;
            }
        }
        else if (type == "Program")
        {
            if ((user + i)->program == target)
            {
                counter++;
                displayUser(user, i, counter);
                displayStatus(user, i);
                cout << endl;
                find = true;
            }
        }
        else if (type == "Sure Name")
        {
            if ((user + i)->sureName == target)
            {
                counter++;
                displayUser(user, i, counter);
                displayStatus(user, i);
                cout << endl;
                find = true;
            }
        }
        else if (type == "Last Name")
        {
            if ((user + i)->lastName == target)
            {
                counter++;
                displayUser(user, i, counter);
                displayStatus(user, i);
                cout << endl;
                find = true;
            }
        }
    }

    if (!find)
    {
        cout << "\n[!] No User Match\n"
             << endl;
    }
    cout << line2 << endl;
    cout << line1 << endl;
}

// Overloading function to search attendance in different conditions
void attendanceSmartSearch(User *user, int numUser)
{
    string role = (user + 0)->role; // Role Detection
    int choice = 0;
    int basicCadetChoice = 0;
    int advanceCadetChoice = 0;
    const int width = 40;
    string target = "";
    string line1 = string(width, '=');
    string line2 = string(width, '-');

    // Searh Menu
    if (role == "Basic Cadet")
    {
        basicCadetChoice = userMenu("Basic Cadet Search Menu", "");
    }
    else if (role == "Advance Cadet")
    {
        advanceCadetChoice = userMenu("Advance Cadet Search Menu", "");
    }

    // Search by Company
    if (basicCadetChoice == 1)
    {
        choice = userMenu("Company Menu");
        target = companyConverter(choice);

        attendanceSmartSearch(user, numUser, target, "Company");
    }
    // Search by Platoon
    else if (basicCadetChoice == 2 || advanceCadetChoice == 1)
    {
        if (basicCadetChoice == 2)
        {
            attendanceSmartSearch(user, numUser, target, "Company Platoon");
        }
        else if (advanceCadetChoice == 1)
        {
            choice = userMenu("Platoon Menu");
            target = platoonConverter(choice);
            attendanceSmartSearch(user, numUser, target, "Platoon");
        }
    }
    // Search by Program
    else if (basicCadetChoice == 3 || advanceCadetChoice == 2)
    {
        displayHeader("Program Search", width);
        cout << line2 << endl;
        cout << "Enter Program: ";
        getline(cin, target);
        cout << line2 << endl;
        cout << line1 << endl;
        pressEnter();
        attendanceSmartSearch(user, numUser, target, "Program");
    }
    // Search by Name
    else if (basicCadetChoice == 4 || advanceCadetChoice == 3)
    {
        choice = userMenu("Search By Name Menu");
        if (choice == 1)
        {
            displayHeader("Last Name Search", width);
            cout << line2 << endl;
            cout << "Enter Last Name: ";
            getline(cin, target);
            cout << line2 << endl;
            cout << line1 << endl;
            pressEnter();
            attendanceSmartSearch(user, numUser, target, "Last Name");
        }
        else if (choice == 2)
        {
            displayHeader("Sure Name Search", width);
            cout << line2 << endl;
            cout << "Enter Sure Name: ";
            getline(cin, target);
            cout << line2 << endl;
            cout << line1 << endl;
            pressEnter();
            attendanceSmartSearch(user, numUser, target, "Sure Name");
        }
    }
}

// Overloading function for accessing the basic and advance cadets files
void attendanceSmartSearch()
{
    int choice = 0;
    // Load basic and advance cadets
    User *basicCadets = new User[MAX_USER];
    User *advanceCadets = new User[MAX_USER];

    // Store the number of user based on type
    int numCadets = loadUser(basicCadets, FILE_BASIC);
    int numAdvanceCadets = loadUser(advanceCadets, FILE_ADVANCE);

    choice = userMenu("Role Smart Search", "");

    switch (choice)
    {
    case 1:
        attendanceSmartSearch(basicCadets, numCadets);
        break;
    case 2:
        attendanceSmartSearch(advanceCadets, numAdvanceCadets);
        break;
    }

    // Free Memory
    delete[] basicCadets;
    delete[] advanceCadets;
}

// Function for the brigade staff to create an announcement
void createAnnouncement()
{
    Announcement *announcement = new Announcement;

    // Temporary variable for to bring and note variable in the structure
    string item = "";
    string note = "";
    const int width = 50;

    displayHeader("CREATE ANNOUNCEMENT", width);
    displaySubHeader("ANNOUNCEMENT DETAILS", width);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "WHAT: ";
    getline(cin, announcement->what);
    cout << "WHO: ";
    getline(cin, announcement->who);
    cout << "WHERE: ";
    getline(cin, announcement->where);
    cout << "WHEN: ";
    getline(cin, announcement->when);
    cout << "ATTIRE: ";
    getline(cin, announcement->attire);

    cout << "TO BRING\n";
    cout << "(Type 'done' when finished)\n";

    while (item != "done") // Loop until the user enter 'done'
    {
        cout << "- ";
        getline(cin, item);
        if (item != "done")
        {
            // Store the user item into the toBring variable
            announcement->toBring += "[/] " + item + "\n";
        }
    }

    cout << "NOTE\n";
    cout << "(Type 'done' when finished)\n";

    while (note != "done") // Loop until the user enter 'done'
    {
        cout << "- ";
        getline(cin, note);
        if (note != "done")
            // Store the user note into the note variable
            announcement->note += "[/] " + note + "\n";
    }

    cout << "\nAnnouncement Created!\n";

    saveAnnouncement(announcement);
}

// Function for all user feature
void usersFeature(User *user, int index)
{
    string role = (user + index)->role;   // Store the user's role
    string staff = (user + index)->staff; // Store the staff designation of the user
    bool running = true;

    int choice = 0;

    // Load all users
    User *basicCadets = new User[MAX_USER];
    User *advanceCadets = new User[MAX_USER];
    User *staffOfficers = new User[MAX_USER];
    // Load Announcement
    Announcement *announcement = new Announcement;
    loadAnnouncement(announcement);

    // Store the number of user based on type
    int numCadets = loadUser(basicCadets, FILE_BASIC);
    int numAdvanceCadets = loadUser(advanceCadets, FILE_ADVANCE);
    int numOfficers = loadUser(staffOfficers, FILE_OFFICER);

    // Feature for basic and advance cadets
    if (role == "Basic Cadet" || role == "Advance Cadet")
    {

        while (running)
        {
            choice = userMenu("Cadets Menu", (user + index)->userName); // Display the menu

            switch (choice)
            {
            case 1: // Display the user's personal information
                viewProfile(user, index);
                break;
            case 2: // View Announcements
                displayAnnouncement(announcement);
                break;
            case 3: // Display the user's attendance and status
                viewAttendance(user, index);
                break;
            case 4: // Log out
                displayLogout((user + index)->userName);
                running = false;
                break;
            default:
                break;
            }
            pressEnter();
        }
    }
    else if (role == "Staff Officer")
    {
        while (running)
        {
            choice = userMenu("Staff Officer Menu", (user + index)->userName); // Display the menu

            switch (choice)
            {
            case 1: // Display the user's personal information
                viewProfile(user, index);
                break;
            case 2: // Take Attendance: Basic Cadets or Advance Cadets
                if (staff == "S1")
                {
                    takeAttendance(basicCadets, numCadets, staff);
                }
                else if (staff == "S3")
                {
                    takeAttendance(advanceCadets, numAdvanceCadets, staff);
                }
                break;
            case 3: // View Announcements
                displayAnnouncement(announcement);
                break;
            case 4: // Log out
                displayLogout((user + index)->userName);
                running = false;
                break;
            default:
                break;
            }
            pressEnter();
        }
    }
    else if (role == "Brigade Staff")
    {
        while (running)
        {
            choice = userMenu("Brigade Staff Menu", (user + index)->userName); // Display the menu

            switch (choice)
            {
            case 1: // Display the user's personal information
                viewProfile(user, index);
                break;
            case 2: // Create an Announcement
                createAnnouncement();
                break;
            case 3: // List of User
                listOfUsers();
                break;
            case 4: // Attendance Smart Search
                attendanceSmartSearch();
                break;
            case 5: // Log out
                displayLogout((user + index)->userName);
                running = false;
                break;
            default:
                break;
            }
            pressEnter();
        }
    }

    delete[] basicCadets;
    delete[] advanceCadets;
    delete[] staffOfficers;
    delete announcement;
}