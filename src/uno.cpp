#include "mini_classes.h"
#include <random>

class UNO {
public:
    Node CARDS[108];
    int no_players;
    int no_cards;
    List draw_pile = List(-1, CARDS);
    List discard_pile = List(-1, CARDS);
    List players[4] {List(-1, CARDS), List(-1, CARDS), List(-1, CARDS), List(-1, CARDS)};
    
    UNO(int Players, int Cards) : no_players(Players), no_cards(Cards) {
        prepare_and_shuffle();
        divide_cards();
    }

    void DRAW_CARD(List &PLAYER) {
        draw_card(PLAYER);
    }

    void DISCARD(int card_index, List &PLAYER) {
        discard_card(card_index, PLAYER);
    }

private:
    int get_random(std::mt19937& gen, int min, int max) {
        std::uniform_int_distribution<> distr(min, max);
        return distr(gen);
    }

    void prepare_and_shuffle() {
        // Prepare
        Card temp[108];

        // Colorless
        for (int i = 0; i < 8; i += 2) {
            temp[i] = Card(13, "");
            temp[i + 1] = Card(14, "");
        }

        // Colored
        for (int i = 0; i < 4; i++) {
            // Prepare color
            std::string current_color;
            switch (i) {
            case 0: current_color = "Red"; break;
            case 1: current_color = "Yellow"; break;
            case 2: current_color = "Green"; break;
            case 3: current_color = "Blue"; break;
            }

            // Calculate starting index for the current color
            int start_index = i * 25 + 8;

            // Place a colored 0, There is only one 0 in each color
            temp[start_index] = Card(0, current_color);

            // Place all other colored cards twice
            for (int j = 0; j < 2; j++) {
                start_index += j * 12;
                for (int k = 1; k <= 12; k++) {
                    temp[k + start_index] = Card(k, current_color);
                }
            }
        }

        // Shuffle
        Card Shuffled_Deck[108];
        std::random_device rd;
        std::mt19937 Generator(rd());


        for (int i = 0; i < 108; i++) {
            int card_index;
            do {
                card_index = get_random(Generator, 0, 107);
            } while (temp[card_index].null);

            CARDS[i] = Node(temp[card_index]);

            temp[card_index] = Card();
        }

        draw_pile.start = 0;
    }

    void divide_cards() {
        int current_index;
        for (int i = 0; i < no_players; i++) {
            players[i].start = i * no_cards;
            for (int j = 0; j < no_cards - 1; j++) {
                current_index = i * no_cards + j;
                CARDS[current_index].next = current_index + 1;
            }
        }
        for (int i = no_players; i < 4; i++) { players[i].start = -1; }
        discard_pile.start = no_players * no_cards;
        draw_pile.start = discard_pile.start + 1;
        for (int i = draw_pile.start; i < 107; i++) {
            CARDS[i].next = i + 1;
        }
    }

    void draw_card(List &player_list) {
        int player_start = player_list.start;
        player_list.start = draw_pile.start;
        draw_pile.start = CARDS[draw_pile.start].next;
        CARDS[player_list.start].next = player_start;
    }

    void discard_card(int card_index, List &player_list) {
        if (!player_list.search_index(card_index)) return;
        if (player_list.remove(card_index)) {
            CARDS[card_index].next = discard_pile.start;
            discard_pile.start = card_index;
        }
    }
};

void print_entire_deck(UNO game) {
    for (int i = 0; i < game.no_players; i++) {std::cout << "Player " << (i+1) << ":\n"; game.players[i].print_with_index(); std::cout << "\n\n";}
    std::cout << "Discard Pile:\n";
    game.discard_pile.print_with_index();
    std::cout << "\n\nDraw Pile\n";
    game.draw_pile.print_with_index();
}

UNO start_game() {
    int players;
    do {
        std::cout << "Enter number of players [2-4]: ";
        std::cin >> players;
    } while (players < 2 || players > 4);
    int cards;
    int limit = int((108 - 21) / players);
    do {
        std::cout << "Enter number of cards [7-" << limit << "]: ";
        std::cin >> cards;
    } while (cards < 7 || cards > limit);
    return UNO(players, cards);
}

int main() {
    UNO uno = start_game();
    uno.DRAW_CARD(uno.players[0]);
    uno.DRAW_CARD(uno.players[0]);
    uno.DRAW_CARD(uno.players[0]);
    uno.DRAW_CARD(uno.players[0]);
    uno.DISCARD(8, uno.players[1]);
    uno.DISCARD(3, uno.players[0]);
    uno.DISCARD(9, uno.players[1]);
    print_entire_deck(uno);
    return 0;
}