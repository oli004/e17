#include "../src/Edb.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static int
sort_compare(const void *v1, const void *v2);

static int
sort_compare(const void *v1, const void *v2)
{
   return strcmp(*(char **)v1, *(char **)v2);
}

static int
unescape_string(char *str)
{
  int  i, len = 0;
  unsigned char val = 0;
  char c;
  char *s;

  for (s = str ; *str != '\0'; str++, s++)
    {
      val = 0;
      len++;

      if (*str == '\\')
	{	 
	  if ((c = *(str+1)) == '\0')
	    break;

	  switch (c)
	    {
	    case 'a':       /* bell */
	      *s = '\a';
	      break;
	    case 'b':       /* backspace */
	      *s = '\b';
	      break;
	    case 'f':       /* formfeed */
	      *s = '\f';
	      break;
	    case 'n':       /* newline */
	      *s = '\n';
	      break;
	    case 'r':       /* return */
	      *s = '\r';
	      break;
	    case 't':       /* tab */
	      *s = '\t';
	      break;
	    case 'v':       /* vtab */
	      *s = '\v';
	      break;
	    case 'x':       /* \xn or \xnn -- hex value */	      
	      {		  
		char digit;
		char *ptr;
		
		ptr = str;
		
		for (i = 0; i < 2; i++)
		  {
		    if (*(str+2+i) == '\0')
		      break;
		    
		    digit = *(str+2+i); 
		    
		    if (!isxdigit(digit))
		      break;
		    
		    val *= 16;
		    
		    if (digit >= 97)
		      val += 10 + (digit - 'a');
		    else if (digit >= 65)
		      val += 10 + (digit - 'A');
		    else
		      val += digit - '0';
		    
		    *s = val;
		    ptr++;
		  }
		
		str = --ptr;
	      }
	      break;
	    case '0':       /* \nnn -- octal value */
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	      {		  
		char digit;
		char *ptr;
		
		ptr = str;
		
		for (i = 0; i < 3; i++)
		  {
		    if (*(str+1+i) == '\0')
		      break;
		    
		    digit = *(str+1+i); 
		    
		    if (!isdigit(digit))
		      break;

		    val *= 8;
		    val += (digit - '0');
		    
		    *s = val;
		    ptr++;
		  }
		
		str = --ptr;
	      }
	      break;
	    default:
	      /* Everything else -- just write what's after backslash. */
	      *s = *(str+1);
	      break;
	  }
	  str++;
        }
      else
	{
	  *s = *str;
	}
    }
  
  *s = '\0';
  return len;
}


int
main(int argc, char **argv)
{
   int i;
   int add = 0, del = 0, get = 0;
   char *key = NULL;
   char *type = NULL;
   char *data = NULL;
   char *dbfile = NULL;
   E_DB_File *db;
   
   for (i = 1; i < argc; i++)
     {
        if ((!strcmp(argv[i], "add")) && (i < (argc - 3)))
	  {
	     add = 1;
	     i++;
	     key = argv[i];
	     i++;
	     type = argv[i];
	     i++;
	     data = argv[i];
	  }
        else if ((!strcmp(argv[i], "get")) && (i < (argc - 2)))
	  {
	     get = 1;
	     i++;
	     key = argv[i];
	     i++;
	     type = argv[i];
	  }
        else if ((!strcmp(argv[i], "del")) && (i < (argc - 1)))
	  {
	     del = 1;
	     i++;
	     key = argv[i];
	  }
	else if ((!strcmp(argv[i], "-h")) ||
		 (!strcmp(argv[i], "-help")) ||
		 (!strcmp(argv[i], "--h")) ||
		 (!strcmp(argv[i], "--help")))
	  {
	     printf("Usage:\n"
		    "List keys & types: %s database_file.db\n"
		    "Add / Set value:   %s database_file.db add [key] [str|int|float] value\n"
		    "Get value:         %s database_file.db get [key] [str|int|float]\n"
		    "Delete value:      %s database_file.db del [key]\n"
		    ,
		    argv[0], argv[0], argv[0], argv[0]);
	     exit(1);
	  }
	else
	   dbfile = argv[i];
     }
   if (!dbfile)
     {
	fprintf(stderr, "No database file specified!\n  %s -h for details\n", argv[0]);
	exit(-1);
     }
   if (((add) || (del) || (get)) && (dbfile) && (!key))
     {
	fprintf(stderr, "No key specified!\n  %s -h for details\n", argv[0]);
	exit(-1);
     }
   if ((add) && (dbfile) && (!data))
     {
	fprintf(stderr, "No data specified!\n  %s -h for details\n", argv[0]);
	exit(-1);
     }
   if ((add) || (del) || (get))
     {
	db = e_db_open(dbfile);
	if (!db)
	  {
	     fprintf(stderr, "Database file %s cannot be opened!\n  %s -h for details\n", dbfile, argv[0]);
	     exit(-1);
	  }
	if (add)
	  {
	     if (!strcmp(type, "int"))
	       {
		  e_db_int_set(db, key, atoi(data));
	       }
	     else if (!strcmp(type, "str"))
	       {
		  int len;

		  len = unescape_string(data);
		  e_db_bytestr_set(db, key, data, len);
	       }
	     else if (!strcmp(type, "float"))
	       {
		  e_db_float_set(db, key, (float)atof(data));
	       }
	     else
	       {
		  fprintf(stderr, "Unknown type %s!\n  %s -h for details\n", type, argv[0]);
		  exit(-1);
	       }
	  }
	else if (del)
	  {
	     e_db_data_del(db, key);
	  }
	else if (get)
	  {
	     if (!strcmp(type, "int"))
	       {
		  int data;
		  
		  if (e_db_int_get(db, key, &data))
		    {
		       printf("%i\n", data);
		    }
		  else
		    {
		       fprintf(stderr, "Key %s does not exist!\n  %s -h for details\n", key, argv[0]);
		       exit(-1);
		    }
	       }
	     else if (!strcmp(type, "str"))
	       {
		  char *data;
		  
		  data = e_db_str_get(db, key);
		  if (data)
		    {
		       printf("%s\n", data);
		    }
		  else
		    {
		       fprintf(stderr, "Key %s does not exist!\n  %s -h for details\n", key, argv[0]);
		       exit(-1);
		    }
	       }
	     else if (!strcmp(type, "float"))
	       {
		  float data;
		  
		  if (e_db_float_get(db, key, &data))
		    {
		       printf("%1.6f\n", data);
		    }
		  else
		    {
		       fprintf(stderr, "Key %s does not exist!\n  %s -h for details\n", key, argv[0]);
		       exit(-1);
		    }
	       }
	     else
	       {
		  fprintf(stderr, "Unknown type %s!\n  %s -h for details\n", type, argv[0]);
		  exit(-1);
	       }
	  }
	e_db_close(db);
     }
   else
     {
	char **keys;
	int keys_num;
	
	db = e_db_open(dbfile);
	if (!db)
	  {
	     fprintf(stderr, "Database file %s cannot be opened!\n  %s -h for details\n", dbfile, argv[0]);
	     exit(-1);
	  }
	keys = e_db_dump_key_list(dbfile, &keys_num);
	printf("---------------------------------------------------------------\n");
	printf("Keys in Database: %s\n", dbfile);
	printf("\n");
	printf("[   type   ] key\n");
	printf("---------------------------------------------------------------\n");
	printf("\n");
	qsort(keys, keys_num, sizeof(char *), sort_compare);
	for (i = 0; i < keys_num; i++)
	  {
	     char *t;
	     
	     type = e_db_type_get(db, keys[i]);
	     if (type) t = type;
	     else t = "?";
	     printf("[ %8s ] %s\n", t, keys[i]);
	     if (type) free(type);
	  }
	e_db_close(db);
     }
   e_db_flush();
   return 0;
}
