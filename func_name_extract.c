#include <stdio.h>
#include <string.h>

void func_name_parser(FILE* fs)
{
    char str[100] = {0};
    const char str_name[9] = "declname"; // 함수명을 가리키는 key
    char func_name[10000] = {0};
    size_t str_length = 0;
    char* newline_ptr = 0;
    int i = 1;

    // fs = fopen("ast.json", "r");
    if (fs == NULL) printf("파일 열기 실패\n");

    while (fgets(str, 100, fs) != NULL)
    {   
        char* name_ptr = strstr(str, str_name); // 함수 이름이 나올때마다 name_ptr에 위치 저장
        
        if (name_ptr != NULL) {
            char* newline_ptr = strchr(name_ptr, '\n'); // 개행이 나올 때까지의 길이 저장
            str_length = newline_ptr - name_ptr; // 함수 이름이 있는 키 : 값 총 길이
            strncpy(func_name, name_ptr, str_length-1);

            name_ptr = strchr(func_name, ' '); // 키 : 값 중에 값, 함수 이름만 파싱
            printf("%d번째 함수 이름 :%s\n", i, name_ptr);
            i += 1;
        }

    }
    fclose(fs);
}