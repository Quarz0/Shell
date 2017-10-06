#include <stdlib.h>
#include "environment.h"
#include "variables.h"

void setup_environment(void) {
    initialize_variable_table();
    set_variable("PATH", getenv("PATH"));
    set_variable("HOME", getenv("HOME"));
    set_variable("ROOT", getenv("ROOT"));
}