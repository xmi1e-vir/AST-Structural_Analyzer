#include <jansson.h>
#include <stdio.h>
#include <string.h>

int count_ifs(json_t *root) {
    int count = 0;
    const char *key;
    json_t *value;

    if(!json_is_object(root)) {
        return 0;
    }

    json_object_foreach(root, key, value) {
        if (json_is_string(value) && strcmp(key, "_nodetype") == 0 && strcmp(json_string_value(value), "If") == 0)
				{
            count++;
        }

        if (json_is_object(value)) {
            count += count_ifs(value);
        } else if (json_is_array(value)) {
            size_t index;
            json_t *value_element;

            json_array_foreach(value, index, value_element) {
                count += count_ifs(value_element);
            }
        }
    }

    return count;
}

void analyze_func(const char* func_name,json_t* func_body){
   int num_ifs = count_ifs(func_body);
   
   printf("Number of 'if' statements in function %s: %d\n", func_name,num_ifs);
}

int main() {
   json_error_t error;
   json_t* root = json_load_file("ast.json", 0, &error);

   const char* key; 
   json_t* value;

	json_object_foreach(root,key,value){
		if(strcmp(key,"ext")==0 && json_is_array(value)){
			size_t index; 
			json_t* func_node;

			json_array_foreach(value,index,func_node){
				if(strcmp(json_string_value(json_object_get(func_node,"_nodetype")),"FuncDef")==0){
					const char* func_name=json_string_value(json_object_get(json_object_get(func_node,"decl"),"name"));
					json_t* body=json_object_get(func_node,"body");
					analyze_func(func_name,body);
				}
			}
		}
     }

   json_decref(root);

   return 0;
}
