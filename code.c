#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    int bedID;
    int isOccupied;
    char patient[100];
}Bed;
Bed beds[50];
void initializebeds() {
    for (int i=0;i<50;i++) {
        beds[i].bedID=i+1;
        beds[i].isOccupied=0;
        strcpy(beds[i].patient,"None");
    }
}

void assignbed() {
    char name[100];
    printf("Enter patient name: ");
    scanf("%s",name);
    for (int i = 0; i < 50; i++){
        if (beds[i].isOccupied==0){
            beds[i].isOccupied = 1;
            strcpy(beds[i].patient, name);
            printf("Patient '%s' assigned to bed ID %d\n",name,beds[i].bedID);
            return;
        }
    }
    printf("No available beds right now.\n");
}


void showtotal() {
    int occupied = 0;
    for (int i = 0; i < 50; i++) {
        if (beds[i].isOccupied) {
            occupied++;
        }
    }
    printf("\n--- Bed Status ---\n");
    printf("Total Beds     : %d\n",50);
    printf("Occupied Beds  : %d\n",occupied);
    printf("Available Beds : %d\n",50-occupied);
}
#define MAX_HISTORY 100
#define FILE_NAME "patients.txt"

// Medical History Record Node
typedef struct Record {
    char data[200];
    struct Record* next;
} Record;

// Patient Node
typedef struct Patient {
    char name[50];
    int age;
    char disease[50];
    int severity;
    Record* history;
    struct Patient* next;
    struct Patient* prev;
} Patient;

Patient* head = NULL;

// Function declarations
void savePatientsToFile();
Patient* findPatient(char name[]);

// Stack operations
void addMedicalRecord(Patient* patient, char record[]) {
    Record* newRecord = (Record*)malloc(sizeof(Record));
    strcpy(newRecord->data, record);
    newRecord->next = patient->history;
    patient->history = newRecord;
    printf("Medical record added for %s.\n", patient->name);
}

void getLatestRecord(Patient* patient) {
    if (!patient->history) {
        printf("No medical history available for %s.\n", patient->name);
        return;
    }
    Record* temp = patient->history;
    printf("🩺 Latest Record: %s\n", temp->data);
    patient->history = temp->next;
    free(temp);
}

void viewAllHistory(Patient* patient) {
    printf("Medical History for %s:\n", patient->name);
    Record* temp = patient->history;
    if (!temp) {
        printf("No records found.\n");
        return;
    }
    while (temp) {
        printf("- %s\n", temp->data);
        temp = temp->next;
    }
}

// Patient management
void savePatientsToFile() {
    FILE* file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }
    Patient* temp = head;
    while (temp) {
        fprintf(file, "%s %d %s %d\n", temp->name, temp->age, temp->disease, temp->severity);
        temp = temp->next;
    }
    fclose(file);
}

void registerPatient(char name[], int age, char disease[], int severity) {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    strcpy(newPatient->name, name);
    newPatient->age = age;
    strcpy(newPatient->disease, disease);
    newPatient->severity = severity;
    newPatient->next = NULL;
    newPatient->prev = NULL;
    newPatient->history = NULL;

    if (!head) {
        head = newPatient;
    } else {
        Patient* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newPatient;
        newPatient->prev = temp;
    }
    savePatientsToFile();
    printf("Patient Registered!\n");
}

void loadPatientsFromFile() {
    FILE* file = fopen(FILE_NAME, "r");
    if (!file) return;

    char name[50], disease[50];
    int age, severity;
    while (fscanf(file, "%s %d %s %d", name, &age, disease, &severity) != EOF) {
        registerPatient(name, age, disease, severity);
    }
    fclose(file);
}

Patient* findPatient(char name[]) {
    Patient* temp = head;
    while (temp) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void displayPatients() {
    Patient* temp = head;
    if (!temp) {
        printf("No patients.\n");
        return;
    }
    while (temp) {
        printf("Name: %s | Age: %d | Disease: %s | Severity: %d\n",
               temp->name, temp->age, temp->disease, temp->severity);
        temp = temp->next;
    }
}

void deletePatient(char name[]) {
    Patient* temp = findPatient(name);
    if (!temp) {
        printf("Patient not found.\n");
        return;
    }

    // Remove from list
    if (temp->prev) temp->prev->next = temp->next;
    if (temp->next) temp->next->prev = temp->prev;
    if (temp == head) head = temp->next;

    // Free history stack
    Record* r = temp->history;
    while (r) {
        Record* del = r;
        r = r->next;
        free(del);
    }
    free(temp);
    savePatientsToFile();
    printf("Patient Deleted.\n");
}

void menu() {
    int choice;
    char name[50], disease[50], record[200];
    int age, severity;

    while (1) {
        printf("\n Menu:\n");
        printf("1. Register Patient\n");
        printf("2. View Patients\n");
        printf("3. Search Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Add Medical History\n");
        printf("6. View Latest Record\n");
        printf("8. Exit\n");
        printf("7. Assign Bed\n");
        
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        Patient* p;

        switch (choice) {
            case 1:
                printf("Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Age: ");
                scanf("%d", &age);
                getchar();

                printf("Disease: ");
                fgets(disease, sizeof(disease), stdin);
                disease[strcspn(disease, "\n")] = 0;

                printf("Severity (1-15): ");
                scanf("%d", &severity);
                getchar();

                registerPatient(name, age, disease, severity);
                break;

            case 2:
                displayPatients();
                break;

            case 3:
                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                p = findPatient(name);
                if (p) {
                    printf("Found: %s | Age: %d | Disease: %s | Severity: %d\n",
                           p->name, p->age, p->disease, p->severity);
                } else {
                    printf("Not Found\n");
                }
                break;

            case 4:
                printf("Enter Name to Delete: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                deletePatient(name);
                break;

            case 5:
                printf("Patient Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                p = findPatient(name);
                if (p) {
                    printf("Enter Record: ");
                    fgets(record, sizeof(record), stdin);
                    record[strcspn(record, "\n")] = 0;
                    addMedicalRecord(p, record);
                } else {
                    printf("Patient not found.\n");
                }
                break;

            case 6:
                printf("Patient Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                p = findPatient(name);
                if (p) {
                    getLatestRecord(p);
                } else {
                    printf("Patient not found.\n");
                }
                break;

            
            case 8:
                printf("Exiting...\n");
                return;
            case 7:
            printf("Patient Name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;
            p = findPatient(name);
            if (p) {
                printf("Let me reconfirm the name:\n");
                assignbed();
                showtotal();
            } else {
                printf("Patient not found.\n");
            }
            break;



            
            default:
            printf("Invalid choice.\n");
        }
    }
}
typedef struct doctor {
    char name[50];
    char title[50];
    char department[50];
    struct doctor*left; 
    struct doctor*right; 
}doctor;
doctor* create(const char* name, const char* title, const char* department) {
    doctor* newdoc = (doctor*)malloc(sizeof(doctor));
    strncpy(newdoc->name, name, 49);
    strncpy(newdoc->title, title, 49);
    strncpy(newdoc->department, department, 49);
    newdoc->name[49] = '\0';
    newdoc->title[49] = '\0';
    newdoc->department[49] = '\0';
    newdoc->left = NULL;
    newdoc->right = NULL;
    return newdoc;
}
void preorderTraversal(doctor* root,int level) {
    if (root == NULL) return;
    int i;
    for (i=0;i<level;i++) {
        printf(" ");
    }
    printf("+-- %s (%s, %s)\n", root->name, root->title, root->department);
    preorderTraversal(root->left,level+1);
    preorderTraversal(root->right,level+1);
}
void inorder(doctor* root,int level) {
    if (root==NULL) return;
    inorder(root->left,level+1);
    int i;
    for(i=0;i<level;i++){
    	printf("  ");
	}
    printf("+-- %s (%s,%s)\n", root->name, root->title, root->department);
    inorder(root->right,level+1);
}
void postorder(doctor* root,int level){
    if (root == NULL) return;
    postorder(root->left,level+1);
    postorder(root->right,level+1);
    int i;
    for(i=0;i<level;i++){
    	printf("  ");
	}
    printf("+-- %s (%s,%s)\n", root->name, root->title, root->department);
}
void view_doctor_heirarchy (){
    doctor* root = create("Dr.Snehal B Shinde","Chief Medical Officer","Administration");
    root->left = create("Dr. Orange", "Head of Surgery", "Surgery");
    root->right = create("Dr. Blue", "Head of Cardiology", "Cardiology");
    root->left->left = create("Dr. Aditya", "Paediatric Surgeon", "Surgery");
    root->left->right = create("Dr. Tushar", "Surgeon", "Brain Surgery");
    root->right->left = create("Dr. Raveena", "Cardiologist", "Cardiology");
    root->right->right = create("Dr. Aryan", "Cardiologist", "Cardiology");
    printf("\nPre-order Traversal:\n");
    preorderTraversal(root, 0);
    printf("\nIn-order Traversal:\n");
    inorder(root,0);
    printf("\nPost-order Traversal:\n");
    postorder(root,0);
}


int main() {
initializebeds();
loadPatientsFromFile();
    menu();
view_doctor_heirarchy();
      return 0;
  }