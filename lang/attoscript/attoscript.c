/* Attoscript
 * Version 0.1 (18/8-07)
 * Copyright 2007 Kjetil Erga (kobolt.anarion -AT- gmail -DOT- com)
 * 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* For seeding the randomizer. */
#include <unistd.h> /* For sleep(). */

#define ATTOSCRIPT_LINESIZE 1024
#define ATTOSCRIPT_MAX_VARS 64
#define ATTOSCRIPT_VARNAME_LEN 32
#define ATTOSCRIPT_SPACE_ENCODING 0x01
#define ATTOSCRIPT_INTERPOLATE_CHAR '$'



typedef struct attoscript_var_s {
  char name[ATTOSCRIPT_VARNAME_LEN];
  int value;
} attoscript_var_t;

static attoscript_var_t vars[ATTOSCRIPT_MAX_VARS];
static int var_counter = 0;



/* An "alpha" is a variable or label name. */
static int attoscript_str_is_alpha(char *string)
{
  int i;

  if (strlen(string) == 0)
    return 0;

  for (i = 0; i < strlen(string); i++)
    if (! isalpha(string[i]))
      return 0;

  return 1;
}



static int attoscript_str_is_numeric(char *string)
{
  int i;

  if (strlen(string) == 0)
    return 0;

  if (string[0] == '-') /* Accept leading minus. */
    i = 1;
  else
    i = 0;

  for (; i < strlen(string); i++)
    if (! isdigit(string[i]))
      return 0;

  return 1;
}



static int attoscript_str_is_string(char *string)
{
  if (strlen(string) <= 1)
    return 0;

  if (string[0] == '\"') {
    if (string[strlen(string)-1] == '\"') {
      return 1;
    }
  }
    
  return 0;
}



static void attoscript_set_var(char *name, int value)
{
  int i;

  for (i = 0; i < var_counter; i++) {
    if (strcmp(name, vars[i].name) == 0) {
      vars[i].value = value;
      return;
    }
  }

  fprintf(stderr, "Warning: Variable '%s' does not exist!\n", name);
}



static int attoscript_get_var(char *name)
{
  int i;

  for (i = 0; i < var_counter; i++) {
    if (strcmp(name, vars[i].name) == 0) {
      return vars[i].value;
    }
  }

  fprintf(stderr, "Warning: Variable '%s' does not exist!\n", name);
  return 0;
}



static void attoscript_convert_string(char *string, int space_used)
{
  int i, j, n, n2;
  char var_name[ATTOSCRIPT_VARNAME_LEN];
  char var_value[16]; /* Should be large enough for an integer. */
  int in_interpolation = 0;

  n = 0;
  n2 = 0;
  for (i = 0; i < strlen(string); i++) {
    if (string[i] == ATTOSCRIPT_SPACE_ENCODING)
      string[i] = ' ';
    if (string[i] == '\"') {
      /* Do nothing. */
    } else {
      if (in_interpolation) {
        var_name[n2] = string[i];
        n2++;
        if (n2 > ATTOSCRIPT_VARNAME_LEN) {
          fprintf(stderr, "Warning: Interpolated variable name too long!\n");
          in_interpolation = 0;
          n2 = 0;
        } else if (string[i] == ATTOSCRIPT_INTERPOLATE_CHAR) {
          var_name[n2 - 1] = '\0';
          in_interpolation = 0;
          n2 = 0;
          snprintf(var_value, sizeof(var_value), "%d",
            attoscript_get_var(var_name));
          for (j = 0; j < strlen(var_value); j++) {
            string[n] = var_value[j];
            n++;
            /* Measure to prevent buffer overflow. */
            if (n == ATTOSCRIPT_LINESIZE - space_used - 2) {
              fprintf(stderr, "Warning: Interpolation overflow!\n");
              string[n] = '\0';
              return;
            }
          }
        }
      } else if (string[i] == ATTOSCRIPT_INTERPOLATE_CHAR) {
        in_interpolation = 1;
      } else {
        string[n] = string[i];
        n++;
        if (n == ATTOSCRIPT_LINESIZE - space_used - 2) {
          fprintf(stderr, "Warning: Interpolation overflow!\n");
          string[n] = '\0';
          return;
        }
      }
    }
  }
  string[n] = '\0'; /* New termination. */
}



static void attoscript_declare_var(char *name)
{
  int i;

  for (i = 0; i < var_counter; i++) {
    if (strcmp(name, vars[i].name) == 0) {
      fprintf(stderr, "Warning: Variable name '%s' already in use!\n", name);
      return;
    }
  }

  if (var_counter > ATTOSCRIPT_MAX_VARS) {
    fprintf(stderr, "Warning: Maximum amount of variables reached!\n");
    return;
  }

  if (! attoscript_str_is_alpha(name)) {
    fprintf(stderr,
      "Warning: Variable name '%s' contains non-alphabetic characters\n", name);
    return;
  }

  strncpy(vars[var_counter].name, name, ATTOSCRIPT_VARNAME_LEN);
  vars[var_counter].value = 0; /* Always starts as zero. */
  var_counter++;
}



static int attoscript_index_by_label(char *label, char *script, int max)
{
  int n;
  char target[ATTOSCRIPT_LINESIZE];

  snprintf(&target[0], ATTOSCRIPT_LINESIZE, "label %s", label);

  n = 0;
  while (n < max) {
    if (strcmp(target, &script[n]) == 0)
      return n;
    n++;
  }

  fprintf(stderr, "Warning: Label '%s' not found!\n", label);
  return -1;
}



int main(int argc, char *argv[])
{
  FILE *fh;
  int i, n;
  int string_flag;
  char input[ATTOSCRIPT_LINESIZE];
  char expression[ATTOSCRIPT_LINESIZE];

  char *script = NULL; /* Start as NULL to make realloc() act like malloc(). */
  int script_index = 0;
  int script_size;

  int en;
  int old_en = -1;
  char ep_buffer[ATTOSCRIPT_LINESIZE];
  char *ep;
  char *cmd;
  char *param;
  char *param_2;
  char *param_3;
  char *param_4;
  int temp;
  int temp_2;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <scriptfile>\n", argv[0]);
    exit(1);
  }

  fh = fopen(argv[1], "r");
  if (! fh) {
    fprintf(stderr, "Error: Could not open scriptfile.\n");
    exit(1);
  }

  srand(time(NULL));

  while (fgets(&input[0], ATTOSCRIPT_LINESIZE, fh) != NULL) {
    /* Filter input: */
    n = 0;
    string_flag = 0;
    for (i = 0; i < strlen(input); i++) {
      if (! string_flag) {
        if (isupper(input[i])) {
          expression[n] = tolower(input[i]);
          n++;
        } else if (islower(input[i])) {
          expression[n] = input[i];
          n++;
        } else if (isdigit(input[i])) {
          expression[n] = input[i];
          n++;
        } else if (input[i] == ' ') {
          if (n != 0) { /* Skip leading spaces. */
            expression[n] = ' ';
            n++;
          }
        } else if (input[i] == '\"') {
          string_flag = 1; /* Always 1 since we are not inside a string now. */
          expression[n] = '"';
          n++;
        } else if (input[i] == '#') { /* Comment. */
          break;
        }
      } else {
        /* Avoid non-printing characters in strings, including newlines. */
        if (isprint(input[i])) {
          /* Space must be encoded to fool strtok() later. */
          if (input[i] == ' ')
            expression[n] = ATTOSCRIPT_SPACE_ENCODING;
          else
            expression[n] = input[i];
          n++;
        }
        if (input[i] == '\"')
          string_flag = 0;
      }
    }

    if (string_flag == 1)
      fprintf(stderr, "Warning: Unterminated string found.\n");

    if (n != 0) { /* Do not store "empty" lines. */
      expression[n++] = '\0'; /* The NULL separates each line. */
      script = (char *)realloc(script, (script_index + n) * sizeof(char));
      strncpy(&script[script_index], &expression[0], n);
      script_index += n;
    }
  }
  script_size = script_index;
  fclose(fh);

  /* Start exeuction: */
  en = 0; /* Execution begins at top. */
  while (1) {
    if (en >= script_size)
      exit(0); /* Moved beyond size of script, terminate. */

    /* Copy to safe buffer, to void strtok() to destroy the original script. */
    strncpy(&ep_buffer[0], &script[en], ATTOSCRIPT_LINESIZE);
    ep = &ep_buffer[0]; /* Set expression/instruction pointer every line. */
    en += strlen(ep) + 1; /* Set next expression to next line by default. */

    cmd = strtok(ep, " ");
    if (cmd == NULL)
      continue;
    ep = NULL; /* Must be NULL in subsequent calls to strtok(). */
    
    /* Parse command: */
    if (strcmp(cmd, "end") == 0) {
      exit(0);

    } else if (strcmp(cmd, "var") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot define variable, no name given.\n");
        continue;
      }
      attoscript_declare_var(param);

    } else if (strcmp(cmd, "set") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot set variable, no name given.\n");
        continue;
      }
      if ((param_2 = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot set variable, no value given.\n");
        continue;
      }
      if (attoscript_str_is_alpha(param_2)) {
        attoscript_set_var(param, attoscript_get_var(param_2));
      } else if (attoscript_str_is_numeric(param_2)) {
        attoscript_set_var(param, atoi(param_2));
      } else {
        fprintf(stderr, "Warning: Cannot set variable, unknown value.\n");
      }

    } else if (strcmp(cmd, "echo") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot echo, no variable or string given.\n");
        continue;
      }
      if (attoscript_str_is_alpha(param)) {
        printf("%s: %d\n", param, attoscript_get_var(param));
      } else if (attoscript_str_is_string(param)) {
        /* A hacky way to calculate space used in static string so far
           is to find the delta value between the two pointers. */
        attoscript_convert_string(param, (param - cmd));
        printf("%s\n", param);
      }

    } else if (strcmp(cmd, "sleep") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot sleep, no time (in seconds) given.\n");
        continue;
      }
      sleep(atoi(param));

    } else if (strcmp(cmd, "incr") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot increment, no variable given.\n");
        continue;
      }
      attoscript_set_var(param, attoscript_get_var(param) + 1);

    } else if (strcmp(cmd, "decr") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot decrement, no variable given.\n");
        continue;
      }
      attoscript_set_var(param, attoscript_get_var(param) - 1);

    } else if (strcmp(cmd, "rand") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot randomize variable, no name given.\n");
        continue;
      }
      if ((param_2 = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot randomize variable, no 'from' value given.\n");
        continue;
      }
      if ((param_3 = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot randomize variable, no 'to' value given.\n");
        continue;
      }
      if (atoi(param_2) >= atoi(param_3)) {
        fprintf(stderr,
          "Warning: Cannot randomize variable, 'from' is higher than 'to'.\n");
        continue;
      }
      attoscript_set_var(param,
        (rand() % (atoi(param_3) - atoi(param_2) + 1)) + atoi(param_2));

    } else if (strcmp(cmd, "input") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr, "Warning: Cannot gather input, no variable given.\n");
        continue;
      }
      fscanf(stdin, "%d", &temp);
      attoscript_set_var(param, temp);

    } else if (strcmp(cmd, "sys") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot run system command, no variable given.\n");
        continue;
      }
      if ((param_2 = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot run system command, no 'command' given.\n");
        continue;
      }
      attoscript_convert_string(param_2, (param_2 - cmd));
      temp = system(param_2) >> 8; /* Shift to get the real return code. */
      attoscript_set_var(param, temp);

    } else if (strcmp(cmd, "label") == 0) {
      /* Do nothing, execution may jump to this point later. */

    } else if (strcmp(cmd, "goto") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot redirect execution, no label given.\n");
        continue;
      }
      temp = attoscript_index_by_label(param, script, script_size);
      if (temp != -1) {
        old_en = en;
        en = temp;
      }

    } else if (strcmp(cmd, "if") == 0) {
      if ((param = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot check condition, first value/variable missing.\n");
        continue;
      }
      if ((param_2 = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot check condition, no comparison operator given.\n");
        continue;
      }
      if ((param_3 = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot check condition, second value/variable missing.\n");
        continue;
      }
      if ((param_4 = strtok(ep, " ")) == NULL) {
        fprintf(stderr,
          "Warning: Cannot check condition, no label given.\n");
        continue;
      }
      if (attoscript_str_is_alpha(param)) {
        temp = attoscript_get_var(param);
      } else if (attoscript_str_is_numeric(param)) {
        temp = atoi(param);
      } else {
        fprintf(stderr, "Warning: Cannot check condition, unknown value.\n");
        continue;
      }
      if (attoscript_str_is_alpha(param_3)) {
        temp_2 = attoscript_get_var(param_3);
      } else if (attoscript_str_is_numeric(param_3)) {
        temp_2 = atoi(param_3);
      } else {
        fprintf(stderr, "Warning: Cannot check condition, unknown value.\n");
        continue;
      }
      if (strcmp(param_2, "eq") == 0) {
        if (temp == temp_2) {
          temp = attoscript_index_by_label(param_4, script, script_size);
          if (temp != -1) {
            old_en = en;
            en = temp;
          }
        }
      } else if (strcmp(param_2, "ne") == 0) {
        if (temp != temp_2) {
          temp = attoscript_index_by_label(param_4, script, script_size);
          if (temp != -1) {
            old_en = en;
            en = temp;
          }
        }
      } else if (strcmp(param_2, "gt") == 0) {
        if (temp > temp_2) {
          temp = attoscript_index_by_label(param_4, script, script_size);
          if (temp != -1) {
            old_en = en;
            en = temp;
          }
        }
      } else if (strcmp(param_2, "lt") == 0) {
        if (temp < temp_2) {
          temp = attoscript_index_by_label(param_4, script, script_size);
          if (temp != -1) {
            old_en = en;
            en = temp;
          }
        }
      } else {
        fprintf(stderr,
          "Warning: Cannot check condition, unknown comparison operator.\n");
        continue;
      }

    } else if (strcmp(cmd, "return") == 0) {
      if (old_en == -1)
        fprintf(stderr, "Warning: no previous position remembered.\n");
      else {
        en = old_en;
        old_en = -1;
      }

    } else {
      fprintf(stderr, "Warning: Unknown command '%s'.\n", cmd);
    }
  }

  return 0;
}

