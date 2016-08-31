#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct person{
    size_t s2_fn;
    size_t s2_ln;

    char* f_name;
    char* l_name;
};

int main(){
    struct person p;
    p.f_name = "liza";
    p.l_name = "gilman";
    p.s2_fn = 5;
    p.s2_ln = 6;

    char* buf = malloc(sizeof(size_t)*2+2+strlen(p.f_name)+(p.f_name));

    memcpy(buf, p.s2_fn, sizeof(p.s2_fn));
    int iPtr = buf+sizeof(size_t);
    memcpy(buf+iPtr, p.s2_ln, sizeof(p.s2_ln));
    iPtr += sizeof(p.s2_ln);
    //memcpy(buf)




    return 0;
}
