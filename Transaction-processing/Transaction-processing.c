#include <stdio.h>

#define MAX_ACC 100

typedef struct clientData{
    int id;
    char name[15];
    char surname[20];
    double balance;
}ClientData;

//prototypes
void inizitializeFile(FILE* ptrData);
void printSelection();
void createAccount(FILE* ptrData);
void deleteAccount(FILE* ptrData);
void updateAccount(FILE* ptrData);
void ListAccounts(FILE* ptrData);

void main(){

    static bool init = false;

    FILE* ptrF = fopen("Data.dat", "rb+");
    if(ptrF == NULL){

        ptrF = fopen("Data.dat", "wb+");
        if(ptrF == NULL){
            puts("Unable to open file .dat");
            return;
        }
    }

    fseek(ptrF, 0, SEEK_END);
    long size = ftell(ptrF);
    if (size == 0) inizitializeFile(ptrF);
    
    void (*f[4])(FILE* ptr) = {createAccount, deleteAccount, updateAccount, ListAccounts};
    
    while(true){
        int account = 0;
        printSelection();
        scanf("%d", &account);

        while(account<1 || account>5){
            printf("Select a correct option.\n");
            printSelection();
            scanf("%d", &account);
        }
        if(account==5) break;
        (*f[account-1])(ptrF);
    }
    
    fclose(ptrF);
}

void inizitializeFile(FILE* ptrData){

    ClientData client = {0, "", "", 0.0};
    for(size_t i=0; i<MAX_ACC; i++){
        fwrite(&client, sizeof(ClientData), 1, ptrData);
    }
    rewind(ptrData);
}

void printSelection(){
    printf("Select an option:\n"
            "1 - Create a new account.\n"
            "2 - Delete an account.\n"
            "3 - Update an account.\n"
            "4 - List all accounts.\n"
            "5 - Exit\n"
        );
}

void createAccount(FILE* ptrData){
    
    int account = 0;
    printf("Select account number (1-100): ");
    scanf("%d", &account);

    while(account<1 || account>MAX_ACC){
        printf("Select a correct number.\n");
        printf("Select account number (1-100): ");
        scanf("%d", &account);
    }

    ClientData client = {0, "", "", 0.0};
    fseek(ptrData, (account-1)*sizeof(ClientData), SEEK_SET);

    int numR = fread(&client, sizeof(ClientData), 1, ptrData);
    if(numR == 0){
        puts("File could not be read.");
        return;
    }
    if(client.id != 0){
        puts("Selected account id already used.");
        return;
    }

    client.id = account;
    client.balance = 0;

    printf("Client name: ");
    scanf("%15s", client.name);

    printf("Client surname: ");
    scanf("%15s", client.surname);
    
    fseek(ptrData, (client.id-1)*sizeof(ClientData), SEEK_SET);
    fwrite(&client, sizeof(ClientData), 1, ptrData); 
}

void deleteAccount(FILE* ptrData){
    
    int account = 0;
    printf("Select account number to delete (1-100): ");
    scanf("%d", &account);

    while(account<1 || account>MAX_ACC){
        printf("Select a correct number.\n");
        printf("Select account number to delete (1-100): ");
        scanf("%d", &account);
    }

    ClientData client = {0, "", "", 0.0};
    fseek(ptrData, (account-1)*sizeof(ClientData), SEEK_SET);
    fwrite(&client, sizeof(ClientData), 1, ptrData);
}

void updateAccount(FILE* ptrData){
    
    int account = 0;
    printf("Select account number to update (1-100): ");
    scanf("%d", &account);

    while(account<1 || account>MAX_ACC){
        printf("Select a correct number.\n");
        printf("Select account number to update (1-100): ");
        scanf("%d", &account);
    }

    ClientData client = {0, "", "", 0.0};
    fseek(ptrData, (account-1)*sizeof(ClientData), SEEK_SET);
    int result = fread(&client, sizeof(ClientData), 1, ptrData);
    if(client.id == 0){
        printf("Incorrect account number\n");
        return;
    }

    double balance = 0.0;
    printf("Select a balance to add: ");
    scanf("%lf", &balance);

    client.balance += balance;
    fseek(ptrData, (account-1)*sizeof(ClientData), SEEK_SET);
    fwrite(&client, sizeof(ClientData), 1, ptrData);
}

void ListAccounts(FILE* ptrData){
    
    FILE* listF = fopen("List.txt", "w");
    if(listF == NULL){
        puts("Unable to open file .txt");
        return;
    }

    rewind(ptrData);
    fprintf(listF, "%-10s\t%-17s\t%-22s\t%s\n", "Account", "Name", "Surname", "Balance");
    ClientData client = {0, "", "", 0.0};

    while(!feof(ptrData)){
        int result = fread(&client, sizeof(ClientData), 1, ptrData);
        if(client.id != 0){
            fprintf(listF, "%-10d\t%-17s\t%-22s\t%.3f\n", client.id, client.name, client.surname, client.balance);
        }
    }
    fclose(listF);
}