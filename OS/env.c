/* prints envp
 */
#include <stdio.h>
int main(int argc, char **argv, char **envp){
		char **curr = envp;
		while (*curr){
				printf("%s\n", *curr++);
		}
}
