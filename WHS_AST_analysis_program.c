#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h> // jansson 라이브러리 사용
#include "cJSON.h"

#define _CRT_SECURE_NO_WARNINGS 
#define MAX_LEN 100 * 1024 * 1024


char buffer[MAX_LEN];


// 정준수
// 인자로 들어온 함수에서 if문을 재귀적으로 찾음 => 중첩 if문 때문에
int count_ifs(json_t *root) 
{
    int count = 0;
    const char *key;
    json_t *value;

    if(!json_is_object(root))return 0;

    json_object_foreach(root, key, value)
     {
        if (json_is_string(value) && strcmp(key, "_nodetype") == 0 && strcmp(json_string_value(value), "If") == 0)count++;

        if (json_is_object(value))count += count_ifs(value);
        else if (json_is_array(value))
        {
            size_t index;
            json_t *value_element;
            json_array_foreach(value, index, value_element)count += count_ifs(value_element);
        }
    }
    return count;
}

// 정준수
// 전체 JSON 객체로부터 함수들을 구분함
void find_if_inFunc(json_t *root)
{
   const char* key; 
   json_t* value;

	json_object_foreach(root,key,value)
    {
		if(strcmp(key,"ext")==0 && json_is_array(value))
        {
			size_t index; 
			json_t* func_node;
			json_array_foreach(value,index,func_node)
            {
				if(strcmp(json_string_value(json_object_get(func_node,"_nodetype")),"FuncDef")==0)
                {
					const char* func_name=json_string_value(json_object_get(json_object_get(func_node,"decl"),"name"));
					json_t* body=json_object_get(func_node,"body");
                    int num_ifs = count_ifs(body);
                    printf("Number of if in %s function : %d\n", func_name, num_ifs);
				}
			}
		}
     }
     printf("\n");
}

// 공백, 쌍따옴표, 콤마 제거
void remove_a(char *str)
{
    int i, j = 0;
    int len = strlen(str);
    for (i = 0; i < len; i++)
    {
        if (str[i] != ' ' && str[i] != '"' && str[i] != ',')str[j++] = str[i];
    }
    str[j] = '\0';
}

// 최석규
void func_name_extract(FILE* fs)
{
    char str[100] = {0};
    int i = 1;

    while (fgets(str, 100, fs) != NULL)
    {                                           // decl 노드 밑에 (_nodetype으로 불필요한거 걸러줌) name 속성 파싱
        char* name_ptr = strstr(str, "decl"); 
        if (name_ptr != NULL )
        {
            fgets(str, 100, fs);
            name_ptr = strstr(str, "_nodetype");
            if (name_ptr != NULL)
            {
                for (int i = 0; i < 6; i++) fgets(str, 100, fs);
                name_ptr = strstr(str, "name");
                name_ptr = strchr(name_ptr, ' ');
                remove_a(name_ptr);
                printf("%d번째 함수 이름 : %s", i, name_ptr);
                i += 1;      
            }
        }
    }
    printf("\n");
}

// 정기현
void func_type(FILE* fp)
{
    char str[1000] = {0}; // 문자열 받는 곳  
    const char str_func[9] = "\"decl\""; // "decl". 
    const char str_args[5]  = "args"; // args.
    const char str_type[6] = "names"; // names.
	
    while(fgets(str, 1000, fp) != NULL)
    {   
        char* name_ptr = strstr(str, str_func); //"decl"이 나올때마다 name_ptr에 위치 저장
        if (name_ptr != NULL) 
        {
			while(fgets(str,1000,fp) != NULL)
            {
	        	char* args = strstr(str, str_args); //args 위치 확인. 
    	    	char* isinstance_null = strstr(str, "null"); // args(인자) 유무 확인용.  
    	    	if(args != NULL)// args를 찾음. 
                { 
    	    		// 함수의 인자값 없을때.
					if(isinstance_null != NULL)
                    { 
						while(fgets(str,1000,fp) != NULL)
                        {
		        			char* type_ptr = strstr(str, str_type); // 함수 리턴값 확인하면 type_ptr에 위치 저장. 
    		    			if(type_ptr != NULL)
                            {
    	    					fgets(name_ptr,1000,fp); // 다음 줄 읽기.
    	    					remove_a(name_ptr); //공백 및 따움표 제거
    	        				printf("Type : %s\n",name_ptr);
    	        				break;
    	       				}
    	       			}
    	       			break;
					}
					else // 인자값 있을 때, 스택 구조 활용.
                    {
						int cnt = 1;
						while(fgets(str,1000,fp) != NULL)
                        {
							char* ptr1 = strstr(str,"{");
							char* ptr2 = strstr(str,"}");
							if(ptr1 != NULL) cnt++;
							if(ptr2 != NULL) cnt--;
							if(cnt == 0) break;
						}
						//args 파라미터 탈출 후.
						while(fgets(str,1000,fp) != NULL)
                        {
		        			char* type_ptr = strstr(str, str_type); // 함수 리턴값 확인하면 type_ptr에 위치 저장. 
	    	    			if(type_ptr !=NULL)
                            {
	        					fgets(name_ptr,1000,fp); // 다음 줄 읽기.
	        					remove_a(name_ptr); //공백 및 따움표 제거
	            				printf("Type : %s",name_ptr);
	            				break;
	           				}
	           			} 
					break;
					}	
				}
			}
		}
    }
}

// 이현규
//'FuncDef' 노드를 카운트하는 함수
int count_functions(json_t *root)
{
    int count = 0;   
    json_t *stack[1000];  
    int top = 0; 
    stack[top++] = root;

    //반복
    while (top > 0)
    {
        json_t *node = stack[--top]; 
        if (json_is_object(node))
         {
            // check
            if (strcmp(json_string_value(json_object_get(node, "_nodetype")), "FuncDef") == 0)count++;

            const char *key;
            json_t *value;
            json_object_foreach(node, key, value)stack[top++] = value;
        } 
        else if (json_is_array(node))
        {
            size_t size = json_array_size(node);
            for (size_t i = 0; i < size; i++)stack[top++] = json_array_get(node, i); 
        }
    }
    return count;
}

// 정다은
void parameter_parser(FILE* fs)
{
    fseek(fs, 0, SEEK_END);
    int size = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    fread(buffer, size, 1, fs);

    cJSON* root = cJSON_Parse(buffer);

    // 오류 발생시 입력 문자열의 오류 위치에 대한 포인터 출력
    if (root == NULL)
    {
        fprintf(stderr, "Root is null\n");
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    cJSON* ext = cJSON_GetObjectItem(root, "ext");
    if (cJSON_IsArray(ext))
    {
        int array_size = cJSON_GetArraySize(ext);

        // Parameter 정보를 담은 노드를 찾아가는 과정
        for (int i = 0; i < array_size; i++)
        {
            cJSON* func_def = cJSON_GetArrayItem(ext, i);
            cJSON* decl = cJSON_GetObjectItem(func_def, "decl");
            cJSON* type = cJSON_GetObjectItem(decl, "type");
            cJSON* args = cJSON_GetObjectItem(type, "args");
            cJSON* param_list = cJSON_GetObjectItem(args, "params");

            // Parameter 정보를 담은 노드(배열)를 찾으면 다시 순회하며 이름과 타입을 담은 노드를 찾아냄
            if (cJSON_IsArray(param_list))
            {
                int param_size = cJSON_GetArraySize(param_list);

                for (int j = 0; j < param_size; j++)
                {
                    cJSON* param = cJSON_GetArrayItem(param_list, j);
                    cJSON* param_type = cJSON_GetObjectItem(param, "type");
                    cJSON* param_nodetype = cJSON_GetObjectItem(param_type, "_nodetype");
                    cJSON* param_type_type = cJSON_GetObjectItem(param_type, "type");

                    if (strcmp(param_nodetype->valuestring, "PtrDecl") == 0)
                    {
                        cJSON* param_type_type_declname = cJSON_GetObjectItem(param_type_type, "declname");
                        const char* name_str = param_type_type_declname->valuestring;
                        cJSON* param_type_type_type = cJSON_GetObjectItem(param_type_type, "type");
                        cJSON* param_type_type_type_nodetype = cJSON_GetObjectItem(param_type_type_type, "_nodetype");
                        if (strcmp(param_type_type_type_nodetype->valuestring, "IdentifierType") == 0)
                        {
                            cJSON* param_type_type_type_names = cJSON_GetObjectItem(param_type_type_type, "names");
                            int name_val = cJSON_GetArraySize(param_type_type_type_names);
                            cJSON* name_arr = cJSON_GetArrayItem(param_type_type_type_names, 0);
                            const char* type_str = name_arr->valuestring;
                            printf("parameter name : %s ", name_str);
                            printf("  parameter type : *%s", type_str);
                        }
                    }
                    else if (strcmp(param_nodetype->valuestring, "TypeDecl") == 0)
                    {
                        cJSON* param_nodetype_declname = cJSON_GetObjectItem(param_type, "declname");
                        const char* name_str = param_nodetype_declname->valuestring;
                        printf("parameter name : %s ", name_str);
                        cJSON* param_nodetype_type = cJSON_GetObjectItem(param_type, "type");
                        cJSON* param_nodetype_type_names = cJSON_GetObjectItem(param_nodetype_type, "names");
                        int name_size = cJSON_GetArraySize(param_nodetype_type_names);
                        printf("  parameter type : ");
                        for (int i = 0; i < name_size; i++)
                        {
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
    printf("\n");
    cJSON_Delete(root);
}


int main()
{
    json_t* root = json_load_file("binary_tree.json", 0, NULL);
    
    printf("\n1. 함수 개수 추출하기 (이현규)\n\n");
    printf("Number of functions: %d\n\n", count_functions(root));     // 이현규
    
    for(int x=0;x<3;x++)
    {
        FILE *fp = fopen("binary_tree.json","r");
        if(fp == NULL)printf("Cannot open file. :( \n");

        if(x == 0){ printf("\n2. 함수들의 리턴타입 추출하기 (정기현)\n\n"); func_type(fp); }// 정기현
        if(x == 1){ printf("\n3. 함수들의 이름 추출하기 (최석규)\n\n"); func_name_extract(fp); }// 최석규
        if(x == 2){ printf("\n4. 함수들의 파라미터 타입, 변수명 추출하기 (정다은)\n\n"); parameter_parser(fp); }// 정다은
        fclose(fp);
    }
    
    printf("\n5. 함수들의 if조건 개수 추출하기 (정준수)\n\n");
    find_if_inFunc(root);       // 정준수

    json_decref(root); // 메모리 해제

   return 0;
}
