#include "resource.h"

int registerResource(ResourceManager *res, char tag[LABEL_SHORT], void *data);
int retrieveResource(ResourceManager *res, char tag[LABEL_SHORT]);

