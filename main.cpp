#include "FirstWindow.h"
#include "DimaTyper.h"


int main() {
    DimaTyper dimaTyper;
    FirstWindow firstWindow(dimaTyper);
    if(firstWindow.run()){
        dimaTyper.run();
    }
}