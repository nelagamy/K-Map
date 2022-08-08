//Nermien Elassy
//900196006

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cmath>
using namespace std;

pair<vector<int>,int> validate()
{
    int number_variables;
    int number_minterms;
    vector <int> minterms;
    do
    {
    cout<<"Please enter the number of variables"<<"\n";
    cin>>number_variables;
    } while(number_variables <1 || number_variables >3);
    
    do
    {
    cout<<"Please enter the number of minterms"<<"\n";
    cin>>number_minterms;
    } while(number_minterms<0 || number_minterms> pow(2,number_variables));
    
    set<int>check;
    for (int i=0; i < number_minterms; i++)
    {
        int element;
        do
        {
            cout<<"Enter your minterm"<<"\n";
            cin>>element;
        } while ((element<0 || element >= pow(2,number_variables)) || check.count(element));
        check.insert(element);
    }
    for(int element: check)
    {
        minterms.push_back(element);
    }
    return {minterms, number_variables};
}
int flip(int x)
{
    if (x==2)
        x=3;
    else if(x==3)
        x=2;
    return x;
}

bool valid_kmap(set<int> st)
{
    set<int> comp_1;
    set<int> comp_1_2;
    comp_1.insert(1);
    comp_1_2.insert(1);
    comp_1_2.insert(2);
    return (st == comp_1 || st == comp_1_2);
}

string function_simplification(vector<int> minterms, int num_varbs)
{
    vector<vector<int>> kmap(2,vector<int>(4));
    
    // Printing K-Map
    for (int term : minterms)
        kmap[term / 4][flip(term % 4)] = 1;
    cout << "K-Map = " << "\n";
    if ( num_varbs == 1 )
    {
        cout<<kmap[0][0]<<"\t"<<kmap[0][1]<<"\n";
        kmap[0][3]=kmap[0][0];
        kmap[0][2]=kmap[0][1];
        for(int j=0; j<4; j++)
        {
            kmap[1][j] = kmap[0][j];
        }
    }
    else if (num_varbs == 2)
    {
            for(int j=0; j<4; j++)
            {
                if (j==2)
                cout<<"\n";
                cout<<kmap[0][flip(j)]<<"\t";
                kmap[1][j]=kmap[0][j];
            }
        cout<<"\n";
    }
    else
    {
        for (auto& row : kmap)
        {
            for (auto& element : row)
                cout << element << "\t";
            cout << "\n";
        }
    }
    // Simplification
    set<int> comp;
    for (auto& row : kmap)
        for (auto& element : row)
            comp.insert(element);
    if (valid_kmap(comp))
    {   cout<<"F = ";
        return "1";
    }
    string ans;
    for (int i = 0; i < 2; i++)
    {
        auto& row = kmap[i];
        comp.clear();
        for (auto& element : row)
            comp.insert(element);
        if (valid_kmap(comp))
        {
            ans += (i ? "A\'" : "A");
            ans += " + ";
            for (auto& element : row)
                element = 2;
        }
    }
    for (int j = 0 ; j<4; j++)
    {
        comp.clear();
        comp.insert(kmap[0][j]);
        comp.insert(kmap[1][j]);
        comp.insert(kmap[0][(j + 1) % 4]);
        comp.insert(kmap[1][(j + 1) % 4]);
        if (valid_kmap(comp))
        {
            if (flip(j)/2 == flip((j + 1) % 4)/2)
            {
                if (flip(j)/2)
                    ans += "B + ";
                else
                    ans += "B\' + ";
            }
            else
            {
                if (flip(j)%2)
                    ans += "C + ";
                else
                    ans += "C\' + ";
            }
            kmap[0][j]=2;
            kmap[1][j]=2;
            kmap[0][(j + 1) % 4]=2;
            kmap[1][(j + 1) % 4]=2;
        }
    }
    string temp;
    for (int j = 0 ; j<4; j++)
    {
        comp.clear();
        comp.insert(kmap[0][j]);
        comp.insert(kmap[1][j]);
        if (valid_kmap(comp))
        {
            temp = "B";
            if (!(flip(j) / 2))
                temp += "\'";
            ans += temp;
            temp = "C";
            if (!(flip(j) % 2))
                temp += "\'";
            ans += temp;
            ans += " + ";
            kmap[0][j]=2;
            kmap[1][j]=2;
        }
        for (int i = 0; i < 2; i++)
        {
            comp.clear();
            comp.insert(kmap[i][j]);
            comp.insert(kmap[i][(j + 1) % 4]);
            if (valid_kmap(comp))
            {
                ans += (i ? "A" : "A\'");
                if (flip(j)/2 == flip((j + 1) % 4)/2)
                {
                    if (flip(j)/2)
                        ans += "B + ";
                    else
                        ans += "B\' + ";
                }
                else
                {
                    if (flip(j)%2)
                        ans += "C + ";
                    else
                        ans += "C\' + ";
                }
                kmap[i][j] = 2;
                kmap[i][(j + 1) % 4] = 2;
            }
        }
    }
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 4; j++)
            if (kmap[i][j] == 1)
            {
                ans += (i ? "A" : "A\'");
                ans += (flip(j)/2 ? "B" : "B\'");
                ans += (flip(j) % 2 ? "C" : "C\'");
                ans += " + ";
            }
    for (int times = 0; times < 3; times++)
        if (!ans.empty())
            ans.pop_back();
    cout<<"F = ";
    return (ans.empty() ? "0" : ans);
}

int main()
{
    auto ans  = validate();
    cout<<function_simplification(ans.first, ans.second);
    cout<<"\n";
}
