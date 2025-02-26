#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* �����û��ͷ���ṹ�� */
typedef struct {
    char username[50];
    char password[50];
    char role[20]; /* ��ɫ��admin, member, guest�� */
    double balance; /* �����˿���Ҫ�� */
} User;

typedef struct {
    int roomId;       /* ������ */
    char type[50];     /* �������� */
    double price;      /* �۸� */
    int isBooked;      /* �Ƿ�Ԥ����0: δԤ��, 1: ��Ԥ���� */
} Room;

/* ȫ�ֱ��� */
User users[100];
int userCount = 0;

Room rooms[100];
int roomCount = 0;

User *currentUser = NULL;

/* �������� */
void addUser(User user);
void deleteUser(char *username);
User* findUser(char *username);
void addRoom(Room room);
void deleteRoom(int roomId);
Room* findRoom(int roomId);
void saveUsers();
void loadUsers();
void saveRooms();
void loadRooms();
void login();
void registerUser();
void adminMenu();
void customerMenu();

int main() {
    loadUsers();
    loadRooms();

    int choice;
    while (1) {
        if (currentUser == NULL) {
            printf("1. ��¼\n2. ע��\n3. �˳�\n");
            printf("������ѡ��: ");
            scanf("%d", &choice);
            if (choice == 1) login();
            else if (choice == 2) registerUser();
            else break;
        } else {
            if (strcmp(currentUser->role, "admin") == 0) {
                adminMenu();
            } else {
                customerMenu();
            }
        }
    }

    saveUsers();
    saveRooms();
    return 0;
}

/* ����û� */
void addUser(User user) {
    if (userCount < 100) {
        users[userCount++] = user;
    } else {
        printf("�û������Ѵ����ޣ�\n");
    }
}

/* ɾ���û� */
void deleteUser(char *username) {
    int i, j;
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            for (j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            printf("�û�ɾ���ɹ���\n");
            return;
        }
    }
    printf("δ�ҵ����û���\n");
}

/* �����û� */
User* findUser(char *username) {
    int i;
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

/* ��ӷ��� */
void addRoom(Room room) {
    if (roomCount < 100) {
        rooms[roomCount++] = room;
    } else {
        printf("���������Ѵ����ޣ�\n");
    }
}

/* ɾ������ */
void deleteRoom(int roomId) {
    int i, j;
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomId == roomId) {
            for (j = i; j < roomCount - 1; j++) {
                rooms[j] = rooms[j + 1];
            }
            roomCount--;
            printf("����ɾ���ɹ���\n");
            return;
        }
    }
    printf("δ�ҵ��÷��䣡\n");
}

/* ���ҷ��� */
Room* findRoom(int roomId) {
    int i;
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomId == roomId) {
            return &rooms[i];
        }
    }
    return NULL;
}

/* �����û����ݵ��ļ� */
void saveUsers() {
    FILE *file;
    int i;
    file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("�޷������û����ݣ�\n");
        return;
    }
    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s,%s,%s,%.2f\n", users[i].username, users[i].password, users[i].role, users[i].balance);
    }
    fclose(file);
}

/* ���ļ������û����� */
void loadUsers() {
    FILE *file;
    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("δ�ҵ��û������ļ���\n");
        return;
    }
    while (fscanf(file, "%[^,],%[^,],%[^,],%lf\n", users[userCount].username, users[userCount].password, users[userCount].role, &users[userCount].balance) != EOF) {
        userCount++;
    }
    fclose(file);
}

/* ���淿�����ݵ��ļ� */
void saveRooms() {
    FILE *file;
    int i;
    file = fopen("rooms.txt", "w");
    if (file == NULL) {
        printf("�޷����淿�����ݣ�\n");
        return;
    }
    for (i = 0; i < roomCount; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n", rooms[i].roomId, rooms[i].type, rooms[i].price, rooms[i].isBooked);
    }
    fclose(file);
}

/* ���ļ����ط������� */
void loadRooms() {
    FILE *file;
    file = fopen("rooms.txt", "r");
    if (file == NULL) {
        printf("δ�ҵ����������ļ���\n");
        return;
    }
    while (fscanf(file, "%d,%[^,],%lf,%d\n", &rooms[roomCount].roomId, rooms[roomCount].type, &rooms[roomCount].price, &rooms[roomCount].isBooked) != EOF) {
        roomCount++;
    }
    fclose(file);
}

/* ��¼���� */
void login() {
    char username[50], password[50];
    User *user;
    printf("�û���: ");
    scanf("%s", username);
    printf("����: ");
    scanf("%s", password);

    user = findUser(username);
    if (user != NULL && strcmp(user->password, password) == 0) {
        currentUser = user;
        printf("��¼�ɹ���\n");
    } else {
        printf("�û������������\n");
    }
}

/* ע�Ṧ�� */
void registerUser() {
    char username[50], password[50], role[20];
    User user;
    printf("�û���: ");
    scanf("%s", username);
    printf("����: ");
    scanf("%s", password);
    printf("��ɫ (member/guest): ");
    scanf("%s", role);

    strcpy(user.username, username);
    strcpy(user.password, password);
    strcpy(user.role, role);
    user.balance = 0;

    addUser(user);
    printf("ע��ɹ���\n");
}

/* ����Ա�˵� */
void adminMenu() {
    int choice;
    while (1) {
        printf("1. ��ӷ���\n2. ɾ������\n3. ��ѯ���з���\n4. �˳�\n");
        printf("������ѡ��: ");
        scanf("%d", &choice);
        if (choice == 1) {
            Room room;
            printf("�����뷿����: ");
            scanf("%d", &room.roomId);
            printf("�����뷿������: ");
            scanf("%s", room.type);
            printf("�����뷿��۸�: ");
            scanf("%lf", &room.price);
            room.isBooked = 0;
            addRoom(room);
            printf("������ӳɹ���\n");
        } else if (choice == 2) {
            int roomId;
            printf("�����뷿����: ");
            scanf("%d", &roomId);
            deleteRoom(roomId);
        } else if (choice == 3) {
            int i;
            printf("�����б�:\n");
            for (i = 0; i < roomCount; i++) {
                printf("%d. ����: %s, �۸�: %.2f, ״̬: %s\n", rooms[i].roomId, rooms[i].type, rooms[i].price, rooms[i].isBooked ? "��Ԥ��" : "δԤ��");
            }
        } else if (choice == 4) {
            currentUser = NULL;
            break;
        }
    }
}

/* �˿Ͳ˵� */
void customerMenu() {
    int choice;
    while (1) {
        printf("1. ��ѯ����\n2. Ԥ������\n3. �˳�\n");
        printf("������ѡ��: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int i;
            printf("�����б�:\n");
            for (i = 0; i < roomCount; i++) {
                printf("%d. ����: %s, �۸�: %.2f, ״̬: %s\n", rooms[i].roomId, rooms[i].type, rooms[i].price, rooms[i].isBooked ? "��Ԥ��" : "δԤ��");
            }
        } else if (choice == 2) {
            int roomId;
            Room *room;
            printf("�����뷿����: ");
            scanf("%d", &roomId);
            room = findRoom(roomId);
            if (room != NULL && !room->isBooked) {
                if (currentUser->balance >= room->price) {
                    room->isBooked = 1;
                    currentUser->balance -= room->price;
                    printf("Ԥ���ɹ�����������ѿۼ� %.2f Ԫ��\n", room->price);
                } else {
                    printf("���㣬�޷�Ԥ����\n");
                }
            } else {
                printf("���䲻���ڻ��ѱ�Ԥ����\n");
            }
        } else if (choice == 3) {
            currentUser = NULL;
            break;
        }
    }
}
