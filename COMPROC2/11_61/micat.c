#include <unistd.h>

int main(int argc, char **argv){
	char c;

	while(read(10, &c, sizeof(char))>0)
		write(20, &c, sizeof(char));

	return 0;
}
