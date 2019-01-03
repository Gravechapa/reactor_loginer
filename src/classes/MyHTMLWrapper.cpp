#include "MyHTMLWrapper.hpp"
#include <stdexcept>


MyHTMLWrapper::MyHTMLWrapper(std::string &html)
{
    _myhtml = myhtml_create();
    if (!_myhtml)
    {
        throw std::runtime_error("Can't create myhtml");
    }
    auto status = myhtml_init(_myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    if (status != MyHTML_STATUS_OK)
    {
        throw std::runtime_error("Can't init myhtml, status: " + std::to_string(status));
    }

    _tree = myhtml_tree_create();
    if (!_tree)
    {
        throw std::runtime_error("Can't create myhtml tree");
    }
    status = myhtml_tree_init(_tree, _myhtml);
    if (status != MyHTML_STATUS_OK)
    {
        throw std::runtime_error("Can't init myhtml tree, status: " + std::to_string(status));
    }

    status = myhtml_parse(_tree, MyENCODING_UTF_8, html.c_str(), html.size());
    if (status != MyHTML_STATUS_OK)
    {
        throw std::runtime_error("Can't parse html, status: " + std::to_string(status));
    }
}

MyHTMLWrapper::~MyHTMLWrapper()
{
    myhtml_tree_destroy(_tree);
    myhtml_destroy(_myhtml);
}

MyHTMLWrapper::Collection MyHTMLWrapper::getNodesByAttributeValue(std::string key, std::string value)
{
    mystatus_t status;
    myhtml_collection_t* collection = myhtml_get_nodes_by_attribute_value(_tree, NULL, NULL, false,
                                                                          key.c_str(), key.size(),
                                                                          value.c_str(), value.size(),
                                                                          &status);
    if (status != MyHTML_STATUS_OK || !collection)
    {
        throw std::runtime_error("Can't create myhtml collection, status"+ std::to_string(status));
    }
    return MyHTMLWrapper::Collection(collection);
}


MyHTMLWrapper::Collection::~Collection()
{
    myhtml_collection_destroy(_collection);
}

std::string MyHTMLWrapper::Collection::getAttributeValue(std::string key, size_t elementNumber) const
{
    if (_collection->length < elementNumber)
       {
           throw std::runtime_error("Myhtml node collection: element number "
                                    + std::to_string(elementNumber) + " is missing");
       }
    auto attr = myhtml_attribute_by_key(_collection->list[elementNumber], key.c_str(), key.size());
    if (!attr)
    {
        throw std::runtime_error("Attribute with key \"" + key + "\" not found");
    }

    size_t attrValueSize;
    const char* attrValue = myhtml_attribute_value(attr, &attrValueSize);
    if (!attrValue)
    {
        throw std::runtime_error("Attribute value is empty");
    }
    return std::string(attrValue, attrValueSize);
}
