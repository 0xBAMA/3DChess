typedef enum contents_t{

    empty,
    invalid,

    whitepawn,    blackpawn,
    whiteknight,  blackknight,
    whitebishop,  blackbishop,
    whiterook,    blackrook,
    whitequeen,   blackqueen,
    whiteking,    blackking

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
