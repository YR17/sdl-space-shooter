/*******************************************************************//*
 * This class is responsible for parsing through XML data of state
 * entities
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2015-02-07
 * @modified    2015-02-07
 *********************************************************************/
#ifndef SPACESHOOTER_STATEENTITYPARSER_H_
    #define SPACESHOOTER_STATEENTITYPARSER_H_

#include <string>
#include <vector>

class ApplicationState;

struct EntityXmlStruct;

class StateEntityParser
{
    private:

    public:
        //Constructor
        StateEntityParser();

        //Destructor
        ~StateEntityParser();

        //Methods
        std::vector<EntityXmlStruct> parse(ApplicationState* state,
                                        int parsingLayer);
};

struct EntityXmlStruct
{
    std::string id;
    std::string name;
    std::string type;
    std::string texture;
    std::string function;

    int x;
    int y;
    int width;
    int height;
};

enum ParsingLayer
{
    PARSE_BACKGROUND,
    PARSE_PHYSICAL,
    PARSE_EFFECT,
    PARSE_UI,
};

#endif