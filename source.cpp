#include "Header.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <cctype>
using namespace std;

void createStack(Stack& S) {
    S.top = -1;
}
bool isEmpty(const Stack& S) {
    return S.top == -1;
}
bool isFull(const Stack& S) {
    return S.top == MAXSIZE - 1;
}
void push(Stack& S, adrNode item) {
    if (isFull(S)) {
        cout << "Stack is full!" << endl;
        return;
    }
    S.info[++S.top] = item;
}
void pop(Stack& S, adrNode& item) {
    if (isEmpty(S)) {
        cout << "Stack is empty!" << endl;
        item = nullptr;
        return;
    }
    item = S.info[S.top--];
}
adrNode peek(const Stack& S) {
    if (isEmpty(S)) {
        cout << "Stack is empty!" << endl;
        return nullptr;
    }
    return S.info[S.top];
}
adrNode CreateNode(infotype data){
    adrNode kata=new Node;
    kata->info=data;
    kata->Next=nullptr;
    kata->Prev=nullptr;
    return kata;
}
adrLine CreateLine(){
    adrLine linebaru=new Line;
    linebaru->kalimat=nullptr;
    linebaru->Next=nullptr;
    linebaru->Prev=nullptr;
    return linebaru;
}
void initializeEditor(adrLine& head, adrLine& currentLine, adrNode& cursor){
    head =CreateLine();
    currentLine = head;
    cursor = nullptr;
}

void createNewLine(adrLine& currentLine, adrNode& cursor) {
    adrLine newLine = CreateLine();
    if (currentLine != nullptr) {
        newLine->Prev = currentLine;
        currentLine->Next = newLine;
    }
    currentLine = newLine;
    cursor = nullptr;
}


void deleteChar(adrLine& currentLine, adrNode& cursor, Stack& undoStack){
     if (!cursor) return;

    if (isFull(undoStack)) {
        cout << "Undo stack is full. Cannot delete." << endl;
        return;
    }
    undoStack.info[++undoStack.top] = cursor;
    moveCursorLeft(cursor,undoStack);
}
void moveCursorLeft(adrNode& cursor, const Stack& undoStack) {
    if (!cursor) return;

    while (cursor->Prev && isAddressInStack(cursor->Prev, undoStack)) {
        cursor = cursor->Prev;
    }

    if (cursor->Prev) {
        cursor = cursor->Prev;
    }
}

void moveCursorRight(adrNode& cursor, const Stack& undoStack) {
    if (!cursor) return;

    while (cursor->Next && isAddressInStack(cursor->Next, undoStack)) {
        cursor = cursor->Next;
    }

    if (cursor->Next) {
        cursor = cursor->Next;
    }
}
void moveCursorUp(adrLine& currentLine, adrNode& cursor, const Stack& undoStack) {
    if (!currentLine || !currentLine->Prev) return;

    currentLine = currentLine->Prev;
    cursor = currentLine->kalimat;
    while (cursor && cursor->Next) {
        if (!isAddressInStack(cursor->Next, undoStack)) {
            cursor = cursor->Next;
        } else {
            break;
        }
    }
}

void moveCursorDown(adrLine& currentLine, adrNode& cursor, const Stack& undoStack) {
    if (!currentLine || !currentLine->Next) return;

    currentLine = currentLine->Next;
    cursor = currentLine->kalimat;
    while (cursor && isAddressInStack(cursor, undoStack)) {
        cursor = cursor->Next;
    }
}
void displayText(adrLine head, adrNode cursor, const Stack& undoStack,adrLine& currentLine) {
    adrLine line = head;
    int lineNumber = 1;
    cout << "Simple Text Editor" << endl;
    cout << "-----------------------"<<endl;
    cout << "Welcome to Typing Mode: " << endl;
    while (line) {
        adrNode node = line->kalimat;

        while (node) {
            if (node == cursor) {
                cout<< node->info<<"|";
            } else {
                if (!isAddressInStack(node, undoStack)) {
                    cout << node->info;
                }
            }
            node = node->Next;
        }
        cout << endl;
        line = line->Next;
    }

    if (cursor) {
        cout << "\nCursor at: " << cursor->info << endl;
    }
    cout <<"Count words: "<< countWords(head,cursor,undoStack)<< endl;
    cout <<"Count characters: "<< countCharacter(head,cursor,undoStack)<<endl;



}

void typingMode(adrLine& head, adrLine& currentLine, adrNode& cursor, Stack& undoStack) {
    char ch;
    bool isFirstDisplay = true;

    while (true) {
        if (isFirstDisplay) {
            cout << "Simple Text Editor" << endl;
            cout << "-----------------------"<<endl;
            cout << "Welcome to Typing Mode: " << endl;
            isFirstDisplay = false;
        } else {
            system("cls");
            displayText(head, cursor, undoStack,currentLine);
        }

        ch = getch();
        if (ch == 27) { // ESC key to exit
            cout << "Exiting the editor." << endl;
            exit(0);
        } else if (ch == 26) { // CTRL+Z to switch to editing mode
            editingMode(head, currentLine, cursor, undoStack);
            break;
        } else if (ch == '\r') { // Enter key to create a new line
            createNewLine(currentLine, cursor);
        } else {
            insertNode(currentLine, cursor, ch);
        }
    }
}


void editingMode(adrLine& head, adrLine& currentLine, adrNode& cursor, Stack& undoStack) {
    cout << "Welcome to Editing Mode!" << endl;
    char key;
    bool needRefresh = true;

    while (true) {
        if (needRefresh) {
            system("cls");
            displayText(head, cursor, undoStack,currentLine);
            needRefresh = false;
        }

        key = getch();

        if (key == 26) { // CTRL+Z: switch to typing mode
            typingMode(head, currentLine, cursor, undoStack);
            break;
        } else if (key == 27) { // ESC: exit
            exit(0);
        } else if (key == 'w' || key == 'W') { // Move up
            moveCursorUp(currentLine, cursor, undoStack);
            needRefresh = true;
        } else if (key == 'a' || key == 'A') { // Move left
            moveCursorLeft(cursor, undoStack);
            needRefresh = true;
        } else if (key == 's' || key == 'S') { // Move down
            moveCursorDown(currentLine, cursor, undoStack);
            needRefresh = true;
        } else if (key == 'd' || key == 'D') { // Move right
            moveCursorRight(cursor, undoStack);
            needRefresh = true;
        } else if (key == 'u' || key == 'U') { // Undo
            undoDelete(currentLine, cursor, undoStack);
            needRefresh = true;
        } else if (key == 'r' || key == 'R') { // Redo
            redoDelete(currentLine, cursor, undoStack);
            needRefresh = true;
        } else if (key == 8) { // Backspace: delete
            deleteChar(currentLine, cursor, undoStack);
            needRefresh = true;
        } else if (key == '\n') { // Enter: new line
            createNewLine(currentLine, cursor);
            needRefresh = true;
        }
    }
}

int countWords(adrLine head, adrNode cursor, const Stack undoStack) {
    int count = 0;
    adrLine line = head;

    while (line) {
        adrNode node = line->kalimat;
        string fullText = ""; // Untuk menggabungkan semua karakter

        while (node) {
            if (!isAddressInStack(node, undoStack)) {
                fullText += node->info; // menggabungkan karakter
            }
            node = node->Next;
        }

        // Hitung kata berdasarkan spasi
        bool inWord = false;
        for (char ch : fullText) {
            if (!isspace(ch)) {
                if (!inWord) {
                    count++;
                    inWord = true;
                }
            } else {
                inWord = false;
            }
        }

        line = line->Next;
    }

    return count;
}

int countCharacter(adrLine head,adrNode cursor,const Stack undoStack) {
    int count = 1;
    adrLine line = head;
    while (line) {
        adrNode node = line->kalimat;
        while (node) {
            if (node == cursor) {

            } else {
                if (!isAddressInStack(node, undoStack)) {
                    count++;
                }
            }
            node = node->Next;
        }
        line = line->Next;
    }
    return count;
}


void insertFirst(adrLine& currentLine,adrNode& cursor ,infotype c) {
    adrNode newNode = CreateNode(c);
    if (currentLine->kalimat == nullptr) {
        currentLine->kalimat = newNode;
        cursor=newNode;
    } else {
        newNode->Next = currentLine->kalimat;
        currentLine->kalimat->Prev = newNode;
        currentLine->kalimat = newNode;
        cursor=newNode;
    }

}

void insertLast(adrLine& currentLine,adrNode& cursor, infotype c) {
    adrNode newNode = CreateNode(c);
    if (currentLine->kalimat == nullptr) {
        currentLine->kalimat = newNode;
        cursor=newNode;
    } else {
        adrNode temp = currentLine->kalimat;
        while (temp->Next != nullptr) {
            temp = temp->Next;
        }
        temp->Next = newNode;
        newNode->Prev = temp;
        cursor=newNode;
    }

}
void insertAfter(adrLine& currentLine,adrNode& cursor, infotype c) {
    if (cursor == nullptr) return;
    adrNode newNode = CreateNode(c);
    newNode->Next = cursor->Next;
    newNode->Prev = cursor;

    if (cursor->Next != nullptr) {
        cursor->Next->Prev = newNode;
    }
    cursor->Next = newNode;
}

void insertNode(adrLine& currentLine, adrNode& cursor, infotype c) {
    adrNode newNode = CreateNode(c);
    if (cursor == nullptr) {
        if (currentLine->kalimat == nullptr) {
            currentLine->kalimat = newNode;
        } else {
            newNode->Next = currentLine->kalimat;
            currentLine->kalimat->Prev = newNode;
            currentLine->kalimat = newNode;
        }
        cursor = newNode;
    } else {
        newNode->Next = cursor->Next;
        newNode->Prev = cursor;
        if (cursor->Next != nullptr) {
            cursor->Next->Prev = newNode;
        }
        cursor->Next = newNode;
        cursor = newNode;
    }
}

void redoDelete(adrLine& currentLine, adrNode& cursor, Stack& undoStack) {
    if (isFull(undoStack)) {
        cout << "Redo stack is full. Cannot redo." << endl;
        return;
    }

    // Temporarily delete the current character
    adrNode nodeToDelete = cursor;
    if (!nodeToDelete) {
        cout << "Nothing to redo." << endl;
        return;
    }

    // Save to the undo stack
    undoStack.info[++undoStack.top] = nodeToDelete;

    if (nodeToDelete->Prev) {
        nodeToDelete->Prev->Next = nodeToDelete->Next;
    } else {
        currentLine->kalimat = nodeToDelete->Next; // Node is the first in the line
    }
    if (nodeToDelete->Next) {
        nodeToDelete->Next->Prev = nodeToDelete->Prev;
    }

    cursor = nodeToDelete->Next; // Move the cursor to the next character
}
void undoDelete(adrLine& currentLine, adrNode& cursor, Stack& undoStack) {
    if (isEmpty(undoStack)) {
        cout << "Nothing to undo." << endl;
        return;
    }
    adrNode nodeToRestore = undoStack.info[undoStack.top--];

    // Restore the deleted character to its original position
    if (nodeToRestore->Prev) {
        nodeToRestore->Prev->Next = nodeToRestore;
    } else {
        currentLine->kalimat = nodeToRestore; // Node is the first in the line
    }
    if (nodeToRestore->Next) {
        nodeToRestore->Next->Prev = nodeToRestore;
    }

    cursor = nodeToRestore; // Move the cursor to the restored character
}
bool isAddressInStack(adrNode node, const Stack& undoStack) {
    for (int i = 0; i <= undoStack.top; i++) {
        if (undoStack.info[i] == node) {
            return true;
        }
    }
    return false;
}
