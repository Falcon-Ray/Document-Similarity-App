#include <string>
#include <fstream>
#include <iostream>
#include<vector>
#include<ctype.h>
#include<math.h>
#include <windows.h>
#include <mysql.h>

using namespace std;

const int AlPHABET_SIZE=26;
const int CASE='a';
int innerAB=0, innerAA=0, innerBB=0;

struct Node
{
    Node* parent = NULL;
    Node* children[26]= {};
    int occurences=0;
};

void InsertNode(Node* trieTree, string word)
{
    Node* currentNode= trieTree;

    string::iterator it=word.begin();
    while(it !=word.end() && ((*it>=97 && *it<=122)||(*it>=65 && *it<=90)))
    {
        *it=tolower(*it);
        if(currentNode->children[*it - CASE]==NULL)
        {
            currentNode->children[*it - CASE]=new Node();
            currentNode->children[*it - CASE]->parent= currentNode;
        }

        currentNode= currentNode->children[*it - CASE];
        ++it;
    }

    ++(currentNode->occurences);
    //cout<<"fuck";
}

int Search(Node* trieTree, vector<char> word)
{   int j;
    vector<char>::iterator it=word.begin();

    while(it != word.end())
    {
        if(trieTree->children[*it - CASE] != NULL)
        {
            trieTree=trieTree->children[*it - CASE];
            ++it;
        }
        else
            return 0;
    }
    j=trieTree->occurences;

    if(trieTree->occurences)
        return j;
    else
        return 0;
}

void PreOrderPrint(Node* trieTree, Node* trieTree_2, vector<char> word)
{
    if(trieTree->occurences && trieTree->parent)
    {
        for(vector<char>::iterator it= word.begin(); it != word.end(); ++it)
        {
            //cout<<*it;
        }
        //cout<<" "<<trieTree->occurences<<endl;
        int TT2occ;
        TT2occ=Search(trieTree_2, word);

        innerAB=innerAB+(trieTree->occurences*TT2occ);
        innerAA=innerAA+(trieTree->occurences*trieTree->occurences);
    }

    for(int i=0; i<AlPHABET_SIZE; i++)
    {
        if(trieTree->children[i] != NULL)
        {
            word.push_back(CASE + i);
            PreOrderPrint(trieTree->children[i], trieTree_2, word);
            word.pop_back();
        }
    }
}

void finish_with_error(MYSQL *con)
{
  cout<<mysql_error(con)<<endl;
  mysql_close(con);
  exit(1);
}

int main()
{
     MYSQL *con = mysql_init(NULL);
     int k=0;

      if (con == NULL)
      {
          cout<<"mysql_init() failed\n";
          exit(1);
      }

      if (mysql_real_connect(con, "localhost", "root", "",
              "research_papers", 0, NULL, 0) == NULL)
      {
          finish_with_error(con);
      }

      if (mysql_query(con, "SELECT * FROM tbl_uploads"))
      {
          finish_with_error(con);
      }

      MYSQL_RES *result = mysql_store_result(con);

      if (result == NULL)
      {
          finish_with_error(con);
      }

      int num_fields = mysql_num_fields(result);
      //cout<<num_fields;
      MYSQL_ROW row;

      while ((row = mysql_fetch_row(result)))
      {
          for(int i = 0; i < num_fields; i++)
          {
              if(row[i])
              {
                 if(i==1)
                 cout<<row[i]<<" ";
                 if(i==4)
                 {
                      vector<char> word;
                      char* vec;
                      vec=row[i];
                      //cout<<vec;
                      //vec.push_back(*row[i]);
                      //cout<<*chava;
                      Node* trieTree= new Node();
                      Node* trieTree_2= new Node();
                      const int SIZE = 1000;
                      string words[SIZE];
                      string str;
                      int j;

                      //cout <<endl<< "Start reading from a file 1-" <<endl;

                      ifstream fin("C:\\Users\\HP\\Documents\\faint.txt");
                      for (int l = 0; (fin >> str) && (l < SIZE); ++l) // Will read up to eof() and stop at every
                      {                                                // whitespace it hits. (like spaces!)

                        //cout << "This string is " <<l<<" "<<str<<endl;
                        words[l] = str;
                        j=l;
                      } // We now also need to stop before we hit the no define array space.
                      fin.close();

                      cout << endl;
                      for (int m = 0; m <= j; ++m)
                        {
                            InsertNode(trieTree,words[m]);
                        }

                      //cout << "Start reading from a file 2-" <<endl;

                      ifstream fin_2(row[i]);
                      for (int n = 0; (fin_2 >> str) && (n < SIZE); ++n)
                      {
                        //cout << "This string is " <<n<<" "<<str<<endl;
                        words[n] = str;
                        j=n;
                      }
                      fin_2.close();

                      cout << endl;
                      for (int p = 0; p <= j; ++p)
                        {
                            InsertNode(trieTree_2,words[p]);
                        }

                      PreOrderPrint(trieTree, trieTree_2, word);
                      //cout<<"Inner Product of document 1 with document 1 is-"<<innerAA<<"\n";
                      //cout<<"Inner Product of document 1 with document 2 is-"<<innerAB<<"\n";
                      innerBB=innerAA;
                      innerAA=0;
                      innerAB=0;
                      PreOrderPrint(trieTree_2, trieTree, word);
                      //cout<<"Inner Product of document 2 with document 2 is-"<<innerAA<<"\n";
                      //cout<<"Inner Product of document 1 with document 2 is-"<<innerAB<<"\n";
                      cout<<"Document Similarity measure is- "<<acos(innerAB/sqrt(innerAA*innerBB))<<"\n";
                      cout<<"\nDocument 1 and 2 are "<<100-(acos(innerAB/sqrt(innerAA*innerBB))*200)/3.141592654<<" % similar\n";
                      k++;
                      innerAB=0, innerAA=0, innerBB=0;
                      cout<<"---------------------------------------------------------------------------------------------";
                 }
              }

              else
                cout<<"NULL";
          }
              cout<<"\n";
      }

      mysql_free_result(result);
      mysql_close(con);



    /*if (mysql_query(con, "INSERT INTO author_info VALUES(3,'Conan','Germany')"))
    {
      finish_with_error(con);
    }*/



    return 0;
}
