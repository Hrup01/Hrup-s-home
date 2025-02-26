#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 定义用户和房间结构体 */
typedef struct {
    char username[50];
    char password[50];
    char role[20]; /* 角色（admin, member, guest） */
    double balance; /* 余额（仅顾客需要） */
} User;

typedef struct {
    int roomId;       /* 房间编号 */
    char type[50];     /* 房间类型 */
    double price;      /* 价格 */
    int isBooked;      /* 是否被预订（0: 未预订, 1: 已预订） */
} Room;

/* 全局变量 */
User users[100];
int userCount = 0;

Room rooms[100];
int roomCount = 0;

User *currentUser = NULL;

/* 函数声明 */
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
            printf("1. 登录\n2. 注册\n3. 退出\n");
            printf("请输入选择: ");
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

/* 添加用户 */
void addUser(User user) {
    if (userCount < 100) {
        users[userCount++] = user;
    } else {
        printf("用户数量已达上限！\n");
    }
}

/* 删除用户 */
void deleteUser(char *username) {
    int i, j;
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            for (j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            printf("用户删除成功！\n");
            return;
        }
    }
    printf("未找到该用户！\n");
}

/* 查找用户 */
User* findUser(char *username) {
    int i;
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

/* 添加房间 */
void addRoom(Room room) {
    if (roomCount < 100) {
        rooms[roomCount++] = room;
    } else {
        printf("房间数量已达上限！\n");
    }
}

/* 删除房间 */
void deleteRoom(int roomId) {
    int i, j;
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomId == roomId) {
            for (j = i; j < roomCount - 1; j++) {
                rooms[j] = rooms[j + 1];
            }
            roomCount--;
            printf("房间删除成功！\n");
            return;
        }
    }
    printf("未找到该房间！\n");
}

/* 查找房间 */
Room* findRoom(int roomId) {
    int i;
    for (i = 0; i < roomCount; i++) {
        if (rooms[i].roomId == roomId) {
            return &rooms[i];
        }
    }
    return NULL;
}

/* 保存用户数据到文件 */
void saveUsers() {
    FILE *file;
    int i;
    file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("无法保存用户数据！\n");
        return;
    }
    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s,%s,%s,%.2f\n", users[i].username, users[i].password, users[i].role, users[i].balance);
    }
    fclose(file);
}

/* 从文件加载用户数据 */
void loadUsers() {
    FILE *file;
    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("未找到用户数据文件！\n");
        return;
    }
    while (fscanf(file, "%[^,],%[^,],%[^,],%lf\n", users[userCount].username, users[userCount].password, users[userCount].role, &users[userCount].balance) != EOF) {
        userCount++;
    }
    fclose(file);
}

/* 保存房间数据到文件 */
void saveRooms() {
    FILE *file;
    int i;
    file = fopen("rooms.txt", "w");
    if (file == NULL) {
        printf("无法保存房间数据！\n");
        return;
    }
    for (i = 0; i < roomCount; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n", rooms[i].roomId, rooms[i].type, rooms[i].price, rooms[i].isBooked);
    }
    fclose(file);
}

/* 从文件加载房间数据 */
void loadRooms() {
    FILE *file;
    file = fopen("rooms.txt", "r");
    if (file == NULL) {
        printf("未找到房间数据文件！\n");
        return;
    }
    while (fscanf(file, "%d,%[^,],%lf,%d\n", &rooms[roomCount].roomId, rooms[roomCount].type, &rooms[roomCount].price, &rooms[roomCount].isBooked) != EOF) {
        roomCount++;
    }
    fclose(file);
}

/* 登录功能 */
void login() {
    char username[50], password[50];
    User *user;
    printf("用户名: ");
    scanf("%s", username);
    printf("密码: ");
    scanf("%s", password);

    user = findUser(username);
    if (user != NULL && strcmp(user->password, password) == 0) {
        currentUser = user;
        printf("登录成功！\n");
    } else {
        printf("用户名或密码错误！\n");
    }
}

/* 注册功能 */
void registerUser() {
    char username[50], password[50], role[20];
    User user;
    printf("用户名: ");
    scanf("%s", username);
    printf("密码: ");
    scanf("%s", password);
    printf("角色 (member/guest): ");
    scanf("%s", role);

    strcpy(user.username, username);
    strcpy(user.password, password);
    strcpy(user.role, role);
    user.balance = 0;

    addUser(user);
    printf("注册成功！\n");
}

/* 管理员菜单 */
void adminMenu() {
    int choice;
    while (1) {
        printf("1. 添加房间\n2. 删除房间\n3. 查询所有房间\n4. 退出\n");
        printf("请输入选择: ");
        scanf("%d", &choice);
        if (choice == 1) {
            Room room;
            printf("请输入房间编号: ");
            scanf("%d", &room.roomId);
            printf("请输入房间类型: ");
            scanf("%s", room.type);
            printf("请输入房间价格: ");
            scanf("%lf", &room.price);
            room.isBooked = 0;
            addRoom(room);
            printf("房间添加成功！\n");
        } else if (choice == 2) {
            int roomId;
            printf("请输入房间编号: ");
            scanf("%d", &roomId);
            deleteRoom(roomId);
        } else if (choice == 3) {
            int i;
            printf("房间列表:\n");
            for (i = 0; i < roomCount; i++) {
                printf("%d. 类型: %s, 价格: %.2f, 状态: %s\n", rooms[i].roomId, rooms[i].type, rooms[i].price, rooms[i].isBooked ? "已预订" : "未预订");
            }
        } else if (choice == 4) {
            currentUser = NULL;
            break;
        }
    }
}

/* 顾客菜单 */
void customerMenu() {
    int choice;
    while (1) {
        printf("1. 查询房间\n2. 预订房间\n3. 退出\n");
        printf("请输入选择: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int i;
            printf("房间列表:\n");
            for (i = 0; i < roomCount; i++) {
                printf("%d. 类型: %s, 价格: %.2f, 状态: %s\n", rooms[i].roomId, rooms[i].type, rooms[i].price, rooms[i].isBooked ? "已预订" : "未预订");
            }
        } else if (choice == 2) {
            int roomId;
            Room *room;
            printf("请输入房间编号: ");
            scanf("%d", &roomId);
            room = findRoom(roomId);
            if (room != NULL && !room->isBooked) {
                if (currentUser->balance >= room->price) {
                    room->isBooked = 1;
                    currentUser->balance -= room->price;
                    printf("预订成功！您的余额已扣减 %.2f 元。\n", room->price);
                } else {
                    printf("余额不足，无法预订！\n");
                }
            } else {
                printf("房间不存在或已被预订！\n");
            }
        } else if (choice == 3) {
            currentUser = NULL;
            break;
        }
    }
}
