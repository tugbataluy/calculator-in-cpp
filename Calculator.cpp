#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <map>

/* First Step: Read the input line by line, then cut all white spaces between characters to split them correctly.
* Second Step: Splitting the input according to its purpose like assignment, arithmetic operator or logical operators. I inspired from this code to tokenize my input.
* void split(const string& s, char c,
           vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}
I decided to use maps instead of vectors. Because I need to take substrings as a pair.
3. step: I decided is there any arithmetic operators (first i make simple operations then i implemented process priority by definig pharanteses.
4. step: I tried to find conditions then logical operations.
5. step: I try to detect loops
6. step : I check for last line by searching OUT experssion
7. step : create an output file and store the final result,
*/
using namespace std;

string find_loop(string right, string loop_val);
string find_conditional(string right); // defining functions first makes easier to avoid errors :)
string find_operation(string right);

map <string, string> degiskenler;

bool search_operator(string line)
{
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == '+' || line[i] == '-' || line[i] == '/' || line[i] == '*') // i need to define if there is any operator exists
            return true;
    }
    return false;
}

string find_loop(string right, string loop_val)
{
    if (degiskenler.find(loop_val) == degiskenler.end())
        degiskenler[loop_val] = "0";
    int loop_time = right.find("TIMES"); // i need to split loop expression by times keyword
    string loop_ex = right.substr(0, loop_time);

    // cout << "loop_ex :" << loop_ex;
    int counter = stoi(right.substr(loop_time + 5));

    for (int i = 0; i < counter; i++)
    {
        if (right.find("IF") != string::npos)
        {
            degiskenler[loop_val] = find_conditional(loop_ex);

        }
        else
        {
            degiskenler[loop_val] = find_operation(loop_ex);

        }
    }

    return "";
}

////////
// OUTC12-0580192481
// index = 0
// A+B
// substr(index+3)
// if (index != npos::string) {}
///////

string find_conditional(string right) // IF....ELSE....THEN
{
    int con1 = right.find("IF");
    int con2 = right.find("THEN", con1 + 2);
    int con3 = right.find("ELSE", con2 + 4);

    string conditional_ex = (right.substr(con1 + 2, con2 - con1 - 2));// defining conditional part
    string then_part = right.substr(con2 + 4, con3 - con2 - con1 - 4);
    string else_part = right.substr(con3 + 4);
    string lv, rv, op_c = "<=";
    //cout << size(op_c);

     //cout << "conditional_ex: " << conditional_ex << " then_part: " << then_part << " else_part: " << else_part << endl; // debug

    then_part = search_operator(then_part) ? find_operation(then_part) : degiskenler[then_part];
    else_part = search_operator(else_part) ? find_operation(else_part) : degiskenler[else_part];
    // continue with defining logical operators
    int conditional = conditional_ex.find("<=");
    if (conditional == string::npos)
    {
        conditional = conditional_ex.find(">");
        op_c = ">";
    }

    if (conditional == string::npos)
    {
        conditional = conditional_ex.find("<");
        op_c = "<";
    }
    if (conditional == string::npos)
    {
        conditional = conditional_ex.find(">=");
        op_c = ">=";
    }
    if (conditional == string::npos)
    {
        conditional = conditional_ex.find("==");
        op_c = "==";
    }
    if (conditional == string::npos)
    {
        conditional = conditional_ex.find("!=");
        op_c = "!=";
    }
    // mantıksal var

    if (conditional != string::npos)
    {

        string lc = conditional_ex.substr(0, conditional);
        string rc = conditional_ex.substr(conditional + op_c.size());

        // left side check
        if (degiskenler.find(lc) != degiskenler.end())
            lv = degiskenler[lc];
        else if (search_operator(lc))
        {
            lv = find_operation((lc));
        }
        else
            lv = lc;
        // right side check
        if (degiskenler.find(rc) != degiskenler.end())
            rv = degiskenler[rc];
        else if (search_operator(rc))
            rv = find_operation(rc);
        else
        {
            rv = rc;
        }
        if (op_c == ">")       return stod(lv) > stod(rv) ? then_part : else_part;
        else if (op_c == "<")  return stod(lv) < stod(rv) ? then_part : else_part;
        else if (op_c == "<=") return stod(lv) <= stod(rv) ? then_part : else_part;
        else if (op_c == ">=") return stod(lv) >= stod(rv) ? then_part : else_part;
        else if (op_c == "==") return stod(lv) == stod(rv) ? then_part : else_part;
        else if (op_c == "!=") return stod(lv) != stod(rv) ? then_part : else_part;

    }
    return "";
}

string find_operation(string right)
{   // priority for pharanteses
    // cout << right << endl; 
    int pr = right.find("(");// trying to find left pharantese
    while (pr != string::npos)
    {
        int pr2 = right.find(")");// trying to find right pharantase
        right.replace(pr, pr2 - pr + 1, find_operation(right.substr(pr + 1, pr2 - pr - 1)));
        pr = right.find(right, pr + 1);
    }

    char op = '+';
    int operand = right.find("+");
    string lv;//left value
    string rv;//right value
    if (operand == string::npos)
    {
        operand = right.find("-"); op = '-';
    }
    if (operand == string::npos)
    {
        operand = right.find("/"); op = '/';
    }
    if (operand == string::npos)
    {
        operand = right.find("*"); op = '*';
    }
    // there is an operator
    if (operand != string::npos)
    {
        int lt = right.find("TIMES");
        string lc = right.substr(0, operand);// left child
        string rc = (right.find("TIMES") != string::npos) ? right.substr(operand + 1, lt) : right.substr(operand + 1);//right  child

        
        //lc check
        if (degiskenler.find(lc) != degiskenler.end())
            lv = degiskenler[lc];
        else if (search_operator(lc))
            lv = find_operation(lc);
        else
            lv = lc;
        //rc check
        if (degiskenler.find(rc) != degiskenler.end())
            rv = degiskenler[rc];
        else if (search_operator(rc))
            rv = find_operation(rc);
        else
            rv = rc;
        bool i_or_d = false;

        // checking values integer or double
        if (lv.find(".") == string::npos && rv.find(".") == string::npos) i_or_d = true;
        switch (op)
        {
        case '+':
            return i_or_d ? to_string(stoi(lv) + stoi(rv)) : to_string(stod(lv) + stod(rv));
            break;
        case '-':
            return i_or_d ? to_string(stoi(lv) - stoi(rv)) : to_string(stod(lv) - stod(rv));
            break;
        case '/':
            return i_or_d ? to_string(stoi(lv) / stoi(rv)) : to_string(stod(lv) / stod(rv));
            break;
        case '*':
            return i_or_d ? to_string(stoi(lv) * stoi(rv)) : to_string(stod(lv) * stod(rv));
            break;
        }
    }

    return right;
}


void eq(string line)
{
    int equal = line.find("=");
    string left = line.substr(0, equal); // A
    string right = line.substr(equal + 1); // B + C
    string result;
    int loop_d = line.find("LOOP");

    string loop_val = (loop_d != string::npos) ? line.substr(loop_d + 4, equal - (loop_d + 4)) : line.substr(0, equal);

    if (right.find("TIMES") != string::npos) result = find_loop(right, loop_val);
    else {
        if (right.find("IF") != string::npos) result = find_conditional(right);
        else result = find_operation(right); 
        degiskenler[left] = result;
    }
}

bool Final_cut(string line) // If line has out expression, i need to cut it from line so i write a function that detect final line
{
    if (line.find("OUT") != string::npos)
        return true;
    else return false;
}

int main()
{
    int a = 1;
    string line;
    ifstream inp("Challenge" + to_string(a) + ".inp");
    ofstream outs;

    while (inp.is_open())
    {
        while (getline(inp, line))
        {
            regex r("\\s+");
            line = regex_replace(line, r, "");
            if (Final_cut(line))// if my func returns true stops taking the line.
                break;
            eq(line);
        }
        inp.close();
        outs.open("Challenge" + to_string(a) + ".out");
        string output = degiskenler[line.substr(line.find("OUT") + 3)];

        // cout << a << " : " << output << endl;

        outs << output;

        outs.close();

        a++;
        inp.open("Challenge" + to_string(a) + ".inp");
        degiskenler.clear(); // i clear map at the beginning of every step 

        // inp()
        //system("pause");
    }

    return 0;
}