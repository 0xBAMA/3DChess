typedef enum contents_t{

    empty,                          //0
    invalid,                        //1

    whitepawn,    blackpawn,        //2, 3
    whiteknight,  blackknight,      //4, 5
    whitebishop,  blackbishop,      //6, 7
    whiterook,    blackrook,        //8, 9
    whitequeen,   blackqueen,       //10, 11
    whiteking,    blackking         //12, 13

} contents;




class space
{
public:

    space();
    contents get_contents();
    void set_contents(contents input);

    bool is_empty();
    bool get_color();
    void set_color(bool input_color);

private:
    contents my_contents;
    bool color;
};

space::space(){

    my_contents = empty;
}


contents space::get_contents()
{
    return my_contents;
}

void space::set_contents(contents input)
{
    my_contents = input;
}

bool space::is_empty()
{
    return (my_contents == empty);
}

bool space::get_color()
{
    return color;
}

void space::set_color(bool input_color){

    color = input_color;
}
