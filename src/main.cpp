#include "player.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Helper function to check if a file exists
inline bool file_exists(const string& name) {
    ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char **argv) {
    // 1. Strict argument checking
    if (argc < 2) {
        cerr << "[jcostd::xDama] FATAL: Missing player number." << endl;
        cerr << "Usage: " << argv[0] << " <1|2>" << endl;
        return 1;
    }

    int player_nr = atoi(argv[1]);
    if (player_nr != 1 && player_nr != 2) {
        cerr << "[jcostd::xDama] FATAL: Player number must be 1 or 2." << endl;
        return 1;
    }

    cout << "[jcostd::xDama] Daemon started. Allocating Player " << player_nr << "..." << endl;

    try {
        Player p(player_nr);

        string initial_board = "boards/board_1.txt";
        string next_board    = "boards/board_2.txt";

        // 2. Auto-generate the board if it's missing (Bulletproof execution)
        if (!file_exists(initial_board)) {
            cout << "[jcostd::xDama] WARN: " << initial_board << " not found." << endl;
            cout << "[jcostd::xDama] Generating default initial board..." << endl;
            p.init_board(initial_board);
        }

        // 3. Core Logic Loop
        cout << "[jcostd::xDama] Loading board state..." << endl;
        p.load_board(initial_board);

        cout << "[jcostd::xDama] Computing optimal move via Minimax..." << endl;
        p.move();

        cout << "[jcostd::xDama] Committing move to " << next_board << "..." << endl;
        p.store_board(next_board, 0);

        // 4. Status checks
        if (p.valid_move()) {
            cout << "[jcostd::xDama] Validation: PASS (Valid Move)" << endl;
        } else {
            cerr << "[jcostd::xDama] Validation: FAIL (Invalid Move)" << endl;
        }

        if (p.wins()) cout << "[jcostd::xDama] Status: VICTORY DETECTED" << endl;
        if (p.loses()) cout << "[jcostd::xDama] Status: LOSS DETECTED" << endl;

    } catch (const player_exception &pe) {
        // Safe exception catching
        cerr << "\n[jcostd::xDama] EXCEPTION CAUGHT!" << endl;
        cerr << "Details: " << pe.msg << endl;
        return 1;
    }

    cout << "[jcostd::xDama] Execution complete. Shutting down." << endl;
    return 0;
}
