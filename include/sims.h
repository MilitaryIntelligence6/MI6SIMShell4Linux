#define USER_NAME_LEN 20
#define USER_PASSWORD_LEN 8
#define COURSE_NUM 3


enum
{
    ADMIN, GUEST
};

typedef struct user
{
    char user_name[USER_NAME_LEN + 1];
    char user_password[USER_PASSWORD_LEN + 1];
    int users_limit;
    struct user *next;
} USER_ACCOUNT, *pUSER_ACCOUNT;

typedef struct course
{
    int course_id;
    double course_score;
} COURSE, *pCOURSE;

typedef struct student
{
    int stu_id;
    char stu_name[USER_NAME_LEN + 1];
    COURSE course[COURSE_NUM];
    struct student *next;
} STUDENT, *pSTUDENT;

void systemInit(pUSER_ACCOUNT *userList, pSTUDENT *stuList, char *str1, char *str2);

void interfaceInit();

void getPassword(char *password);

int checkUserAccount(pUSER_ACCOUNT, USER_ACCOUNT);

void adminAccount(pUSER_ACCOUNT *, pSTUDENT *, char *, char *);

void guestAccount(pSTUDENT);

void adminInterfaceInit();

void searchStudentInformation(pSTUDENT, int);

void searchInit(int);

void searchById(pSTUDENT);

void addStudentInformation(pSTUDENT *, char *);

void updateStudentDatabase(pSTUDENT *, char *);

void updateStudentInformation(pSTUDENT *, char *);

void deleteStudentInformation(pSTUDENT *, char *);

void searchUserAccount(pUSER_ACCOUNT);

void searchAccountInit();

void searchAllUser(pUSER_ACCOUNT);

void searchByUserLevel(pUSER_ACCOUNT);

void searchByUsername(pUSER_ACCOUNT);

void addUserAccount(pUSER_ACCOUNT *, char *);

void updateUserAccount(pUSER_ACCOUNT *, char *);

void updateUserDatabase(pUSER_ACCOUNT *, char *);

void deleteUserAccount(pUSER_ACCOUNT *, char *);

void guestInterfaceInit();

void searchByName(pSTUDENT);

void searchAll(pSTUDENT);

void systemExit();
