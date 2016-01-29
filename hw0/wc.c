#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *file;
	char buf[1];

	int charNum = 0;
	int wordNum = 0;
	int lineNum = 0;
	char prev;
	char cur;

	if (argc >1){
		file = fopen(argv[1], "r");
	}else{
		file = stdin;
	}
	while (!feof(file)){
		fread(buf, 1, 1, file);
		if (feof(file)){
			if (!isspace(prev)){
				wordNum += 1;
			}
			break;
		}
		charNum += 1;
		cur = buf[0];
		if (isspace(cur)){
			if ((charNum != 0) && (!isspace(prev))){
				wordNum += 1;
			}
			if (cur == '\n'){
				lineNum += 1;
			}
		}
		prev = cur;
	}

	if (argc >1){
		printf("%d %d %d %s\n", lineNum, wordNum, charNum, argv[1]);
	}else{
		printf("%d %d %d\n", lineNum, wordNum, charNum);
	}

    return 0;
}
