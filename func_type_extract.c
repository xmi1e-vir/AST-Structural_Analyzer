#include <stdio.h>
#include <string.h>

//			while(fgets(str,1000,fp) != NULL){
//        		char* type_ptr = strstr(str, str_type); // 함수 리턴값 확인하면 type_ptr에 위치 저장. 
//        		if(type_ptr !=NULL){
//        			fgets(name_ptr,1000,fp); // 다음 줄 읽기.
//        			remove_spaces(name_ptr); //공백 및 따움표 제거
//            		printf("Type : %s\n",name_ptr);
//            		break;
//           		}
//           	}
//          

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
    const char str_func[9] = "\"decl\""; // "decl". 
    const char str_args[5]  = "args"; // args.
    const char str_type[6] = "names"; // names.

	
	
    while(fgets(str, 1000, fp) != NULL)
    {   
        char* name_ptr = strstr(str, str_func); //"decl"이 나올때마다 name_ptr에 위치 저장
        if (name_ptr != NULL) {
			while(fgets(str,1000,fp) != NULL){
	        	char* args = strstr(str, str_args); //args 위치 확인. 
    	    	char* isinstance_null = strstr(str, "null"); // args(인자) 유무 확인용.  
    	    	if(args != NULL){ // args를 찾음. 
    	    		
    	    		// 함수의 인자값 없을때.
					if(isinstance_null != NULL){ 
						while(fgets(str,1000,fp) != NULL){
		        			char* type_ptr = strstr(str, str_type); // 함수 리턴값 확인하면 type_ptr에 위치 저장. 
    		    			if(type_ptr != NULL){
    	    					fgets(name_ptr,1000,fp); // 다음 줄 읽기.
    	    					remove_spaces(name_ptr); //공백 및 따움표 제거
    	        				printf("Type : %s\n",name_ptr);
    	        				break;
    	       				}
    	       			}
    	       			break;
					}
					
					// 인자값 있을 때
					else{
						// 인자값 있을 때, 스택 구조 활용.
						int cnt = 1;
						while(fgets(str,1000,fp) != NULL){
							char* ptr1 = strstr(str,"{");
							char* ptr2 = strstr(str,"}");
							if(ptr1 != NULL) cnt++;
							if(ptr2 != NULL) cnt--;
							if(cnt == 0) break;
						}
						//args 파라미터 탈출 후.
						while(fgets(str,1000,fp) != NULL){
		        			char* type_ptr = strstr(str, str_type); // 함수 리턴값 확인하면 type_ptr에 위치 저장. 
	    	    			if(type_ptr !=NULL){
	        					fgets(name_ptr,1000,fp); // 다음 줄 읽기.
	        					remove_spaces(name_ptr); //공백 및 따움표 제거
	            				printf("Type : %s\n",name_ptr);
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

// int main(){
//  	FILE* fp = NULL;
//  	fp = fopen("./file.json","r");
//  	func_type(fp);
//  	fclose(fp);
//  }
