#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define MAX_LEN 5000

void parameter_parser(FILE *fs) {

    char file[MAX_LEN];

    while (fgets(file, MAX_LEN, fs) != NULL) {
        cJSON *root = cJSON_Parse(file);

        // 오류 발생시 입력 문자열의 오류 위치에 대한 포인터 출력
        if (root == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) {
                fprintf(stderr, "Error before: %s\n", error_ptr);
            }
            return;
        }

        cJSON *ext = cJSON_GetObjectItem(root, "ext");
        if (cJSON_IsArray(ext)) {
            int array_size = cJSON_GetArraySize(ext);
            
            // Parameter 정보를 담은 노드를 찾아가는 과정
            for (int i = 0; i < array_size; i++) {
                cJSON *func_def = cJSON_GetArrayItem(ext, i);
                cJSON *decl = cJSON_GetObjectItem(func_def, "decl");

                cJSON *type = cJSON_GetObjectItem(decl, "type");
                cJSON *args = cJSON_GetObjectItem(type, "args");
                cJSON *param_list = cJSON_GetObjectItem(args, "params");

                // Parameter 정보를 담은 노드(배열)를 찾으면 다시 순회하며 이름과 타입을 담은 노드를 찾아냄
                if (cJSON_IsArray(param_list)) {
                    int param_size = cJSON_GetArraySize(param_list);

                    for (int j = 0; j < param_size; j++) {
                        cJSON *param = cJSON_GetArrayItem(param_list, j);

                        cJSON *param_type = cJSON_GetObjectItem(param, "type");
                        cJSON *param_name = cJSON_GetObjectItem(param, "name");

                        const char *type_str = cJSON_GetObjectItem(param_type, "names")->child->valuestring;
                        const char *name_str = param_name->valuestring;

                        cJSON *ptr_decl = cJSON_GetObjectItem(param_type, "type");
                        // Parameter 타입이 포인터(PtrDecl) 인지 여부에 따라 다르게 출력
                        if (cJSON_IsObject(ptr_decl) && cJSON_GetObjectItem(ptr_decl, "_nodetype")->valuestring != NULL &&
                            strcmp(cJSON_GetObjectItem(ptr_decl, "_nodetype")->valuestring, "PtrDecl") == 0) {
                            printf("%s *%s, ", type_str, name_str);
                        } else {
                            printf("%s %s, ", type_str, name_str);
                        }
                    }
                    printf("\n");
                }
            }
        }

        cJSON_Delete(root);
    }
}

/*
int main() {
    FILE *fs = fopen("temp.json", "r");
    
    // 파일이 정상적으로 열렸는지 확인
    if (fs == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    parameter_parser(fs);

    fclose(fs);

    return 0;
}
*/
