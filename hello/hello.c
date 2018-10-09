#include <stdio.h>
#include <stdlib.h>

#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

int main()
{
  /* Setting the i18n environment */
  setlocale (LC_ALL, "");
  bindtextdomain ("hello",  getenv("PWD"));
  textdomain ("hello");

  /* Example of i18n usage */
  printf(_("Hello World\n"));

  return EXIT_SUCCESS;
}
