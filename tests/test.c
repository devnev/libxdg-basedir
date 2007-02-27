#include <basedir.h>
#include <stdio.h>

main(int argc, char* argv[])
{
	const char * const * item;
	xdgHandle handle = xdgAllocHandle();
	printf("${XDG_DATA_HOME:-$HOME/.local/share}=%s\n", xdgDataHome(handle));
	printf("${XDG_CONFIG_HOME:-$HOME/.config}=%s\n", xdgConfigHome(handle));
	printf("${XDG_DATA_DIRS:-/usr/local/share/:/usr/share/}=");
	for (item = xdgDataDirectories(handle); *item; item++)
		printf("%s%c", *item, (item[1] ? ':' : '\n'));
	printf("${XDG_DATA_HOME:-$HOME/.local/share}:${XDG_DATA_DIRS:-/usr/local/share/:/usr/share/}=");
	for (item = xdgSearchableDataDirectories(handle); *item; item++)
		printf("%s%c", *item, (item[1] ? ':' : '\n'));
	printf("${XDG_CONFIG_DIRS:-/etc/xdg}=");
	for (item = xdgConfigDirectories(handle); *item; item++)
		printf("%s%c", *item, (item[1] ? ':' : '\n'));
	printf("${XDG_CONFIG_HOME:-$HOME/.config}:${XDG_CONFIG_DIRS:-/etc/xdg}=");
	for (item = xdgSearchableConfigDirectories(handle); *item; item++)
		printf("%s%c", *item, (item[1] ? ':' : '\n'));
	printf("${XDG_CACHE_HOME:-$HOME/.cache}=%s\n", xdgCacheHome(handle));
	return 0;
}
