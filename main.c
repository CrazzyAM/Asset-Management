#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
// #include "struct.h"
// #include "functions.h"

struct Asset
{
    int Asset_ID;
    char Asset_Name[15];
    char Purchase_Data[11];
    char EOL_Date[11];
    char Tagging_Status[10];
    int Is_Active;
    struct Asset *next;
};

struct Employee
{
    int Employee_ID;
    char First_Name[15];
    char Last_Name[15];
    char Designation[10];
    char Joining_Date[11];
    char Location[15];
    struct Employee *next;
};

struct Employee_Asset_Tagging
{
    int Employee_ID;
    int Asset_ID;
    char Tagging_Date[11];
    char Release_Date[11];
    struct Employee_Asset_Tagging *next;
};

struct Asset_Requests
{
    int Employee_ID;
    int Request_ID;
    char Request_Date[11];
    char Device[15];
    int Request_Status;
    struct Asset_Requests *next;
};

struct Login_Details
{
    char Username[10];
    char Password[10];
    char User_type;
    int Login_Status;
};

char Login( char username[], char password[]);
void adminlib(char username[]);
void emplib(char username[]);
int Logout( char username[]);
void creatfiles();
// ASSETS LL
void BuildAssetList(struct Asset **headref);
void displayAsset(struct Asset **headref);
void Add_Asset(struct Asset *add, struct Asset **headref);
void Update_Asset(int A_ID, struct Asset **headref);
void DeleteAsset(int A_ID, struct Asset **headref);
void SearchAsset(int A_ID, struct Asset **headref);
void Save_ChangesAsset(struct Asset **headref);
//END ASSETS
//FREE POOL
void Free_Pool(struct Asset **headref);

void EOL_Asset(struct Asset **headref);

//EMPLOYEE LL
void displayEmployee(struct Employee **headref);
void BuildEmployeeList(struct Employee **headref);
void Addlogin(struct Employee *ptr);
void Add_Employee(struct Employee *add, struct Employee **headref);
void Update_Employee(int E_ID, struct Employee **headref);
void DeleteEmployee(int E_ID, struct Employee **headref);
void SearchEmployee(int E_ID, struct Employee **headref);
void Save_ChangesEmployee(struct Employee **headref);
//END EMPLOYEE LL
//TAGGING ASSETS
void BuildTaggingList(struct Employee_Asset_Tagging **headref);
void displayTaggingList(struct Employee_Asset_Tagging **headref);
void viewTagged(int E_ID, struct Employee_Asset_Tagging **headref);
void BuildAssetRequest(struct Asset_Requests **headref);
void displayAssetRequest(struct Asset_Requests **headref);
int RequestAsset(int E_ID, char device[], struct Asset_Requests **headref);
void SaveChangesRequest(struct Asset_Requests **headref);
void viewAssetStatus(int E_ID, struct Asset_Requests **headref);

void Tagged_Asset(struct Employee_Asset_Tagging **headref);

int tagAsset(int E_ID, int A_ID, char date[], struct Employee_Asset_Tagging **headref);
int de_tagAsset(int E_ID, int A_ID, char date[], struct Employee_Asset_Tagging **headref);
void SaveChangesTagging(struct Employee_Asset_Tagging **headref);
void postTaggedAsset(int E_ID, int A_ID, struct Asset **headref1, struct Asset_Requests **headref2);
void postdeTaggedAsset(int E_ID, struct Asset **headref);

int main()
{
    char username[10], password[10], u_type;
    int op, ch;
    FILE *fp;
    fp = fopen("Login.txt", "r");
    if(fp != NULL)
    {
        fclose(fp);
        l2:
        do
        {
            printf("MENU\n\t1) Admin\n\t2) Employee\nEnter op: ");
            scanf("%d", &op);
            switch(op)
            {
                case 1:
                    printf("\nEnter Username: ");
                    scanf("%s", username);
                    getchar();
                    printf("\nEnter Password: ");
                    scanf("%s", password);
                    getchar();
                    u_type = Login( username, password);
                    printf("\n%c", u_type);
                    if(u_type == 'A')
                        adminlib(username);
                    break;
                case 2:
                    printf("\nEnter Username: ");
                    scanf("%s", username);
                    getchar();
                    printf("\nEnter Password: ");
                    scanf("%s", password);
                    getchar();
                    u_type = Login( username, password);
                    printf("\n%c", u_type);
                    if(u_type == 'E')
                        emplib(username);
                    break;
                case 3:
                    break;
                default:
                    printf("Enter Valid Option");
                    break;
            }
        }while(op != 3);
    }
    else
    {
        printf("\nWelcome User:\n\tAs your first time login your Default Username and Password is: admin");
        l1:
        printf("\nEnter Username: ");
        scanf("%s", username);
        printf("\nEnter Password: ");
        scanf("%s", password);
        if(strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
        {
            struct Login_Details login;
            printf("\nEnter New Username: ");
            scanf("%s", login.Username);
            printf("\nEnter New Password: ");
            scanf("%s", login.Password);
            login.User_type = 'A';
            login.Login_Status = 0;
            fp = fopen("Login.txt", "w");
            fprintf(fp, "%s %s %c %d\n", login.Username, login.Password, login.User_type, login.Login_Status);
            fclose(fp);
            goto l2;
        }
        else
        {
            printf("\nIncorrect Username or Password");
            printf("\nPress Enter to continue...");
            ch = getchar();
            goto l1;
        }
    }
    return 0;
}

char Login( char username[], char password[])
{
    char u_type = '0';
    int found;
    struct Login_Details l_det; 
    FILE *fp, *fp1;
    fp = fopen("Login.txt", "r");
    fp1 = fopen("y1.txt", "w");
    if(fp == NULL)
        printf("\nFile Login.txt not opened");
    else
    {
        while(!feof(fp))
        {
            fscanf(fp, "%s %s %c %d\n", l_det.Username, l_det.Password, &l_det.User_type, &l_det.Login_Status);
            if(strcmp(l_det.Username, username) == 0)
            {
                found = 1;
                    if(strcmp(l_det.Password, password) == 0)
                    {
                        u_type = l_det.User_type;
                        l_det.Login_Status = 1;
                        fprintf(fp1, "%s %s %c %d\n", l_det.Username, l_det.Password, l_det.User_type, l_det.Login_Status);
                    }
                    else
                    {
                        printf("\nIncorrect password");
                        fprintf(fp1, "%s %s %c %d\n", l_det.Username, l_det.Password, l_det.User_type, l_det.Login_Status);
                    }
            }
            else
                fprintf(fp1, "%s %s %c %d\n", l_det.Username, l_det.Password, l_det.User_type, l_det.Login_Status);
        }
        fclose(fp);
        fclose(fp1);
        remove("Login.txt");
        rename("y1.txt", "Login.txt");
        if(found)
            return u_type;
        else
            return u_type;
    }
}

int Logout( char username[])
{
    int flag = 0;
    struct Login_Details l_det; 
    FILE *fp, *fp1;
    fp = fopen("Login.txt", "r");
    fp1 = fopen("y1.txt", "w");
    if(fp == NULL)
        printf("\nFile Login.txt not opened");
    else
    {
        while(!feof(fp))
        {
            fscanf(fp, "%s %s %c %d\n", l_det.Username, l_det.Password, &l_det.User_type, &l_det.Login_Status);
            if(strcmp(l_det.Username, username) == 0)
            {
                flag = 1;
                l_det.Login_Status = 0;
                fprintf(fp1, "%s %s %c %d\n", l_det.Username, l_det.Password, l_det.User_type, l_det.Login_Status);
            }
            else
                fprintf(fp1, "%s %s %c %d\n", l_det.Username, l_det.Password, l_det.User_type, l_det.Login_Status);
        }
        fclose(fp);
        fclose(fp1);
        remove("Login.txt");
        rename("y1.txt", "Login.txt");
        
        if(flag)
            return 1;
        else
            return 0;
    }
}

void creatfiles()
{
    FILE *fp;
    fp = fopen("Asset.txt", "r");
    if(fp != NULL)
        fclose(fp);
    else
    {
        fp = fopen("Asset.txt", "w");
        fclose(fp);
    }
    fp = fopen("Employee.txt", "r");
    if(fp != NULL)
        fclose(fp);
    else
    {
        fp = fopen("Employee.txt", "w");
        fclose(fp);
    }
    fp = fopen("Asset_Tagging.txt", "r");
    if(fp != NULL)
        fclose(fp);
    else
    {
        fp = fopen("Asset_Tagging.txt", "w");
        fclose(fp);
    }
    fp = fopen("Asset_Requests.txt", "r");
    if(fp != NULL)
        fclose(fp);
    else
    {
        fp = fopen("Asset_Requests.txt", "w");
        fclose(fp);
    }
}
// ADMIN LIBRARIES:::::::::::::::::::::::::::::::::::::::::::::::::::::
void adminlib(char username[])
{
    FILE *fp;
    time_t t = time(NULL);
    struct tm date = *gmtime(&t);
    int op, Employee_ID, Asset_ID, size;
    char Date[11], ch;
    struct Asset *Asset_head = NULL;
    creatfiles();
    fp = fopen("Asset.txt", "r");
    if (fp != NULL) 
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        if(size != 0)
        {
            BuildAssetList(&Asset_head);
        }
    }
    struct Employee *Emp_head = NULL;
    fp = fopen("Employee.txt", "r");
    if (fp != NULL) 
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        if(size != 0)
        {
            BuildEmployeeList(&Emp_head);
        }
    }
    struct Employee_Asset_Tagging *Tagging_head = NULL;
    fp = fopen("Asset_Tagging.txt", "r");
    if (fp != NULL) 
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        if(size != 0)
        {
            BuildTaggingList(&Tagging_head);
        }
    }
    struct Asset_Requests *Request_head = NULL;
    fp = fopen("Asset_Requests.txt", "r");
    if (fp != NULL) 
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        if(size != 0)
        {
            BuildAssetRequest(&Request_head);
        }
    }
    do{
        printf("\nAdmin MENU\n\t1) Asset\n\t2) Employee\n\t3) Free Pool\n\t4) Tagged Asset\n\t5) EOL assets\n\t6) Tag an asset\n\t7) De-tag an asset\n\t8) Log out\n Enter your option: ");
        scanf("%d", &op);
        switch(op)
        {
            case 1:
                printf("\n");
                int op1;
                l3:
                do{
                    printf("\nAsset MENU\n\t1) Add Asset\n\t2) Update Asset\n\t3) Search\n\t4) Delete Asset\n\t5) Display Assets\n\t6)Save Changes\n Enter your option: ");
                    scanf("%d", &op1);
                    switch(op1)
                    {
                        case 1:
                            printf("\n");
                            fp = fopen("Asset.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                            }
                            fclose(fp);
                            struct Asset add, *temp;
                            temp = Asset_head;
                            if (size == 0) 
                            {
                                add.Asset_ID = 1;
                            }
                            else
                            {
                                while(temp->next != NULL)
                                    temp = temp->next;
                                add.Asset_ID = temp->Asset_ID + 1;

                            }
                            printf("\nAsset Name: ");
                            scanf("%s", add.Asset_Name);
                            sprintf(add.Purchase_Data, "%d-%02d-%02d", date.tm_year + 1900 , date.tm_mon + 1, date.tm_mday);
                            sprintf(add.EOL_Date, "%d-%02d-%02d", date.tm_year + 1900 + 4, date.tm_mon + 1, date.tm_mday);
                            strcpy(add.Tagging_Status, "FREE");
                            add.Is_Active = 1;
                            if(size == 0)
                            {
                                fp = fopen("Asset.txt", "w");
                                fprintf(fp, "%d %s %s %s %s %d\n", add.Asset_ID, add.Asset_Name, add.Purchase_Data, add.EOL_Date, add.Tagging_Status, add.Is_Active);
                                fclose(fp);
                                BuildAssetList(&Asset_head);
                            }
                            else
                                Add_Asset(&add, &Asset_head);
                            break;
                        case 2:
                            printf("\n");
                            fp = fopen("Asset.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Asset Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l3;
                                }
                            }
                            
                            printf("\nEnter Asset ID: ");
                            scanf("%d", &Asset_ID);
                            Update_Asset(Asset_ID, &Asset_head);
                            break;
                        case 3:
                            printf("\n");
                            fp = fopen("Asset.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Asset Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l3;
                                }
                            }
                            printf("\nEnter Asset ID: ");
                            scanf("%d", &Asset_ID);
                            SearchAsset(Asset_ID, &Asset_head);
                            break;
                        case 4:
                            printf("\n");
                            fp = fopen("Asset.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Asset Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l3;
                                }
                            }
                            printf("\nEnter Asset ID: ");
                            scanf("%d", &Asset_ID);
                            DeleteAsset(Asset_ID, &Asset_head);
                            break;
                        case 5:
                            fp = fopen("Asset.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Asset Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l3;
                                }
                            }
                            displayAsset(&Asset_head);
                        case 6:
                            Save_ChangesAsset(&Asset_head);
                            break;
                        default:
                            
                            break;
                    }
                }while(op1 != 6);
                break;
            case 2:
                printf("\n");
                int op2;
                l4:
                do{
                    printf("\nEmployee MENU\n\t1) Add Employee\n\t2) Update Employee\n\t3) Search Employee\n\t4) Delete Employee\n\t5) Display Employee List\n\t6) Save Changes\n Enter your option: ");
                    scanf("%d", &op2);
                    switch(op2)
                    {
                        case 1:
                            printf("\n");
                            fp = fopen("Employee.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                            }
                            fclose(fp);
                            struct Employee add, *temp;
                            temp = Emp_head;
                            if(size == 0)
                            {
                                add.Employee_ID = 1;
                            }
                            else
                            {
                                while(temp->next != NULL)
                                    temp = temp->next;
                                add.Employee_ID = temp->Employee_ID + 1;
                            }
                            printf("\Employee First Name: ");
                            scanf("%s", add.First_Name);
                            printf("\nEmployee Last Name: ");
                            scanf("%s", add.Last_Name);
                            printf("\nDesignation: ");
                            scanf("%s", add.Designation);
                            sprintf(add.Joining_Date, "%d-%02d-%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
                            printf("\nEnter Location: ");
                            scanf("%s", add.Location);
                            if(size == 0)
                            {
                                fp = fopen("Employee.txt", "w");
                                fprintf(fp, "%d %s %s %s %s %s\n", add.Employee_ID, add.First_Name, add.Last_Name, add.Designation, add.Joining_Date, add.Location);
                                fclose(fp);
                                Addlogin(&add);
                                BuildEmployeeList(&Emp_head);
                            }
                            else
                                Add_Employee(&add, &Emp_head);
                            break;
                        case 2:
                            printf("\n");
                            fp = fopen("Employee.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Employee.txt Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l4;
                                }
                            }
                            printf("\nEnter Employee ID: ");
                            scanf("%d", &Employee_ID);
                            Update_Employee(Employee_ID, &Emp_head);
                            break;
                        case 3:
                            printf("\n");
                            fp = fopen("Employee.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Employee.txt Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l4;
                                }
                            }
                            printf("\nEnter Employee ID: ");
                            scanf("%d", &Employee_ID);
                            SearchEmployee(Employee_ID, &Emp_head);
                            break;
                        case 4:
                            printf("\n");
                            fp = fopen("Employee.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Employee.txt Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l4;
                                }
                            }
                            printf("\nEnter Employee ID: ");
                            scanf("%d", &Employee_ID);
                            DeleteEmployee(Employee_ID, &Emp_head);
                            break;
                        case 5:
                            fp = fopen("Employee.txt", "r");
                            if (fp != NULL) 
                            {
                                fseek (fp, 0, SEEK_END);
                                size = ftell(fp);
                                fclose(fp);
                                if(size == 0)
                                {
                                    printf("\nFile Employee.txt Does not have any content\nPlease Add an Asset First\nPress enter to Continue...");
                                    ch = getchar();
                                    ch = getchar();
                                    goto l4;
                                }
                            }
                            displayEmployee(&Emp_head);
                            break;
                        case 6:
                            Save_ChangesEmployee(&Emp_head);
                            break;
                        default:
                            
                            break;
                    }
                }while(op2 != 6);
                break;
            case 3:
                Free_Pool(&Asset_head);
                break;
            case 4:
                Tagged_Asset(&Tagging_head);
                break;
            case 5:
                EOL_Asset(&Asset_head);
                break;
            case 6:
                printf("\n");
                fp = fopen("Asset_Requests.txt", "r");
                if (fp != NULL) 
                {
                    fseek (fp, 0, SEEK_END);
                    size = ftell(fp);
                    fclose(fp);
                    if(size == 0)
                    {
                        printf("\nNo employee has Requested\nPress enter to Continue...");
                        ch = getchar();
                        ch = getchar();
                        goto l4;
                    }
                }
                printf("Enter Employee ID: ");
                scanf("%d", &Employee_ID);
                printf("Enter Asset ID: ");
                scanf("%d", &Asset_ID);
                sprintf(Date, "%d-%02d-%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
                fp = fopen("Asset_Tagging.txt", "r");
                if (fp != NULL) 
                {
                    fseek (fp, 0, SEEK_END);
                    size = ftell(fp);
                    fclose(fp);
                    if(size != 0)
                    {
                        if(tagAsset(Employee_ID, Asset_ID, Date, &Tagging_head))
                        {
                            postTaggedAsset(Employee_ID, Asset_ID, &Asset_head, &Request_head);
                        }
                        else
                            printf("\nWENT OFF");
                    }
                    else
                    {
                        fp = fopen("Asset_Tagging.txt", "w");
                        fprintf(fp, "%d %d %s NULL\n", Employee_ID, Asset_ID, Date);
                        fclose(fp);
                        BuildTaggingList(&Tagging_head);
                        postTaggedAsset(Employee_ID, Asset_ID, &Asset_head, &Request_head);
                    }
                }
                break;
            case 7:
                printf("\n");
                fp = fopen("Asset_Tagging", "r");
                if(fp != NULL)
                {
                    fseek (fp, 0, SEEK_END);
                    size = ftell(fp);
                    fclose(fp);
                    if(size == 0)
                    {
                        printf("\nEmpty Asset_Tagging File\nPress Enter to Continue....");
                        ch = getchar();
                        ch = getchar();
                        goto l4;
                    }
                }
                printf("Enter Employee ID: ");
                scanf("%d", &Employee_ID);
                printf("Enter Asset ID: ");
                scanf("%d", &Asset_ID);
                sprintf(Date, "%d-%02d-%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
                if(de_tagAsset(Employee_ID, Asset_ID, Date, &Tagging_head))
                {
                    postdeTaggedAsset(Asset_ID, &Asset_head);
                }
                else
                    printf("\nWENT OFF");
                break;
            case 8:
                Save_ChangesAsset(&Asset_head);
                Save_ChangesEmployee(&Emp_head);
                SaveChangesTagging(&Tagging_head);
                SaveChangesRequest(&Request_head);
                Logout(username);
                break;
            default:
                
                break;
        }
    }while(op != 8);
}

//ASSET FUNCTIONS:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void displayAsset(struct Asset **headref)
{
    struct Asset *p;
    p = *headref;
    printf("\n");
    int count = 0;
   
    while(p != NULL) 
    {
        count++;
        printf("%d-%s-%s-%s-%s-%d\n", p->Asset_ID, p->Asset_Name, p->Purchase_Data, p->EOL_Date, p->Tagging_Status, p->Is_Active);
        p = p->next;
    }
    printf("\n%d",count);
}

void BuildAssetList(struct Asset **headref)
{
    struct Asset *newnode, *temp;
    temp = *headref;
    FILE *fp;
    fp = fopen("Asset.txt", "r");
    if(fp == NULL)
        printf("Asset.txt file not opened");
    else
    {
        while(!feof(fp))
        {
            if(*headref == NULL)
            {
                newnode = malloc(sizeof(struct Asset));
                fscanf(fp, "%d %s %s %s %s %d\n", &newnode->Asset_ID, newnode->Asset_Name, newnode->Purchase_Data, newnode->EOL_Date, newnode->Tagging_Status, &newnode->Is_Active);
                newnode->next = *headref;
                *headref = newnode;
            }
            else
            {
                temp = *headref;
                newnode = malloc(sizeof(struct Asset));
                while (temp->next != NULL)  
                {  
                    temp = temp->next;  
                }  
                fscanf(fp, "%d %s %s %s %s %d\n", &newnode->Asset_ID, newnode->Asset_Name, newnode->Purchase_Data, newnode->EOL_Date, newnode->Tagging_Status, &newnode->Is_Active);
                if(newnode->Asset_ID == 0)
                {
                    free(newnode);
                    continue;
                }
                temp->next = newnode;  
                newnode->next = NULL;  
            }
        }
    }
    fclose(fp);
}

void Add_Asset(struct Asset *add, struct Asset **headref)
{
    struct Asset *newnode, *temp;
    temp = *headref;
    newnode = malloc(sizeof(struct Asset));
    newnode->Asset_ID = add->Asset_ID;
    strcpy(newnode->Asset_Name, add->Asset_Name);
    strcpy(newnode->Purchase_Data, add->Purchase_Data);
    strcpy(newnode->EOL_Date, add->EOL_Date);
    strcpy(newnode->Tagging_Status, add->Tagging_Status);
    newnode->Is_Active = add->Is_Active;
    if(*headref == NULL)
    {
        newnode->next = *headref;
        *headref = newnode;
    }
    else
    {
        while (temp->next != NULL)  
        {  
            temp = temp->next;  
        }  
        temp->next = newnode;  
        newnode->next = NULL; 
    }
    
}

void Update_Asset(int A_ID, struct Asset **headref)
{
    struct Asset *temp;
    temp = *headref;
    while (temp->Asset_ID != A_ID)
    {
        temp = temp->next;
    }
    printf("Update Tagged Status of Asset: ");
    scanf("%s", temp->Tagging_Status);
    printf("Update IS Active Status of Asset: ");
    scanf("%d", &temp->Is_Active);
}

void DeleteAsset(int A_ID, struct Asset **headref)
{
    struct Asset *temp, *prev;
    temp = *headref;
    
    if (temp == NULL) 
        return;
    
    if (temp->Asset_ID == A_ID) 
    {
        *headref = (*headref)->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->Asset_ID != A_ID) 
    {
        prev = temp;
        temp = temp->next;
    }

    prev->next = temp->next;
    printf("\nnode deleted\n");   
}

void SearchAsset(int A_ID, struct Asset **headref)
{
    struct Asset *temp;
    temp = *headref;
    int found = 1;
    while (temp != NULL)
    {
        if(temp->Asset_ID == A_ID)
        {
            found = 0;
            printf("\nElement found");
            break;
        }
        else
            temp = temp -> next;
    }
    if(found)
        printf("\nNot Found");
}

void Save_ChangesAsset(struct Asset **headref)
{
    struct Asset *temp;
    if(*headref == NULL)
        return;
    temp = *headref;
    FILE *fp;
    fp = fopen("y2.txt", "w");
    if(fp == NULL)
        printf("not opened");
    while(temp != NULL)
    {
        fprintf(fp, "%d %s %s %s %s %d\n", temp->Asset_ID, temp->Asset_Name, temp->Purchase_Data, temp->EOL_Date, temp->Tagging_Status, temp->Is_Active);
        temp = temp->next;
    }
    fclose(fp);
    remove("Asset.txt");
    rename("y2.txt", "Asset.txt");
}
//END OF ASSET FUNCTIONS:::::::::::::::::::::::::::::::::::::::

//FREE POOL
void Free_Pool(struct Asset **headref)
{
    struct Asset *p;
    p = *headref;
    printf("\n");
    int count = 0;
   
    while(p != NULL) 
    {
        if(strcmp(p->Tagging_Status, "FREE") == 0 && p->Is_Active == 1)
        {
            count++;
            printf("%d-%s-%s-%s-%s-%d\n", p->Asset_ID, p->Asset_Name, p->Purchase_Data, p->EOL_Date, p->Tagging_Status, p->Is_Active);
        }
        p = p->next;
    }
    if(count == 0)
    {
        printf("\n\t*** NO FREE Assets ***");
    }
}
//EOL DATE
void EOL_Asset(struct Asset **headref)
{
    struct Asset *p;
    time_t t = time(NULL);
    struct tm date = *gmtime(&t);
    int year1, month1, day1, year2, month2, day2;
    //c_year = date.tm_year + 1900;
    p = *headref;
    printf("\n");
    int count = 0;
    
    while(p != NULL) 
    {
        sscanf(p->Purchase_Data, "%d-%02d-%02d", &year1, &month1, &day1);
        sscanf(p->EOL_Date, "%d-%02d-%02d", &year2, &month2, &day2);
        if(year2 < year1)
        {
            count++;
            printf("%d-%s-%s-%s-%s-%d\n", p->Asset_ID, p->Asset_Name, p->Purchase_Data, p->EOL_Date, p->Tagging_Status, p->Is_Active);
        }
        p = p->next;
    }
    if(count == 0)
    {
        printf("\n\t*** NO EOL Asset ***");
    }
}

//EMPLOYEE FUNCTIONS::::::::::::::::::::::::::::::::::::::::::::
void displayEmployee(struct Employee **headref)
{
    struct Employee *p;
    p = *headref;
    printf("\n");
    int count = 0;
   
    while(p != NULL) 
    {
        count++;
        printf("%d-%s-%s-%s-%s-%s\n", p->Employee_ID, p->First_Name, p->Last_Name, p->Designation, p->Joining_Date, p->Location);
        p = p->next;
    }
    printf("\n%d",count);
}

void BuildEmployeeList(struct Employee **headref)
{
    struct Employee *newnode, *temp;
    temp = *headref;
    FILE *fp;
    fp = fopen("Employee.txt", "r");
    if(fp == NULL)
        printf("Employee.txt file not opened");
    else
    {
        while(!feof(fp))
        {
            if(*headref == NULL)
            {
                newnode = malloc(sizeof(struct Employee));
                fscanf(fp, "%d %s %s %s %s %s\n", &newnode->Employee_ID, newnode->First_Name, newnode->Last_Name, newnode->Designation, newnode->Joining_Date, newnode->Location);
                newnode->next = *headref;
                *headref = newnode;
            }
            else
            {
                temp = *headref;
                newnode = malloc(sizeof(struct Employee));
                while (temp->next != NULL)  
                {  
                    temp = temp->next;  
                }  
                fscanf(fp, "%d %s %s %s %s %s\n", &newnode->Employee_ID, newnode->First_Name, newnode->Last_Name, newnode->Designation, newnode->Joining_Date, newnode->Location);
                if(newnode->Employee_ID == 0 || newnode->Employee_ID <= 0)
                {
                    free(newnode);
                    continue;
                }
                temp->next = newnode;  
                newnode->next = NULL;  
            }
        }
    }
    fclose(fp);
}

void Addlogin(struct Employee *ptr)
{
    struct Login_Details l;
    time_t t = time(NULL);
    struct tm date = *gmtime(&t);
    int len = strlen(ptr->Last_Name);
    char month[13][4] = {"jan\0", "feb\0", "mar\0", "apr\0", "may\0", "jun\0", "jul\0", "aug\0", "sep\0", "oct\0", "nov\0", "dec\0"};
    sprintf(l.Username, "%d", ptr->Employee_ID);
    sprintf(l.Password, "%s%c%c%c%c", month[date.tm_mon],ptr->First_Name[0],ptr->First_Name[1],ptr->Last_Name[len - 2], ptr->Last_Name[len-1]);
    l.User_type = 'E';
    l.Login_Status = 0;
    FILE *fp;
    fp = fopen("Login.txt", "a");
    fprintf(fp, "%s %s %c %d\n", l.Username, l.Password, l.User_type, l.Login_Status);
    fclose(fp);
}

void Add_Employee(struct Employee *add, struct Employee **headref)
{
    struct Employee *newnode, *temp;
    temp = *headref;
    newnode = malloc(sizeof(struct Employee));
    while (temp->next != NULL)  
    {  
        temp = temp->next;  
    }  
    newnode->Employee_ID = add->Employee_ID;
    strcpy(newnode->First_Name, add->First_Name);
    strcpy(newnode->Last_Name, add->Last_Name);
    strcpy(newnode->Designation, add->Designation);
    strcpy(newnode->Joining_Date, add->Joining_Date);
    strcpy(newnode->Location, add->Location);
    temp->next = newnode;
    newnode->next = NULL; 
    Addlogin(newnode);
}

void Update_Employee(int E_ID, struct Employee **headref)
{
    struct Employee *temp = malloc(sizeof(struct Employee));
    temp = *headref;
    while (temp != NULL && temp->Employee_ID != E_ID)
    {
        temp = temp->next;
    }
    printf("Update Designation: ");
    scanf("%s", temp->Designation);
    printf("Update Location: ");
    scanf("%s", temp->Location);
}

void DeleteEmployee(int E_ID, struct Employee **headref)
{
    struct Employee *temp, *prev;
    temp = *headref;
    
    if (temp == NULL) 
        return;
    
    if (temp->Employee_ID == E_ID) 
    {
        *headref = (*headref)->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->Employee_ID != E_ID) 
    {
        prev = temp;
        temp = temp->next;
    }

    prev->next = temp->next;
    printf("\nnode deleted\n");  
}

void SearchEmployee(int E_ID, struct Employee **headref)
{
    struct Employee *temp;
    temp = *headref;
    int found = 1;
    while (temp != NULL)
    {
        if(temp->Employee_ID == E_ID)
        {
            found = 0;
            printf("\nElement found");
            break;
        }
        else
            temp = temp -> next;
    }
    if(found)
        printf("\nNot Found");
}

void Save_ChangesEmployee(struct Employee **headref)
{
    struct Employee *temp;
    if(*headref == NULL)
        return;
    temp = *headref;
    FILE *fp;
    fp = fopen("y2.txt", "w");
    if(fp == NULL)
        printf("not opened");
    while(temp != NULL)
    {
        fprintf(fp, "%d %s %s %s %s %s\n", temp->Employee_ID, temp->First_Name, temp->Last_Name, temp->Designation, temp->Joining_Date, temp->Location);
        temp = temp->next;
    }
    fclose(fp);
    remove("Employee.txt");
    rename("y2.txt", "Employee.txt");
}
//END OF EMPLOYEE FUNCTIONS:::::::::::::::::::::::::::::::::::::::::::::::::::::
int tagAsset(int E_ID, int A_ID, char date[], struct Employee_Asset_Tagging **headref)
{
    struct Employee_Asset_Tagging *newnode, *temp;
    temp = *headref;
    newnode = malloc(sizeof(struct Employee_Asset_Tagging));
    if(newnode == NULL)
        return 0;
    newnode->Employee_ID = E_ID;
    newnode->Asset_ID = A_ID;
    strcpy(newnode->Tagging_Date, date);
    strcpy(newnode->Release_Date, "NULL");
    while(temp->next != NULL)
        temp = temp->next;
    newnode->next = NULL;
    temp->next = newnode;
    if(temp->next != newnode || newnode->next != NULL)
        return 0;
    else
        return 1;
}

int de_tagAsset(int E_ID, int A_ID, char date[], struct Employee_Asset_Tagging **headref)
{
    struct Employee_Asset_Tagging *temp = malloc(sizeof(struct Employee_Asset_Tagging));
    temp = *headref;
    while(temp->next != NULL && temp->Employee_ID != E_ID && temp->Asset_ID != A_ID)
        temp = temp->next;
    if(temp == NULL)
        return 0;
    strcpy(temp->Release_Date, date);
    return 1;
}

void SaveChangesTagging(struct Employee_Asset_Tagging **headref)
{
    struct Employee_Asset_Tagging *temp;
    if(*headref == NULL)
        return;
    temp = *headref;
    FILE *fp;
    fp = fopen("y3.txt", "w");
    if(fp == NULL)
        printf("not opened");
    while(temp != NULL)
    {
        fprintf(fp, "%d %d %s %s\n", temp->Employee_ID, temp->Asset_ID, temp->Tagging_Date, temp->Release_Date);
        temp = temp->next;
    }
    fclose(fp);
    remove("Asset_Tagging.txt");
    rename("y3.txt", "Asset_Tagging.txt");
    printf("SAVED");
}

void postTaggedAsset(int E_ID, int A_ID, struct Asset **headref1, struct Asset_Requests **headref2)
{
    struct Asset *temp = malloc(sizeof(struct Asset));
    temp = *headref1;
    while (temp != NULL && temp->Asset_ID != A_ID)
    {
        temp = temp->next;
    }
    strcpy(temp->Tagging_Status, "Tagged");
    temp->Is_Active = 0;
    struct Asset_Requests *tmp;
    tmp = *headref2;
    while (tmp != NULL && tmp->Employee_ID != E_ID)
    {
        tmp = tmp->next;
    }
    tmp->Request_Status = 0;
}

void postdeTaggedAsset(int E_ID, struct Asset **headref)
{
    struct Asset *temp = malloc(sizeof(struct Asset));
    temp = *headref;
    while (temp != NULL && temp->Asset_ID != E_ID)
    {
        temp = temp->next;
    }
    strcpy(temp->Tagging_Status, "Free");
    temp->Is_Active = 1;
}

//END OF ADMIN LIBRARIES::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//View Only Tagged Assets
void Tagged_Asset(struct Employee_Asset_Tagging **headref)
{
    struct Employee_Asset_Tagging *p;
    p = *headref;
    printf("\n");
    int count = 0;
   
    while(p != NULL) 
    {
        if(strcmp(p->Release_Date, "NULL") == 0)
        {
            count++;
            printf("%d-%d-%s-%s\n", p->Employee_ID, p->Asset_ID, p->Tagging_Date, p->Release_Date);
        }
        p = p->next;
    }
    printf("\n%d",count);
}

//EMPLOYEE LIBRARIES::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void emplib(char username[])
{
    int op, E_ID, size;
    FILE *fp;
    char device[15], ch;
    struct Employee_Asset_Tagging *Tagging_head = NULL;
    fp = fopen("Asset_Tagging.txt", "r");
    if (fp != NULL) 
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        if(size != 0)
        {
            BuildTaggingList(&Tagging_head);
        }
    }
    struct Asset_Requests *Request_head = NULL;
    fp = fopen("Asset_Requests.txt", "r");
    if (fp != NULL) 
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        if(size != 0)
        {
            BuildAssetRequest(&Request_head);
        }
    }
    l5:
    do {
        printf("\nEmployee MENU\n\t1) View Tagged Asset\n\t2) Request an Asset\n\t3) View the Asset Request Status\n\t4) Log out\n Enter your option: ");
        scanf("%d", &op);
        switch(op)
        {
            case 1:
                printf("\n");
                fp = fopen("Asset_Tagging.txt", "r");
                if (fp != NULL) 
                {
                    fseek (fp, 0, SEEK_END);
                    size = ftell(fp);
                    fclose(fp);
                    if(size == 0)
                    {
                        printf("\nAsset Not Allocated\nPress enter to Continue...");
                        ch = getchar();
                        ch = getchar();
                        goto l5;
                    }
                }
                printf("\nEnter your Employee ID: ");
                scanf("%d", &E_ID);
                if(E_ID == atoi(username))
                {
                    viewTagged(E_ID, &Tagging_head);
                }
                else
                    printf("\nIncorrect Employee ID");
                break;
            case 2:
                printf("\nEnter your Employee ID: ");
                scanf("%d", &E_ID);
                printf("\nWhich Asset do you want to request: ");
                scanf("%s", device);
                if(E_ID == atoi(username))
                {
                    fp = fopen("Asset_Requests.txt", "r");
                    if (fp != NULL) 
                    {
                        fseek (fp, 0, SEEK_END);
                        size = ftell(fp);
                        fclose(fp);
                        if(size != 0)
                        {
                            if(RequestAsset(E_ID, device, &Request_head))
                                SaveChangesRequest(&Request_head);
                            else
                                printf("\nwent off");
                        }
                        else
                        {
                            time_t t = time(NULL);
                            struct tm date = *gmtime(&t);
                            fp = fopen("Asset_Requests.txt", "w");
                            fprintf(fp, "%d 1 %d-%02d-%02d %s 1\n", E_ID, date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, device);
                            fclose(fp);
                            BuildAssetRequest(&Request_head);
                        }
                    }
                }
                else
                    printf("\nIncorrect Employee ID");
                break;
            case 3:
                printf("\n");
                fp = fopen("Asset_Requests.txt", "r");
                if (fp != NULL) 
                {
                    fseek (fp, 0, SEEK_END);
                    size = ftell(fp);
                    fclose(fp);
                    if(size == 0)
                    {
                        printf("\nRequest Asset First\nPress enter to Continue...");
                        ch = getchar();
                        ch = getchar();
                        goto l5;
                    }
                }
                printf("\nEnter your Enployee ID");
                scanf("%d", &E_ID);
                if(E_ID == atoi(username))
                    viewAssetStatus(E_ID, &Request_head);
                else
                    printf("\nIncorrect Employee ID");
                break;
            case 4:
                Logout(username);
                break;
        }
    }while(op != 4);
}

void BuildTaggingList(struct Employee_Asset_Tagging **headref)
{
    struct Employee_Asset_Tagging *newnode, *temp;
    temp = *headref;
    FILE *fp;
    fp = fopen("Asset_Tagging.txt", "r");
    if(fp == NULL)
        printf("Asset_Tagging.txt file not opened");
    else
    {
        while(!feof(fp))
        {
            if(*headref == NULL)
            {
                newnode = malloc(sizeof(struct Employee_Asset_Tagging));
                fscanf(fp, "%d %d %s %s\n", &newnode->Employee_ID, &newnode->Asset_ID, newnode->Tagging_Date, newnode->Release_Date);
                newnode->next = *headref;
                *headref = newnode;
            }
            else
            {
                temp = *headref;
                newnode = malloc(sizeof(struct Employee_Asset_Tagging));
                while (temp->next != NULL)  
                {  
                    temp = temp->next;  
                }  
                fscanf(fp, "%d %d %s %s\n", &newnode->Employee_ID, &newnode->Asset_ID, newnode->Tagging_Date, newnode->Release_Date);
                if(newnode->Employee_ID == 0)
                {
                    free(newnode);
                    continue;
                }
                temp->next = newnode;  
                newnode->next = NULL;  
            }
        }
    }
    fclose(fp);
}
void displayTaggingList(struct Employee_Asset_Tagging **headref)
{
    struct Employee_Asset_Tagging *p;
    p = *headref;
    printf("\n");
    int count = 0;
   
    while(p != NULL) 
    {
        count++;
        printf("%d-%d-%s-%s\n", p->Employee_ID, p->Asset_ID, p->Tagging_Date, p->Release_Date);
        p = p->next;
    }
    printf("\n%d",count);
}
void viewTagged(int E_ID, struct Employee_Asset_Tagging **headref)
{
    struct Employee_Asset_Tagging *temp;
    temp = *headref;
    int found = 1, i = 1;
    while (temp != NULL)
    {
        if(temp->Employee_ID == E_ID)
        {
            found = 0;
            printf("\n%d: %d",i, temp->Asset_ID);
            i++;
            temp = temp -> next;
        }
        else
            temp = temp -> next;
    }
    if(found)
        printf("\nNot Found");
}
void BuildAssetRequest(struct Asset_Requests **headref)
{
    struct Asset_Requests *newnode, *temp;
    temp = *headref;
    FILE *fp;
    fp = fopen("Asset_Requests.txt", "r");
    if(fp == NULL)
        printf("Asset_Requests.txt file not opened");
    else
    {
        while(!feof(fp))
        {
            if(*headref == NULL)
            {
                newnode = malloc(sizeof(struct Asset_Requests));
                fscanf(fp, "%d %d %s %s %d\n", &newnode->Employee_ID, &newnode->Request_ID, newnode->Request_Date, newnode->Device, &newnode->Request_Status);
                newnode->next = *headref;
                *headref = newnode;
            }
            else
            {
                temp = *headref;
                newnode = malloc(sizeof(struct Asset_Requests));
                while (temp->next != NULL)  
                {  
                    temp = temp->next;
                }  
                fscanf(fp, "%d %d %s %s %d\n", &newnode->Employee_ID, &newnode->Request_ID, newnode->Request_Date, newnode->Device, &newnode->Request_Status);
                if(newnode->Employee_ID == 0)
                {
                    free(newnode);
                    continue;
                }
                temp->next = newnode;  
                newnode->next = NULL;  
            }
        }
    }
    fclose(fp);
}
void displayAssetRequest(struct Asset_Requests **headref)
{
    struct Asset_Requests *p;
    p = *headref;
    printf("\n");
    int count = 0;
   
    while(p != NULL) 
    {
        count++;
        printf("%d-%d-%s-%s-%d\n", p->Employee_ID, p->Request_ID, p->Request_Date, p->Device, p->Request_Status);
        p = p->next;
    }
    printf("\n%d",count);
}
int RequestAsset(int E_ID, char device[], struct Asset_Requests **headref)
{
    struct Asset_Requests *temp, *newnode;
    time_t t = time(NULL);
    struct tm date = *gmtime(&t);
    newnode = malloc(sizeof(struct Asset_Requests));
    if(newnode == NULL)
        return 0;
    temp = *headref;
    while(temp->next != NULL)
        temp = temp->next;
    newnode->Request_ID = temp->Request_ID + 1;
    newnode->Employee_ID = E_ID;
    strcpy(newnode->Device, device);
    sprintf(newnode->Request_Date, "%d-%02d-%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
    newnode->Request_Status = 1;
    newnode->next = NULL;
    temp->next = newnode;
    if(temp->next && newnode->next == NULL)
        return 1;
    else
        return 0;
}
void SaveChangesRequest(struct Asset_Requests **headref)
{
    struct Asset_Requests *temp;
    if(*headref == NULL)
        return;
    temp = *headref;
    FILE *fp;
    fp = fopen("y2.txt", "w");
    if(fp == NULL)
        printf("not opened");
    while(temp != NULL)
    {
        fprintf(fp, "%d %d %s %s %d\n", temp->Employee_ID, temp->Request_ID, temp->Request_Date, temp->Device, temp->Request_Status);
        temp = temp->next;
    }
    fclose(fp);
    remove("Asset_Requests.txt");
    rename("y2.txt", "Asset_Requests.txt");
    printf("SAVED");
}
void viewAssetStatus(int E_ID, struct Asset_Requests **headref)
{
    struct Asset_Requests *temp;
    temp = *headref;
    int found = 1;
    while (temp != NULL)
    {
        if(temp->Employee_ID == E_ID)
        {
            found = 0;
            if(temp->Request_Status == 1)
                printf("\nAsset Request Status still under process...");
            else
                printf("\nAsset has been alloted...");
            return ;
        }
        else
            temp = temp -> next;
    }
    if(found)
        printf("\nNot Found");
}

//END OF EMPLOYEE LIBRARIES:::::::::::::::::::::::::::::::::::::::::::::::::::::