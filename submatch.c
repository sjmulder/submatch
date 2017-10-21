/* Substitution matcher

   Example: ABCA matches 'that' (A=t, B=h, C=a)
                      or 'else' (A=e, B=l, C=s)
   
   Input lines are printed if matched, skipped if not.

   A 'line' array is defined which, for every iteration, starts out as all
   \0 characters except one \n character lining up with the end of the
   pattern, e.g.:
             
     pat  [ A  B  C  A \0 ]
     line [\0 \0 \0 \0 \n \0 ]

   As characters are read from input, they set in the array, e.g. after
   reading a 't' character:

     pat  [ A  B  C  A \0 ]
     line [ t \0 \0 \0 \n \0 ]
     i      ^

   If the character is a newline, the line doesn't match the pattern and is
   rejected. Otherwise, all other elements with the same pattern character are
   set to the same value, the final 't' in this example:

     pat  [ A  B  C  A ]
     line [ t \0 \0  t \n \0 ]
     i      ^

   The forward characters in the array, including the newline, serve as
   expectations. If an expectation exists and the read character doesn't match
   it, the line does not match the pattern and is rejected, e.g. if at:

     pat  [ A  B  C  A \0 ]
     line [ t  h  a  t \n \0 ]
     i               ^

   ...the read character is not 't', the line does not match the ABCA pattern.
   The same goes for the newline character, e.g if at:

     pat  [ A  B  C  A \0 ]
     line [ t  h  a  t \n \0 ]
     i                  ^

   ...the read character is not \n it is longer than the pattern and must be
   rejected. The rest of the line is skipped.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
	char *pat, line[64];
	int patlen, i, j, c;

	if (argc != 2) {
		fprintf(stderr, "usage: submatch <pattern>\n");
		return 1;
	}

	pat = argv[1];
	if ((patlen = strlen(pat)) >= sizeof(line)-2) {
		fprintf(stderr, "pattern too long\n");
		return 1;
	}

	while (!feof(stdin)) {
		/* patlen nulls with \n\0 at end */
		memset(line, 0, patlen+2);
		line[patlen] = '\n';

		for (i=0; i < patlen+1; i++) {
			if ((c = getchar()) == EOF)
				return 0;

			if (!line[i] && c != '\n') {
				/* no expectation yet */
				line[i] = (char)c;
				for (j = i+1; j<patlen; j++)
					if (pat[i] == pat[j])
						line[j] = (char)c;
			} else if (line[i] != (char)c) {
				/* expectation mismatch */
				while (c != '\n' && c != EOF)
					c = getchar();
				break;
			}
		}

		if (i == patlen+1)
			fputs(line, stdout);
	}

	return 0;
}
