#include <iostream>
#include <queue>
#include <string>

using namespace std;


    

int main ( void )
{
    //cout << "Vstup" << endl;
    int n, k;
    string canteen[2];
    cin >> n >> k >> canteen[0] >> canteen[1];
    //cout << n << " " << k << endl << canteen[0] << endl << canteen[1] << endl;
    pair <int, int> vrchol;
    vrchol = make_pair(0, 0);
    queue <pair<int, int> > fronta;
    fronta . push (vrchol);
    int range[2][100010];
              
     for (int i = 0; i < 2; ++i)
    {
    for (int j = 0; j < 100010; ++j)
    {
        range[i][j] = 0;
     }
    }

    canteen[0] . at(0) = '_';

    while ( !fronta.empty() )
    {
        pair <int, int> s = fronta.front();
        fronta.pop();
        //cout << "Jsem na: " << s.first << " " << s.second << endl;
    
        if ( s.second + k >= n )
        {
            cout << range[s.first][s.second] + 1 << endl;
            return 0;
        }
        
        if ( canteen[s.first].at( s.second + 1 ) == '.' )
        {
            canteen[s.first].at(s.second+1) = '_';
            pair <int, int> tmp = make_pair(s.first, s.second+1);
            fronta.push(tmp);
          //  cout << "Na " << range[s.first][s.second+1] << " vkladam " << range[s.first][s.second] + 1 << endl;
            range[s.first][s.second+1] = ( range[s.first][s.second]) + 1; 

                   
            /* cout << "==============================================" << endl;
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                std::cout << range[i][j] << ' ';
                }
            std::cout << std::endl;
            }  
            cout << canteen[0] << endl << canteen[1] << endl;*/
        }
        

        if ( ( s.second > 0 ) && (canteen[s.first].at( s.second - 1 ) == '.') && range[s.first][s.second] < s.second - 1 )
        {
            canteen[s.first].at(s.second-1) = '_';
            pair <int, int> tmp2 = make_pair(s.first, s.second-1);
            range[s.first][s.second-1] = range[s.first][s.second] + 1; 
            fronta.push(tmp2);
            
            /* cout << "Zabloudim semka" << endl;
            cout << "==============================================" << endl;
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                std::cout << range[i][j] << ' ';
                }
            std::cout << std::endl;
            }  
            cout << canteen[0] << endl << canteen[1] << endl;*/
        }




        if ( s.first == 0 && canteen[1].at( s.second + k ) == '.' )
        {
           // cout << "Zabloudim semka 2" << endl;
            canteen[1].at(s.second+k) = '_';
            pair <int, int> tmp3 = make_pair(1, s.second+k);
           // cout << "Na: " <<  range[1][s.second+k] << " vkladam " << range[s.first][s.second] + 1 << endl;
           // cout << "Na: " <<  "1 " << s.second+k << " vkladam " << s.first << " " << s.second << endl;
            range[1][s.second+k] = range[s.first][s.second] + 1;    
            fronta.push(tmp3);
                        
            /* cout << "==============================================" << endl;
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                std::cout << range[i][j] << ' ';
                }
            std::cout << std::endl;
            }  
            cout << canteen[0] << endl << canteen[1] << endl;*/
        }

        if ( s.first == 1 && canteen[0].at( s.second + k ) == '.' )
        {
          //  cout << "Zabloudim semka 3" << endl;
            canteen[0].at(s.second+k) = '_';
            pair <int, int> tmp4 = make_pair(0, s.second+k);
            range[0][s.second+k] = range[s.first][s.second] + 1; 
            fronta.push(tmp4);
           // cout << "==============================================" << endl;
           /*  for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                std::cout << range[i][j] << ' ';
                }
            std::cout << std::endl;
            }  
            cout << canteen[0] << endl << canteen[1] << endl;*/
                
        }

    }
    cout << "-1" << endl;

    return 0;
}