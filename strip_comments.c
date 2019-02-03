/*
    Strips c like comments from source file
    
    TO DO: 
    
    - error in the last character 
    - blank line(s) at the beginning of dest. file
    
    - test, test, test...
    
*/

#include <stdio.h>

int main()
{
    enum strip_comments_states{
        write,
        slash,
        single_start, 
        multi_start,
        multi_stop
        } state;
    
    state = write;
    
    FILE *fin;
    FILE *fout;
    
    char line[180];
    char origin_name[] = "test.c";
    char destination_name[] = "test.noc.c";
    
    fin = fopen(origin_name, "r");
    if (fin == NULL) {
        printf("Error opening %s or file doesn't exists\n", origin_name);
        return 1;
    }
    
    fout = fopen(destination_name, "w");
    if (fout == NULL) {
        printf("Error opening %s or file doesn't exists\n", destination_name);
        return 2;        
    }
    
    char c;
    while (!feof(fin)) {        
        c = fgetc(fin);
        
        switch(state) {
            default:
            case write:
                if (c == '/') {
                    state = slash;
                } else {
                    printf("%c", c);
                    fputc(c, fout);
                }
                break;
            
            case slash:
                if (c == '/') {
                    state = single_start;
                } else 
                if (c == '*') {
                    state = multi_start;
                } else {
                    state = write;
                    printf("%c", '/');
                    fputc('/', fout);                  
                    printf("%c", c);
                    fputc(c, fout);                  
                }
                
                break;
            
            case single_start:
                if (c == '\n') {
                    state = write;
                }
                
                if ((c == '\n') || (c == '\r') || (c == '\t')) {
                    printf("%c", c);
                    fputc(c, fout);
                }
                
                break;
                
            case multi_start:
                if (c == '*') {
                    state = multi_stop;
                }
                                    
                break;
                
            case multi_stop:
                if (c == '/') {
                    state = write;
                } else {
                    state = multi_start;
                }
                
                break;
            }
            
                
                    
        }  

    fclose(fout);
    fclose(fin);    
    
    return 0;
}
