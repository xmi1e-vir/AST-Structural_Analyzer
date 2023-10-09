#include <stdio.h>
#include <string.h>

//공백 및 따움표 제거
void remove_spaces(char* str) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ' && str[i] != '"') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0'; 
}


void func_type(FILE* fp)
{
    char str[1000] = {0}; // 문자열 받는 곳  
    const char str_key[5] = "names"; // 키값. 
  	int cnt = 0;
    

    while (fgets(str, 1000, fp) != NULL)
    {   
        char* name_ptr = strstr(str, str_key); // 함수 이름이 나올때마다 name_ptr에 위치 저장
        if (name_ptr != NULL) {
        	cnt++;
        	fgets(name_ptr,1000,fp); // 다음 줄 읽기.
        	if(cnt%2 == 1){ //(함수선언 FuncDecl, 타입선언 TypeDecl) 두개 중복 제거. 
        		remove_spaces(name_ptr); //공백 및 따움표 제거
            	printf("Type : %s\n",name_ptr);
            }
		}
    }
    
}

// int main(){
// 	FILE* fp = NULL;
// 	fp = fopen("./file.json","r");
// 	func_type(fp);
// 	fclose(fp);
// }
