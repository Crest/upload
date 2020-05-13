#include <stdio.h>
#include <string.h>
#include <errno.h>

static const int line_max = 200;

int
main(int argc, const char *argv[])
{
	FILE *file   = NULL;
	int   status = 0;

	if ( argc < 2 ) {
		fprintf(stderr, "usage: %s <file>...\n", argv[1]);
		return 1;
	}
	
	for ( int i = 1; i < argc && !status; i++ ) {
		const char *name     = argv[i];
		int         line_pos = 1;
		int         line_num = 1;

		file = fopen(name, "r");
		if ( !file ) {
			fprintf(stderr, "*** FAILED TO OPEN %s : %s ***\n", name, strerror(errno));
			status = 1;
			break;
		}
		fprintf(stderr, "*** UPLOADING %s ***%s", name, i == 1 ? "\n" : "");

		int ch;
		while ( !status && (ch = getc_unlocked(file)) != EOF ) {
			if ( line_pos > line_max ) {
				fprintf(stderr, "\n*** LINE %i OF %s IS TOO LONG ***\n", line_num, name);	
				status = 1;
				break;
			}
			putchar_unlocked(ch);
			if ( ch == '\n' ) {
				while ( !status && (ch = getchar_unlocked()) != 0x06 ) {
					switch ( ch ) {
						case 0x15:
							fprintf(stderr, "*** ERROR IN LINE %i OF %s ***\n", line_num, name);
							status = 1;
							break;

						default:
							putc_unlocked(ch, stderr);
							break;
					}
				}
				line_pos = 1;
				line_num++;
			} else {
				line_pos++;
			}
		}

		if ( status ) {
			fprintf(stderr, "\n*** FAILED TO UPLOAD %s ***\n\n", name);
		} else {
			fprintf(stderr, "\n*** SUCCESSFUL UPLOAD OF %s ***\n\n", name);
		}
		fclose(file); file = NULL;
	}

	if ( file ) {
		fclose(file);
	}

	return status;
}
