
#include "Interface.h"

int main() {
    Controller cc;
    Interface ii(true, cc);
    ifstream s("run.txt");
    ii.run(s);
}
