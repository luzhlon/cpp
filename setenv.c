

//set environment variables


#include <stdio.h>
#include <string.h>
#include <windows.h>

HKEY hKey;
int g_Machine = 0;
int g_Append = 0;
int g_Show = 0;
int g_RegType = REG_SZ;

void ParseCommandLine(int argn, char **argv)
{
	int i = 0;
	for (i = 1; i < argn; i++)
		if (!strcmp(argv[i], "-a"))g_Append = 1;
		else if (!strcmp(argv[i], "-m"))g_Machine = 1;
		else if (!strcmp(argv[i], "-s"))g_Show = 1;
		else if (!strcmp(argv[i], "-e"))g_RegType = REG_EXPAND_SZ;
		else
		{
			printf("%s  Invalid argument.", argv[i]);
			exit(1);
		}
}

HKEY OpenUserPath()
{
	HKEY hUser;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_READ | KEY_WRITE, &hUser))
	{
		puts("Open user key failue.");
		exit(1);
	}
	return hUser;
}

HKEY OpenMatchinePath()
{
	HKEY hMatch;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\ CurrentControlSet\\Services", 0, KEY_READ | KEY_WRITE, &hMatch))
	{
		puts("Open user key failue.");
		exit(1);
	}
	return hMatch;
}

void NotifySystemChange()
{
	//SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, _TEXT("Environment"));
	//PostMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, TEXT("Environment"));
	system("setx ~n~n~n NULL");
}

int QueryEnv(char *var, char *val)
{
	DWORD size;
	DWORD type;
	if (RegQueryValueEx(hKey, var, 0, &type, val, &size))
		return 0;
	else
		return 1;
}

int SetEnv(char *var, char *val)
{
	int ret = 1;
	if (RegSetValueEx(hKey, var, 0, g_RegType, val, strlen(val)))
		ret = 0;
	NotifySystemChange();
	return ret;
}

void OnShow()
{
	char var[260] = { 0 };
	char val[1024] = { 0 };
	puts("Please input the environment vairable to show:");
	gets(var);
	if (!QueryEnv(var, val))
	{
		puts("Query reg failue.");
		exit(1);
	}
}

void OnAppend()
{
	char var[260] = { 0 };
	char val[2048] = { 0 };
	char buf[1024] = { 0 };
	puts("Please input the environment vairable to append:");
	gets(var);
	if (!QueryEnv(var, val))
	{
		puts("Query reg failue.");
		exit(1);
	}
	puts("Please input the vairable content to append:");
	gets(buf);
	strcat(val, buf);
	if (!SetEnv(var, val))
	{
		puts("Set reg failue.");
		exit(1);
	}
}

void OnSet()
{
	char var[260] = { 0 };
	char val[2048] = { 0 };
	puts("Please input the environment vairable to set:");
	gets(var);
	puts("Please input the vairable content to set:");
	gets(val);
	if (!SetEnv(var, val))
	{
		puts("Set reg failue.");
		exit(1);
	}
}

int main(int argn, char **argv)
{
	ParseCommandLine(argn, argv);
	if (g_Machine)
		hKey = OpenMatchinePath();
	else
		hKey = OpenUserPath();

	if (g_Show)
		OnShow();
	else if (g_Append)
		OnAppend();
	else
		OnSet();

	CloseHandle(hKey);
	system("pause");
    return 0;
}


