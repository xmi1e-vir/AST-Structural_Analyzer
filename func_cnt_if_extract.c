#include <stdio.h>
#include <string.h>
#include <jansson.h> // jansson 라이브러리 사용

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
}

/*
int main()
{
   json_t* root = json_load_file("ast.json", 0, NULL);

   find_if_inFunc(root);

   json_decref(root); // 메모리 해제

   return 0;
}
*/