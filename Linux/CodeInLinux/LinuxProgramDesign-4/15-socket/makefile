ALL_PROGRAM= domain_client domain_server get_hostinfo

all: $(ALL_PROGRAM)

# 所有的.o文件都是依赖于对应的.c文件
.c.o:
	gcc -c $<

domain_client.o: domain_client.c
domain_server.o: domain_server.c
get_hostinfo.o: get_hostinfo.c

domain_client: domain_client.o
	gcc -o domain_client domain_client.o

domain_server: domain_server.o
	gcc -o domain_server domain_server.o

get_hostinfo: get_hostinfo.o
	gcc -o get_hostinfo get_hostinfo.o
	
cleanall: cleanobj 
	rm -rf $(ALL_PROGRAM)

cleanobj:
	rm -Rf *.o