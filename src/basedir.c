/** @file basedir.c
  * @brief Implementation of the XDG basedir specification. */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <basedir.h>

static const char
	DefaultRelativeDataHome[] = "/.local/share",
	DefaultRelativeConfigHome[] = "/.config",
	DefaultDataDirectories1[] = "/usr/local/share",
	DefaultDataDirectories2[] = "/usr/share",
	DefaultConfigDirectories[] = "/etc/xdg",
	DefaultRelativeCacheHome[] = "/.cache";

typedef struct _xdgCachedData
{
	const char * dataHome;
	const char * configHome;
	const char * cacheHome;
	// Note: string lists are null-terminated and all items
	// except the first are assumed to be allocated using malloc.
	// The first item is assumed to be allocated by malloc only if
	// it is not equal to the appropriate home directory string above.
	const char ** searchableDataDirectories;
	const char ** searchableConfigDirectories; 
} xdgCachedData;

#define GET_CACHE(handle) ((xdgCachedData*)(handle->reserved))

xdgHandle xdgAllocHandle()
{
	xdgHandle handle = (xdgHandle)malloc(sizeof(*handle));
	if (!handle) return 0;
	handle->reserved = 0; // So xdgUpdateData() doesn't free it
	if (xdgUpdateData(handle))
		return handle;
	else
		free(handle);
	return 0;
}

/** Free all memory used by a NULL-terminated string list */
static void xdgFreeStringList(const char * const * list)
{
	if (!list) return;
	for (; *list; list++)
		free(*list);
	free(list);
}

/** Free all data in the cache and set pointers to null. */
static void xdgFreeData(xdgCachedData *cache)
{
	const char ** item;
	if (cache->dataHome);
	{
		// the first element of the directory lists is usually the home directory
		if (cache->searchableDataDirectories[0] != cache->dataHome)
			free(cache->dataHome);
		cache->dataHome = 0;
	}
	if (cache->configHome);
	{
		if (cache->searchableConfigDirectories[0] != cache->configHome)
			free(cache->configHome);
		cache->configHome = 0;
	}
	xdgFreeStringList(cache->searchableDataDirectories);
	cache->searchableDataDirectories = 0;
	xdgFreeStringList(cache->searchableConfigDirectories);
	cache->searchableConfigDirectories = 0;
}

void xdgFreeHandle(xdgHandle handle)
{
	xdgCachedData* cache = (xdgCachedData*)(handle->reserved);
	xdgFreeData(cache);
	free(cache);
	free(handle);
}

/** Get value for environment variable $name, defaulting to "defaultValue".
 *	@param name Name of environment variable.
 *	@param defaultValue Value to assume for environment variable if it is
 *		unset or empty.
 */
static char* xdgGetEnv(const char* name, const char* defaultValue)
{
	const char* env;
	char* value;

	env = getenv(name);
	if (env && env[0])
	{
		if (!(value = (char*)malloc(strlen(env)+1))) return 0;
		strcpy(value, env);
	}
	else
	{
		if (!(value = (char*)malloc(strlen(defaultValue)+1))) return 0;
		strcpy(value, defaultValue);
	}
	return value;
}

/** Split string at ':', return null-terminated list of resulting strings.
 * @param string String to be split
 */
static char** xdgSplitPath(const char* string)
{
	unsigned int size, i, j, k;
	char** itemlist;
	const char* ptr;

	// Get the number of paths
	size=2; // One item more than seperators + terminating null item
	for (i = 0; string[i]; ++i)
	{
		if (string[i] == '\\' && string[i+1]) ++i; // skip escaped characters including seperators
		else if (string[i] == ':') ++size;
	}
	
	if (!(itemlist = (char**)malloc(sizeof(char*)*size))) return 0;
	memset(itemlist, 0, sizeof(char*)*size);

	ptr = string;
	i = 0;
	while (*ptr)
	{
		for (j = 0; ptr[j] && ptr[j] != ':'; ++j)
			if (ptr[j] == '\\' && ptr[j+1]) ++j;
		if (!(itemlist[i] = (char*)malloc(j+1))) { xdgFreeStringList(itemlist); return 0; }
		for (k = j = 0; ptr[j] && ptr[j] != ':'; ++j, ++k)
		{
			if (ptr[j] == '\\' && ptr[j+1] == ':') ++j; // replace escaped ':' with just ':'
			else if (ptr[j] == '\\' && ptr[j+1]) // skip escaped characters so escaping remains aligned to pairs.
			{
				itemlist[i][k]=ptr[j];
				++j, ++k;
			}
			itemlist[i][k] = ptr[j];
		}
		ptr += j;
		if (*ptr == ':') ptr++; // skip seperator
	}
}

/** Get $PATH-style environment variable as list of strings.
 * If $name is unset or empty, use default strings specified by variable arguments.
 * @param name Name of environment variable
 * @param numDefaults Number of default paths in variable argument list
 * @param ... numDefaults number of strings to be copied and used as defaults
 */
static char** xdgGetPathListEnv(const char* name, int numDefaults, ...)
{
	const char* env;
	va_list ap;
	char* item;
	const char* arg;
	char** itemlist;
	int i;

	env = getenv(name);
	if (env && env[0])
	{
		if (!(item = malloc(strlen(env)+1))) return 0;
		strcpy(item, env);

		itemlist = xdgSplitPath(item);
		free(item);
	}
	else
	{
		if (!(itemlist = (char**)malloc(sizeof(char*)*numDefaults+1))) return 0;
		memset(itemlist, 0, sizeof(char*)*(numDefaults+1));

		// Copy the varargs into the itemlist
		va_start(ap, numDefaults);
		for (i = 0; i < numDefaults; i++)
		{
			arg = va_arg(ap, const char*);
			if (!(item = (char*)malloc(strlen(arg)+1))) { xdgFreeStringList(itemlist); return 0; }
			strcpy(item, arg);
			itemlist[i] = item;
		}
		va_end(ap);
	}
	return itemlist;
}

/** Update all *Home variables of cache.
 * This includes xdgCachedData::dataHome, xdgCachedData::configHome and xdgCachedData::cacheHome.
 * @param cache Data cache to be updated
 */
static bool xdgUpdateHomeDirectories(xdgCachedData* cache)
{
	const char* env;
	char* home, *defVal;

	env = getenv("HOME");
	if (!env || !env[0])
		return false;
	if (!(home = (char*)malloc(strlen(env)+1))) return false;
	strcpy(home, env);

	// Allocate maximum needed for any of the 3 default values
	defVal = (char*)malloc(strlen(home)+
		MAX(MAX(sizeof(DefaultRelativeDataHome), sizeof(DefaultRelativeConfigHome)), sizeof(DefaultRelativeCacheHome)));
	if (!defVal) return false;

	strcpy(defVal, home);
	strcat(defVal, DefaultRelativeDataHome);
	if (!(cache->dataHome = xdgGetEnv("XDG_DATA_HOME", defVal))) return false;

	defVal[strlen(home)] = 0;
	strcat(defVal, DefaultRelativeConfigHome);
	if (!(cache->configHome = xdgGetEnv("XDG_CONFIG_HOME", defVal))) return false;

	defVal[strlen(home)] = 0;
	strcat(defVal, DefaultRelativeCacheHome);
	if (!(cache->cacheHome = xdgGetEnv("XDG_CACHE_HOME", defVal))) return false;

	free(defVal);
	free(home);

	return true;
}

/** Update all *Directories variables of cache.
 * This includes xdgCachedData::searchableDataDirectories and xdgCachedData::searchableConfigDirectories.
 * @param cache Data cache to be updated.
 */
static bool xdgUpdateDirectoryLists(xdgCachedData* cache)
{
	char** itemlist;
	int size;

	itemlist = xdgGetPathListEnv("XDG_DATA_DIRS", 2,
			DefaultDataDirectories1, DefaultDataDirectories2);
	if (!itemlist) return false;
	for (size = 0; itemlist[size]; size++) ; // Get list size
	if (!(cache->searchableDataDirectories = (const char**)malloc(sizeof(char*)*(size+2))))
	{
		xdgFreeStringList(itemlist);
		return false;
	}
	cache->searchableDataDirectories[0] = cache->dataHome;
	memcpy(&(cache->searchableDataDirectories[1]), itemlist, sizeof(char*)*(size+1));
	free(itemlist);
	
	itemlist = xdgGetPathListEnv("XDG_CONFIG_DIRS", 1, DefaultConfigDirectories);
	if (!itemlist) return false;
	for (size = 0; itemlist[size]; size++) ; // Get list size
	if (!(cache->searchableConfigDirectories = (const char**)malloc(sizeof(char*)*(size+2))))
	{
		xdgFreeStringList(itemlist);
		return false;
	}
	cache->searchableConfigDirectories[0] = cache->configHome;
	memcpy(&(cache->searchableConfigDirectories[1]), itemlist, sizeof(char*)*(size+1));
	free(itemlist);

	return true;
}

bool xdgUpdateData(xdgHandle handle)
{
	xdgCachedData* cache = (xdgCachedData*)malloc(sizeof(xdgCachedData));
	if (!cache) return false;
	memset(cache, 0, sizeof(xdgCachedData));

	if (xdgUpdateHomeDirectories(cache) &&
		xdgUpdateDirectoryLists(cache))
	{
		if (handle->reserved) free(handle->reserved);
		handle->reserved = cache;
		return true;
	}
	else
	{
		xdgFreeData(cache);
		free(cache);
		return false;
	}
}

/** Find all existing files corresponding to relativePath relative to each item in dirList.
  * @param relativePath Relative path to search for.
  * @param dirList <tt>NULL</tt>-terminated list of directory paths.
  * @return A sequence of null-terminated strings terminated by a double-<tt>NULL</tt> (empty string)
  * 	and allocated using malloc().
  */
static const char* xdgFindExisting(const char * relativePath, const char * const * dirList)
{
	char * fullPath;
	char * returnString = 0;
	char * tmpString;
	int strLen = 0;
	FILE * testFile;
	const char * const * item;

	for (item = dirList; *item; item++)
	{
		if (!(fullPath = (char*)malloc(strlen(*item)+strlen(relativePath)+1)))
		{
			if (returnString) free(returnString);
			return 0;
		}
		strcpy(fullPath, *item);
		strcat(fullPath, relativePath);
		testFile = fopen(fullPath, "r");
		if (testFile)
		{
			if (!(tmpString = realloc(returnString, strLen+strlen(fullPath)+2)))
			{
				free(returnString);
				free(fullPath);
				return 0;
			}
			returnString = tmpString;
			strcpy(&returnString[strLen], fullPath);
			strLen = strLen+strlen(fullPath)+1;
			fclose(testFile);
		}
		free(fullPath);
	}
	if (returnString)
	{
		returnString[strLen+1] = 0;
		return returnString;
	}
	else
		return "\0";
}

/** Open first possible config file corresponding to relativePath.
  * @param relativePath Path to scan for.
  * @param mode Mode with which to attempt to open files (see fopen modes).
  * @param dirList <tt>NULL</tt>-terminated list of paths in which to search for relativePath.
  * @return File pointer if successful else @c NULL. Client must use @c fclose to close file.
  */
static FILE * xdgFileOpen(const char * relativePath, const char * mode, const char * const * dirList)
{
	char * fullPath;
	FILE * testFile;
	const char * const * item;

	for (item = dirList; *item; item++)
	{
		if (fullPath = (char*)malloc(strlen(*item)+strlen(relativePath)+1))
			return 0;
		strcpy(fullPath, *item);
		strcat(fullPath, relativePath);
		testFile = fopen(fullPath, mode);
		free(fullPath);
		if (testFile)
			return testFile;
	}
	return 0;
}

const char * xdgDataHome(xdgHandle handle)
{
	return GET_CACHE(handle)->dataHome;
}
const char * xdgConfigHome(xdgHandle handle)
{
	return GET_CACHE(handle)->configHome;
}
const char * const * xdgDataDirectories(xdgHandle handle)
{
	return &(GET_CACHE(handle)->searchableConfigDirectories[1]);
}
const char * const * xdgSearchableDataDirectories(xdgHandle handle)
{
	return GET_CACHE(handle)->searchableConfigDirectories;
}
const char * const * xdgConfigDirectories(xdgHandle handle)
{
	return &(GET_CACHE(handle)->searchableConfigDirectories[1]);
}
const char * const * xdgSearchableConfigDirectories(xdgHandle handle)
{
	return GET_CACHE(handle)->searchableConfigDirectories;
}
const char * xdgCacheHome(xdgHandle handle)
{
	return GET_CACHE(handle)->cacheHome;
}
const char * xdgDataFind(const char * relativePath, xdgHandle handle)
{
	return xdgFindExisting(relativePath, xdgSearchableDataDirectories(handle));
}
const char * xdgConfigFind(const char * relativePath, xdgHandle handle)
{
	return xdgFindExisting(relativePath, xdgSearchableConfigDirectories(handle));
}
FILE * xdgDataOpen(const char * relativePath, const char * mode, xdgHandle handle)
{
	return xdgFileOpen(relativePath, mode, xdgSearchableDataDirectories(handle));
}
FILE * xdgConfigOpen(const char * relativePath, const char * mode, xdgHandle handle)
{
	return xdgFileOpen(relativePath, mode, xdgSearchableConfigDirectories(handle));
}
