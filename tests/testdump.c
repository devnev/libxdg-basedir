/* Copyright (c) 2007 Mark Nevill
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <basedir.h>
#include <stdio.h>
#include <stdlib.h>

static void print_item_list(const char * const * item_list, int do_free)
{
  const char * const * item;
  for (item = item_list; *item; item++) {
    printf("%s%c", *item, (item[1] ? ':' : '\n'));
    if (do_free) free((void *)*item);
  }
  if (do_free) free((void *)item_list);
}

static void do_test(xdgHandle *handle) {
  int do_free = (handle == NULL);

  char *dataHome = xdgDataHome(handle);
  printf("${XDG_DATA_HOME:-$HOME/.local/share}=%s\n", dataHome);
  if (do_free) free(dataHome);

  char *configHome = xdgConfigHome(handle);
  printf("${XDG_CONFIG_HOME:-$HOME/.config}=%s\n", configHome);
  if (do_free) free(configHome);

  printf("${XDG_DATA_DIRS:-/usr/local/share/:/usr/share/}=");
  print_item_list(xdgDataDirectories(handle), do_free);

  printf("${XDG_DATA_HOME:-$HOME/.local/share}:${XDG_DATA_DIRS:-/usr/local/share/:/usr/share/}=");
  print_item_list(xdgSearchableDataDirectories(handle), do_free);

  printf("${XDG_CONFIG_DIRS:-/etc/xdg}=");
  print_item_list(xdgConfigDirectories(handle), do_free);

  printf("${XDG_CONFIG_HOME:-$HOME/.config}:${XDG_CONFIG_DIRS:-/etc/xdg}=");
  print_item_list(xdgSearchableConfigDirectories(handle), do_free);

  char *cacheHome = xdgCacheHome(handle);
  printf("${XDG_CACHE_HOME:-$HOME/.cache}=%s\n", cacheHome);
  if (do_free) free(cacheHome);
}

int main(int argc, char* argv[])
{
	xdgHandle handle;

        printf("UNCACHED\n");
        do_test(0);
	if (!xdgInitHandle(&handle)) return 1;
        printf("CACHED\n");
        do_test(&handle);
	xdgWipeHandle(&handle);
        printf("DONE\n");
	return 0;
}
