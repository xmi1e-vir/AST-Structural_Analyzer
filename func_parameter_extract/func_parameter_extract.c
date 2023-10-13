#define _CRT_SECURE_NO_WARNINGS 
// vs에서 작업하여 fopen 문제를 해결하기 위한 문구

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define MAX_LEN 100 * 1024 * 1024
char buffer[MAX_LEN];

void parameter_parser(FILE* fs);

int main() {
    FILE* fs = fopen("FILE_PATH.json", "r");

    // 파일이 정상적으로 열렸는지 확인
    if (fs == NULL) {
        printf("Cannot open file.\n");
        return 1; // 비정상 종료
    }

    parameter_parser(fs);

    fclose(fs);

    return 0;
}

void parameter_parser(FILE* fs) {
    // 파일을 읽어오는 과정
    fseek(fs, 0, SEEK_END);
    int size = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    fread(buffer, size, 1, fs);

    cJSON* root = cJSON_Parse(buffer);

    // 오류 발생시 입력 문자열의 오류 위치에 대한 포인터 출력
    if (root == NULL) {
        fprintf(stderr, "Root is null\n");
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    cJSON* ext = cJSON_GetObjectItem(root, "ext");
    if (cJSON_IsArray(ext)) {
        int array_size = cJSON_GetArraySize(ext);

        // Parameter 정보를 담은 노드를 찾아가는 과정
        for (int i = 0; i < array_size; i++) {
            cJSON* func_def = cJSON_GetArrayItem(ext, i);
            cJSON* decl = cJSON_GetObjectItem(func_def, "decl");

            cJSON* type = cJSON_GetObjectItem(decl, "type");
            cJSON* args = cJSON_GetObjectItem(type, "args");
            cJSON* param_list = cJSON_GetObjectItem(args, "params");

            // Parameter 정보를 담은 노드(배열)를 찾으면 다시 순회하며 이름과 타입을 담은 노드를 찾아냄
            if (cJSON_IsArray(param_list)) {
                int param_size = cJSON_GetArraySize(param_list);

                for (int j = 0; j < param_size; j++) {
                    cJSON* param = cJSON_GetArrayItem(param_list, j);
                    cJSON* param_type = cJSON_GetObjectItem(param, "type");
                    cJSON* param_nodetype = cJSON_GetObjectItem(param_type, "_nodetype");
                    cJSON* param_type_type = cJSON_GetObjectItem(param_type, "type");

                    // _nodetype이 PtrDecl인지 TypeDecl인지에 따라 포인터 변수 여부 구분(json 구조 차이 존재)
                    if (strcmp(param_nodetype->valuestring, "PtrDecl") == 0) {
                        cJSON* param_type_type_declname = cJSON_GetObjectItem(param_type_type, "declname");
                        const char* name_str = param_type_type_declname->valuestring;
                        cJSON* param_type_type_type = cJSON_GetObjectItem(param_type_type, "type");
                        cJSON* param_type_type_type_nodetype = cJSON_GetObjectItem(param_type_type_type, "_nodetype");
                        if (strcmp(param_type_type_type_nodetype->valuestring, "IdentifierType") == 0) {
                            cJSON* param_type_type_type_names = cJSON_GetObjectItem(param_type_type_type, "names");
                            int name_val = cJSON_GetArraySize(param_type_type_type_names);
                            cJSON* name_arr = cJSON_GetArrayItem(param_type_type_type_names, 0);
                            const char* type_str = name_arr->valuestring;
                            printf("  parameter name : %s ", name_str);
                            printf("  parameter type : *%s", type_str);
                        }
                    }
                    else if (strcmp(param_nodetype->valuestring, "TypeDecl") == 0) {
                        cJSON* param_nodetype_declname = cJSON_GetObjectItem(param_type, "declname");
                        const char* name_str = param_nodetype_declname->valuestring;
                        printf("  parameter name : %s ", name_str);
                        cJSON* param_nodetype_type = cJSON_GetObjectItem(param_type, "type");
                        cJSON* param_nodetype_type_names = cJSON_GetObjectItem(param_nodetype_type, "names");
                        int name_size = cJSON_GetArraySize(param_nodetype_type_names);
                        printf("  parameter type : ");
                        // long int와 같은 두자리 이상 데이터 타입을 모두 출력하기 위해 배열 순회회
                        for (int i = 0; i < name_size; i++) {
                            cJSON* name_arr = cJSON_GetArrayItem(param_nodetype_type_names, i);
                            const char* type_str = name_arr->valuestring;
                            printf("%s ", type_str);
                        }
      
                    }
                    else
                        continue;
                    printf("\n");
                }
            }
        }
    }
    // 해제
    cJSON_Delete(root);
}
