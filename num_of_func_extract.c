#include <stdio.h>
#include <jansson.h> // jansson.h 라이브러리 이용
#include <string.h>

//'FuncDef' 노드를 카운트하는 함수
int count_functions(json_t *root) {
    int count = 0;   
    json_t *stack[1000];  
    int top = 0; 

    stack[top++] = root;

    //반복
    while (top > 0) {
        json_t *node = stack[--top]; 

        if (json_is_object(node)) {
            // check
            if (strcmp(json_string_value(json_object_get(node, "_nodetype")), "FuncDef") == 0) {
                count++;
            }

            const char *key;
            json_t *value;
            json_object_foreach(node, key, value) {
                stack[top++] = value;
            }
        } 
        else if (json_is_array(node)) {
            size_t size = json_array_size(node);
            for (size_t i = 0; i < size; i++) {
                stack[top++] = json_array_get(node, i); 
            }
        }
    }

    return count;
}

/*
int main() {
    json_t *root = json_load_file("ast.json", 0, NULL);

    printf("Number of functions: %d\n", count_functions(root));

    json_decref(root);  // 메모리를 해제
    return 0;
}
*/

// node & stack  
