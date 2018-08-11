/*
 * main.cpp
 *
 *  Created on: 2016年12月22日
 *      Author: root
 */

#include "use_pthread_key.h"

int main ()

{
    printf("#main #start\n");

    //use_pthread_key();
    use_pthread_key_class();
    printf("#main #end\n");
	return 0;

}


