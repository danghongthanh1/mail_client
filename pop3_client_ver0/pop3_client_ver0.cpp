#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>	
#include<string>
#include "stdafx.h"
#include <winsock2.h>
#include "ws2tcpip.h"


char username[] = "tester1@bksoft.net";
char password[] = "ke2vJZgjwW";

int computemail(char *buffer)
{
	int mailnum;
	char *tok = strtok(buffer, " ");

	tok = strtok(NULL, " ");

	mailnum = atoi(tok);
	return(mailnum);

}

int main(int argc, char argv[])
{
	char buf[256];
	char sendBuf[256];
	int ret = 0;
	int count = 0;
	int totmail = 0;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;

	addrinfo *res;
	ret = getaddrinfo("mail.bksoft.net", "http", NULL, &res);
	if (ret == 0)
	{
		memcpy(&addr, res->ai_addr, res->ai_addrlen);
	}
	else
	{
		printf("Khong phan giai duoc ten mien");
		return 1;
	}

	addr.sin_family = AF_INET;

	addr.sin_port = htons(110);

	connect(client, (SOCKADDR *)&addr, sizeof(addr));
	ret = recv(client, buf, sizeof(buf), 0);

	if (ret < sizeof(buf))
		buf[ret] = 0;

	printf("%s\n", buf);

	strcpy(sendBuf, "");
	strcpy(buf, "");
	sprintf(sendBuf, "USER %s\n\0", username);
	send(client, sendBuf, strlen(sendBuf), 0);

	ret = recv(client, buf, sizeof(buf), 0);

	if (ret < sizeof(buf))
		buf[ret] = 0;

	printf("%s\n", buf);

	strcpy(sendBuf, "");
	strcpy(buf, "");
	sprintf(sendBuf, "PASS %s\n\0", password);
	send(client, sendBuf, strlen(sendBuf), 0);

	ret = recv(client, buf, sizeof(buf), 0);

	if (ret < sizeof(buf))
		buf[ret] = 0;

	printf("%s\n", buf);

	strcpy(sendBuf, "");
	strcpy(buf, "");
	sprintf(sendBuf, "STAT\n\0");
	send(client, sendBuf, strlen(sendBuf), 0);

	ret = recv(client, buf, sizeof(buf), 0);

	if (ret < sizeof(buf))
		buf[ret] = 0;

	printf("%s\n", buf);
	totmail = computemail(buf);
	for (int count = 1; count <= totmail; count++)
	{
		strcpy(sendBuf, "");
		sprintf(sendBuf, "%s%d", username, count);
		FILE *f = fopen("C:\\Users\\nfhgghjh\\source\\repos\\POP3\\BTL\\out.txt", "w+");

		strcpy(sendBuf, "");
		strcpy(buf, "");
		sprintf(sendBuf, "RETR %d\n\0\n\0", count);
		send(client, sendBuf, strlen(sendBuf), 0);

		ret = recv(client, buf, sizeof(buf), 0);

		if (ret < sizeof(buf))
			buf[ret] = 0;

		printf("%s\n", buf);
		while (1)
		{
			strcpy(buf, "");
			ret = recv(client, buf, sizeof(buf), 0);

			if (ret < sizeof(buf))
				buf[ret] = 0;

			printf("%s\n", buf);

			fprintf(f, "%s\n", buf);
		}
		fclose(f);
	}

	strcpy(sendBuf, "");
	strcpy(buf, "");
	sprintf(sendBuf, "QUIT\n\0");
	send(client, sendBuf, strlen(sendBuf), 0);

	ret = recv(client, buf, sizeof(buf), 0);

	if (ret < sizeof(buf))
		buf[ret] = 0;

	printf("%s\n", buf);

	closesocket(client);
	WSACleanup();
	return 0;
}