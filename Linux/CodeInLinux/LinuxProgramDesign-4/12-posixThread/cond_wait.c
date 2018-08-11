#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct node
{
	int n_number;
	struct node* p_next;
};

struct node* head = NULL;

/*释放节点内存*/
static void cleanup_handler(void*arg) {
	printf("Clean up handler of second thread.\n");
	
	if (arg)
	{
		printf("cleanup_handler: %d.....\n", ((struct node*)arg)->n_number);
		free(arg);
	}
	(void)pthread_mutex_unlock(&mtx);
}

void *thread_func(void* arg)
{
	struct node* p = NULL;
	pthread_cleanup_push(cleanup_handler, p);

	pthread_mutex_lock(&mtx);
	//这个mutex_lock主要是用来保护wait等待临界时期的情况，
	//当在wait为放入队列时，这时，已经存在Head条件等待激活
	//的条件，此时可能会漏掉这种处理
	//这个while要特别说明一下，单个pthread_cond_wait功能很完善，
	//为何这里要有一个while(head==NULL)呢？因为pthread_cond_wait
	//里的线程可能会被意外唤醒，如果这个时候head==NULL，
	//则不是我们想要的情况。这个时候，
	//应该让线程继续进入pthread_cond_wait
	while (1)
	{
		while (!head)
		{
			pthread_cond_wait(&cond, &mtx);
		}

		//pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，
		//然后阻塞在等待队列休眠， 直到再次被唤醒
		//（大多数情况下是等待的条件成立而被唤醒，唤醒后，
		//该进程会先锁定mtx ， 流程如下 
		/*block-->unlock-->wait()return-->lock*/
		p = head;
		head = head->p_next;

		printf("Got: %d from font of queue..\n", p->n_number);
		free(p);
		p = NULL;
	}

	pthread_mutex_unlock(&mtx);
	pthread_cleanup_pop(0);
}

int main()
{
	pthread_t tid;
	int res;
	struct node* p;
	res = pthread_create(&tid, NULL, &thread_func, NULL);
	if (0 != res)
	{
		perror("pthread_create failed");
		exit(EXIT_FAILURE);
	}

	//子线程会一直等待资源，类似生产者和消费者，
	//但是这里的消费者可以是多个消费者，
	//而不仅仅支持普通的单个消费者，这个模型虽然简单，
	//但是很强大
	int i;
	for(i = 0; i < 10; i++)
	{
		p = (struct node*)malloc(sizeof(struct node));
		p->n_number = i;
		// 操作线程共享全局变量head临界资源前先加锁
		pthread_mutex_lock(&mtx);
		p->p_next = head;
		head = p;

		// 激发条件 ，cond_signal激发一个等待该条件的线程
		pthread_cond_signal(&cond);

		// 解锁
		pthread_mutex_unlock(&mtx);

		sleep(1);
	}

	printf("main thread want cancel thread_func...\n");
	//关于pthread_cancel，有一点额外的说明，它是从外部终止子线程，
	//子线程会在最近的取消点，退出线程，而在我们的代码里，最近的
	//取消点肯定就是pthread_cond_wait()了
	pthread_cancel(tid);
	pthread_join(tid, NULL);
	printf("All done....exiting...\n");
	exit(EXIT_FAILURE);
}