//WORDPEDIA USING TRIE

#include <iostream>
#include <string.h>
#include <strings.h>
#include <vector>
#include <fstream>
#include <conio.h>
#include <unistd.h>
#include "json.hpp"
using namespace std;

using json = nlohmann::json;

#define RESET "\033[0m"             // Reset to default color
#define RED "\033[31m"              // Red text
#define GREEN "\033[32m"            // Green text
#define YELLOW "\033[33m"           // Yellow text
#define MAGENTA "\033[1m\033[35m"   //Magenta text
#define BLUE    "\033[1m\033[34m"   //Bold Blue
#define CYAN "\033[36m"             // Cyan text
#define BOLDCYAN "\033[1m\033[36m"  //Bold Cyan



json load_json_file(const std::string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw ("Failed to open JSON file: " + filename);
    }

    json data;
    file >> data;
    return data;
}

void tab()
{
    cout <<"\n\t\t";
}

void line()
{
    cout <<MAGENTA <<"  *********************************************************************************************************************************************************" <<RESET <<endl;
}

void title()
{
    line();
    cout <<endl;
    tab(); cout <<BOLDCYAN <<"\t\t\t\t\t\t WORDPEDIA" <<RESET <<endl;
    cout <<endl;
    line();

    tab(); cout <<"\t\t\t\t Type " <<RED <<"QUIT!" <<RESET <<" to quit from Wordpedia" <<endl;
    tab(); cout <<"\t\t\t\t Type " <<GREEN <<"HELP?" <<RESET <<" to know about how to work with Wordpedia" <<endl <<endl;
}

class TrieNode
{
 public:
    TrieNode* children[28];  //26 alphabets + ' ' + -
    bool is_end_of_word;
    string meaning;

    TrieNode()
    {
        is_end_of_word=false;
        for (int i = 0; i < 28; i++)
        {
            children[i] = NULL;
        }
    }
};

class Trie
{
    TrieNode* root;

  public:
    Trie()
    {
        root = new TrieNode();
    }

    void insert(string word, string meaning)
    {
        TrieNode* node = root;
        for (char c : word)
        {
            int index;
            if(c == '-')
                index = 26;
            else if(c == ' ')
                index = 27;
            else
            {
                index = c - 'a';
            }

            if (node->children[index]==NULL)
            {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->is_end_of_word = true;
        node->meaning = meaning;
    }

    string search(string word)
    {
        TrieNode* node = root;
        for (char c : word)
        {
            int index;
            if(c == '-')
                index = 26;
            else if(c == ' ')
                index = 27;
            else
            {
                index = c - 'a';
            }

            if (node->children[index]==NULL)
            {
                return "Word not found.";
            }
            node = node->children[index];
        }

        if (node->is_end_of_word)
            return node->meaning;
        else
            return "Word not found.";
    }



    vector<string> check_match(string& word)
    {
        vector<string> suggestions;
        string match = "";
        TrieNode* node = root;
        for (char c : word)
        {
            int index;
            if(c == '-')
                index = 26;
            else if(c == ' ')
                index = 27;
            else
            {
                index = c - 'a';
            }

            if (node->children[index]==NULL)
            {
                break;
            }
            match = match + c;
            node = node->children[index];
        }
        find_words_with_prefix(node, match, suggestions);
        return suggestions;
    }

  private:
    int count;
    void find_words_with_prefix(TrieNode* node, string match, vector<string>& suggestions)
    {
        if (node->is_end_of_word)
        {
            if(suggestions.size()!=5)
                suggestions.push_back(match);
        }
        for (int i = 0; i < 28; i++)
        {
            if (node->children[i])
            {
                char c;
                if(i == 26)
                    c = '-';
                else if(i == 27)
                    c = ' ';
                else
                    c = 'a' + i;

                find_words_with_prefix(node->children[i], match + c, suggestions);
            }
        }
    }
};


int main()
{

    // Load the JSON file
    json dictionary = load_json_file("dictionary.json");

    Trie trie;

    // load dictionary into trie
    for (auto& word : dictionary.items())
    {

        trie.insert(word.key(), word.value());
    }

    while(true)
    {
        string word;
        title();

        start:

        tab(); cout <<YELLOW <<"\t Enter the word you want to search: " <<RESET;
        getline(cin,word);

        if (word == "QUIT!")
        {
            cout <<"\n\n\n";
            tab(); cout <<BOLDCYAN <<"\t\t\t\t Thank you for using Wordpedia! Goodbye! ^_^ " <<RESET <<endl <<endl <<endl <<endl;
            break;
        }

        else if (word == "HELP?")
        {
            cout <<"\n\n";
            tab(); cout <<BLUE <<"About: " <<RESET <<endl;
            tab(); cout <<"Wordpedia is a versatile language companion package designed using TRIE!";
            tab(); cout <<"It helps users to find meanings in the dictionary with an efficient retrieval/search method.";
            tab(); cout <<"\n\n\n";
            tab(); cout <<BLUE <<"How to work with Wordpedia?" <<RESET <<endl;
            tab(); cout <<"Just type the word whose meaning you wanna search from the dictionary.";
            tab(); cout <<"It will provide the meaning of that word if it is found.";
            tab(); cout <<"If the word is not found, it will provide suggestions based on partial match found!";
            tab(); cout <<"You can either choose a word from the suggestions or add that word as a new word to the dictionary by providing its meaning!";
            tab(); cout <<"If you wanna quit Wordpedia, type 'QUIT!'.";
        }

        else
        {
           //to change all the letters to lower case
            transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
                return tolower(c);
            });


            string meaning = trie.search(word);

            if (meaning != "Word not found.")
            {
                cout <<"\n";
                tab(); cout <<CYAN <<"Word: " <<RESET << word <<endl <<endl;
                tab(); cout <<CYAN <<"Meaning: " <<RESET << meaning << endl;
            }

            else
            {
                vector<string> suggestions = trie.check_match(word);
                if (!suggestions.empty())
                {

                    tab(); cout <<CYAN <<"\t\t No meaning found for '" <<RESET <<word <<CYAN <<"'. " <<RESET <<endl;
                    tab(); cout <<GREEN <<"\t\t\t Suggestions:" <<RESET ;

                    for (const string& suggestion : suggestions)
                    {
                        tab(); cout <<"\t\t\t  " <<suggestion;
                    }

                    cout <<endl;
                    tab(); cout <<YELLOW <<"\t\t or" <<RESET <<endl;

                    char ch;
                    string mean;
                    ask:
                    tab(); cout <<CYAN <<"\t\t Do you want to add it to the dictionary (" <<GREEN <<"y" <<CYAN <<"/" <<RED <<"n" <<CYAN <<")? " <<RESET;
                    cin >>ch;
                    cin.ignore();

                    if(ch=='Y'||ch=='y')
                    {
                        cout <<endl;
                        tab(); cout <<YELLOW <<"\t\t Word: " <<RESET <<word <<endl;
                        tab(); cout <<YELLOW <<"\t\t Enter its meaning: " <<RESET;
                        getline(cin,mean);
                        trie.insert(word,mean);
                        cout <<"\n\n";
                        tab(); cout <<GREEN <<"\t **Successfully added the word '" <<RESET <<word <<GREEN <<"' to the dictionary!**" <<RESET <<endl;
                    }
                    else if(ch=='n'||ch=='N')
                    {
                        cout <<endl <<endl;
                        goto start;
                    }
                    else
                    {
                        cout <<endl <<endl;
                        tab(); cout <<RED <<"\t\t\tEnter a valid choice!" <<RESET <<endl <<endl;
                        goto ask;
                    }
                    cout <<endl;
                }
                else
                {
                    cout <<RED <<"No suggestions found for '" <<RESET <<word <<RED <<"'." <<RESET <<endl;
                }

            }
        }
        getch();
        system("cls");
    }

    return 0;

}
