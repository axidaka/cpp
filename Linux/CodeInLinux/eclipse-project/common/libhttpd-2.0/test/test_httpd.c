/*
** Copyright (c) 2002  Hughes Technologies Pty Ltd.  All rights
** reserved.
**
** Terms under which this software may be used or copied are
** provided in the  specific license associated with this product.
**
** Hughes Technologies disclaims all warranties with regard to this
** software, including all implied warranties of merchantability and
** fitness, in no event shall Hughes Technologies be liable for any
** special, indirect or consequential damages or any damages whatsoever
** resulting from loss of use, data or profits, whether in an action of
** contract, negligence or other tortious action, arising out of or in
** connection with the use or performance of this software.
**
**
** $Id: test_httpd.c,v 1.17 2011/02/24 06:13:04 bambi Exp $
**
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#ifdef _WIN32
#  include <getopt.h>
#else
#  include <sys/time.h>
#endif

#include "httpd.h"

typedef struct {
	httpd	*server;
	httpReq	*request;
} arg_t;


/*
** This is a static page of HTML.  It is loaded into the content
** tree using httpdAddStaticContent( ).
*/
#define test1_html "<HTML><BODY>This is just a test</BODY>"

#define ember_code1 "printf(\"This is from ember in process %d\n\",getpid());"
#define ember_code "load \"/usr/local/nk/www/content.lib\"; dispatchWebPage(\"/index.html\");"

/*
** Below are 2 dynamic pages, each generated by a C function.  The first
** is a simple page that offers a little dynamic info (the process ID)
** and the setups up a test link and a simple form.
** 
** The second page processes the form.  As you can see, you can access
** the form data from within your C code by accessing the symbol table
** using httpdGetVariableByName() (and other similar functions).  You
** can also include variables in the string passed to httpdOutput( ) and
** they will be expanded automatically.
*/
void index_html(server, request)
	httpd	*server;
	httpReq	*request;
{
	httpdPrintf(server, request,
	    "Welcome to the httpd server running in process number %d<P>\n",
	    getpid());
	httpdPrintf(server, request,
	    "Click <A HREF=/test1.html>here</A> to view a test page<P>\n");
	httpdPrintf(server, request,
	    "Click <A HREF=/login.html>here</A> to authenticate<P>\n");
	httpdPrintf(server, request,
	    "Or <A HREF=/wildcard/foo>here</A> for a test wildcard page<P>\n");
	httpdPrintf(server, request,
	    "<P><FORM ACTION=test2.html METHOD=POST>\n");
	httpdPrintf(server, request,
	    "Enter your name <INPUT NAME=name SIZE=10>\n");
	httpdPrintf(server, request,
	    "<INPUT TYPE=SUBMIT VALUE=Click!><P></FORM>\n");
	return;
}

void test2_html(server, request)
	httpd	*server;
	httpReq	*request;
{
	httpVar	*variable;

	/*
	** Grab the symbol table entry to see if the variable exists
	*/
	variable = httpdGetVariableByName(server, request, "name");
	if (variable == NULL)
	{
		httpdPrintf(server,request,"Missing form data!");
		return;
	}

	/*
	** Use httpdOutput() rather than httpdPrintf() so that the variable
	** embedded in the text is expanded automatically
	*/
	httpdOutput(server, request, "Hello ${name}");
}

void test3_html(server, request)
	httpd	*server;
	httpReq	*request;
{
	char	*path;

	path = httpdRequestPath(request);
	httpdOutput(server,request,"Wilcard dynamic request received<P>");
	httpdPrintf(server,request,"The requested path was %s<P>", path);
}


void login_html(server, request)
	httpd	*server;
	httpReq	*request;
{
	if (httpdAuthenticate(server, request, "LibHTTPD Test") == 0)
		return;
	httpdPrintf(server, request, "Your username is '%s'<P>\n",
		request->authUser);
	httpdPrintf(server, request, "Your password is '%s'<P>\n",
 		request->authPassword);
	httpdOutput(server, request,
		"Click <A HREF=login2.html>here</A> to force reauthentication");
	httpdOutput(server, request,
		".  Use a username = test password = 123");
}



void login2_html(server, request)
	httpd	*server;
	httpReq	*request;
{
	if (httpdAuthenticate(server, request, "LibHTTPD Test") == 0)
	{
		httpdOutput(server, request, "Authentication failure(1).");
		return;
	}
	if (strcmp(request->authUser, "test") != 0 ||
	    strcmp(request->authPassword, "123") != 0)
	{
		httpdForceAuthenticate(server, request, "LibHTTPD Test");
		httpdOutput(server, request, "Authentication failure (2).");
		return;
	}
	httpdOutput(server, request, "Your login was accepted.");
}

void myError404Handler(server, request, error)
	httpd	*server;
	httpReq	*request;
	int	error;
{
	httpdOutput(server, request,
	    "Whoa there.  You hit a page that doesn't exist! <P><BR><BR>\n");
	httpdOutput(server, request,
	    "Or in other words : <B>Error 404</B>\n\n");
}


void *handleRequest(arg)
	void	*arg;
{
	arg_t	*data;

	data = (arg_t*)arg;
	pthread_t self;

	self = pthread_self();
	printf("Processing request in thread '%d'\n", (int)self);
	httpdProcessRequest(data->server, data->request);
	httpdEndRequest(data->server, data->request);
	return(NULL);
}


int main(argc, argv)
	int	argc;
	char	*argv[];
{
	httpd	*server;
	httpReq	*request;
	char 	*host;
	int 	port,
		errFlag,
		result;
	extern char *optarg;
	extern int optind, opterr, optopt;
	int c;
	struct	timeval timeout;
	arg_t	data;
	pthread_t thread;

	host = NULL;
	port = 8000;
	errFlag = 0;
	while ( (c=getopt(argc,argv,"h:p:")) != -1 )
	{
		switch ( c ) 
		{
			case 'h':
				host=optarg;
				break;

			case 'p':
				port = atoi(optarg);
				break;
			default:
				errFlag++;
		}
	}
	if (errFlag)
	{
		fprintf(stderr,"usage: [-h <host IP>] [ -p <port >]\n");
		fprintf(stderr,"\nLibHTTPD version %s\n\n",LIBHTTPD_VERSION);
		exit(1);
	}


	/*
	** Ensure that PIPE signals are either handled or ignored.
	** If a client connection breaks while the server is using
	** it then the application will be sent a SIGPIPE.  If you
	** don't handle it then it'll terminate your application.
	*/
	signal(SIGPIPE, SIG_IGN);


	/*
	** Create a server and setup our logging
	*/
	server = httpdCreate(host,port);
	if (server == NULL)
	{
		perror("Can't create server");
		exit(1);
	}
	httpdSetAccessLog(server, stdout);
	httpdSetErrorLog(server, stdout);

	/*
	** We are fussy and don't want the default Error 404 page
	*/
	httpdSetErrorFunction(server,404, myError404Handler);


	/*
	** Setup some content for the server
	*/
	httpdAddCContent(server,"/", "index.html", HTTP_TRUE, 
		NULL, index_html);
	httpdAddCContent(server,"/", "test2.html", HTTP_FALSE, 
		NULL, test2_html);
	httpdAddCContent(server,"/", "login.html", HTTP_FALSE,
		NULL, login_html);
	httpdAddCContent(server,"/", "login2.html", HTTP_FALSE,
		NULL, login2_html);
	httpdAddCWildcardContent(server,"/wildcard", NULL, test3_html);
	httpdAddStaticContent(server, "/", "test1.html", HTTP_FALSE,
		NULL, test1_html);
#ifdef HAVE_EMBER
	httpdAddEmberContect(server,"/","ember.html", HTTP_FALSE, 
		NULL, ember_code);
#endif

	/*
	** Go into our service loop
	*/

	while(1 == 1)
	{
		/*
		** Linux modifies the timouet value during the
		** select call so we must set it everyt ime.  Most
		** other UNIX implementations do not modify timeout
		** but it doesn't hurt to set it each time anyway
		*/
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		request = httpdReadRequest(server, &timeout, &result);
		if (request == NULL && result == 0)
		{
			printf("Timeout ... \n");
			continue;
		}
		if (result < 0)
		{
			printf("Error ... \n");
			continue;
		}
		data.server = server;
		data.request = request;
		pthread_create(&thread, NULL, handleRequest, (void*)&data);
		/*
		httpdProcessRequest(server, request);
		httpdEndRequest(server, request);
		*/
	}
}
