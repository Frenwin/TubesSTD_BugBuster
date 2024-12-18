#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <cstdlib>
using namespace std;

const int MAXSIZE = 1000;

typedef char infotype;
typedef struct Node* adrNode;
typedef struct Line* adrLine;

struct Node {
    infotype info;
    adrNode Next;
    adrNode Prev;
};

struct Line {
    adrNode kalimat;
    adrLine Next;
    adrLine Prev;
};

struct Stack {
    adrNode info[MAXSIZE];
    int top;
};

void createStack(Stack& S);
bool isEmpty(const Stack& S);
bool isFull(const Stack& S);
void push(Stack& S, adrNode item);
void pop(Stack& S, adrNode& item);
adrNode peek(const Stack& S);
int sumStack(const Stack& S);
adrNode CreateNode(infotype data);
adrLine CreateLine();
void initializeEditor(adrLine& head, adrLine& currentLine, adrNode& cursor);
void createNewLine(adrLine& currentLine, adrNode& cursor);
void deleteChar(adrLine& currentLine, adrNode& cursor, Stack& undoStack);
void moveCursorLeft(adrNode& cursor, const Stack& undoStack);
void moveCursorRight(adrNode& cursor, const Stack& undoStack);
void moveCursorUp(adrLine& currentLine, adrNode& cursor, const Stack& undoStack);
void moveCursorDown(adrLine& currentLine, adrNode& cursor, const Stack& undoStack);
void displayText(adrLine head, adrNode cursor, const Stack& undoStack,adrLine& currentLine);
void insertFirst(adrLine& currentLine, infotype c);
void insertLast(adrLine& currentLine, infotype c);
void insertAfter(adrNode& cursor, infotype c);
void insertNode(adrLine& currentLine, adrNode& cursor, infotype c);
void redoDelete(adrLine& currentLine, adrNode& cursor, Stack& undoStack);
void undoDelete(adrLine& currentLine, adrNode& cursor, Stack& undoStack);
bool isAddressInStack(adrNode node, const Stack& undoStack);
void typingMode(adrLine& head, adrLine& currentLine, adrNode& cursor, Stack& undoStack);
void editingMode(adrLine& head, adrLine& currentLine, adrNode& cursor, Stack& undoStack);
int countWords(adrLine head,adrNode cursor,const Stack undoStack);
int countCharacter(adrLine head,adrNode cursor,const Stack undoStack);


#endif // HEADER_H_INCLUDED
