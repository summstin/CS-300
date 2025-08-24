// ProjectTwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

struct Course {

    string courseNumber;
    string courseName;
    vector<string> prereqList;
    Course(std::string& courseNumber, std::string& name, std::vector<std::string>& prereqList);
    Course();
};

Course::Course(std::string& courseNumber, std::string& courseName, std::vector<std::string>& prereqList) {
    this->courseNumber = courseNumber;
    this->courseName = courseName;
    this->prereqList = prereqList;
}

Course::Course()
{
}

class LinkedList {

private:

    struct Node {
        Course course;
        Node* next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) {
            course = aCourse;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Course course);
    void Prepend(Course course);
    void SortedInsert(Node** sortedHead, Node* newNode);
    void SortList();
    void PrintList();
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    //Initialize housekeeping variables
    //set head and tail equal to nullptr
    head = nullptr;
    tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

void LinkedList::Append(Course course) {
    //Create new node
    Node* node = new Node(course);
    //if there is nothing at the head...
    if (nullptr == head) {
        // new node becomes the head and the tail
        head = tail = node;
    }
    //else 
    else {
        // make current tail node point to the new node
        tail->next = node;
        // and tail becomes the new node
        tail = node;
    }
    //increase size count
    size++;
}

void LinkedList::Prepend(Course course) {
    // Create new node
    Node* node = new Node(course);
    // if there is already something at the head...
    if (nullptr == head) {
        // new node points to current head as its next node
        head = tail = node;
    }
    else {
        // head now becomes the new node
        tail->next = node;
        tail = node;
    }
    //increase size count
    size++;

}

void LinkedList::SortedInsert(Node** sortedHead, Node* newNode) {
    // Case 1: Sorted list is empty OR newNode should go before the head
    if (*sortedHead == nullptr || (*sortedHead)->course.courseNumber >= newNode->course.courseNumber) {
        newNode->next = *sortedHead; // link newNode in front
        *sortedHead = newNode;       // newNode becomes the new head
    }
    // Case 2: Traverse list to find correct position
    else {
        Node* current = *sortedHead;
        // Move forward until we find a node whose next is greater or end of list
        while (current->next != nullptr &&
            current->next->course.courseNumber < newNode->course.courseNumber) {
            current = current->next;
        }
        // Insert newNode after current
        newNode->next = current->next;
        current->next = newNode;
    }
}

void LinkedList::SortList() {
    Node* sorted = nullptr;  // start with an empty sorted list
    Node* current = head;    // begin at the current head of the unsorted list

    // Traverse original list
    while (current != nullptr) {
        Node* next = current->next;   // save pointer to next node
        SortedInsert(&sorted, current); // insert current node into sorted list
        current = next;                // move to the next node in original list
    }

    // Update head to point to new sorted list
    head = sorted;

    // Update tail (last node in list)
    tail = head;
    while (tail != nullptr && tail->next != nullptr) {
        tail = tail->next;
    }
}

void LinkedList::PrintList() {
    // start at the head
    Node* current = head;
    // while loop over each node looking for a match
    while (current)
    {
        std::cout << current->course.courseNumber << " | " << current->course.courseName << endl;
        //set current equal to next
        current = current->next;
    }
}

Course LinkedList::Search(string courseNumber) {
    Node* current = head;
    // special case if matching bid is the head
    while (current) {
        // start at the head of the list
        if (courseNumber == current->course.courseNumber) {
            // keep searching until end reached with while loop (current != nullptr)
                // if the current node matches, return current course
            cout << "Match found: " << current->course.courseNumber << " | " << current->course.courseName << endl;
            cout << "Prerequisites: ";

            //if the prereqList is empty
            if (current->course.prereqList.empty()) {

                cout << "None" << endl;
            }
            else {
                //loop prereqList contents
                for (unsigned int i = 0; i < current->course.prereqList.size(); i++) {

                    cout << current->course.prereqList.at(i);

                    //makes list comma separated
                    if (current->course.prereqList.size() > 1 && i < current->course.prereqList.size() - 1) {

                        cout << ", ";
                    }
                }
            }

            cout << endl;
            return current->course;
        }
        // else current node is equal to next node
        else {
            current = current->next;
        }
    }
    //(the next two statements will only execute if search item is not found)
        //create new empty course
    cout << "No course found.";
    return Course();
    //return empty course
}

// Load courses from a CSV file into the linked list
void loadCourses(const std::string& inputFilePath, LinkedList& coursesList) {
    std::cout << "Loading input file " << inputFilePath << std::endl;

    std::ifstream file(inputFilePath);

    // Check if file opened correctly
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << inputFilePath << std::endl;
        return; // Exit function early if file invalid
    }

    std::string currentLine;

    try {
        // Read file line by line
        while (std::getline(file, currentLine)) {
            std::stringstream ss(currentLine);
            std::string word, id, name;
            std::vector<std::string> prerequisites;
            int index = 0;

            // Split line by commas
            while (getline(ss, word, ',')) {
                // Remove unused data
                word = std::regex_replace(word, std::regex(R"(\r\n|\r|\n)"), "");

                if (index == 0) {
                    id = word;
                }
                else if (index == 1) {
                    name = word;
                }
                else {
                    prerequisites.push_back(word);
                }
                index++;
            }

            // Create Course object and append to list
            Course course = Course(id, name, prerequisites);
            coursesList.Append(course);
        }
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "File read error: " << e.what() << std::endl;
    }

    file.close();
}

// Clears leftover characters from the input buffer until newline or EOF
void clearInputBuffer() {
    int c;
    while ((c = std::cin.get()) != '\n' && c != EOF);
}

int main()
{
    LinkedList courseList;
    std::string inputPath;
    std::string courseNumber;
    int choice = 0;

    while (choice != 9) {
        // Display menu
        std::cout << "\n  1. Load Data Structure\n";
        std::cout << "  2. Print Course List\n";
        std::cout << "  3. Print Course\n";
        std::cout << "  9. Exit\n";
        std::cout << "What would you like to do? ";

        // Read menu choice safely
        if (!(std::cin >> choice)) {
            std::cin.clear();           // clear error flag
            clearInputBuffer();         // flush bad input
            std::cout << "Invalid input, please enter a number.\n";
            continue;
        }

        clearInputBuffer(); // remove leftover newline

        switch (choice) {
        case 1:
            std::cout << "Enter the path to the input file: ";
            std::getline(std::cin, inputPath); // allow spaces
            loadCourses(inputPath, courseList);
            break;

        case 2:
            //sort then print list
            courseList.SortList();
            courseList.PrintList();
            break;

        case 3:
            //prompt for course number
            std::cout << "What course do you want to know about?";
            std::getline(std::cin, courseNumber); // allow spaces
            courseList.Search(courseNumber);
            break;

        case 9:
            std::cout << "Exiting program.\n";
            break;

        default:
            std::cout << "Invalid choice, please select a menu option.\n";
            break;
        }
    }

    return 0;
}