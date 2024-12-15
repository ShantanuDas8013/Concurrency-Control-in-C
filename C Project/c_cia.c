#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count = 0;
static int time = 0;
char conflict[100]="";

 void print_resouce();

static struct Resource
{
    int resource[10][3];
} resource;

int transaction_operations[10] = {0};

static struct Transaction_table
{
    int sl_no[20];
    struct Transaction 
    {
        int id;
        int time;
        struct Data 
        {
            int index;
            int value;
        } transaction_holds;
        char tFun[100];
    } transaction[10][20];
} table;

int inti_resource()
{
    FILE *file = fopen("resource.txt", "r"); 
    if (file == NULL)
    {
        printf("Error: Could not open resource file.\n");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        fscanf(file, "%d %d", &resource.resource[i][0], &resource.resource[i][1]);
        resource.resource[i][2] = -1;
    }

    fclose(file); 

    printf("Resource initialized from file:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("Resource[%d]: Value = %d, Metadata = %d, Lock Status = %d\n", i, resource.resource[i][0], resource.resource[i][1], resource.resource[i][2]);
    }

    return 0;
}

int incremeant_time()
{
    time = time + 1;
    return 0;
}

int create_Transaction()
{
    struct Transaction transaction;
    transaction.id = count;
    transaction.time = time;
    strcpy(transaction.tFun, "init");

    table.sl_no[count] = count;
    table.transaction[count][0] = transaction;
    transaction.transaction_holds.value = -1;
    
    printf("\n\nA transaction created.........: %d\n", transaction.id);
    printf("Transaction details\nTransaction id: %d\nTransaction time: %d\nTransaction value: %d\nTransaction function: %s\n",
           transaction.id, transaction.time, transaction.transaction_holds.value, transaction.tFun);
    
    count++;
    return transaction.id;
}

int read_operation(int tid, int resource_index)
{
    printf("\n...........The read operation...........:\n");
    printf("on T%d is being performed\n", tid);

    int op_count = transaction_operations[tid];  
    struct Transaction new_transaction = table.transaction[tid][op_count];
    
    if (resource.resource[resource_index][2] != -1 && resource.resource[resource_index][2] != tid)
    {
        printf("Resource %d is locked by another transaction (T%d).\n", resource_index, resource.resource[resource_index][2]);
        return -1;
    }

    printf("Do you want to lock this resource? (1 for Yes, 0 for No): ");
    int lock_choice;
    scanf("%d", &lock_choice);

    if (lock_choice == 1)
    {
        resource.resource[resource_index][2] = tid;
        printf("Resource %d locked by T%d.\n", resource_index, tid);
    }

    new_transaction.transaction_holds.index = resource_index;
    new_transaction.transaction_holds.value = resource.resource[resource_index][0];

    resource.resource[resource_index][1]++;
    new_transaction.time = time;

    int r_value = resource.resource[resource_index][0];
    snprintf(new_transaction.tFun, sizeof(new_transaction.tFun), "read of resource_%d = %d", r_value);

    table.transaction[tid][op_count + 1] = new_transaction;
    transaction_operations[tid]++;

    printf("The transaction sl_no in the table is %d\n", table.sl_no[tid]);
    printf("Read resource value: %d from resource[%d]\n", new_transaction.transaction_holds.value, resource_index);

    return 0;
}

int Artithmatic_operation(int tid)
{
    printf("\n...........The Arithmetic operation...........:\n");
    printf("on T%d is being performed\n", tid);

    int op_count = transaction_operations[tid];  
    struct Transaction new_transaction = table.transaction[tid][op_count];
    
    int before_value = new_transaction.transaction_holds.value;
    int add_it = 100;

    if(before_value <= -1 || before_value > 10000)
    {
        printf("\nThe transactions does not hold any value as of now");
        return -1;
    }

    int choice = -1;
    printf("press 1 for Add opertion\npress any key to perform subtract operation");
    scanf("%d",&choice);
    
    if(choice == 1)
    {
            printf("Enter the value need to be added:");
        scanf("%d",&add_it);

        new_transaction.transaction_holds.value += add_it;
        new_transaction.time = time;

        snprintf(new_transaction.tFun, sizeof(new_transaction.tFun), "Arithmetic operation of %d + %d", before_value, add_it);

        table.transaction[tid][op_count + 1] = new_transaction;
        transaction_operations[tid]++;

        printf("The transaction sl_no in the table is %d\n", table.sl_no[tid]);
        printf("Resource value: %d from resource[%d]\n", new_transaction.transaction_holds.value, new_transaction.transaction_holds.index);
        return 1;
    }

    printf("Enter the value need to be subtracted:");
    scanf("%d",&add_it);

    new_transaction.transaction_holds.value -= add_it;
    new_transaction.time = time;

    snprintf(new_transaction.tFun, sizeof(new_transaction.tFun), "Arithmetic operation of %d - %d", before_value, add_it);

    table.transaction[tid][op_count + 1] = new_transaction;
    transaction_operations[tid]++;

    printf("The transaction sl_no in the table is %d\n", table.sl_no[tid]);
    printf("Resource value: %d from resource[%d]\n", new_transaction.transaction_holds.value, new_transaction.transaction_holds.index);

    return 2;
}

int write_operation(int tid)
{
    printf("\n...........The write operation...........:\n");
    printf("on T%d is being performed\n", tid);
    
    print_resouce();

    int op_count = transaction_operations[tid];
    struct Transaction new_transaction = table.transaction[tid][op_count];

    if (resource.resource[new_transaction.transaction_holds.index][2] != tid)
    {
        printf("Resource %d is not locked by T%d. Cannot perform write operation.\n", new_transaction.transaction_holds.index, tid);
        return -1;
    }
    // printf("transaction_holds.value = %d\n", table.transaction[tid][op_count].transaction_holds.value);

    
    // if(table.transaction[tid][op_count].transaction_holds.value == -1)
    // {
    //     printf("\n#############################\nThere might be a Inconsistency in the transaction\n Transaction holds no resources\n#############################\n");
        
    //     return -1;
    // }
    int resource_index = new_transaction.transaction_holds.index; 
    int before_value = resource.resource[resource_index][0];

    resource.resource[resource_index][0] = new_transaction.transaction_holds.value;
    new_transaction.time = time;

    snprintf(new_transaction.tFun, sizeof(new_transaction.tFun), "Writing on resource[%d]: %d -> %d", resource_index, before_value, resource.resource[resource_index][0]);

    table.transaction[tid][op_count + 1] = new_transaction;
    transaction_operations[tid]++;

    printf("The transaction sl_no in the table is %d\n", table.sl_no[tid]);
    printf("Resource value: %d from resource[%d]\n", new_transaction.transaction_holds.value, new_transaction.transaction_holds.index);
    
    resource.resource[resource_index][1]--;
    resource.resource[resource_index][2] = -1;  
    print_resouce();

    return 0;
}


int commit_operation(int tid)
{
    printf("\n...........The commit operation...........:\n");
    printf("on T%d is being performed\n", tid);

    print_resouce();

    int op_count = transaction_operations[tid];
    struct Transaction new_transaction = table.transaction[tid][op_count];

    // Commit the transaction
    new_transaction.time = time;
    snprintf(new_transaction.tFun, sizeof(new_transaction.tFun), "commit on t%d", tid);

    table.transaction[tid][op_count + 1] = new_transaction;
    transaction_operations[tid]++;

    FILE *file = fopen("resource.txt", "w");
    if (file == NULL)
    {
        printf("Error: Could not open resource file for writing.\n");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        fprintf(file, "%d %d\n", resource.resource[i][0], resource.resource[i][1]);
    }

    fclose(file); 

    printf("The transaction sl_no in the table is %d\n", table.sl_no[tid]);
    printf("Resource value: %d from resource[%d]\n", new_transaction.transaction_holds.value, new_transaction.transaction_holds.index);

    return 0;
}


void print_resouce()
{   
    printf("\nResource values : \n");
    for(int i = 0; i < 10; i++)
    {
        printf("\t%d ", resource.resource[i][0]);
    }
    printf("\n");
}

void print_table()
{
    printf("\n========== The Transaction Table ==========\n");
    printf("\n\tsl_no\tid\ttime\tR_value\tfunction\n");

    for (int i = 0; i < count; i++)
    {
        printf("\n-----------------------\n");
        printf("\t %d", table.sl_no[i]);
        for (int j = 0; j <= transaction_operations[i]; j++)
        {
            printf("\t\t\t%d \t%d \t%d \t%s \n",
                   table.transaction[i][j].id,
                   table.transaction[i][j].time,
                   table.transaction[i][j].transaction_holds.value,
                   table.transaction[i][j].tFun);
        }
    }
    printf("\n===========================================\n");
}
int menu()
{
    int choice;
    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Create Transaction\n");
        printf("2. Read Operation\n");
        printf("3. Arithmetic Operation\n");
        printf("4. Write Operation\n");
        printf("5. Commit Operation\n");
        printf("6. Print Transaction Table\n");
        printf("7. Print Resource Status\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)  
        {
            printf("Invalid input. Please enter an integer.\n");
            while (getchar() != '\n');  
            continue;
        }

        int tid, resource_index;

        switch (choice)
        {
            case 1:
                create_Transaction();
                break;
            case 2:
                printf("Enter Transaction ID: ");
                if (scanf("%d", &tid) != 1)                  {
                    printf("Invalid input. Please enter an integer.\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("Enter Resource Index: ");
                if (scanf("%d", &resource_index) != 1)
                {
                    printf("Invalid input. Please enter an integer.\n");
                    while (getchar() != '\n'); 
                    continue;
                }
                read_operation(tid, resource_index);
                break;
            case 3:
                printf("Enter Transaction ID: ");
                if (scanf("%d", &tid) != 1) 
                {
                    printf("Invalid input. Please enter an integer.\n");
                    while (getchar() != '\n');
                    continue;
                }
                Artithmatic_operation(tid);
                break;
            case 4:
                printf("Enter Transaction ID: ");
                if (scanf("%d", &tid) != 1)  
                {
                    printf("Invalid input. Please enter an integer.\n");
                    while (getchar() != '\n');
                    continue;
                }
                write_operation(tid);
                break;
            case 5:
                printf("Enter Transaction ID: ");
                if (scanf("%d", &tid) != 1)
                {
                    printf("Invalid input. Please enter an integer.\n");
                    while (getchar() != '\n');
                    continue;
                }
                commit_operation(tid);
                break;
            case 6:
                print_table();
                break;
            case 7:
                print_resouce();
                break;
            case 8:
                printf("Exiting...\n");
                exit(0);
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }

        incremeant_time();
    }
    return 0;
}

int main()
{
    inti_resource();
    // print_table();

    // int t1 = create_Transaction();
    // incremeant_time();
    // read_operation(t1, 1);  


    // int t2 = create_Transaction();
    // read_operation(t2,1);
    // incremeant_time();
    // read_operation(t2, 1); 
    // incremeant_time();
    // Artithmatic_operation(t1);  
    // write_operation(t1);  
    // commit_operation(t1);
  
    // Artithmatic_operation(t2);  
    // write_operation(t2);  
    
    // int t3 = create_Transaction();
    // incremeant_time();
    
    // incremeant_time();

    // print_table();
    // print_resouce();

    menu();

    return 0;
}   