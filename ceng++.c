//2014510028 Çağrı Anıl Erbey

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char fpeek(FILE *fp){
    char ch;

    ch = fgetc(fp);
    ungetc(ch, fp);

    return ch;
}

char* char_to_str(char ch){
    char* temp = (char*) malloc(2);
    temp[0] = ch;
    temp[1] = '\0';
    return temp;
}

char* two_char_to_str(char c1, char c2){
    char* temp = (char*) malloc(sizeof(char) * 3);
    temp[0] = c1;
    temp[1] = c2;
    temp[2] = '\0';
    return temp;
}

int isOperator(char* op){//findig operator
    char operators[7][3] = {"+","-","*","/","++","--",":="};
    int i, flag = 0;

    for(i = 0; i < sizeof(operators)/sizeof(operators[0]); ++i){
        if(strcmp(operators[i], op) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}

int combine2OP(char c1, char c2){//was used to combine 2 ch operators 
    char* temp = two_char_to_str(c1, c2);
    int rv;
    rv = isOperator(temp);
    free(temp);
    return rv;
}

char* define_BN(char ch){//this was used to define bracket names
    if(ch == '('){
        return "LeftPar";
    }else if(ch == ')'){
        return "RightPar";
    }else if(ch == '{'){
        return "LeftCurlyBracket";
    }else if(ch == '}'){
        return "RightCurlyBracket";
    }else{
        return NULL;
    }
}

int find2last_index(char* str, char ch, int rs){ // is used to return the last index of a character, if not found returns a negative
    int length = strlen(str) - rs;
    char* ptr = str;
    for(; length>=0;length--){
        if( *(ptr+length) == ch){
            return length;
        }
    }
    return -1;
}

char* bracketer(char *str, char ch, char chh, int closed){//returns the inside of a bracket
    int isOpen, isClose;
    char* result;
    int length = 0;
    isClose = find2last_index(str, chh, closed);
    if( isClose >= 0 ){
        isOpen = find2last_index(str, ch, isClose);
        if( isOpen >= 0 ){
            length = isClose - isOpen - 1;
            result = (char*)malloc(length);
            memcpy(result, str + isOpen + 1, length);
            result[length] = '\0';
        }else{
            return NULL;// this means brackets are unclosed
        }
    }else{
       return NULL;
    }
    return result;
}

int isIdentifier(char* id){
    char* ptr = NULL;
    if( strlen(id) > 20){// identifier must be shorter than 20 characters
        return -1;
    }
    id = strlwr(id);//the incoming id was done lowercase
    if( id[0] > 'z' || id[0] < 'a'){
        return -2;
    }
    for(ptr = id; *ptr != '\0'; ++ptr){//must continue with alphanumeric chars or underscore
        if(isalnum(*ptr)){//Checks whether the c parameter value passed to the function is a letter and a number in the alphabet.
        }else if (*ptr == '_') {//var_18
        }else{
            return -3;
        }
    }
    return 1;
}

int isKeyword(char* keyword){//to find keyword
    char keywords[18][10] = {"break","case","char","const","continue","do","else","enum",
"float","for","goto","if","int","long","record","return","static","while"};
    int i, flag = 0;

    for(i = 0; i < 18; ++i){
        if(strcmp(keywords[i], keyword) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}

int main(){
    char ch, ch2, buffer[15] = {0} ;
    char* temp = NULL;
    int i = 0, index, comment_count = 0;

    FILE* fp, *fp_out;
    fp = fopen("code.ceng","r");
    fp_out = fopen("code.lex", "w");

    if(fp == NULL || fp_out == NULL){
        printf("Error while opening the file\n");
        exit(1);
    }

    while((ch = fgetc(fp)) != EOF){//char by char
        if(isdigit(ch) && (isspace(fpeek(fp)) || isdigit(fpeek(fp)) )){//integer constants
            temp = (char*) malloc(11);
            i = 0;
            temp[i] = ch;
            while((ch = fgetc(fp)) != EOF && i <= 10){//Maximum integer size is 10 digits
                temp[++i] = ch;
                if(!isdigit(fpeek(fp))){
                    temp[++i] = '\0';
                    break;
                }
            }
            if(i > 10){
                fprintf(fp_out, "Error Message: Maximum integer size can be 10 digits.\n");
            }else if(ch == EOF){
                fprintf(fp_out, "Error out file.\n");
            }

            fprintf(fp_out, "IntConst (%s)\n", temp);
            free(temp);
        }
        else if(isalnum(ch) || ch == '_'){//identifier,keyword
            buffer[index++] = ch;

            if(isalnum(fpeek(fp))){
            }
            else if (fpeek(fp) == '_') {
            }
            else{
                buffer[index] = '\0';
                index = 0;

                if(isKeyword(buffer))
                    fprintf(fp_out, "Keyword (%s)\n", buffer);
                i = -10;
                if((i = isIdentifier(buffer))==1)
                    fprintf(fp_out, "Identifier (%s)\n", strlwr(buffer));
                if(i == -1)
                    fprintf(fp_out, "Error Message: <%s> is too long\n", buffer);
                if(i == -2)
                    fprintf(fp_out, "Error Message: <%s> is not starting with alphabetic character\n", buffer);
                if(i == -3)
                    fprintf(fp_out, "Error Message: <%s> is contains non-alphanumeric characters\n", buffer);
            }
        }
 
        else{ // if isalnum false

            if (ch == '"'){//string starts
                temp = (char*) malloc(200);
                i = 0;
                while((ch = fgetc(fp)) != EOF){
                    temp[i++] = ch;
                    if(fpeek(fp) == '"') {
                        fgetc(fp);
                        temp[i] = '\0';
                        break;
                    }
                }
                if(ch == EOF){
                    fprintf(fp_out, "Error Message: String not closed\n");
                }
                fprintf(fp_out, "StringConst (%s)\n", temp);
                free(temp);
            }
            else if(ch == '(' && fpeek(fp) == '*'){// (*comment*)
                fgetc(fp);
                while((ch = fgetc(fp)) != EOF){
                    if(ch == '*' && fpeek(fp) == ')'){
                        fgetc(fp);
                        break;
                    }
                }
                if(ch == EOF){
                    fprintf(fp_out, "Error Message: Comment not closed\n");
                }
                comment_count++;
            }

            else if(define_BN(ch) != NULL) {//bracket
                fprintf(fp_out, "%s\n", define_BN(ch));
            }

            else if(combine2OP(ch, fpeek(fp))){//operators 2 digits
                ch2 = fgetc(fp); // waste one char
                fprintf(fp_out, "Operator (%c%c)\n", ch, ch2);

            }
            else if(isOperator(char_to_str(ch))){// operators 1 digit
                fprintf(fp_out, "Operator (%c)\n", ch);
            }

            else if(ch == ';'){// ";"
                fprintf(fp_out, "EndOfLine\n");
            }
        }
    }
    if(comment_count == 1){
        printf("%d comment was entered.\n",comment_count);
    }
    else{
        printf("%d comments were entered.\n",comment_count);
    }
    printf("Lexical analysis was completed, please check the <code.lex> file.");
    fclose(fp);
    fclose(fp_out);
    return 0;
}
