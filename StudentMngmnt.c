#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32 // Check if compiling on Windows
#include <Windows.h>
#else // Assuming Unix-like system
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// #ifdef _WIN32 // Check if the operating system is Windows
// #include <windows.h>
// #endif
#define MAX_COURSES 10 // Adjust the maximum number of courses as needed

#define MAX_ATTENDANCE_DAYS 30 // Adjust the maximum number of attendance days as needed
#define MAX_NAME_LENGTH 50

struct Course
{
    char courseName[MAX_NAME_LENGTH];
    float courseGrade;
    struct Course *nextCourse;
};

struct Student
{
    int id;
    char name[50];
    char phone[50];
    char adress[50];
    int age;
    float grades;
    // Add more fields as needed
    struct Course *courses;              // Linked list pointer for courses
    struct Student *next;                // Linked list pointer for students
    int attendance[MAX_ATTENDANCE_DAYS]; // Array to store attendance (0 for absent, 1 for present)
};

struct Student *createStudent(int id, const char *name, int age, float grades);
void insertStudent(struct Student **head, struct Student *student);
void deleteStudent(struct Student **head, int studentIdToDelete);
void displayStudents(struct Student *head);
void markAttendance(struct Student *student, int day);
void viewAttendance(struct Student *student);
void exportStudentDataToCSV(const char *filename, struct Student *head);
void addCourse(struct Course courses[], int *courseCount);

void addCourseToStudent(struct Student *student, const char *courseName, float courseGrade);
void centerText(const char *text)
{
    int textLength = strlen(text);

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int padding = (csbi.srWindow.Right - csbi.srWindow.Left + 1 - textLength) / 2;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Get terminal width
    int padding = (w.ws_col - textLength) / 2;
#endif

    for (int i = 0; i < padding; i++)
    {
        putchar(' ');
    }

    printf("%s", text);
}

void clearScreen()
{
    // Use the "cls" command for Windows or "clear" for Unix-like systems (Linux, macOS)
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main()
{
#ifdef _WIN32
    // Set the console title for Windows
    SetConsoleTitle("Student Management System | DIU");
#else
    // Set the terminal title for non-Windows (assuming ANSI escape codes work)
    printf("\033]0;Student Management System | DIU\007");
#endif

    struct Student *studentList = NULL;

    centerText("Welcome to the Student Management System");
    printf("\n");
    // clearScreen();  // Clear the screen

    while (1)
    {

        printf("\n");
        centerText("1.Add New Student");
        printf("\n");
        centerText("2.View All Student");
        printf("\n");
        centerText("3.Remove A Student");
        printf("\n");
        centerText("4.Search A Student");
        printf("\n");
        centerText("5.Take Attendence");
        printf("\n");
        centerText("6.View Attendence");
        printf("\n");
        centerText("7.Export Data as csv");
        printf("\n");
        centerText("8.Add Course");
        printf("\n");
        centerText("9.Exit");
        printf("\n");
        printf("\n");

        int choice;
        printf("Enter Choice:");
        scanf("%d", &choice);
        int studentId;
        char courseName[MAX_NAME_LENGTH];
        float courseGrade;

        switch (choice)
        {
        case 1:
        {
            clearScreen();
            int id;
            char name[50];
            int age;
            float grades;

            // Prompt the user for student information
            printf("Enter student ID: ");
            scanf("%d", &id);
            printf("Enter student name: ");
            scanf("%s", name);
            printf("Enter student age: ");
            scanf("%d", &age);
            printf("Enter student grades: ");
            scanf("%f", &grades);

            // Create a new student and add it to the list
            struct Student *newStudent = createStudent(id, name, age, grades);
            insertStudent(&studentList, newStudent);

            printf("Student added successfully!\n");
            break;
        }
        case 2:
            // View all students
            clearScreen();
            displayStudents(studentList);
            break;
        case 3:
            int studentIdToDelete;
            printf("Enter student Id to delete:");
            scanf("%d", &studentIdToDelete);
            deleteStudent(&studentList, studentIdToDelete);

        case 4:
            // Search for a student (implement this)
            break;

        case 5:
            clearScreen();
            // Mark Attendance
            int studentId;
            int day;

            printf("Enter student ID: ");
            scanf("%d", &studentId);

            // Find the student with the given ID
            struct Student *currentStudent = studentList;
            while (currentStudent != NULL && currentStudent->id != studentId)
            {
                currentStudent = currentStudent->next;
            }

            if (currentStudent == NULL)
            {
                printf("Student not found.\n");
            }
            else
            {
                printf("Enter the day/session to mark attendance (1-%d): ", MAX_ATTENDANCE_DAYS);
                scanf("%d", &day);

                if (day < 1 || day > MAX_ATTENDANCE_DAYS)
                {
                    printf("Invalid day for marking attendance.\n");
                }
                else
                {
                    markAttendance(currentStudent, day - 1); // Adjust for 0-based index
                    printf("Attendance marked as present for %s on day %d.\n", currentStudent->name, day);
                }
            }
            break;

        case 6:
            // View Attendance
            clearScreen();
            printf("Enter student ID: ");
            scanf("%d", &studentId);

            // Find the student with the given ID
            currentStudent = studentList;
            while (currentStudent != NULL && currentStudent->id != studentId)
            {
                currentStudent = currentStudent->next;
            }

            if (currentStudent == NULL)
            {
                printf("Student not found.\n");
            }
            else
            {
                viewAttendance(currentStudent);
            }
            break;
        case 7:
            // Export Data to CSV
            clearScreen();
            printf("Enter the filename to export data to (e.g., student_data.csv): ");
            char exportFilename[100];
            scanf("%s", exportFilename);

            exportStudentDataToCSV(exportFilename, studentList);
            printf("Data exported to %s successfully.\n", exportFilename);
            break;

        case 8:

            printf("Enter student ID: ");
            scanf("%d", &studentId);

            struct Student *current = studentList;
            while (current != NULL && current->id != studentId)
            {
                current = current->next;
            }

            if (current == NULL)
            {
                printf("Student not found with ID %d\n", studentId);
                break;
            }

            printf("Enter course name: ");
            scanf("%s", courseName);

            printf("Enter course grade: ");
            scanf("%f", &courseGrade);

            addCourseToStudent(current, courseName, courseGrade);
            break;
        case 9:
            printf("Good Bye !!!!.\n");
            // Exit the program
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

struct Student *createStudent(int id, const char *name, int age, float grades)
{
    struct Student *student = (struct Student *)malloc(sizeof(struct Student));
    if (student == NULL)
    {
        // Handle memory allocation error
        return NULL;
    }

    student->id = id;
    strncpy(student->name, name, sizeof(student->name));
    student->age = age;
    student->grades = grades;
    for (int i = 0; i < MAX_ATTENDANCE_DAYS; i++)
    {
        student->attendance[i] = 0; // Initialize attendance to absent
    }
    student->courses = NULL; // Initialize the course list to NULL
    student->next = NULL;    // Initialize the next pointer to NULL

    return student;
}

void insertStudent(struct Student **head, struct Student *student)
{
    if (*head == NULL)
    {
        // If the list is empty, make this student the head
        *head = student;
    }
    else
    {
        // Otherwise, add the student to the end of the list
        struct Student *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = student;
    }
}

// void displayStudents(struct Student *head)
// {
//     if (head == NULL)
//     {
//         printf("No students to display.\n");
//         return;
//     }

//     printf("| %-4s | %-20s | %-4s | %-6s |\n", "ID", "Name", "Age", "Grades");
//     printf("|------|----------------------|------|--------|\n");

//     while (head != NULL)
//     {
//         printf("| %-4d | %-20s | %-4d | %-6.2f |\n", head->id, head->name, head->age, head->grades);
//         printf("|------|----------------------|------|--------|\n");
//         head = head->next;
//     }
// }

void displayStudents(struct Student *head)
{
    if (head == NULL)
    {
        printf("No students to display.\n");
        return;
    }

    printf("| %-4s | %-20s | %-4s | %-6s | %-40s | %-10s |\n", "ID", "Name", "Age", "Grades", "Course Name", "Course Grade");
    printf("|------|----------------------|------|--------|------------------------------------------|--------------|\n");

    while (head != NULL)
    {
        printf("| %-4d | %-20s | %-4d | %-6.2f |", head->id, head->name, head->age, head->grades);

        // Assuming a student can have multiple courses, iterate through them and display each one
        struct Course *course = head->courses;
        while (course != NULL)
        {
            printf(" %-40s | %-10.4f |\n", course->courseName, course->courseGrade);
            if (course->nextCourse != NULL)
                printf("|      |                      |      |        |"); // Separate lines for multiple courses
            course = course->nextCourse;
        }

        printf("|------|----------------------|------|--------|------------------------------------------|------------|\n");
        head = head->next;
    }
}

void markAttendance(struct Student *student, int day)
{
    if (day >= 0 && day < MAX_ATTENDANCE_DAYS)
    {
        student->attendance[day] = 1; // Mark as present
    }
    else
    {
        printf("Invalid day for marking attendance.\n");
    }
}

void viewAttendance(struct Student *student)
{
    printf("Attendance Calendar for %s:\n", student->name);
    printf("+-----+-----+-----+-----+-----+-----+-----+\n");
    printf("| Sun | Mon | Tue | Wed | Thu | Fri | Sat |\n");
    printf("+-----+-----+-----+-----+-----+-----+-----+\n");

    for (int i = 0; i < MAX_ATTENDANCE_DAYS; i++)
    {
        if (i % 7 == 0)
        {
            // Start a new row after each 7 days
            printf("|");
        }

        printf("  %c  |", student->attendance[i] ? 'P' : 'A');

        if (i % 7 == 6)
        {
            // End the row after each 7 days
            printf("\n+-----+-----+-----+-----+-----+-----+-----+\n");
        }
    }

    printf("\n");
}

void exportStudentDataToCSV(const char *filename, struct Student *head)
{
    FILE *file = fopen(filename, "w"); // "w" mode overwrites the file
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "ID,Name,Age,Grades,Attendance\n");

    struct Student *currentStudent = head;
    while (currentStudent != NULL)
    {
        fprintf(file, "%d,%s,%d,%.2f,", currentStudent->id, currentStudent->name, currentStudent->age, currentStudent->grades);

        // Write attendance data (0 for absent, 1 for present) to the CSV
        for (int i = 0; i < MAX_ATTENDANCE_DAYS; i++)
        {
            fprintf(file, "%d", currentStudent->attendance[i]);
            if (i < MAX_ATTENDANCE_DAYS - 1)
            {
                fprintf(file, ",");
            }
        }

        fprintf(file, "\n");

        currentStudent = currentStudent->next;
    }

    fclose(file);
}

void addCourseToStudent(struct Student *student, const char *courseName, float courseGrade)
{
    struct Course *newCourse = (struct Course *)malloc(sizeof(struct Course));
    if (newCourse == NULL)
    {
        // Handle memory allocation error
        return;
    }

    strncpy(newCourse->courseName, courseName, sizeof(newCourse->courseName));
    newCourse->courseGrade = courseGrade;
    newCourse->nextCourse = NULL;

    if (student->courses == NULL)
    {
        student->courses = newCourse;
    }
    else
    {
        struct Course *current = student->courses;
        while (current->nextCourse != NULL)
        {
            current = current->nextCourse;
        }
        current->nextCourse = newCourse;
    }
}

void deleteStudent(struct Student **head, int studentIdToDelete)
{
    struct Student *current = *head;
    struct Student *prev = NULL;

    while (current != NULL && current->id != studentIdToDelete)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        // Student with the given ID not found
        return;
    }

    if (prev == NULL)
    {
        // The student to delete is the head of the list
        *head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    // Free the memory allocated for the deleted student
    free(current);

    printf("Student with ID %d has been deleted.\n", studentIdToDelete);
}
