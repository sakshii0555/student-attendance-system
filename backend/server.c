#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define PORT 8080

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[8192];

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    listen(server_socket, 3);

    printf("Attendance Server running on http://localhost:%d\n", PORT);

    c = sizeof(struct sockaddr_in);

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {

        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);

        /* ---------- VIEW ATTENDANCE ---------- */
        if (strncmp(buffer, "GET /view", 9) == 0) {

            FILE *fp = fopen("attendance.txt", "r");
            char data[4096] = "";

            if (fp != NULL) {
                fread(data, 1, sizeof(data), fp);
                fclose(fp);
            } else {
                strcpy(data, "No attendance records found.");
            }

            char response[5000];
            sprintf(response,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Access-Control-Allow-Origin: *\r\n\r\n"
                "%s", data);

            send(client_socket, response, strlen(response), 0);
        }

        /* ---------- MARK ATTENDANCE ---------- */
        else if (strncmp(buffer, "POST", 4) == 0) {

            char *body = strstr(buffer, "\r\n\r\n");
            if (body != NULL) {
                body += 4;
                FILE *fp = fopen("attendance.txt", "a");
                fprintf(fp, "%s\n", body);
                fclose(fp);
            }

            char response[] =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Access-Control-Allow-Origin: *\r\n\r\n"
                "Attendance marked successfully!";

            send(client_socket, response, strlen(response), 0);
        }

        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
