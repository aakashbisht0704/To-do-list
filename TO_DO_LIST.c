#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>

#define MAX_TASKS 100
#define MAX_TASK_LENGTH 50
#define MAX_BIRTHDAYS 100
#define DATE_LENGTH 11

// Structure to represent a task
typedef struct {
    char description[MAX_TASK_LENGTH];
    int completed;
} Task;

// Structure to represent a birthday
typedef struct {
    char name[MAX_TASK_LENGTH];
    int day;
    int month;
} Birthday;

// Structure to represent a festival or holiday
typedef struct {
    char name[MAX_TASK_LENGTH];
    int day;
    int month;
} Festival;

// Function prototypes
void displayMenu();
void addTask(Task tasks[], int *taskCount);
void viewTasks(Task tasks[], int taskCount);
void markTaskComplete(Task tasks[], int taskCount);
void addBirthday(Birthday birthdays[], int *birthdayCount);
void checkBirthdays(Birthday birthdays[], int birthdayCount);
void checkFestivalsAndHolidays();

// Function to show desktop notification using Windows API
void showNotification(const char *title, const char *message);

int main() {
    Task tasks[MAX_TASKS];
    Birthday birthdays[MAX_BIRTHDAYS];
    int taskCount = 0;
    int birthdayCount = 0;
    int choice; 

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1:
                addTask(tasks, &taskCount);
                break;
            case 2:
                viewTasks(tasks, taskCount);
                break;
            case 3:
                viewTasks(tasks, taskCount);
                markTaskComplete(tasks, taskCount);
                break;
            case 4:
                addBirthday(birthdays, &birthdayCount);
                break;
            case 5:
                checkBirthdays(birthdays, birthdayCount);
                break;
            case 6:
                checkFestivalsAndHolidays();
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        Sleep(1500); // Delay of 2000 milliseconds (2 seconds)
    } while (choice != 7);

    return 0;
}

// Display the menu options
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Add Task\n");
    printf("2. View Tasks\n");
    printf("3. Mark Task as Complete\n");
    printf("4. Add Birthday\n");
    printf("5. Check Birthdays\n");
    printf("6. Check Festivals and Holidays\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

// Add a task to the list
void addTask(Task tasks[], int *taskCount) {
    if (*taskCount >= MAX_TASKS) {
        printf("Cannot add more tasks. List is full.\n");
        return;
    }

    printf("Enter task description: ");
    fgets(tasks[*taskCount].description, MAX_TASK_LENGTH, stdin);
    tasks[*taskCount].description[strcspn(tasks[*taskCount].description, "\n")] = 0; // Remove newline character
    tasks[*taskCount].completed = 0; // New task is not completed initially
    (*taskCount)++;
}

// View all tasks
void viewTasks(Task tasks[], int taskCount) {
    printf("\nTasks:\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s - %s\n", i + 1, tasks[i].description, tasks[i].completed ? "Completed" : "Not Completed");
    }
}

// Mark a task as complete
void markTaskComplete(Task tasks[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks to mark.\n");
        return;
    }

    int taskIndex;
    printf("Enter the index of the task to mark as complete: ");
    scanf("%d", &taskIndex);
    getchar(); // Consume newline character

    if (taskIndex < 1 || taskIndex > taskCount) {
        printf("Invalid task index.\n");
        return;
    }

    tasks[taskIndex - 1].completed = 1;
    printf("Task marked as complete.\n");
}

// Add a birthday to the list
void addBirthday(Birthday birthdays[], int *birthdayCount) {
    if (*birthdayCount >= MAX_BIRTHDAYS) {
        printf("Cannot add more birthdays. List is full.\n");
        return;
    }

    printf("Enter name: ");
    fgets(birthdays[*birthdayCount].name, MAX_TASK_LENGTH, stdin);
    birthdays[*birthdayCount].name[strcspn(birthdays[*birthdayCount].name, "\n")] = 0; // Remove newline character
    
    printf("Enter birthday month (1-12): ");
    scanf("%d", &birthdays[*birthdayCount].month);
    getchar(); // Consume newline character
    
    printf("Enter birthday day (1-31): ");
    scanf("%d", &birthdays[*birthdayCount].day);
    getchar(); // Consume newline character
    
    (*birthdayCount)++;
}

// Check birthdays and send notifications
void checkBirthdays(Birthday birthdays[], int birthdayCount) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_month = tm.tm_mon + 1; // Month is zero-based
    int current_day = tm.tm_mday;

    printf("\nBirthdays Today:\n");
    for (int i = 0; i < birthdayCount; i++) {
        if (birthdays[i].month == current_month && birthdays[i].day == current_day) {
            char message[100];
            snprintf(message, sizeof(message), "%s's birthday is today!", birthdays[i].name);
            showNotification("Birthday Notification", message);
        }
    }
}

// Check festivals and holidays and send notifications
void checkFestivalsAndHolidays() {
    FILE *file = fopen("festivals.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open festivals.txt\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_month = tm.tm_mon + 1; // Month is zero-based
    int current_day = tm.tm_mday;

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        char name[MAX_TASK_LENGTH];
        int month, day;
        sscanf(line, "%[^,], %d, %d", name, &month, &day);
        
        if (current_month == month && current_day == day) {
            char message[100];
            snprintf(message, sizeof(message), "%s is today!", name);
            showNotification("Festival/Holiday Notification", message);
        } else if (current_month == month && current_day == day - 1) {
            char message[100];
            snprintf(message, sizeof(message), "%s is tomorrow!", name);
            showNotification("Festival/Holiday Notification", message);
        }
    }

    fclose(file);
}

// Function to show desktop notification using Windows API
void showNotification(const char *title, const char *message) {
    MessageBox(NULL, message, title, MB_ICONINFORMATION | MB_OK);
}