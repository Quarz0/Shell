#include <stdlib.h>
#include "environment.h"
#include "variables.h"

void setup_environment(void) {
    set_variable("PATH", getenv("PATH"));
    set_variable("HOME", getenv("HOME"));
    set_variable("ROOT", getenv("ROOT"));
}