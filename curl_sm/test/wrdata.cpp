#include <stdio.h>
#include "streambuffer.h"
#include <string>


int main() {	
	FILE* pfile1 = fopen("test.txt", "wb+");
	std::string buffer;

	if (pfile1 == NULL) {	
		printf("%s\n", "pfile1 in invalid");
		return 1;
	}

	OStream* sb = OStream::Initialize("test.txt");

	if (sb == NULL) {	
		printf("%s\n", "streambuffer is invalid");
		return 1;
	}

	fputs("pfile1 put string\n", pfile1);

	sb->WriteString("sb put string\n");

	fflush(pfile1);

	sb->WriteString("sb put string again\n");

	// std::string buffer;

	printf("sb buffer : %s\n", sb->buffer());

	sb->Flush();

	sb->WriteString("sb put string again again\n");

	printf("sb buffer : %s\n", sb->buffer());

	sb->Flush();

	for (size_t i = 0; i < BUFSIZ + 2; ++i) {	
		sb->WriteString("a");
	}

	// sb->Flush();

	for (size_t i = 0; i < 500; ++i) {
		sb->WriteString("b");	
	}

	printf("filled %d\n", sb->filled());
		
	printf("sb buffer : %s\n", sb->buffer());

	sb->WriteString("test\n");
		
	printf("sb buffer : %s\n", sb->buffer());

	printf("%s\n", sb->filename());

	delete sb;

	return 0;
}