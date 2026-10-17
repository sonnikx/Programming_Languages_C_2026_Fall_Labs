// week5_task3_student_management_system.c
// Name: Shokhrukh Surname: Abdukhusenov  Student ID: 251A
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
// TODO: Implement the functions to load, save, add, and list students.
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
  char name[NAME_LEN];
  int id;
  float gpa;
} Student;

// Function prototypes (do not change these signatures - the autograder calls
// them directly)
int load_students(Student arr[], int max);

int save_students(const Student arr[], int count);
void add_student(Student arr[], int* count);

void list_students(const Student arr[], int count);
int read_line(const char* prompt, char* buf, size_t size);

int main(void) {
  Student students[MAX_STUDENTS];
  int count = 0;
  char input[LINE_LEN];
  int running = 1;

  count = load_students(students, MAX_STUDENTS);
  printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

  while (running) {
    printf("\n=== Student Management System ===\n");
    printf("1. List students\n");
    printf("2. Add student\n");
    printf("3. Save and Exit\n");

    int choice;
    if (read_line("Select an option: ", input, sizeof input) != 0) {
      choice = 3;
    } else {
      char* end;
      long value = strtol(input, &end, 10);
      choice = (end != input && *end == '\0') ? (int)value : 0;
    }

    switch (choice) {
      case 1:
        list_students(students, count);
        break;

      case 2:
        add_student(students, &count);
        break;

      case 3:
        if (save_students(students, count) != 0) {
          fprintf(stderr, "Error saving students.\n");
          return EXIT_FAILURE;
        }
        printf("Data saved, Goodbye!\n");
        running = 0;
        break;

      default:
        printf("Invalid option. Try again.\n");
    }
  }

  return EXIT_SUCCESS;
}

// Provided helper - do not change
int read_line(const char* prompt, char* buf, size_t size) {
  printf("%s", prompt);
  fflush(stdout);
  if (fgets(buf, (int)size, stdin) == NULL) {
    return -1;
  }
  size_t len = strcspn(buf, "\r\n");
  if (buf[len] == '\0' && len == size - 1) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
  }
  buf[len] = '\0';
  return 0;
}

// student load
int load_students(Student arr[], int max) {
  FILE* fp = fopen(DATA_FILE, "r");
  if (fp == NULL) {
    return 0;
  }

  char line[LINE_LEN];
  int count = 0;

  while (count < max && fgets(line, sizeof(line), fp) != NULL) {
    Student temp = {0};
    int pos = 0;

    int result =
        sscanf(line, "%49[^,],%d,%f %n", temp.name, &temp.id, &temp.gpa, &pos);

    if (result == 3 && pos > 0 && line[pos] == '\0' && temp.name[0] != '\0' &&
        isfinite(temp.gpa) && temp.gpa >= 0.0f && temp.gpa <= 4.0f) {
      arr[count] = temp;
      count++;
    }
  }

  fclose(fp);
  return count;
}

// student save
int save_students(const Student arr[], int count) {
  FILE* fp = fopen(DATA_FILE, "w");
  if (fp == NULL) {
    return -1;
  }

  for (int i = 0; i < count; i++) {
    if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
      fclose(fp);
      return -1;
    }
  }

  if (fclose(fp) != 0) {
    return -1;
  }

  return 0;
}

// studentt
void add_student(Student arr[], int* count) {
  if (*count >= MAX_STUDENTS) {
    printf("Cannot add more students (limit reached).\n");
    return;
  }

  char name[LINE_LEN];
  char id_input[LINE_LEN];
  char gpa_input[LINE_LEN];

  if (read_line("Enter name: ", name, sizeof(name)) != 0) {
    return;
  }

  if (read_line("Enter ID: ", id_input, sizeof(id_input)) != 0) {
    return;
  }

  if (read_line("Enter GPA: ", gpa_input, sizeof(gpa_input)) != 0) {
    return;
  }

  char* end;
  errno = 0;
  long id = strtol(id_input, &end, 10);

  if (name[0] == '\0' || strlen(name) >= NAME_LEN ||
      strchr(name, ',') != NULL || id_input[0] == '\0' || end == id_input ||
      *end != '\0' || errno == ERANGE || id < INT_MIN || id > INT_MAX) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  errno = 0;
  float gpa = strtof(gpa_input, &end);

  if (gpa_input[0] == '\0' || end == gpa_input || *end != '\0' ||
      errno == ERANGE || !isfinite(gpa) || gpa < 0.0f || gpa > 4.0f) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  Student* s = &arr[*count];
  strcpy(s->name, name);
  s->id = (int)id;
  s->gpa = gpa;

  (*count)++;
  printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
  if (count == 0) {
    printf("No students found.\n");
    return;
  }

  printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

  for (int i = 0; i < count; i++) {
    printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
  }
}