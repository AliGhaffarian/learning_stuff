#include <dlfcn.h>

int main (){
	char libc_filename[] = "libc.so";
	void *handle = dlopen(libc_filename, RTLD_LAZY);
	void (*addr)(int, char*, int) = dlsym(handle, "write");
	addr(1, "hello!", 6);
}
