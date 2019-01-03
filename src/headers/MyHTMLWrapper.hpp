#pragma once
#include <myhtml/api.h>
#include <string>

class MyHTMLWrapper
{
public:
    class Collection
    {
    public:
        Collection(myhtml_collection_t *collection): _collection(collection){}
        ~Collection();

        std::string getAttributeValue(std::string key, size_t elementNumber) const;

    private:
        myhtml_collection_t* _collection;
    };

    MyHTMLWrapper(std::string &html);
    ~MyHTMLWrapper();

    Collection getNodesByAttributeValue(std::string key, std::string value);

private:
    myhtml_t* _myhtml;
    myhtml_tree_t* _tree;
};
