#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <conio.h>
#include <unistd.h>
#include "../include/sims.h"


void systemInit(pUSER_ACCOUNT *userList, pSTUDENT *stuList, char *str1, char *str2)
{
    FILE *fp1 = fopen(str1, "r+");
    if (NULL == fp1)
    {
        perror("fopen");
        return;
    }
    FILE *fp2 = fopen(str2, "r+");
    if (NULL == fp2)
    {
        perror("fopen1");
        return;
    }
    USER_ACCOUNT *user_head = (pUSER_ACCOUNT) calloc(1, sizeof(USER_ACCOUNT));
    STUDENT *stu_head = (pSTUDENT) calloc(1, sizeof(STUDENT));
    USER_ACCOUNT *puser = NULL, *user_tail = user_head, tmp_user;
    STUDENT *pstu = NULL, *stu_tail = stu_head, *pre = NULL, *pcur = NULL, tmp_stu;
    STUDENT *pvisit = NULL;
    int i;
    *userList = user_head;
    *stuList = stu_head;
    memset(&tmp_user, 0, sizeof(USER_ACCOUNT));
    memset(&tmp_stu, 0, sizeof(STUDENT));
    if (fp1 != NULL)
    {
        fseek(fp1, 0, SEEK_SET);
        while ((fscanf(fp1, "%s%s%d", &tmp_user.user_name, &tmp_user.user_password, &tmp_user.users_limit)) != EOF)
        {
            puser = (pUSER_ACCOUNT) calloc(1, sizeof(USER_ACCOUNT));
            strcpy(puser->user_name, tmp_user.user_name);
            strcpy(puser->user_password, tmp_user.user_password);
            puser->users_limit = tmp_user.users_limit;
            if (NULL == user_head->next)
            {
                puser->next = user_head->next;
                user_head->next = puser;
                user_tail = puser;
            } else
            {
                user_tail->next = puser;
                user_tail = puser;
            }
        }
        fclose(fp1);
    } else
    {
        perror("fopen:userAccount.txt");
    }
    if (fp2 != NULL)
    {
        while ((fscanf(fp2, "%d%s", &tmp_stu.stu_id, &tmp_stu.stu_name)) != EOF)
        {
            pstu = (pSTUDENT) calloc(1, sizeof(STUDENT));
            pstu->stu_id = tmp_stu.stu_id;
            strcpy(pstu->stu_name, tmp_stu.stu_name);
            for (i = 0; i < COURSE_NUM; i++)
            {
                fscanf(fp2, "%d%lf", &pstu->course[i].course_id, &pstu->course[i].course_score);
            }
            if (NULL == stu_head->next)
            {
                pstu->next = stu_head->next;
                stu_head->next = pstu;
                stu_tail = pstu;
            } else
            {
                pre = stu_head;
                pcur = stu_head->next;
                while (NULL != pcur)
                {
                    if (pcur->stu_id > pstu->stu_id)
                    {
                        pstu->next = pcur;
                        pre->next = pstu;
                        break;
                    } else
                    {
                        pcur = pcur->next;
                        pre = pre->next;
                    }
                }
                if (NULL == pcur)
                {
                    stu_tail->next = pstu;
                    stu_tail = pstu;
                }
            }
        }
        fclose(fp2);
    } else
        perror("fopen:student.txt");
}

void interfaceInit()
{
    printf("welcome to SimShell\n");
    printf("\nPlease select your account level:\n\n");
    printf("[0] You are a teacher\n");
    printf("[1] You are a student\n");
    printf("[2] Exit\n\n");
}

//void getPassword(char *password)
//{
//	char *tmp;
//	tmp = getpass("Password:");
//	strcpy(password,tmp);
//}


void getPassword(char *password)
{
    int index = 0;
    char ch;
    while ((ch = getchar()) != '\r')
    {
        if (ch != '\b')
        {
            printf("*");
            password[index++] = ch;
        } else
        {
            printf("\b \b");
            index--;
        }
        password[index] = '\0';
    }
}

int checkUserAccount(pUSER_ACCOUNT userList, USER_ACCOUNT user)
{
    while (userList->next != NULL)
    {
        if (!strcmp(userList->next->user_name, user.user_name) &&
            !strcmp(userList->next->user_password, user.user_password))
        {
            return 1;
        }
        userList->next = userList->next->next;
    }
    return 0;
}

void adminAccount(pUSER_ACCOUNT *userList, pSTUDENT *stuList, char *str1, char *str2)
{
    int button;
    while (1)
    {
        system("clear");
        adminInterfaceInit();
        printf("\n");
        printf("select a number:");
        scanf("%d", &button);
        if (button > 9 || button < 0)
        {
            printf("Input Error!\nPress any key to select again\n");
            printf("\n");;
        }
        switch (button)
        {
            case 1:
                searchStudentInformation(*stuList, ADMIN);
                break;
            case 2:
                addStudentInformation(stuList, str2);
                break;
            case 3:
                updateStudentInformation(stuList, str2);
                break;
            case 4:
                deleteStudentInformation(stuList, str2);
                break;
            case 5:
                searchUserAccount(*userList);
                break;
            case 6:
                addUserAccount(userList, str1);
                break;
            case 7:
                updateUserAccount(userList, str1);
                break;
            case 8:
                deleteUserAccount(userList, str1);
                break;
            case 9:
                systemExit();
                break;
        }

    }
}

void guestAccount(pSTUDENT stuList)
{
    int button;
    while (1)
    {
        system("clear");
        guestInterfaceInit();
        printf("select a number:");
        scanf("%d", &button);
        switch (button)
        {
            case 1:
                searchStudentInformation(stuList, GUEST);
                break;
            case 2:
                systemExit();
                break;
            default:
                printf("\n");;
                printf("Input Error!\nPress any key to select again\n");
                char tmp[2];
                scanf("%s", &tmp);

        }
    }
}

void adminInterfaceInit()
{
    printf("[1] %-8s student information\n", "search");
    printf("[2] %-8s student information\n", "add");
    printf("[3] %-8s student information\n", "update");
    printf("[4] %-8s student information\n", "delete");
    printf("[5] %-8s user acount\n", "search");
    printf("[6] %-8s user acount\n", "add");
    printf("[7] %-8s user acount\n", "update");
    printf("[8] %-8s user acount\n", "delete");
    printf("[9] exit\n\n");
}

void guestInterfaceInit()
{
    printf("[1] search student information\n");
    printf("[2] exit\n");
}

void searchStudentInformation(pSTUDENT stuList, int limit)
{
    int button;
    while (1)
    {
        searchInit(limit);
        printf("select a number:");
        scanf("%d", &button);
        switch (button)
        {
            case 1:
                searchById(stuList);
                break;
            case 2:
                searchByName(stuList);
                break;
            case 3:
                return;
                break;
            case 0:
                if (limit == ADMIN)
                {
                    searchAll(stuList);
                }
                break;
            default:
                printf("\n");;
                printf("Input Error!\nPress any key to select again\n");
                char tmp[2];
                scanf("%s", &tmp);
        }
    }
}

void searchInit(int limit)
{
    system("clear");
    if (limit == ADMIN)
    {
        printf("[0] search all\n");
    }
    printf("[1] search by ID\n");
    printf("[2] search by name\n");
    printf("[3] return\n\n");

}

void searchById(pSTUDENT stuList)
{
    pSTUDENT p = stuList->next;
    int id, i;
    int flag = 0;
    system("clear");
    printf("Input ID:");
    scanf("%d", &id);
    while (p != NULL)
    {
        if (p->stu_id == id)
        {
            printf("%4d  %-8s	", p->stu_id, p->stu_name);
            for (i = 0; i < COURSE_NUM; i++)
            {
                printf("%5d  %-7.2f", p->course[i].course_id, p->course[i].course_score);
            }
            printf("\n");
            flag = 1;
            break;
        } else
        {
            p = p->next;
        }
    }
    if (0 == flag)
    {
        printf("Not found\n");
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", tmp);
}

void addStudentInformation(pSTUDENT *stuList, char *str2)
{
    pSTUDENT p = (pSTUDENT) calloc(1, sizeof(STUDENT));
    pSTUDENT pre = NULL, pcur = NULL;
    int flag, i;
    while (1)
    {
        flag = 1;
        system("clear");
        printf("Please input a student's information to add.\n");
        printf("The fomation is [id name] and %d course information [CourseId CourseScore]\n", COURSE_NUM);
        printf("\n");;
        scanf("%d%s", &p->stu_id, &p->stu_name);
        for (i = 0; i < COURSE_NUM; i++)
        {
            scanf("%d%lf", &p->course[i].course_id, &p->course[i].course_score);
        }
        if (NULL == (*stuList)->next)
        {
            p->next = (*stuList)->next;
            (*stuList)->next = p;
        } else
        {
            pre = (*stuList);
            pcur = (*stuList)->next;
            while (NULL != pcur)
            {
                if (pcur->stu_id > p->stu_id)
                {
                    p->next = pcur;
                    pre->next = p;
                    break;
                } else if (pcur->stu_id == p->stu_id)
                {
                    flag = 0;
                    break;
                } else
                {
                    pcur = pcur->next;
                    pre = pre->next;
                }
            }
            if (NULL == pcur)
            {
                pre->next = p;
            }
        }
        if (flag)
        {
            printf("Insert success!\n");
            updateStudentDatabase(stuList, str2);
        } else
        {
            printf("Error!\nThis student's ID has existed in database.\n");
        }
        printf("\nPress 1 to continue add,or any number else to return:");
        int tmp;
        scanf("%d", &tmp);
        if (tmp != 1)
        {
            break;
        }
    }
}

void updateStudentDatabase(pSTUDENT *stuList, char *str2)
{
    FILE *fp = fopen(str2, "w");
    pSTUDENT p = (*stuList)->next;
    int i;
    if (NULL != fp)
    {
        while (NULL != p)
        {
            fprintf(fp, "%4d  %-10s	", p->stu_id, p->stu_name);
            for (i = 0; i < COURSE_NUM; i++)
            {
                fprintf(fp, "%5d  %-7.2f", p->course[i].course_id, p->course[i].course_score);
            }
            fprintf(fp, "\n");
            p = p->next;
        }
        fclose(fp);
    } else
    {
        perror("Update student.txt failed");
    }
}

void updateStudentInformation(pSTUDENT *stuList, char *str2)
{
    pSTUDENT p = (*stuList)->next;
    int flag = 0, id, i;
    system("clear");
    printf("Input ID:");
    scanf("%d", &id);
    while (p != NULL)
    {
        if (p->stu_id == id)
        {
            printf("Please update the student's information.\n");
            printf("The fomation is [ID Name] and %d course information [CourseId CourseScore]\n", COURSE_NUM);
            scanf("%d%s", &p->stu_id, p->stu_name);
            for (i = 0; i < COURSE_NUM; i++)
                scanf("%d%lf", &p->course[i].course_id, &p->course[i].course_score);
            flag = 1;
            break;
        } else
        {
            p = p->next;
        }
    }
    if (0 == flag)
    {
        printf("Not found\n");
    } else
    {
        printf("Update Success!\n");
        updateStudentDatabase(stuList, str2);
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void deleteStudentInformation(pSTUDENT *stuList, char *str2)
{
    pSTUDENT pre = *stuList;
    pSTUDENT pcur = (*stuList)->next;
    int id, flag = 0;
    system("clear");
    printf("Input the deleted ID:");
    scanf("%d", &id);
    while (NULL != pcur)
    {
        if (pcur->stu_id == id)
        {
            pre->next = pcur->next;
            free(pcur);
            pcur = NULL;
            flag = 1;
            break;
        } else
        {
            pcur = pcur->next;
            pre = pre->next;
        }
    }
    if (0 == flag)
    {
        printf("Not found\n");
    } else
    {
        printf("Delete Success!\n");
        updateStudentDatabase(stuList, str2);
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void searchUserAccount(pUSER_ACCOUNT userList)
{
    int button;
    while (1)
    {
        searchAccountInit();
        printf("select a number:");
        scanf("%d", &button);
        switch (button)
        {
            case 0:
                searchAllUser(userList);
                break;
            case 1:
                searchByUserLevel(userList);
                break;
            case 2:
                searchByUsername(userList);
                break;
            case 3:
                return;
                break;
            default:
                printf("\n");;
                printf("Input Error!\nPress any key to select again\n");
                char tmp[2];
                scanf("%s", &tmp);
        }
    }
}

void searchAccountInit()
{
    system("clear");
    printf("[0] search all\n");
    printf("[1] search by level\n");
    printf("[2] search by name\n");
    printf("[3] return\n\n");
}

void searchAllUser(pUSER_ACCOUNT userList)
{
    pUSER_ACCOUNT p = userList->next;
    system("clear");
    while (NULL != p)
    {
        if (p->users_limit == ADMIN)
        {
            printf("Username:%-15s Password:%-10s Userlevel:%s\n", p->user_name, p->user_password, "ADMIN");
            p = p->next;
        } else
        {
            printf("Username:%-15s Password:%-10s Userlevel:%s\n", p->user_name, p->user_password, "GUEST");
            p = p->next;
        }
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void searchByUserLevel(pUSER_ACCOUNT userList)
{
    pUSER_ACCOUNT p = userList;
    int i;
    system("clear");
    printf("Input level:  [0:admin,1:guest]\n");
    scanf("%d", &i);
    if (i != 0 && i != 1)
    {
        system("clear");
        printf("Input level:  [0:admin,1:guest]\n");
    } else
    {
        p = userList->next;
        while (NULL != p)
        {
            if (p->users_limit == i && i == 0)
            {
                printf("Username:%-15s Password:%-10s Userlevel:%s\n", p->user_name, p->user_password, "ADMIN");
                p = p->next;
            } else if (p->users_limit == i && i == 1)
            {
                printf("Username:%-15s Password:%-10s Userlevel:%s\n", p->user_name, p->user_password, "GUEST");
                p = p->next;
            } else
            {
                p = p->next;
            }
        }
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void searchByUsername(pUSER_ACCOUNT userList)
{
    pUSER_ACCOUNT p = userList->next;
    char name[USER_NAME_LEN + 1] = "";
    int i, flag = 0;
    system("clear");
    printf("Input name:");
    scanf("%s", name);
    while (NULL != p)
    {
        if (strcmp(p->user_name, name) == 0)
        {
            if (p->users_limit == 0)
            {
                printf("Username:%-15s Password:%-10s Userlevel:%s\n", p->user_name, p->user_password, "ADMIN");
                flag = 1;
                break;
            } else
            {
                printf("Username:%-15s Password:%-10s Userlevel:%s\n", p->user_name, p->user_password, "GUEST");
                flag = 1;
                break;
            }
        } else
        {
            p = p->next;
        }
    }
    if (0 == flag)
    {
        printf("Not found\n");
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void addUserAccount(pUSER_ACCOUNT *userList, char *str1)
{
    pUSER_ACCOUNT p = (pUSER_ACCOUNT) calloc(1, sizeof(USER_ACCOUNT));
    pUSER_ACCOUNT pcur = (*userList)->next;
    int i;
    while (1)
    {
        system("clear");
        printf("Please input a user's information to add.\n");
        printf("The fomation is [Username] [Password] [Level]\n");
        printf("The level rank: [0:admin, 1:guest]\n");
        printf("\n");;
        scanf("%s%s%d", &p->user_name, &p->user_password, &p->users_limit);
        while (NULL != pcur)
        {
            if (strcmp(pcur->user_name, p->user_name) == 0)
            {
                printf("Error!\nThis user has existed in database.\n");
                goto SELECT;
            }
            pcur = pcur->next;
        }
        p->next = (*userList)->next;
        (*userList)->next = p;
        printf("add user account information success!\n ");
        SELECT:
        printf("\nPress 1 to continue add,or any number else to return:");
        int tmp;
        scanf("%d", &tmp);
        if (tmp != 1)
            break;
    }
    updateUserDatabase(userList, str1);
}

void updateUserAccount(pUSER_ACCOUNT *userList, char *str1)
{
    pUSER_ACCOUNT p = (*userList)->next;
    char name[USER_NAME_LEN + 1] = "";
    int i, flag = 0;
    system("clear");
    printf("Input name:");
    scanf("%s", name);
    while (NULL != p)
    {
        if (strcmp(p->user_name, name) == 0)
        {
            printf("Please update the user's information.\n\n");
            printf("The fomation is [Username] [Password] [Level]\n");
            printf("The level rank: [0:admin, 1:guest]\n");
            flag = 1;
            printf("\n");;
            scanf("%s%s%d", &p->user_name, &p->user_password, &p->users_limit);
            break;
        } else
        {
            p = p->next;
        }
    }
    if (0 == flag)
    {
        printf("Not found\n");
    } else
    {
        printf("Update success!\n");
        updateUserDatabase(userList, str1);
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void updateUserDatabase(pUSER_ACCOUNT *userList, char *str1)
{
    FILE *fp = fopen(str1, "w");
    pUSER_ACCOUNT p = (*userList)->next;
    if (NULL != fp)
    {
        while (NULL != p)
        {
            fprintf(fp, "%-15s %-10s %d\n", p->user_name, p->user_password, p->users_limit);
            p = p->next;
        }
    } else
    {
        perror("Update userAccount.txt failed");
    }
    fclose(fp);
}

void deleteUserAccount(pUSER_ACCOUNT *userList, char *str1)
{
    pUSER_ACCOUNT pre = (*userList);
    pUSER_ACCOUNT pcur = (*userList)->next;
    char name[USER_NAME_LEN + 1];
    int i, flag = 0;
    system("clear");
    printf("Input name:");
    scanf("%s", name);
    while (NULL != pcur)
    {
        if (strcmp(pcur->user_name, name) == 0)
        {
            pre->next = pcur->next;
            free(pre);
            pre = NULL;
            flag = 1;
            break;
        } else
        {
            pcur = pcur->next;
            pre = pre->next;
        }
    }
    if (0 == flag)
    {
        printf("Not found\n");
    } else
    {
        printf("Delete success!\n");
        updateUserDatabase(userList, str1);
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void searchByName(pSTUDENT stuList)
{
    pSTUDENT p = stuList->next;
    int i;
    char name[USER_NAME_LEN + 1];
    int flag = 0;
    system("clear");
    printf("Input name:");
    scanf("%s", name);
    while (NULL != p)
    {
        if (strcmp(p->stu_name, name) == 0)
        {
            printf("%4d  %-8s	", p->stu_id, p->stu_name);
            for (i = 0; i < COURSE_NUM; i++)
            {
                printf("%5d %-8.2f", p->course[i].course_id, p->course[i].course_score);
            }
            printf("\n");
            flag = 1;
            break;
        } else
        {
            p = p->next;
        }
    }
    if (0 == flag)
    {
        printf("Not found\n");
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void searchAll(pSTUDENT stuList)
{
    pSTUDENT p = stuList->next;
    int i;
    system("clear");
    printf("\n");
    while (NULL != p)
    {
        printf("%4d	%-8s	", p->stu_id, p->stu_name);
        for (i = 0; i < COURSE_NUM; i++)
        {
            printf("%5d	%-8.2f", p->course[i].course_id, p->course[i].course_score);
        }
        printf("\n");
        p = p->next;
    }
    printf("\nPress any key to continue\n");
    char tmp[2];
    scanf("%s", &tmp);
}

void systemExit()
{
    system("clear");
    printf("\n");;
    printf("Thank you for you use !\n");
    exit(1);
}
