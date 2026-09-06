#pragma once

#include "ga.h"

void apply_field           (const char *key, const char *value, Config *dst, const char *source);
int  config_domain_max_was_set(void);
