#include <stdio.h>
#include <ctype.h>
#include <assert.h>

/* function checking new line character */
int isnewline(int a){
  if (a == '\n') return 1;
  else return 0;
}
/* function checking '/' character */
int isslash(int a){
  if (a =='/') return 1;
  else return 0;
}
/* function checking '*' character */
int isast(int a){
  if (a =='*') return 1;
  else return 0;
}

/*create 5 states: Inside Word, Outside Word, Comment, Slash, Asterisk*/
enum State {IN, OUT, COM, SLASH, AST};

/*count and print numbers of line, word, and character excluding the comment*/
int main(void){

  /* variables initialization */
  int c,total_char = 0, word = 0, total_line = 1; 
  int character = 0;/* 'character' counts only the characters in words */
  int space = 0, comment_space = 0, newline = 0, comment_line = 0; 
  int slash_temp = 0; /*used to indicate the previous state before SLASH*/
  enum State state = OUT;
  
  /*start counting*/
  while ((c=getchar()) != EOF){
    switch(state){
      case OUT:
	/* OUT -> OUT */
	if (isspace(c)){
	  if (isnewline(c)) newline++; /* count new line character */
	  else space++;
	}
	/* OUT -> SLASH */
	else if (isslash(c)) {
	  state = SLASH; character++; word++;
	  slash_temp=1; /* indicates that before SLASH is OUT state */
	}
	/* OUT -> IN */
	else {state = IN; character++; word++;}
        break;
	
      case IN:
	/* IN -> OUT */
	if (isspace(c)){
	  state = OUT;
	  if (isnewline(c)) newline++;
	  else space++;
	}
	/* IN -> SLASH */
	else if (isslash(c)) {
	  state = SLASH; character++;
	  slash_temp=0; /* indicates that before SLASH is not OUT state */
	}
	/* IN -> IN */
	else character++;
        break;
	  
      case SLASH:
	/* SLASH -> COM */
	if (isast(c)) {
	  state = COM; character--;
	  if (slash_temp) word--; /* delete the previous word counted */
	}
	/* SLASH -> SLASH */
	else if (isslash(c)) {character++; slash_temp=0;}
	/* SLASH -> OUT */
	else if (isspace(c)){
	  state = OUT;
	  if (isnewline(c)) newline++;
	  else space++;
	}
	/* SLASH -> IN */
	else {state = IN; character++;}
	break;

	case COM:
	/* COM -> AST */
	if (isast(c)) state = AST;
	else if (isnewline(c)) comment_line++; /* count new line in comment */
	break;
	
      case AST:
	/* AST -> OUT */
	if (isslash(c)) {
	  state = OUT; comment_space++; /* count whole comment as 1 space */
	  newline += comment_line; /* update line number after end of a comment */
	  comment_line = 0;
	} 
	/* AST -> AST */
	else if (isast(c)) state = AST; /* no changes happen */
	/* AST -> COM */
	else {state = COM; if(isnewline(c)) comment_line++;}
        break;
	
      default:
        assert(0);
        break;
    }
  }
  
  
  /* check status after EOF */
  if (state == COM ||state == AST) {
    /* print out the error message for an unterminated comment */
    fprintf(stderr, "Error: line %d: unterminated comment\n", (1 + newline) );
  }
  else {
    total_line += newline;
    total_char += character + space + comment_space + newline;
    /* print 0 0 0 for an empty file */
    if (total_char == 0) {
      printf("0 0 0\n");
    }  
    /* print the numbers of line, word, and character */
    else printf("%d %d %d\n",total_line, word, total_char);
  }
  
  return 0;
}
