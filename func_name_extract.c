#include <stdio.h>
#include <string.h>

void remove_a(char *str) { // 공백, 쌍따옴표, 콤마 제거
    int i, j = 0;
    int len = strlen(str);

    for (i = 0; i < len; i++) {
        if (str[i] != ' ' && str[i] != '"' && str[i] != ',') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void func_name_extract(FILE* fs)
{
    char str[100] = {0};
    int i = 1;

    if (fs == NULL) printf("파일 열기 실패\n");

    while (fgets(str, 100, fs) != NULL)
    {                                           // decl 노드 밑에 (_nodetype으로 불필요한거 걸러줌) name 속성 파싱
        char* name_ptr = strstr(str, "decl"); 
        
        if (name_ptr != NULL ) {
            fgets(str, 100, fs);
            name_ptr = strstr(str, "_nodetype");
            if (name_ptr != NULL) {

                for (int i = 0; i < 6; i++) fgets(str, 100, fs);
                
                name_ptr = strstr(str, "name");
                name_ptr = strchr(name_ptr, ' ');
                remove_a(name_ptr);
                printf("%d번째 함수 이름 : %s\n", i, name_ptr);
                i += 1;      
            }
        }
    }
    fclose(fs);
}

/*int main() {

    FILE* fs = fopen("ast.json", "r");
    func_name_extract(fs);

    return 0;
}*/
