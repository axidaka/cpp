#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

// ���ؿͻ��˳��� ����Ҫ��������Э��ջ������Ҫ������������У��͡�ά����ź�Ӧ��ȣ�ֻ�ǽ�Ӧ�ò����ݴ�һ�����̿�������һ������

#define  DOMAIN_SERVER_NAME "sever_socket"

int main()
{
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char ch = 'A';

	//AF_UNIX����ͬһ̨�����ϵĽ��̼�ͨ��, ʹ�ñ����ļ��� �����ڹܵ�
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	// ���ݷ�������������׽�������
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, DOMAIN_SERVER_NAME);
	len = sizeof(address);
	
	// ���ӷ�����
	result = connect(sockfd, (struct sockaddr*)&address, len);
	if (-1 == result)
	{
		perror("domain_client oops: clinet");
		exit(EXIT_FAILURE);
	}

	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("domain_client char from server = %c\n", ch);
	close(sockfd);
	exit(EXIT_SUCCESS);
}