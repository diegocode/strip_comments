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
    // states used for the state-machine
    enum strip_comments_states{
        write,
        slash,
        single_start, 
        multi_start,
        multi_stop
        } state;
    
    // state variable - initial state: write
    state = write;
    
    // file handlers fin: input file / fout: output file
    FILE *fin;
    FILE *fout;
   
    char origin_name[] = "test.c";     
    char destination_name[] = "test.noc.c";
    
    // open input file & check errors 
    fin = fopen(origin_name, "r");
    if (fin == NULL) {
        printf("Error opening %s or file doesn't exists\n", origin_name);
        return 1;
    }
    
    // open output file & check errors 
    fout = fopen(destination_name, "w");
    if (fout == NULL) {
        printf("Error opening %s or file doesn't exists\n", destination_name);
        return 2;        
    }
    
    // loop until end of file...
    char c;
    while (!feof(fin)) {        
        // read one char from file
        c = fgetc(fin);
        
        // state machine..
        switch(state) {
            default:
            case write:         
                /* 
                write: 
                - if char is '/' next state is "slash"
                - if char isn't '/' writes char to output file
                */
                if (c == '/') {
                    state = slash;
                } else {              
                    fputc(c, fout);
                }
                break;
            
            case slash:
                /* 
                slash: 
                - if char is '/' => it's a single-line comment
                  next state is "single_start"
                - if char is '*' => it's a multiline comment start
                  next state is "multi_start"
                - if char isn't '/' nor '*' => previous '/' was't the beginning 
                  of a comment, so writes previous '/' and currenr char to output file
                */
                if (c == '/') {
                    state = single_start;
                } else 
                if (c == '*') {
                    state = multi_start;
                } else {
                    state = write;
                    fputc('/', fout);                  
                    fputc(c, fout);                  
                }
                break;
            
            case single_start:
                /* 
                single_start: 
                - if char is new line => single-line comment ends.
                  next state is "write"
                - if char is tab, newline or carriage return, writes to output
                */                
                if (c == '\n') {
                    state = write;
                }
                
                if ((c == '\n') || (c == '\r') || (c == '\t')) {
                    fputc(c, fout);
                }                
                break;
                
            case multi_start:
                /* 
                multi_start: 
                - if char is '*' => possible multi-line comment end
                  next state is "multi-stop"
                */  
                if (c == '*') {
                    state = multi_stop;
                }        
                break;
                
            case multi_stop:
                /* 
                multi_stop: 
                - if char is '/' => found multi-line comment end
                  next state is "write"
                - if char isn't '/' next state is multi_start
                */  
                if (c == '/') {
                    state = write;
                } else {
                    state = multi_start;
                }
                break;
            }
        }  

    // close files
    fclose(fout);
    fclose(fin);    
    
    return 0;
}
