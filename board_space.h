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




class space{
public:
    contents get_contents();
    void set_contents();

    bool is_empty();
    bool get_color();

private:
    contents my_contents;
    bool color;
};




contents space::get_contents()
{

}

void space::set_contents()
{

}

bool space::is_empty()
{

}

bool space::get_color()
{

}
