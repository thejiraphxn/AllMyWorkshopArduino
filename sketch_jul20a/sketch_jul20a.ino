#include <iostream>
using namespace std;
void setup() {


void my_max_mid_min(int a, int b, int c)
{
    if (a >= b && a >= c)
    {
        top = a;
    }
    else if (b >= a && b >= c)
    {
        top = b;
    }
    else
    {
        top = c;
    }
    cout << "max = " << top << endl;

    if (top == c && a >= b || top == b && a >= c || top == a && b > c && top == b && top == c)
    {
        mid = a;
    }
    else if (top == a && c >= b || top == b && c >= a)
    {
        mid = c;
    }
    else
    {
        mid = b;
    }
    cout << "mid = " << mid << endl;

    if (mid == a && a > b || mid == c && c < a)
    {
        btm = b;
    }
    else if (mid == b && c > a || mid == c && a < c)
    {
        btm = a;
    }
    else
    {
        btm = c;
    }
    cout << "min = " << btm << endl;
}

int main()
{
    int a, b, c;
    cin >> a;
    cin >> b;
    cin >> c;
    my_max_mid_min(a, b, c);
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
