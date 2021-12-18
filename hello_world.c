#include <stdio.h>
#include <string.h>
#include <time.h>

extern void initialise_monitor_handles(void);

int main() {
	initialise_monitor_handles();
	
	int i;
	for(i=0;i<10;i++)
		printf("Hello World\n");
	
return 0;
}

void SystemInit() {
}	
