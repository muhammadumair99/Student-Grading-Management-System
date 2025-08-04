#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <climits>
using namespace std;
struct Student {
    string name;
    int rollNumber;
    vector<int> marks;
    char grade;
    int totalMarks;};
vector<Student> students;

// Function Prototypes
void addStudent();
void viewStudents();
void updateStudent();
void deleteStudent();
void calculateGrades(Student &student);
void generateReports();
void saveToFile();
void loadFromFile();
void searchStudent();

int main() {
    loadFromFile();
    int choice;
    do {
        cout << "\n--- Comprehensive Student Grading System ---\n";
        cout << "1. Add Student Record\n";
        cout << "2. View All Records\n";
        cout << "3. Update Student Record\n";
        cout << "4. Delete Student Record\n";
        cout << "5. Generate Reports\n";
        cout << "6. Search Student Record\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: generateReports(); break;
            case 6: searchStudent(); break;
            case 7: saveToFile(); cout << "Exiting... Changes saved.\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}

void addStudent() {
    Student student;
    cout << "Enter student name: ";
    cin.ignore();
    getline(cin, student.name);

    cout << "Enter roll number: ";
    cin >> student.rollNumber;

    int numSubjects;
    cout << "Enter number of subjects: ";
    cin >> numSubjects;

    student.marks.resize(numSubjects);
    student.totalMarks = 0;

    for (int i = 0; i < numSubjects; i++) {
        cout << "Enter marks for subject " << i + 1 << ": ";
        cin >> student.marks[i];
        student.totalMarks += student.marks[i];
    }

    calculateGrades(student);
    students.push_back(student);

    cout << "Student record added successfully!\n";
}

void viewStudents() {
    if (students.empty()) {
        cout << "No records to display.\n";
        return;
    }

    cout << "\n--- Student Records ---\n";
    cout << left << setw(15) << "Name" << setw(15) << "Roll Number" << setw(15) << "Total Marks" << setw(10) << "Grade" << "\n";
    for (const auto &student : students) {
        cout << left << setw(15) << student.name << setw(15) << student.rollNumber << setw(15) << student.totalMarks << setw(10) << student.grade << "\n";
    }
}

void updateStudent() {
    int roll;
    cout << "Enter roll number of the student to update: ";
    cin >> roll;

    for (auto &student : students) {
        if (student.rollNumber == roll) {
            cout << "Updating record for " << student.name << ".\n";

            cout << "Enter new marks for each subject:\n";
            student.totalMarks = 0;
            for (int i = 0; i < student.marks.size(); i++) {
                cout << "Subject " << i + 1 << ": ";
                cin >> student.marks[i];
                student.totalMarks += student.marks[i];
            }

            calculateGrades(student);
            cout << "Record updated successfully!\n";
            return;
        }
    }

    cout << "Student with roll number " << roll << " not found.\n";
}

void deleteStudent() {
    int roll;
    cout << "Enter roll number of the student to delete: ";
    cin >> roll;

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->rollNumber == roll) {
            students.erase(it);
            cout << "Record deleted successfully!\n";
            return;
        }
    }

    cout << "Student with roll number " << roll << " not found.\n";
}

void calculateGrades(Student &student) {
    float average = student.totalMarks / static_cast<float>(student.marks.size());

    if (average >= 85) student.grade = 'A';
    else if (average >= 70) student.grade = 'B';
    else if (average >= 50) student.grade = 'C';
    else student.grade = 'F';
}

void generateReports() {
    if (students.empty()) {
        cout << "No records available to generate reports.\n";
        return;
    }

    int totalMarks = 0, highest = 0, lowest = INT_MAX;
    for (const auto &student : students) {
        totalMarks += student.totalMarks;
        if (student.totalMarks > highest) highest = student.totalMarks;
        if (student.totalMarks < lowest) lowest = student.totalMarks;
    }

    float average = totalMarks / static_cast<float>(students.size());

    cout << "\n--- Class Performance Report ---\n";
    cout << "Class Average: " << average << "\n";
    cout << "Highest Marks: " << highest << "\n";
    cout << "Lowest Marks: " << lowest << "\n";
}

void saveToFile() {
    ofstream file("students.dat", ios::binary);
    if (!file) {
        cout << "Error saving data.\n";
        return;
    }

    size_t size = students.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& student : students) {
        size_t nameLength = student.name.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(student.name.c_str(), nameLength);
        file.write(reinterpret_cast<const char*>(&student.rollNumber), sizeof(student.rollNumber));

        size_t marksSize = student.marks.size();
        file.write(reinterpret_cast<const char*>(&marksSize), sizeof(marksSize));
        file.write(reinterpret_cast<const char*>(student.marks.data()), marksSize * sizeof(int));

        file.write(reinterpret_cast<const char*>(&student.grade), sizeof(student.grade));
        file.write(reinterpret_cast<const char*>(&student.totalMarks), sizeof(student.totalMarks));
    }

    file.close();
    cout << "Data saved successfully!\n";
}


void loadFromFile() {
    ifstream file("students.dat", ios::binary);
    if (!file) {
        return; // No data file available, start fresh
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    students.resize(size);

    for (auto &student : students) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        student.name.resize(nameLength);
        file.read(&student.name[0], nameLength);

        file.read(reinterpret_cast<char*>(&student.rollNumber), sizeof(student.rollNumber));

        size_t marksSize;
        file.read(reinterpret_cast<char*>(&marksSize), sizeof(marksSize));
        student.marks.resize(marksSize);
        file.read(reinterpret_cast<char*>(student.marks.data()), marksSize * sizeof(int));

        file.read(reinterpret_cast<char*>(&student.grade), sizeof(student.grade));
        file.read(reinterpret_cast<char*>(&student.totalMarks), sizeof(student.totalMarks));
    }

    file.close();
}

void searchStudent() {
    int roll;
    cout << "Enter roll number to search: ";
    cin >> roll;

    for (const auto &student : students) {
        if (student.rollNumber == roll) {
            cout << "\n--- Student Details ---\n";
            cout << "Name: " << student.name << "\n";
            cout << "Roll Number: " << student.rollNumber << "\n";
            cout << "Total Marks: " << student.totalMarks << "\n";
            cout << "Grade: " << student.grade << "\n";
            return;
        }
    }

    cout << "Student with roll number " << roll << " not found.\n";
}
