#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>

enum class CardColor {
    Empty,
    Red,
    Yellow,
    Green,
    Blue,
    None
};

enum class PseudoCardColor {
    Empty,
    Red,
    Yellow,
    Green,
    Blue
};

class Card {
public:
    Card() : number(0), color(CardColor::Empty), null(true) {}
    Card(int number, CardColor color) : number(number), color(color), null(false) {}
    std::string text() const {
        std::string color_text;
        switch(color) {
        case CardColor::Empty: color_text = ""; break;
        case CardColor::Red: color_text = "Red"; break;
        case CardColor::Yellow: color_text = "Yellow"; break;
        case CardColor::Green: color_text = "Green"; break;
        case CardColor::Blue: color_text = "Blue"; break;
        case CardColor::None: 
            switch (pseudo_color) {
            case PseudoCardColor::Empty: color_text = ""; break;
            case PseudoCardColor::Red: color_text = "(Red)"; break;
            case PseudoCardColor::Yellow: color_text = "(Yellow)"; break;
            case PseudoCardColor::Green: color_text = "(Green)"; break;
            case PseudoCardColor::Blue: color_text = "(Blue)"; break;
            }
            break;
        }

        std::string num_text;
        switch (number) {
        case 10: num_text = "Skip"; break;
        case 11: num_text = "Reverse"; break;
        case 12: num_text = "Draw Two"; break;
        case 13: num_text = "Wild"; break;
        case 14: num_text = "Draw Four"; break;
        default: num_text = std::to_string(number); break;
        }
        return color_text + "\t" + num_text;
    }

    bool null;
    int number;
    CardColor color;
    PseudoCardColor pseudo_color;

    bool skip() {
        if (number == 10) return true;
        return false;
    }

    bool reverse() {
        if (number == 11) return true;
        return false;
    }

    bool draw_two() {
        if (number == 12) return true;
        return false;
    }

    bool wild() {
        if (number == 13) return true;
        return false;
    }

    bool draw_four() {
        if (number == 14) return true;
        return false;
    }
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

    int length() const {
        if (start == -1) {return 0;}
        int index = start;
        int list_length = 1;
        while (container[index].next != -1) {
            index = container[index].next;
            list_length++;
        }
        return list_length;
    }

    std::vector<int> indices() const {
        std::vector<int> list_indices;
        list_indices.reserve(length());

        if (start == -1) {return list_indices;}
        int index = start;
        list_indices.push_back(index);
        while (container[index].next != -1) {
            index = container[index].next;
            list_indices.push_back(index);
        }
        return list_indices;
    }

    std::vector<int> pointers() const {
        std::vector<int> list_pointers;
        list_pointers.reserve(length());

        if (start == -1) {return list_pointers;}
        int index = start;
        list_pointers.push_back(container[index].next);
        while (container[index].next != -1) {
            index = container[index].next;
            list_pointers.push_back(container[index].next);
        }
        return list_pointers;
    }

    void rebuild_list(std::vector<int> list_indices, std::vector<int> list_pointers) {
        std::unordered_set<int> candidates(list_indices.begin(), list_indices.end());
        for (int i = 0; i < list_indices.size(); i++) {
            container[list_indices[i]].next = list_pointers[i];
            if (list_pointers[i] != -1) {
                candidates.erase(list_pointers[i]);
            }
        }
        start = *candidates.begin();
    }

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

    bool search_index(int search_index) const {
        int index = start;
        if (index == search_index) return true;
        do {
            index = container[index].next;
            if (index == search_index) return true;
        } while (container[index].next != -1);
        return false;
    }

    void remove(int remove_index) {
        if (remove_index == start) {start = container[remove_index].next; container[remove_index].next = -1; return;}
        int index = start;
        Node* parent = nullptr;
        do {
            if (container[index].next == remove_index) { parent = &container[index]; break; }
            index = container[index].next;
        } while (container[index].next != -1);
        if (parent == nullptr) return;
        parent->next = container[remove_index].next;
        container[remove_index].next = -1;
        return;
    }
};