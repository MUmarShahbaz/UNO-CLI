#include <string>
#include <iostream>

class Card {
public:
    Card() : number(0), color(""), null(true) {}
    Card(int number, std::string color) : number(number), color(color), null(false) {}
    std::string text() const {
        std::string num_text;
        switch (number) {
        case 10: num_text = "Skip"; break;
        case 11: num_text = "Reverse"; break;
        case 12: num_text = "Draw Two"; break;
        case 13: num_text = "Wild"; break;
        case 14: num_text = "Draw Four"; break;
        default: num_text = std::to_string(number); break;
        }
        return color + "\t" + num_text;
    }
    bool null;
    int number;
    std::string color;
};

class Node {
public:
    Node() : card(Card()), next(-1) {}
    Node(Card CARD): card(CARD), next(-1) {}
    Node(Card CARD, int NEXT) : card(CARD), next(NEXT) {}
    Card card;
    int next;
};

class List {
public:
    List(int START, Node (&arr)[108]) : start(START), container(arr) {}
    int start;
    Node (&container)[108];

    void print() const {
        if (start == -1) {std::cout << "No List present\n"; return;}
        int index = start;
        std::cout << container[index].card.text() << "\n";
        while (container[index].next != -1) {
            index = container[index].next;
            std::cout << container[index].card.text() << "\n";
        }
    }

    void print_with_index() const {
        if (start == -1) {std::cout << "No List present\n"; return;}
        int index = start;
        std::cout << "[" << index << "]\t" << container[index].card.text() << "\n";
        while (container[index].next != -1) {
            index = container[index].next;
            std::cout << "[" << index << "]\t" << container[index].card.text() << "\n";
        }
    }
};