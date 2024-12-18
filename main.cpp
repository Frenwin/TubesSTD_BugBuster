#include "Header.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>
using namespace std;

int main() {
    adrLine head = nullptr;
    adrLine currentLine = nullptr;
    adrNode cursor = nullptr;
    Stack undoStack;

    createStack(undoStack);
    initializeEditor(head, currentLine, cursor);


    typingMode(head, currentLine, cursor, undoStack);
}
