#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Function to report that the given sequence doesnt belong to the language and exit
void not_in_language() {
    cout << "The given sequence doesnt belong to the language" << endl;
    exit(0);
}

string S;
vector<string> INP;
int LA;

// class for S -> ...
class S_prod {
public:
    string type;
    void solve();
};
// class for A -> ...
class A_prod {
public:
    string type, prev;
    void solve();
};
// class for B -> ...
class B_prod {
public:
    string type, prev;
    void solve();
};
// class for As -> ...
class As_prod {
public:
    string type, prev;
    void solve();
};

// method to use the As rules
void As_prod::solve() {
    // first case, a ; found in the lookahead
    // read ; and go to A
    if (INP[LA].compare(";") == 0) {
        LA++;
        A_prod a;
        a.solve();
        type = a.type;
    }
    // second case a finally, $ or catch in the lookahead, here we do nothing
    // because this is As -> lambda
    else if (INP[LA].compare("finally") == 0 || INP[LA].compare("$") == 0 || INP[LA].compare("catch") == 0) {
        type = prev;
    }
    else
        not_in_language();
}

// method to use the B rules
void B_prod::solve() {
    // first case, a finally found in the lookahead
    // read finally and go to A
    if (INP[LA].compare("finally") == 0) {
        LA++;
        A_prod a;
        a.solve();
        type = a.type;
    }
    // second case a ;, $ or catch in the lookahead, here we do nothing
    // because this is As -> lambda
    else if (INP[LA].compare(";") == 0 || INP[LA].compare("$") == 0 || INP[LA].compare("catch") == 0) {
        type = prev;
    }
    else
        not_in_language();
}

// method to use the A rules
void A_prod::solve() {
    // first case, a try found in the lookahead
    // here we read try, then go to A, then read catch, then go to A again,
    // then update values for B and go to B, then update values for
    // As and go to As, and finally update the current values
    if (INP[LA].compare("try") == 0) {
        ++LA;
        A_prod a1;
        a1.solve();

        if (INP[LA].compare("catch") != 0)
            not_in_language();

        ++LA;
        A_prod a2;
        a2.solve();

        B_prod b;
        b.prev = "(Either " + a1.type + " " + a2.type + ")";
        b.solve();

        As_prod as;
        as.prev = b.type;
        as.solve();
        type = as.type;
    }
    // second case, a instr found in the lookahead
    // here we read instr, update As values, go to As and finally update current values
    else if (INP[LA].compare("instr") == 0) {
        ++LA;
        As_prod as;
        as.prev = "Type" + to_string(LA - 1);
        as.solve();
        type = as.type;
    }
    else
        not_in_language();
}

// method to use the S rules
void S_prod::solve() {
    if (INP[LA].compare("try") == 0 || INP[LA].compare("instr") == 0) {
        A_prod a;
        a.solve();
        type = a.type;
        if (INP[LA].compare("$") != 0)
            not_in_language();
    }
    else
        not_in_language();
}

int main() {
    cout << "De el input (con espacios entre tokens)" << endl;
    getline(cin, S);

    {
        // split input for easier handling
        string aux;
        S.push_back(' ');
        for (auto c : S) {
            if (c == ' ' && !aux.empty()) {
                INP.push_back(aux);
                aux.clear();
            }
            else if (c != ' ')
                aux.push_back(c);
        }
        INP.push_back("$");
        LA = 0;
    }

    S_prod init;
    init.solve();
    cout << "Tipo de el input dado: \n\t";
    cout << init.type << endl;

    return 0;
}