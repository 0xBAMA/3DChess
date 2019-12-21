

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

typedef enum contents_t{

    empty,
    invalid,
    whitepawn,
    blackpawn,
    whiteknight,
    blackknight,
    whitebishop,
    blackbishop,
    whiterook,
    blackrook,
    whitequeen,
    blackqueen,
    whiteking,
    blackking

} contents;