#pragma once

#include <string.h>

int index_of_in_argv(const char *argument, int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], argument))
    {
      return i;
    }
  }
  return -1;
}

bool is_string_present_in_argv(const char *switch_name, int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], switch_name))
    {
      return true;
    }
  }
  return false;
}
