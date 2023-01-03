#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include <set>

struct Node
{
    std::string data;
    Node* left;
    Node* right;
};

void compareMatrixWithTree(std::vector<std::vector<std::string>> matrix, Node* parent);

std::vector<std::string> readFiles(std::string filename, std::vector<std::string> collectionPostingLists)
{
    std::ifstream file1;
    std::string buff;
    std::string word;

    file1.open(filename);

    while (getline(file1, buff))
    {
        for (auto& c : buff)
        {
            if (c > 33 && c < 127)
            {
                word += c;
            }
            else
            {
                if (word != "")
                    collectionPostingLists.push_back(word);
                word = "";
            }
        }
        if (buff.size() > 0 && buff.at(buff.size() - 1) > 33) // in case of one word on row wtihout space
            collectionPostingLists.push_back(word);
        word = "";
    }
    file1.close();
    return collectionPostingLists;
}

Node* getNewNode(std::string data)
{
    Node* bst = new Node();
    bst->data = data;
    bst->left = bst->right = NULL;
    return bst;
}
Node* insertNode(Node* parent, std::string data)
{
    if (parent == NULL)
    {
        parent = getNewNode(data);
        return parent;
    }
    else if (parent->data > data)
    {
        parent->left = insertNode(parent->left, data);
        return parent;
    }
    else
    {
        parent->right = insertNode(parent->right, data);
        return parent;
    }
}

void inorder(Node* parent, std::vector<std::vector<std::string>> matrix)
{
    if (parent == NULL)
    {
        return;
    }
    else
    {
        inorder(parent->left, matrix);
        std::cout << parent->data << " -> ";
        std::cout << std::endl;
        compareMatrixWithTree(matrix, parent);
        inorder(parent->right, matrix);
    }
}

Node* insertIntoTree(Node* parent, std::set<std::string> set1)
{
    for (auto& str : set1)
    {
        parent = insertNode(parent, str);
    }
    return parent;
}

std::vector<std::vector<std::string>> createPage()
{
    std::string buff;
    std::string token;
    std::ifstream file;

    int line = 0;
    int word = 0;
    file.open("index.html");
    while (getline(file, buff))
    {
        line++;
    }
    file.close();

    std::vector<std::vector<std::string>> matrix(line);
    file.open("index.html");
    while (getline(file, buff))
    {
        for (auto& c : buff)
        {
            if (c > 33 && c < 127)
            {
                token += c;
            }
            else
            {
                if (token != "")
                    matrix[word].push_back(token);
                token = "";
            }
        }
        if (buff.size() > 0 && buff.at(buff.size() - 1) > 33) // in case of one word on row wtihout space
            matrix[word].push_back(token);
        token = "";
        word++;

    }
    file.close();

    return matrix;
}
void compareMatrixWithTree(std::vector<std::vector<std::string>> matrix, Node* parent)
{
    int dublicate = 1;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            //std::cout << matrix[i][j] << " ";
            //std::cout << parent->data << std::endl;
            if (parent->data == matrix[i][j])
            {
                //std::cout << parent->data << " -> ";
                std::cout << "(" << dublicate << ")" << i + 1 << ", " << j + 1 << std::endl;
                dublicate++;
            }
        }
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<std::string> words;
    std::set<std::string> uniqueWords;
    words = readFiles("index.html", words);

    for (auto& word : words)
    {
        uniqueWords.insert(word);
    }

    Node* parent = NULL;
    parent = insertIntoTree(parent, uniqueWords);
    std::vector<std::vector<std::string>> matrix = createPage();
    inorder(parent, matrix);
}