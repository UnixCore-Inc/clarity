#ifndef CLARITY_ENGINE_H
#define CLARITY_ENGINE_H

#include <stdio.h>
#include "../clarityParser/CSCParser.h"

int ClarityMain(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "L'ARGOMENTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n");
    return -1;
  }
  return parse(initParser("index.css"), argv[1]);
}

#endif