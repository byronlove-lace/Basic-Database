#include <iostream>
#include <vector>
#include <ncurses.h>
#include <fstream>
#include <cstring>
#include <string>

using std::endl;
using std::cout;
using std::cin;
using std::vector;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::strlen;
using std::string;
using std::getline;
using std::remove;
using std::rename;

constexpr size_t ENTRY_COUNT = 20;
constexpr size_t MAX_FIRSTNAME = 30;
constexpr size_t MAX_SURNAME = 666;
constexpr size_t MAX_PHONENUM = 15;
void addPerson();

class Person
{
        public:
                char name[MAX_FIRSTNAME + 1];
                char surname[MAX_SURNAME + 1];
                short age;
                char phone[MAX_PHONENUM + 1];

        private:
};

vector<Person> people;

void requireEnter();
void addPerson(vector<Person> &peeps);
int showPeople(vector<Person> &peeps);
int saveToFile(vector<Person> &peeps);
int loadFromFile(vector<Person> &peeps);
int searchDatabase(vector<Person> &peeps);
int removeFromDatabase(vector<Person> &peeps);

int main()
{
        initscr();
        cbreak();
        keypad(stdscr, true);

        char test;
        bool exit = false;

        do 
        {
                noecho();
                        
                printw("People in database: %ld\n", people.size());
                printw("MENU\n1. Add Person\n2. Show People\n3. Save to File\n"
                                "4. Load from File\n5. Search Database\n"
                                "6. Delete Person\n\nPress q to quit\n");
                refresh();

                test = getch();

                switch(test)
                {
                        case '1':
                                erase();
                                addPerson(people);
                                break;
                        case '2':
                                erase();
                                showPeople(people);
                                break;
                        case '3':
                                erase();
                                saveToFile(people);
                                break;
                        case '4':
                                erase();
                                loadFromFile(people);
                                break;
                        case '5':
                                erase();
                                searchDatabase(people);
                                break;
                        case '6':
                                erase();
                                removeFromDatabase(people);
                                break;
                        case 'q':
                                erase();
                                printw("Exiting...");
                                exit = true;
                                break;
                        default:
                                printw("Incorrect input. Please choose 1, 2, 3, 4 or q.\n");
                }
                requireEnter();
                erase();
        }
        while (exit == false);

        getch();
        endwin();
}

void requireEnter()
{
        int ch;
        printw("\n\nHit enter to continue...\n");
        while ((ch = getch()) != '\n');
}

void addPerson(vector<Person> &peeps)
{
        echo();
        Person newPerson;

        attron(A_BOLD);
        printw("\nEnter First Name:\t");
        attroff(A_BOLD);
        getstr(newPerson.name);
        refresh();

        attron(A_BOLD);
        printw("\nEnter Surname:\t");
        attroff(A_BOLD);
        getstr(newPerson.surname);
        refresh();

        attron(A_BOLD);
        printw("\nEnter Age:\t");
        attroff(A_BOLD);
        scanw("%hd", &newPerson.age);
        refresh();

        attron(A_BOLD);
        printw("\nEnter Phone Number:\t");
        attroff(A_BOLD);
        getstr(newPerson.phone);
        refresh();

        peeps.push_back(newPerson);

        printw("\n Storing data...\n");
        refresh();
}

int showPeople(vector<Person> &peeps)
{
        if (!(peeps.size() > 0))
        {
                printw("No people in database");
                return 1;
        }

        for (size_t i = 0; i < peeps.size(); ++i)
        {
                printw("Person Index: %ld\n", i+1);
                printw("First Name: %s\n", peeps[i].name);
                printw("Surname: %s\n", peeps[i].surname);
                printw("Age: %hd\n", peeps[i].age);
                printw("Phone Number: %s\n\n", peeps[i].phone);
        }

        return 0;
}

int saveToFile(vector<Person> &peeps)
{
        size_t dbSize = 0;

        ifstream peopleFH("people.txt");

        if (!peopleFH.is_open())
        {
                printw("Failed to access file.\n");
                return 1;
        }

        peopleFH >> dbSize;
        peopleFH.close();

        if (dbSize == 0)
        {
                ofstream peopleFH("people.txt");

                if (!peopleFH.is_open())
                {
                        printw("Failed to access file.\n");
                        return 1;
                }

                peopleFH << peeps.size() << endl;

                for (size_t i = 0; i < peeps.size(); ++i)
                {
                        peopleFH << peeps[i].name << endl;
                        peopleFH << peeps[i].surname << endl;
                        peopleFH << peeps[i].age << endl;
                        peopleFH << peeps[i].phone << endl;
                }

                peopleFH.close();
        }

        else
        {
                fstream peopleFH;

                peopleFH.open("people.txt"); 
                peopleFH << "" << endl;
                peopleFH.seekg(0, std::ios::beg);
                peopleFH << peeps.size() + dbSize << endl;
                peopleFH.close();

                peopleFH.open("people.txt", std::ios::app);

                for (size_t i = 0; i < peeps.size(); ++i)
                {
                        peopleFH << peeps[i].name << endl;
                        peopleFH << peeps[i].surname << endl;
                        peopleFH << peeps[i].age << endl;
                        peopleFH << peeps[i].phone << endl;
                }

                peopleFH.close();
        }

        printw("Saving to File...\n");

        return 0;
}

int loadFromFile(vector<Person> &peeps)
{
        size_t dbSize;

        ifstream peopleFH("people.txt");

        if (!(peopleFH.is_open()))
        {
                printw("Error opening file\n\n");
                return 1;
        }

        peopleFH >> dbSize;

        if (dbSize < 1)
        {
                printw("Database is empty");
                return 1;
        }

        peeps.resize(dbSize); // need to resize vector before reading into it

        for (size_t i = 0; i < dbSize; ++i)
        {
                peopleFH >> peeps[i].name;
                peopleFH >> peeps[i].surname;
                peopleFH >> peeps[i].age;
                peopleFH >> peeps[i].phone;
        }

        printw("Data loaded successfully\n");

        peopleFH.close();

        return 0;
}

int searchDatabase(vector<Person> &peeps)
{
        echo();
        if (peeps.size() < 1)
        {
                printw("Database is empty\n");
                return 1;
        }

        char chName[MAX_FIRSTNAME + MAX_SURNAME];
        printw("Enter name of person to search for:\t");
        getstr(chName);
        string name(chName);

        size_t lastSpace = 0;
        for (size_t i = 0; i < name.size(); ++i)
        {
                char c = name[i];
                if (c == ' ') lastSpace = i; 
        }

        if (lastSpace > 0)
                name = name.substr(lastSpace); 

        int hitCount = 0;

        for (size_t i = 0; i < peeps.size(); ++i)
        {
                if (name == peeps[i].name || name == peeps[i].surname)
                {
                        ++hitCount;
                        printw("Person Index: %ld", i+1);
                        printw("First Name: %s\n", peeps[i].name);
                        printw("Surname: %s\n", peeps[i].surname);
                        printw("Age: %hd\n", peeps[i].age);
                        printw("Phone Number: %s\n\n", peeps[i].phone);
                }
        }

        printw("\nNumber of results found: %d", hitCount);

        return 0;
}

int removeFromDatabase(vector<Person> &peeps)
{
        int removeLines(size_t deleteFrom, size_t deleteTo, vector<Person> &peeps);
        const unsigned int ENTRIES_PER_RECORD = 5;
        size_t deleteIndex;

        echo();

        if (peeps.size() < 1)
        {
                printw("Database is empty\n");
                return 1;
        }

        printw("Enter the index of the record you want to delete:\t");
        scanw("%ld", &deleteIndex);

        size_t end = 1 + (deleteIndex * ENTRIES_PER_RECORD) + (deleteIndex - 1); 
        size_t start = end - (ENTRIES_PER_RECORD - 1);

        int result = removeLines(start, end, peeps);

        if (!(result == 0))
        {
                printw("Error removing record\n");
                return 1;
        }
        
        return 0;
}

int removeLines(size_t deleteFrom, size_t deleteTo, vector<Person> &peeps)
{
        ifstream inputFH("people.txt");
        ofstream outputFH("temp.txt");

        if (!inputFH) 
        {
                printw("Failed to create input file\n");
                return 1;
        }

        if (!outputFH)
        {
                printw("Failed to create output file\n");
                return 1;
        }

        string line;
        size_t lineNum;

        while (getline(inputFH, line))
        {
                if (!(lineNum >= deleteFrom) && !(lineNum <= deleteTo))
                        outputFH << line << endl;

                ++lineNum;
        }

        inputFH.close();
        outputFH.close();

        int result = remove("people.txt");

        if (!(result == 0))
        {
                printw("Error deleting file\n");
                return 1;
        }

        result = rename("temp.txt", "people.txt");

        if (!(result == 0))
        {
                printw("Error renaming file\n");
                return 1;
        }
        
        return 0;
}
