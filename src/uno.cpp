#include "mini_classes.h"
#include <random>
#include <algorithm>
#include <cctype>

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

    int play() {
        int order_of_turns = 1;
        int current_player_number = 0;
        List* current_player = &players[current_player_number];
        int winner = -1;
        Card* played;

        std::cout << "\n\nLet the game, begin!!!!\n\n";
        while (winner == -1) {
            std::cout << "Player " << (current_player_number + 1) << "'s turn\n";
            print_for(current_player);
            std::cout << "[-1] Draw a Card and Play it (if possible)\n";
            int card_to_play = -2;
            do {
                std::cout << "Write the index of the card to play : ";
                std::cin >> card_to_play;
                if (card_to_play == -1) { 
                    draw_card(current_player);
                    std::cout << "You drew " << CARDS[current_player->start].card.text() << "\n";
                    if (validate_play(current_player->start)) {
                        std::cout << "Card was playable and now has been used\n";
                        discard_card(current_player->start, current_player);
                        played = &CARDS[discard_pile.start].card;
                    } else {
                        played = nullptr;
                    }
                    break;
                }
                bool player_owns = current_player->search_index(card_to_play);
                bool playable_card = validate_play(card_to_play);
                if (player_owns && playable_card) {
                    discard_card(card_to_play, current_player);
                    played = &CARDS[discard_pile.start].card;
                } else if (!player_owns) {
                    std::cout << "Player doesn't own the card being played\n";
                    card_to_play = -2;
                } else {
                    std::cout << "Card is not playable\n";
                    card_to_play = -2;
                }
            } while (card_to_play == -2);
            perform_action(played, order_of_turns, current_player_number);
            current_player = &players[current_player_number];
            winner = check_empty(current_player) ? current_player_number : -1;
        }
        return winner;
    }

private:
    void perform_action(Card* played, int& order_of_turns, int& current_player_number) {
        if (played == nullptr) {current_player_number = (current_player_number + order_of_turns + no_players) % no_players; return;}
        if (played->reverse()) order_of_turns -= 2*order_of_turns;
        current_player_number = (current_player_number + order_of_turns + no_players) % no_players;
        if (played->skip()) current_player_number += order_of_turns;
        if (played->draw_two()) {
            draw_card(&players[current_player_number]);
            draw_card(&players[current_player_number]);
        }
        if (played->draw_four()) {
            draw_card(&players[current_player_number]);
            draw_card(&players[current_player_number]);
            draw_card(&players[current_player_number]);
            draw_card(&players[current_player_number]);
        }
        if (played->wild() || played->draw_four()) {
            char card_color;
            do {
                std::cout << "Enter which color to use [R, Y, G, B]: ";
                std::cin >> card_color;
                card_color = std::tolower(card_color);
            } while (card_color != 'r' && card_color != 'y' && card_color != 'g' && card_color != 'b');

            switch (card_color) {
                case 'r': played->pseudo_color = PseudoCardColor::Red; break;
                case 'y': played->pseudo_color = PseudoCardColor::Yellow; break;
                case 'g': played->pseudo_color = PseudoCardColor::Green; break;
                case 'b': played->pseudo_color = PseudoCardColor::Blue; break;
            }
        }
    }

    void prepare_and_shuffle() {
        // Prepare Cards
        // Colorless
        for (int i = 0; i < 8; i += 2) {
            CARDS[i] = Node(Card(13, CardColor::None), -1);
            CARDS[i + 1] = Node(Card(14, CardColor::None), -1);
        }

        // Colored
        for (int i = 0; i < 4; i++) {
            // Prepare color
            CardColor current_color;
            switch (i) {
            case 0: current_color = CardColor::Red; break;
            case 1: current_color = CardColor::Yellow; break;
            case 2: current_color = CardColor::Green; break;
            case 3: current_color = CardColor::Blue; break;
            }

            // Calculate starting index for the current color
            int start_index = i * 25 + 8;

            // Place a colored 0, There is only one 0 in each color
            CARDS[start_index] = Node(Card(0, current_color), -1);

            // Place all other colored cards twice
            for (int j = 0; j < 2; j++) {
                start_index += j * 12;
                for (int k = 1; k <= 12; k++) {
                    CARDS[k + start_index] = Node(Card(k, current_color), -1);
                }
            }
        }

        // Shuffle
        std::mt19937 Generator(std::random_device{}());
        std::shuffle(std::begin(CARDS), std::end(CARDS), Generator);

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

    void draw_card(List* player_list) {
        if (check_empty(&draw_pile)) rebuild_draw();
        int player_start = player_list->start;
        player_list->start = draw_pile.start;
        draw_pile.start = CARDS[draw_pile.start].next;
        CARDS[player_list->start].next = player_start;
    }

    void discard_card(int card_index, List* player_list) {
        player_list->remove(card_index);
        CARDS[card_index].next = discard_pile.start;
        discard_pile.start = card_index;
    }

    bool validate_play(int card_index) {
        Card play_card = CARDS[card_index].card;
        Card discard_card = CARDS[discard_pile.start].card;
        bool is_colorless = play_card.color == CardColor::None;
        bool matches_color = play_card.color == discard_card.color || static_cast<int>(play_card.color) == static_cast<int>(discard_card.pseudo_color);
        bool matches_number = play_card.number == discard_card.number;
        bool validate = is_colorless || matches_color || matches_number;
        return validate ? true : false;
    }

    void print_for(List* player_for) {
        std::cout << "Last Card : " << CARDS[discard_pile.start].card.text() << "\n\nYour cards: \n";
        player_for->print_with_index();
    }

    bool check_empty(List* empty_list) {
        if (empty_list->start == -1) return true;
        return false;
    }

    void rebuild_draw() {
        draw_pile.start = CARDS[discard_pile.start].next;
        CARDS[discard_pile.start].next = -1;

        std::vector<int> main_indices = draw_pile.indices();
        std::vector<int> pointers = draw_pile.pointers();

        std::mt19937 Generator(std::random_device{}());
        std::shuffle(std::begin(pointers), std::end(pointers), Generator);

        // Ensure no self-looping
        for (int i = 0; i < draw_pile.length(); i++) {
            if (main_indices[i] == pointers[i]) {
                if (i > 0) {
                    std::swap(pointers[i], pointers[i - 1]);
                } else {
                    std::swap(pointers[i], pointers[i + 1]); // safe because i == 0
                }
            }
        }

        draw_pile.rebuild_list(main_indices, pointers);
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
    std::cout << "Player " << (uno.play() + 1) << " has won!!!";
    return 0;
}