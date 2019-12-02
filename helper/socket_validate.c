#include "socket_validate.h"

// Validate and input string that contains only digits or not
int valid_digit(char *ip_str){

    while(*ip_str){
        if(*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else 
            return 0;
    }
    return 1;
}

// Validate an iput IP adrress
int is_valid_ip(char *ip_str){
    int i, num, dots = 0;
    char *ptr;

    if(ip_str == NULL)
        return 0;

    ptr = strtok(ip_str, ".");
    if(ptr == NULL)
        return 0;
    
    while(ptr){
        // After parsing a string it must contains only digits
        if(!valid_digit(ptr))
            return 0;
        
        num = atoi(ptr);

        // Check for valid IP
        if(num >= 0 && num <= 255){
            // Parse remaining string
            ptr = strtok(NULL, ".");
            if(ptr != NULL)
                ++dots;
        } else 
            return 0;
    }
    // Valid ip addr must contains 3 dots
    if(dots != 3)
        return 0;
    return 1;
}

int is_valid_port(char* PORT){
    for(int i=0; i<strlen(PORT); i++){
        if(!isdigit(PORT[i]))
            return 0;
    }
    return 1;
}
