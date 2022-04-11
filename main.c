#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>

struct Candidate
{
    int id;
    int age;
    char name[100];
    int numberOfVotes;
    char party[100];
};

struct Voter
{
    int id;
    int age;
    char name[100];
    int voteCasted;
};

int isAdmin = 0;
char candidateFileName[] = "candidates.csv";
char votersFileName[] = "voters.csv";
char metaFileName[] = "election.meta";
char *delimiter = ",";

//BACKEND_BEGIN
struct Voter *ReadCSVVoters(char filename[], char *delimiter, int lines)
{
    FILE *file = fopen(filename, "r");
    const unsigned int skip_header = 1;
    int count = lines;
    if (ferror(file))
    {
        perror("Error : ");
        return NULL;
    }
    else
    {
        fseek(file, 0, SEEK_SET);
        struct Voter *voter = (struct Voter *)malloc((count + 1) * sizeof(struct Voter));
        if (voter == NULL)
        {
            perror("Error : ");
            return NULL;
        }
        unsigned int row = 0;
        unsigned int i = 0;
        while (!feof(file))
        {
            int buffer_size = 4096;
            char buffer[buffer_size];
            fgets(buffer, buffer_size, file);
            if (row >= skip_header && strlen(buffer) > 0)
            {
                char *token = strtok(buffer, delimiter);
                int pos = -1;
                while (token != NULL)
                {
                    switch (pos)
                    {
                    case -1:
                        voter[i].id = atoi(token);
                        break;
                    case 0:
                        voter[i].age = atoi(token);
                        break;
                    case 1:
                        strcpy(voter[i].name, token);
                        break;
                    case 2:
                        token[strcspn(token,"\n")] = 0;
                        voter[i].voteCasted = atoi(token);
                        break;
                    default:
                        break;
                    }
                    token = strtok(NULL, delimiter);
                    pos++;
                }
                i++;
            }
            row += 1;
        }
        fclose(file);
        return voter;
    }
}

struct Candidate *ReadCSVCandidates(char filename[], char *delimiter, int lines)
{
    FILE *file = fopen(filename, "r");
    const unsigned int skip_header = 1;
    int count = lines;
    if (ferror(file))
    {
        perror("Error : ");
        return NULL;
    }
    else
    {
        fseek(file, 0, SEEK_SET);
        struct Candidate *voter = (struct Candidate *)malloc(
            (count + 1) * sizeof(struct Candidate));
        if (voter == NULL)
        {
            perror("Error : ");
            return NULL;
        }
        unsigned int row = 0;
        unsigned int i = 0;
        while (!feof(file))
        {
            int buffer_size = 4096;
            char buffer[buffer_size];
            fgets(buffer, buffer_size, file);
            if (row >= skip_header && strlen(buffer) > 0)
            {
                char *token = strtok(buffer, delimiter);
                int pos = -1;
                while (token != NULL)
                {
                    switch (pos)
                    {
                    case -1:
                        voter[i].id = atoi(token);
                        break;
                    case 0:
                        voter[i].age = atoi(token);
                        break;
                    case 1:
                        strcpy(voter[i].name, token);
                        break;
                    case 2:
                        voter[i].numberOfVotes = atoi(token);
                        break;
                    case 3:
                        token[strcspn(token,"\n")] = 0;
                        strcpy(voter[i].party, token);
                        break;
                    default:
                        break;
                    }
                    token = strtok(NULL, delimiter);
                    pos++;
                }
                i++;
            }
            row += 1;
        }
        fclose(file);
        return voter;
    }
}

int GetNumberOfLines(char filename[])
{
    FILE *f = fopen(filename, "r");
    int count = 0;
    while (!feof(f))
    {
        for (int c = getc(f); c != EOF; c = getc(f))
        {
            if (c == '\n')
                count++;
        }
    }
    fclose(f);
    return count;
}

void DisplayDataVoters(struct Voter *v, int lines)
{
    printf("=============Voters==============\n");
    printf("%s \t%s \t%-2s\n", "ID", "Age", "Name");
    printf("================================\n");
    for (int i = 0; i < lines; i++)
    {
        printf("%2d \t%2d \t%-2s\n", v[i].id, v[i].age,
               v[i].name);
    }
    printf("=================================\n");
}

void DisplayDataCandidates(struct Candidate *v, int lines)
{
    printf("====================Candidates====================\n");
    printf("%s \t%s \t%-2s \t%-2s\n", "ID", "Age", "Name", "Party");
    printf("==================================================\n");
    for (int i = 0; i < lines; i++)
    {
        printf("%2d \t%2d \t%-2s \t%-2s\n", v[i].id, v[i].age,
               v[i].name, v[i].party);
    }
    printf("==================================================\n");
}

struct Voter *AddNewValueVoter(struct Voter *voters, struct Voter v, int *lines)
{
    int l = *lines;
    int i = 0;
    struct Voter *new = realloc(voters, (l + 2) * sizeof(struct Voter));
    if (new == NULL)
    {
        free(voters);
        perror("Error : ");
    }
    else
    {
        new[l].age = v.age;
        new[l].id = v.id;
        new[l].voteCasted = v.voteCasted;
        strcpy(new[l].name, v.name);
        l += 1;
        *lines = l;
        return new;
    }
}

struct Candidate *AddNewValueCandidate(struct Candidate *c,
                                       struct Candidate v, int *lines)
{
    int l = *lines;
    int i = 0;
    struct Candidate *new = realloc(c, (l + 2) * sizeof(struct Candidate));
    if (new == NULL)
    {
        free(c);
        perror("Error : ");
    }
    else
    {
        new[l].age = v.age;
        new[l].id = v.id;
        new[l].numberOfVotes = v.numberOfVotes;
        strcpy(new[l].name, v.name);
        strcpy(new[l].party, v.party);
        l += 1;
        *lines = l;
        return new;
    }
}

void CastVote(int idVoter, int idCandidate, struct Voter *v, struct Candidate *c, int linesVoters, int linesCandidates)
{
    for (int j = 0; j < linesVoters; j++)
    {
        if (v[j].id == idVoter)
        {
            for (int i = 0; i < linesCandidates; i++)
            {
                printf("ID : %d\n", c[i].id);
                if (c[i].id == idCandidate)
                {
                    c[i].numberOfVotes++;
                    v[j].voteCasted = 1;
                    printf("Vote casted by %s to %s of %s\n",
                           v[j].name, c[i].name, c[i].party);
                    return;
                }
            }
        }
    }

    printf("Error : No such candidate exists!!\n");
}

void ShowResults(struct Candidate *c, int lines)
{
    printf("============Results============\n");
    printf("%s \t%-2s \t%-2s\n", "Name", "Party", "Votes");
    for (int i = 0; i < lines; i++)
    {
        printf("%s \t%-2s \t%-2d\n", c[i].name,
               c[i].party, c[i].numberOfVotes);
    }
    printf("===============================\n");
}

void SaveValuesVoters(char filename[], struct Voter *data, int lines)
{
    FILE *f = fopen(filename, "w");
    if (!f)
    {
        perror("Error ");
        printf("Cannot save file %s\n", filename);
        return;
    }
    fprintf(f, "id,age,name,voted\n");
    for (int i = 0; i < lines; i++)
    {
        //printf("%s\n",data[i].uname);
        if (i == lines - 1)
        {
            fprintf(f, "%d,%d,%s,%d", data[i].id, data[i].age, data[i].name,data[i].voteCasted);
            continue;
        }
        fprintf(f, "%d,%d,%s,%d\n", data[i].id, data[i].age, data[i].name,data[i].voteCasted);
    }
    printf("File %s saved successfully\n", filename);
    fclose(f);
}

void SaveValuesCandidates(char filename[], struct Candidate *data, int lines)
{
    FILE *f = fopen(filename, "w");
    if (!f)
    {
        perror("Error ");
        printf("Cannot save file %s\n", filename);
        return;
    }
    fprintf(f, "id,age,name,votes,party\n");
    for (int i = 0; i < lines; i++)
    {
        //printf("%s\n",data[i].uname);
        if (i == lines - 1)
        {
            fprintf(f, "%d,%d,%s,%d,%s", data[i].id, data[i].age, data[i].name,
                    data[i].numberOfVotes, data[i].party);
            continue;
        }
        fprintf(f, "%d,%d,%s,%d,%s\n", data[i].id, data[i].age, data[i].name,
                data[i].numberOfVotes, data[i].party);
    }
    printf("File %s saved successfully\n", filename);
    fclose(f);
}

struct Voter *DeleteDataVoter(struct Voter *v, int *lines, int id)
{
    int l = *lines;
    struct Voter *new = v;
    for (int i = 0; i < l; i++)
    {
        if (v[i].id == id)
        {
            for (int j = i; j < l - 1; j++)
            {
                new[j] = new[j + 1];
            }
            l -= 1;
            new = realloc(new, (l + 1) * sizeof(struct Voter));
            *lines = l;
            return new;
        }
    }
    printf("No such entry exists!!\n");
    return v;
}

struct Candidate *DeleteDataCandidate(struct Candidate *v, int *lines, int id)
{
    int l = *lines;
    struct Candidate *new = v;
    for (int i = 0; i < l; i++)
    {
        if (v[i].id == id)
        {
            for (int j = i; j < l - 1; j++)
            {
                new[j] = new[j + 1];
            }
            l -= 1;
            new = realloc(new, (l + 1) * sizeof(struct Voter));
            *lines = l;
            return new;
        }
    }
    printf("No such entry exists!!\n");
    return v;
}

int fileExists(char fname[])
{
    FILE *f = fopen(fname, "r");
    if (f != NULL)
    {
        fclose(f);
        return 1;
    }
    return 0;
}

struct Voter ReturnLoggedInVoter(int id,
                                 struct Voter *v, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        if (v[i].id == id)
        {
            return v[i];
        }
    }
    struct Voter temp;
    temp.id = -1;
    return temp;
}

struct Candidate ReturnLoggedInCandidate(int id,
                                         struct Candidate *v, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        if (v[i].id == id)
        {
            return v[i];
        }
    }
    struct Candidate temp;
    temp.id = -1;
    return temp;
}

void SaveMetaFile(char fname[], int status)
{
    FILE *f = fopen(fname, "w");
    if (!f)
    {
        perror("Error ");
        printf("Cannot save file %s\n", fname);
        return;
    }
    fprintf(f, "%d", status);
    printf("File %s saved successfully\n", fname);
    fclose(f);
}
//BACKEND_END
void AdminUI(struct Voter *v, struct Candidate *c,
             int *lv, int *lc, int *estat)
{
    char uname[50], passwd[50];
    int trialsLeft = 3;
    printf("Welcome admin!!\n");
    printf("Please enter Admin Username and Password to authenticate\n");
    do
    {
        fflush(stdin);
        printf("Username : ");
        fgets(uname, 50, stdin);
        uname[strcspn(uname, "\n")] = '\0';
        printf("Password : ");
        fgets(passwd, 50, stdin);
        passwd[strcspn(passwd, "\n")] = '\0';
        if (strcmp(uname, "admin") == 0 && strcmp(passwd, "admin") == 0)
        {
            printf("Access granted(admin)\n");
            isAdmin = 1;
        }
        else
        {
            trialsLeft--;
            printf("Access denied(admin), You have %d chances left\n", trialsLeft);
        }
    } while (trialsLeft > 0 && isAdmin == 0);
    if (trialsLeft <= 0)
    {
        return;
    }
    printf("WARNING : You have admin access now, you can create and delete accounts\nUSE YOUR POWERS WISELY\n");
    trialsLeft = 3;
    int choice = 0;
    char temp[100];
    do
    {
        if (*lv == 0)
        {
            printf("WARNING : Voter list is empty, add new voters!!");
        }
        if (*lc == 0)
        {
            printf("WARNING : Candidate list is empty, add new candidates!!");
        }
        printf("Options :-\n");
        printf("1> Display Voters\n2> Display Candidates\n3> Add Voter\n4> Add Candidate\n");
        printf("5> Delete Candidate\n6> Delete Voter\n7> Save Changes\n8> Start Election\n9> Exit");
        printf("\nEnter your choice here(%d chances left after wrong input) : ", trialsLeft);
        scanf("%d", &choice);
        int id;
        switch (choice)
        {
        case 1:
            DisplayDataVoters(v, *lv);
            break;
        case 2:
            DisplayDataCandidates(c, *lc);
            break;
        case 3:
            if ((*estat) == 1)
            {
                printf("Error : New data cannot be entered while elections are going on\n");
                continue;
            }
            printf("Enter details of new voter\n");
            struct Voter newVoter;
            newVoter.id = *lv + 1;
            newVoter.voteCasted = 0;
            printf("Age : ");
            scanf("%d", &newVoter.age);
            if (newVoter.age < 18)
            {
                printf("Error : Ages below 18 are not allowed!!\n");
                continue;
            }
            printf("Name : ");
            fflush(stdin);
            fgets(temp, 100, stdin);
            temp[strcspn(temp, "\n")] = '\0';
            strcpy(newVoter.name, temp);
            v = AddNewValueVoter(v, newVoter, lv);
            break;
        case 4:
            if (*estat == 1)
            {
                printf("Error : New data cannot be entered while elections are going on\n");
                continue;
            }
            printf("Enter details of new candidate\n");
            struct Candidate newc;
            newc.id = *lc + 1;
            newc.numberOfVotes = 0;
            printf("Age : ");
            scanf("%d", &newc.age);
            fflush(stdin);
            printf("Name : ");

            fgets(temp, 100, stdin);
            temp[strcspn(temp, "\n")] = '\0';
            strcpy(newc.name, temp);
            printf("Username : ");

            fgets(temp, 100, stdin);
            temp[strcspn(temp, "\n")] = '\0';
            strcpy(newc.party, temp);
            printf("Password : ");

            c = AddNewValueCandidate(c, newc, lc);
            break;
        case 5:
            if (*estat == 1)
            {
                printf("Error : New data cannot be entered while elections are going on\n");
                continue;
            }
            printf("Enter id of candidate to delete : ");
            scanf("%d", &id);
            fflush(stdin);
            c = DeleteDataCandidate(c, lc, id);
            break;
        case 6:
            if (*estat == 1)
            {
                printf("Error : New data cannot be entered while elections are going on\n");
                continue;
            }
            printf("Enter id of d to delete : ");
            scanf("%d", &id);
            fflush(stdin);
            v = DeleteDataVoter(v, lv, id);
            break;
        case 7:
            printf("Which data do you want to save?\n");
            printf("1> Voter's\n");
            printf("2> Candidate's\n");
            printf("3> Both\n");
            int chc;
            scanf("%d", &chc);
            fflush(stdin);
            switch (chc)
            {
            case 1:
                SaveValuesVoters(votersFileName, v, *lv);
                break;
            case 2:
                SaveValuesCandidates(candidateFileName, c, *lc);
                break;
            case 3:
                SaveValuesVoters(votersFileName, v, *lv);
                SaveValuesCandidates(candidateFileName, c, *lc);
            default:
                printf("Wrong input!!");
                break;
            }
            break;
        case 8:
            printf("Elections started...\nNo change to data is possible now!!\n");
            printf("Select this option to stop elections\n");
            *estat = *estat == 1 ? 0 : 1;
            break;
        case 9:
            printf("Exiting to main menu...\n");
            break;
        default:
            trialsLeft--;
            printf("Wrong Input, %d chances left", trialsLeft);
            break;
        }
    } while (choice != 9);
}

void VoterUI(struct Voter *v, struct Candidate *c,
             int *lv, int *lc, int *estat)
{
    char uname[50], passwd[50];
    int trialsLeft = 3;
    int currId = -1;
    int loggedIn = 0;
    int voted = 0;
    struct Voter cv;
    printf("Welcome voter!!\n");
    printf("Please enter your ID number : ");
    int inputId;
    scanf("%d", &inputId);
    fflush(stdin);
    cv = ReturnLoggedInVoter(inputId, v, *lv);
    if (cv.id == -1)
    {
        printf("Access denied(voter), returning to main menu...\n");
        return;
    }
    else
    {
        printf("Scanning Adhar Card....\n");
        //sleep(2);
        printf("Access granted(voter)\n");
        loggedIn = 1;
        currId = cv.id;
        voted = cv.voteCasted;
    }
    printf("Welcome %s!!\n", cv.name);
    printf("Election Status : %s", *estat == 1 ? "Ongoing\n" : "Stopped\n");
    trialsLeft = 3;
    int choice = -1;
    char temp[100];
    do
    {
        printf("Options:-\n");
        printf("1> Vote\n2> Display Candidates\n");
        printf("3> Exit\n");
        printf("Enter your choice here(%d chances left after wrong input) : ", trialsLeft);
        scanf("%d", &choice);
        //fflush(stdin);
        switch (choice)
        {
        case 1:
            if (*estat == 0)
            {
                printf("Elections are not open yet...\n");
                continue;
            }
            if (voted == 1)
            {
                printf("You've already voted...\n");
                continue;
            }
            DisplayDataCandidates(c, *lc);
            printf("Enter Id of candidate you want to vote for : ");
            int id;
            scanf("%d", &id);
            fflush(stdin);
            CastVote(currId, id, v, c, *lv, *lc);
            voted = 1;
            break;
        case 2:
            DisplayDataCandidates(c, *lc);
            break;
        case 3:
            printf("Exiting...\n");
            break;
        default:
            trialsLeft--;
            printf("Wrong Input, %d chances left\n", trialsLeft);
            break;
        }
    } while (choice < 3 && trialsLeft > 0);
}

int main()
{
    int role = 0;
    int electionBegin = 0;
    struct Voter *voter;
    struct Candidate *candidate;
    printf("Loading data...\n");
    if (!fileExists(metaFileName))
    {
        printf("WARNING : Reqired file %s not found, creating...\n", metaFileName);
        FILE *f = fopen(metaFileName, "w");
        if (f == NULL)
        {
            printf("ERROR : Unable to create file, exiting...\n");
            return 1;
        }
        fprintf(f, "%d", electionBegin);
        fclose(f);
        printf("Done...\n");
    }
    else
    {
        FILE *f = fopen(metaFileName, "r");
        char temp[2];
        fgets(temp, 2, f);
        electionBegin = atoi(temp);
        fclose(f);
    }
    printf("%d\n", electionBegin);
    if (!fileExists(votersFileName))
    {
        printf("WARNING : Reqired file %s not found, creating...\n", votersFileName);
        FILE *f = fopen(votersFileName, "w");
        if (f == NULL)
        {
            printf("ERROR : Unable to create file, exiting...\n");
            return 1;
        }
        fprintf(f, "id,age,name,voted,username,password");
        fclose(f);
        printf("Done...\n");
    }
    if (!fileExists(candidateFileName))
    {
        printf("WARNING : Reqired file %s not found, creating...\n", candidateFileName);
        FILE *f = fopen(candidateFileName, "w");
        if (f == NULL)
        {
            printf("ERROR : Unable to create file, exiting...\n");
            return 1;
        }
        fprintf(f, "id,age,name,votes,party,username,password");
        fclose(f);
        printf("Done...\n");
    }
    int linesVoters = GetNumberOfLines(votersFileName);
    int linesCandidates = GetNumberOfLines(candidateFileName);
    voter = ReadCSVVoters(votersFileName, delimiter, linesVoters);
    candidate = ReadCSVCandidates(candidateFileName, delimiter, linesCandidates);
    printf("Welcome to Election Management System!!\n");
    DisplayDataVoters(voter,linesVoters);
    printf("There are a lot of features and each type (admin,voter) have different menus\n");
    do
    {
        printf("Choose your role :- \n");
        printf("1 > Admin\n2> Voter\n3> Show Results\n4> Exit\n");
        printf("Select number before role(wrong input will terminate program) : ");
        scanf("%d", &role);
        switch (role)
        {
        case 1:
            AdminUI(voter, candidate, &linesVoters, &linesCandidates, &electionBegin);
            break;
        case 2:
            VoterUI(voter, candidate, &linesVoters, &linesCandidates, &electionBegin);
            break;
        case 3:
            ShowResults(candidate, linesCandidates);
            break;
        case 4:
            printf("Exiting...\n");
            break;
        default:
            printf("Wrong input, exiting...\n");
            break;
        }
    } while (role < 4 && role > 0);

    fflush(stdin);
    printf("Press any key to exit...\n");
    getch();
    SaveMetaFile(metaFileName, electionBegin);
    SaveValuesVoters(votersFileName, voter, linesVoters);
    SaveValuesCandidates(candidateFileName, candidate, linesCandidates);
    return 0;
}