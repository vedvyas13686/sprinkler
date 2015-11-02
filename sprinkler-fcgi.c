// -------------------------------------------------------------------------
// This software is furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// sprinkler-fcgi.c 
//    main file for the sprinker configuration FCGI application
//
// Author: Vedvyas Shanbhogue
// -------------------------------------------------------------------------

#ifdef IDENT_C
static const char* const <filename>_c_Id = "$Id$";
#endif

#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcgi_stdio.h>
#include <mysql.h>

typedef struct {
    char *tag;
    char *fmt;
    union {
        char char_val;
        int  int_val;
        long long_val;
        char *string_val;
    };
} form_tag_value_element;

typedef struct {
    int num_elements;
    form_tag_value_element elements[0];
} form_tag_value_list;

// -- fileScope -------------------------------------------------------------
// my_getstr()
//     Read up to (and including) a TERMINATOR from STREAM into *LINEPTR
//     + OFFSET (and null-terminate it). *LINEPTR is a pointer returned from
//     malloc (or NULL), pointing to *N characters of space.  It is realloc'd
//     as necessary.  
// Returns:
//     Return the number of characters read (not including the
//     null terminator), or -1 on error or EOF. 
// -------------------------------------------------------------------------*/
int 
my_getstr(
    char ** lineptr, size_t *n, FILE * stream, char terminator, int offset)
{
    // This function copied from
    // https://code.google.com/p/ea-utils/source/browse/trunk/clipper/getline.c

    int nchars_avail;             /* Allocated but unused chars in *LINEPTR.  */
    char *read_pos;               /* Where we're reading into *LINEPTR. */
    int ret;

    if ( !lineptr || !n || !stream )
        return -1;

    if ( !*lineptr ) {
        *n = 64;
        if ( (*lineptr = (char *) malloc (*n)) == NULL ) {
            return -1;
        }
    }

    nchars_avail = *n - offset;
    read_pos = *lineptr + offset;

    for (;;)
    {
        register int c = getc (stream);

        /* We always want at least one char left in the buffer, since we
           always (unless we get an error while reading the first char)
           NUL-terminate the line buffer.  */

        if (nchars_avail < 1) {
            if (*n > 64)
                *n *= 2;
              else
                *n += 64;

            nchars_avail = *n + *lineptr - read_pos;
            *lineptr = (char *) realloc (*lineptr, *n);
            if (!*lineptr)
                return -1;
            read_pos = *n - nchars_avail + *lineptr;
        }

        if (c == EOF || ferror (stream)) {
            /* Return partial line, if any.  */
            if (read_pos == *lineptr)
                return -1;
            else
                break;
        }

        *read_pos++ = c;
        nchars_avail--;

        if (c == terminator)
            /* Return the line.  */
            break;
    }

    /* Done - NUL terminate and return the number of chars read.  */
    *read_pos = '\0';

    ret = read_pos - (*lineptr + offset);
    return ret;
}
// -- fileScope -------------------------------------------------------------
// my_getline()
//     Replacement for GNU C library function getline
// Returns:
//     Return the number of characters read (not including the
//     null terminator), or -1 on error or EOF. 
// -------------------------------------------------------------------------*/
ssize_t 
my_getline(
    char **lineptr, size_t *n, FILE *stream)
{
    // This function copied from
    // https://code.google.com/p/ea-utils/source/browse/trunk/clipper/getline.c
    return my_getstr (lineptr, n, stream, '\n', 0);
}
// -- fileScope -------------------------------------------------------------
// wash_string()
//     Replaces HTML escape codes from string and replaces with ascii chars
// Returns:
//     NULL if output string allocation failed
//     Returns the washed string otherwise
// -------------------------------------------------------------------------*/
char *
wash_string(
    char *in)
{
    char *out;
    int i = 0, j = 0;
    if ( (out = malloc(strlen(in))) == NULL ) {
        return out; 
    }
    while ( in[i] != '\0' ) {
        if ( in[i] == '%' ) {
            if ( in[i+1] == '2' ) {
                switch (in[i+2]) {
                    case '0': out[j] = ' ';
                        break;
                    case '1': out[j] = '!';
                        break;
                    case '2': out[j] = '"';
                        break;
                    case '3': out[j] = '#';
                        break;
                    case '4': out[j] = '$';
                        break;
                    case '5': out[j] = '%';
                        break;
                    case '6': out[j] = '%';
                        break;
                    case '7': out[j] = '\'';
                        break;
                    case '8': out[j] = '(';
                        break;
                    case '9': out[j] = ')';
                        break;
                    case 'A': out[j] = '*';
                        break;
                    case 'B': out[j] = '+';
                        break;
                    case 'C': out[j] = ',';
                        break;
                    case 'D': out[j] = '-';
                        break;
                    case 'E': out[j] = '.';
                        break;
                    case 'F': out[j] = '/';
                        break;
                    default:
                        free(out);
                        return NULL;
                }
            } else if ( in[i+1] == '3' ) {
                switch (in[i+2]) {
                    case 'A': out[j] = ':';
                        break;
                    case 'B': out[j] = ';';
                        break;
                    case 'C': out[j] = '<';
                        break;
                    case 'D': out[j] = '=';
                        break;
                    case 'E': out[j] = '>';
                        break;
                    case 'F': out[j] = '?';
                        break;
                    default:
                        free(out);
                        return NULL;
                }
            } else if ( in[i+1] == '4' ) {
                switch (in[i+2]) {
                    case '0': out[j] = '@';
                        break;
                    default:
                        free(out);
                        return NULL;
                }
            } else if ( in[i+1] == '5' ) {
                switch (in[i+2]) {
                    case 'B': out[j] = '[';
                        break;
                    case 'C': out[j] = '\\';
                        break;
                    case 'D': out[j] = ']';
                        break;
                    case 'E': out[j] = '^';
                        break;
                    case 'F': out[j] = '_';
                        break;
                    default:
                        free(out);
                        return NULL;
                }
            } else if ( in[i+1] == '7' ) {
                switch (in[i+2]) {
                    case 'B': out[j] = '{';
                        break;
                    case 'C': out[j] = '|';
                        break;
                    case 'D': out[j] = '}';
                        break;
                    case 'E': out[j] = '~';
                        break;
                    default:
                        free(out);
                        return NULL;
                }
            } else {
                free(out);
                return NULL;
            }
            i += 2;
        } else {
            out[j] = in[i];
        }
        i++;
        j++;
    }
    return out;
}
// -- fileScope -------------------------------------------------------------
// parse_form()
//     Parses the form data posted by browser and sets up a Tag-Value structure
// Returns:
//     None
// -------------------------------------------------------------------------*/
void
parse_form(
    char *input, form_tag_value_list *ftvl)
{

    char *in_string = input; 
    char *tv, *tag, *value, *saveptr;
    int i;

    while ( 1 ) {
        if ( (tv = strtok_r(in_string, "&", &saveptr)) == NULL ) {
            break;
        }
        tag = strtok(tv, "=");
        value = strtok(NULL, "");
        if ( tag != NULL && value != NULL ) {
            for ( i = 0; i < ftvl->num_elements; i++ ) {
                if ( strcmp(ftvl->elements[i].tag, tag) == 0 ) {
                    if ( strcmp(ftvl->elements[i].fmt, "%c") == 0 ) {
                        sscanf(value, ftvl->elements[i].fmt, &ftvl->elements[i].char_val);
                    } else if ( strcmp(ftvl->elements[i].fmt, "%d") == 0 ) {
                        sscanf(value, ftvl->elements[i].fmt, &ftvl->elements[i].int_val);
                    } else if ( strcmp(ftvl->elements[i].fmt, "%ld") == 0 ) {
                        sscanf(value, ftvl->elements[i].fmt, &ftvl->elements[i].long_val);
                    } else if ( strcmp(ftvl->elements[i].fmt, "%s") == 0 ) {
                        ftvl->elements[i].string_val = wash_string(value);
                    }
                    break;
                }
            }
        }
        in_string = NULL;
    }
}
// -- fileScope -------------------------------------------------------------
// init_ftvl()
//     Initializes the value elements in the tag-value structure
// Returns:
//     None
// -------------------------------------------------------------------------*/
void
init_ftvl(
    form_tag_value_list *ftvl)
{
    int i;
    // Init the values
    for ( i = 0; i < ftvl->num_elements; i++ ) {
        if ( strcmp(ftvl->elements[i].fmt, "%c") == 0 ) {
            ftvl->elements[i].char_val = 0;
        } else if ( strcmp(ftvl->elements[i].fmt, "%d") == 0 ) {
            ftvl->elements[i].int_val = 0;
        } else if ( strcmp(ftvl->elements[i].fmt, "%ld") == 0 ) {
            ftvl->elements[i].long_val = 0;
        } else if ( strcmp(ftvl->elements[i].fmt, "%s") == 0 ) {
            ftvl->elements[i].string_val = NULL;
        }
    }
}
// -- fileScope -------------------------------------------------------------
// print_ftvl()
//     Utility function to print tag-value structure
// Returns:
//     None
// -------------------------------------------------------------------------*/
void
print_ftvl(
    form_tag_value_list *ftvl)
{
    int i;
    for ( i = 0; i < ftvl->num_elements; i++ ) {
        printf("%s:", ftvl->elements[i].tag);
        if ( strcmp(ftvl->elements[i].fmt, "%c") == 0 ) {
            printf(ftvl->elements[i].fmt, ftvl->elements[i].char_val);
        } else if ( strcmp(ftvl->elements[i].fmt, "%d") == 0 ) {
            printf(ftvl->elements[i].fmt, ftvl->elements[i].int_val);
        } else if ( strcmp(ftvl->elements[i].fmt, "%ld") == 0 ) {
            printf(ftvl->elements[i].fmt, ftvl->elements[i].long_val);
        } else if ( strcmp(ftvl->elements[i].fmt, "%s") == 0 ) {
            if ( ftvl->elements[i].string_val != NULL ) {
                printf(ftvl->elements[i].fmt, ftvl->elements[i].string_val);
            } 
        }
        printf("\n");
    }
    return;
}
// -- fileScope -------------------------------------------------------------
// configure_sprinkler()
//     Use the form data from browser to update sprinkler database
// Returns:
//     None
// -------------------------------------------------------------------------*/
void
configure_sprinkler(
    MYSQL *sqlcon, form_tag_value_list *ftvl)
{
    char *lenstr = NULL;
    char *input = NULL;
    long len;

    if ( (lenstr = getenv("CONTENT_LENGTH")) == NULL ) {
        printf("<p>1.Ooops...someting went wrong! Please try again\n</p>");
        return;
    }
    if ( sscanf(lenstr,"%ld",&len) != 1 ) {
        printf("<p>2.Ooops...someting went wrong! Please try again\n</p>");
        return;
    }
    // Allocate memory for the request message string
    if ( (input = malloc(len)) == NULL ) {
        printf("<p>3.Ooops...someting went wrong! Please try again\n</p>");
        return;
    }
    // Read the POSTed data
    if ( fgets(input, (len + 1), stdin) == NULL ) {
        printf("<p>4.Ooops...someting went wrong! Please try again\n</p>");
        return;
    }
    printf("<p>%s</p>\n", input);

    // initialize the tag-value pairs
    init_ftvl(ftvl);

    // Parse the form into tag value pair
    parse_form(input, ftvl);

    // free the input string
    free(input); 
    
    print_ftvl(ftvl);

    // Update database
    int i;
    long sprinkler_id = -1;
    for ( i = 0; i < ftvl->num_elements; i++ ) {
        if ( strcmp(ftvl->elements[i].tag, "sid") == 0 ) {
            sprinkler_id = ftvl->elements[i].long_val;
        }
    }
    char query[1024];
    sprintf(query, "SELECT sid FROM spconfig WHERE sid=%ld", sprinkler_id);
    if ( mysql_query(sqlcon, query) ) {
        printf("<br>Error: Sprinkler not found\n");
        return;
    }
    MYSQL_RES *result;
    if ( (result = mysql_store_result(sqlcon)) == NULL ) {
        printf("Error storing result\n");
        return;
    }
    if ( mysql_num_rows(result) == 0 ) {
        printf("Error: Sprinkler not found\n");
        return;
    }
    
    for ( i = 0; i < ftvl->num_elements; i++ ) {
        if ( strcmp(ftvl->elements[i].tag, "sid") == 0 ) {
            continue;
        }
        if ( strcmp(ftvl->elements[i].fmt, "%c") == 0 ) {
            sprintf(query, "UPDATE spconfig SET %s=%c WHERE sid=%ld", 
                    ftvl->elements[i].tag, ftvl->elements[i].char_val, sprinkler_id);
        } else if ( strcmp(ftvl->elements[i].fmt, "%d") == 0 ) {
            sprintf(query, "UPDATE spconfig SET %s=%d WHERE sid=%ld", 
                    ftvl->elements[i].tag, ftvl->elements[i].int_val, sprinkler_id);
        } else if ( strcmp(ftvl->elements[i].fmt, "%ld") == 0 ) {
            sprintf(query, "UPDATE spconfig SET %s=%ld WHERE sid=%ld", 
                    ftvl->elements[i].tag, ftvl->elements[i].long_val, sprinkler_id);
        } else if ( strcmp(ftvl->elements[i].fmt, "%s") == 0 ) {
            if ( ftvl->elements[i].string_val == NULL ) {
                continue;
            }
            sprintf(query, "UPDATE spconfig SET %s=\'%s\' WHERE sid=%ld", 
                    ftvl->elements[i].tag, ftvl->elements[i].string_val, sprinkler_id);
        }
        if ( mysql_query(sqlcon, query) ) {
            printf("<br>Database update failed\n");
        }
        printf("<br>%s\n", query);
    }
        printf("<br>\n");

    return;
}
// -- fileScope -------------------------------------------------------------
// init_sprinkler_configuration_form()
//     Uses configuration value to read tag and types for the form data into
//     a tag-value structure
// Returns:
//     tag-value structure on success; null otherwise
// -------------------------------------------------------------------------*/
form_tag_value_list *
init_sprinkler_configuration_form(
    FILE *fp)
{
    form_tag_value_list *ftvl = NULL;
    int len = 0;
    char *line = NULL;
    size_t ll;

    while ( feof(fp) == 0 ) {
        if ( fgetc(fp) == '\n' ) {
            len++;
        }
    }
    rewind(fp);

    // Allocate tag-value elements
    if ( (ftvl = malloc(sizeof(form_tag_value_list) +
                        len * sizeof(form_tag_value_element))) == NULL ) {
        return NULL;
    }

    // Read in the configuration file holding the form tag and formats
    while ( my_getline(&line, &ll, fp) != -1 ) {
        line[strlen(line) - 1] = '\0';
        // Split the line into tag and format
        ftvl->elements[ftvl->num_elements].tag = strtok(line, ",");
        ftvl->elements[ftvl->num_elements].fmt = strtok(NULL, "");
        ftvl->num_elements++;
        line = NULL;
        ll = 0;
    }
    init_ftvl(ftvl);
    return ftvl;
}
// -- fileScope -------------------------------------------------------------
// main()
//     Main FCGI loop - accepts requests to update sprinkler configuration
// Returns:
//     None
// -------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    FILE *fp;
    form_tag_value_list *ftvl = NULL;
    char *username = NULL, *password = NULL;
    MYSQL *sqlcon;
 
    // Check if configuration file exits and mysql password provided
    if ( (argc != 4) || ((fp = fopen(argv[1], "r")) == NULL) ) {
        exit(1);
    }
    username = strdup(argv[2]);
    password = strdup(argv[3]);
    ftvl = init_sprinkler_configuration_form(fp);
    fclose(fp);
    if ( ftvl == NULL ) {
        exit(1);
    }
    sqlcon = mysql_init(NULL);
    if ( mysql_real_connect(sqlcon, "localhost", username, password, 
                            "sprinkler", 0, NULL, 0) == NULL ) {
        mysql_close(sqlcon);
        exit(1);
    }

    while(FCGI_Accept() >= 0) {
        printf("Content-type: text/html\r\n"
               "\r\n"
               "<title>Sprinkler Configuration Page</title>");
        printf("%s", getenv("REQUEST_URI"));
        if ( strncmp(getenv("REQUEST_URI"), "/scripts/configure", strlen("/scripts/configure")) == 0 ) {
            configure_sprinkler(sqlcon, ftvl);
        }
    }
    return 0;
}
