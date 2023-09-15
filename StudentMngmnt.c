#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>  // Add this header for ioctl
#include <termios.h>    // Add this header for TIOCGWINSZ
#ifdef _WIN32 // Check if the operating system is Windows
#include <windows.h>
#endif
#define MAX_COURSES 10 // Adjust the maximum number of courses as needed

struct Course
{
    char courseName[50];
    float courseGrade;
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
    struct Course* courses; // Linked list pointer for courses
    struct Student* next;   // Linked list pointer for students
};

void centerText(const char* text)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Get terminal width

    int textLength = strlen(text);
    int padding = (w.ws_col - textLength) / 2;

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


    centerText("Welcome to the Student Management System");
    printf("\n");
    //clearScreen();  // Clear the screen

    while(1)
    {
        centerText("1.Add New Student");
        printf("\n");
        centerText("2.View All Student");
        printf("\n");
        centerText("3.Remove A Student");
        printf("\n");
        centerText("4.Search A Student");
        printf("\n");
        centerText("5.Exit");
        printf("\n");

    }


    return 0;
}
