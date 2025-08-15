#include <stdarg.h>
#include <stdio.h>

void write_log (const char* msg, ...)
{
    va_list args; 
    va_start (args, msg); 

    FILE * file;
    file = fopen("out.log", "a"); 

    if (file != NULL)
    {
        vfprintf(file, msg, args);
        fprintf(file, "\n");
        fclose(file);
    }
    else 
    {
        perror("Failed to open log file"); 
    }
}
void clear_log()
{
    FILE * file;
    file = fopen("out.log", "w");
    if (file != NULL)
    {
        fprintf(file, "--DEBUGGING LOG--\n");
        fclose(file); 
    }
}