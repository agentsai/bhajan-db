#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BHAJANS 1000
#define MAX_LYRICS_LENGTH 1000

// Bhajan structure to store title, shruti, and date
typedef struct {
    int id;
    char title[100];
    char shruti[20];
    char date[30];
} Bhajan;

Bhajan bhajans[MAX_BHAJANS];
int bhajan_count = 0;

// Function declarations
void trim_newline(char *str);
int is_duplicate(const char *title, const char *shruti, const char *date);
int compare_bhajans(const void *a, const void *b);
void save_to_file();
void load_from_file();
void import_from_file();
void display_bhajans();
void delete_bhajan();
void edit_bhajan();
void add_bhajans_by_date();

// Remove newline characters from string
void trim_newline(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[--len] = '\0';
    }
}

// Check for duplicate bhajan
int is_duplicate(const char *title, const char *shruti, const char *date) {
    for (int i = 0; i < bhajan_count; i++) {
        if (strcmp(bhajans[i].title, title) == 0 &&
            strcmp(bhajans[i].shruti, shruti) == 0 &&
            strcmp(bhajans[i].date, date) == 0) {
            return 1;
        }
    }
    return 0;
}

// Compare function for sorting
int compare_bhajans(const void *a, const void *b) {
    return strcmp(((Bhajan *)a)->title, ((Bhajan *)b)->title);
}

// Save to file in alphabetical order
void save_to_file() {
    qsort(bhajans, bhajan_count, sizeof(Bhajan), compare_bhajans);

    FILE *fp = fopen("bhajans_db.txt", "w");
    if (!fp) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < bhajan_count; i++) {
        fprintf(fp, "%s|%s|%s\n", bhajans[i].title, bhajans[i].shruti, bhajans[i].date);
    }

    fclose(fp);
    printf("Database saved to file 'bhajans_db.txt' successfully.\n");
}

// Load bhajans from file
void load_from_file() {
    FILE *fp = fopen("bhajans_db.txt", "r");
    if (!fp) return;

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        trim_newline(line);
        if (strlen(line) == 0 || strspn(line, " \r\n") == strlen(line)) continue;

        char *title = strtok(line, "|");
        char *shruti = strtok(NULL, "|");
        char *date = strtok(NULL, "|");

        if (title && shruti && date) {
            while (*date == ' ') date++;
            trim_newline(title);
            trim_newline(shruti);
            trim_newline(date);

            if (!is_duplicate(title, shruti, date)) {
                Bhajan *b = &bhajans[bhajan_count];
                b->id = bhajan_count + 1;
                strncpy(b->title, title, sizeof(b->title));
                strncpy(b->shruti, shruti, sizeof(b->shruti));
                strncpy(b->date, date, sizeof(b->date));
                bhajan_count++;
            }
        }
    }
    fclose(fp);
}

// Import bhajans from external file
void import_from_file() {
    char filename[100];
    printf("Enter input file name: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        trim_newline(line);
        if (strlen(line) == 0 || strspn(line, " \r\n") == strlen(line)) continue;

        char *title = strtok(line, "|");
        char *shruti = strtok(NULL, "|");
        char *date = strtok(NULL, "|");

        if (title && shruti && date) {
            while (*date == ' ') date++;
            trim_newline(title);
            trim_newline(shruti);
            trim_newline(date);

            if (!is_duplicate(title, shruti, date)) {
                Bhajan *b = &bhajans[bhajan_count];
                b->id = bhajan_count + 1;
                strncpy(b->title, title, sizeof(b->title));
                strncpy(b->shruti, shruti, sizeof(b->shruti));
                strncpy(b->date, date, sizeof(b->date));
                bhajan_count++;
            }
        }
    }
    fclose(fp);
    printf("Imported bhajan(s) from file.\n");
    save_to_file();
}

// Display all bhajans
void display_bhajans() {
    printf("\n%-5s | %-20s | %-10s | %-12s\n", "ID", "Title", "Shruti", "Date");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < bhajan_count; i++) {
        printf("%-5d | %-20s | %-10s | %-12s\n",
               bhajans[i].id, bhajans[i].title, bhajans[i].shruti, bhajans[i].date);
    }
}

// Delete a bhajan by ID
void delete_bhajan() {
    int id;
    printf("Enter Bhajan ID to delete: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < bhajan_count; i++) {
        if (bhajans[i].id == id) {
            found = 1;
            for (int j = i; j < bhajan_count - 1; j++) {
                bhajans[j] = bhajans[j + 1];
            }
            bhajan_count--;
            printf("Bhajan deleted successfully.\n");
            save_to_file();
            break;
        }
    }
    if (!found) {
        printf("Bhajan ID not found.\n");
    }
}

// Edit a bhajan by ID
void edit_bhajan() {
    int id;
    printf("Enter Bhajan ID to edit: ");
    scanf("%d", &id);

    for (int i = 0; i < bhajan_count; i++) {
        if (bhajans[i].id == id) {
            printf("Enter new title: ");
            getchar();  // consume leftover newline
            fgets(bhajans[i].title, sizeof(bhajans[i].title), stdin);
            trim_newline(bhajans[i].title);

            printf("Enter new shruti: ");
            fgets(bhajans[i].shruti, sizeof(bhajans[i].shruti), stdin);
            trim_newline(bhajans[i].shruti);

            printf("Enter new date: ");
            fgets(bhajans[i].date, sizeof(bhajans[i].date), stdin);
            trim_newline(bhajans[i].date);

            printf("Bhajan updated successfully.\n");
            save_to_file();
            return;
        }
    }
    printf("Bhajan ID not found.\n");
}

// Add multiple bhajans by date
void add_bhajans_by_date() {
    char date[30];
    char title[100];
    char shruti[20];
    char choice[10];

    getchar();
    printf("Enter date for the bhajans (format: dd Mon yyyy): ");
    fgets(date, sizeof(date), stdin);
    trim_newline(date);

    while (1) {
        printf("Enter title: ");
        fgets(title, sizeof(title), stdin);
        trim_newline(title);

        printf("Enter shruti: ");
        fgets(shruti, sizeof(shruti), stdin);
        trim_newline(shruti);

        if (!is_duplicate(title, shruti, date)) {
            Bhajan *b = &bhajans[bhajan_count];
            b->id = bhajan_count + 1;
            strncpy(b->title, title, sizeof(b->title));
            strncpy(b->shruti, shruti, sizeof(b->shruti));
            strncpy(b->date, date, sizeof(b->date));
            bhajan_count++;
        } else {
            printf("Duplicate bhajan skipped.\n");
        }

        save_to_file();

        printf("Do you want to add another bhajan for the same date? (yes/no): ");
        fgets(choice, sizeof(choice), stdin);
        trim_newline(choice);
        if (strcmp(choice, "no") == 0 || strcmp(choice, "No") == 0) {
            break;
        }
    }
}

// Main menu
int main() {
    load_from_file();

    int choice;

    while (1) {
        printf("\n--- Bhajan Database Menu ---\n");
        printf("1. Display All Bhajans\n");
        printf("2. Edit Bhajan\n");
        printf("3. Delete Bhajan\n");
        printf("4. Import from File\n");
        printf("5. Save to File\n");
        printf("6. Add Bhajans by Date\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: display_bhajans(); break;
            case 2: edit_bhajan(); break;
            case 3: delete_bhajan(); break;
            case 4: import_from_file(); break;
            case 5: save_to_file(); break;
            case 6: add_bhajans_by_date(); break;
            case 7: exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

