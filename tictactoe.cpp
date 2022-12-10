#include <iostream>
#include <vector>
using namespace std;

typedef vector<vector<char> > State;
const int SIZE = 3;

// in ra giao diện trò chơi
void renderGame(State state)
{
    cout << " -------------" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << " | " << state[i][j];
        }
        cout << " |" << endl << " -------------" << endl;
    }
}

// người chơi tiếp theo (X hay O đi trước?)
char getNextPlayer(State s)
{
    int countX = 0, countO = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == 'X') countX++;
            else if (s[i][j] == 'O') countO++;
        }
    }
    if (countX == countO) return 'X';
    else return 'O';
}

// Gán X hoặc O vào ma trận
State play(State s, int i, int j, char player)
{
    State newState = State(3, vector<char>(3, ' '));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            newState[i][j] = s[i][j];
        }
    }
    newState[i][j] = player;
    return newState;
}


// kiểm tra ván chơi đã kết thúc chưa (trả về true nếu rồi, ngược lại)
bool isFinalState(State s)
{
    // check rows
    for (int i = 0; i < SIZE; i++)
        if (s[i][0] == s[i][1] && s[i][0] == s[i][2] && s[i][0] != ' ') return true;
    // check cols
    for (int i = 0; i < SIZE; i++)
        if (s[0][i] == s[1][i] && s[0][i] == s[2][i] && s[0][i] != ' ') return true;
    // check diagonals
    if (s[0][0] == s[1][1] && s[0][0] == s[2][2] && s[0][0] != ' ') return true;
    if (s[0][2] == s[1][1] && s[0][2] == s[2][0] && s[0][2] != ' ') return true;
    // check full
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == ' ') return false;
        }
    }
    return true;
}

// kiểm tra điểm ở trạng thái kết thúc (trả về 1 nếu X thắng, 0 nếu hòa, -1 nếu X thua)
int getScoreFinalState(State s)
{
    // check rows
    for (int i = 0; i < SIZE; i++)
        if (s[i][0] == s[i][1] && s[i][0] == s[i][2] && s[i][0] != ' ')
            return s[i][0] == 'X' ? 1 : -1;
    // check cols
    for (int i = 0; i < SIZE; i++)
        if (s[0][i] == s[1][i] && s[0][i] == s[2][i] && s[0][i] != ' ')
            return s[0][i] == 'X' ? 1 : -1;
    // check diagonals
    if (s[0][0] == s[1][1] && s[0][0] == s[2][2] && s[0][0] != ' ')
        return s[0][0] == 'X' ? 1 : -1;
    if (s[0][2] == s[1][1] && s[0][2] == s[2][0] && s[0][2] != ' ')
        return s[0][2] == 'X' ? 1 : -1;
    return 0;
}

// kiểm tra các trường hợp xảy ra trong nước đi kế tiếp
vector<State> getNextStates(State s)
{
    char nextPlayer = getNextPlayer(s);
    vector<State> states;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == ' ') {
                State nextState = play(s, i, j, nextPlayer);
                states.push_back(nextState);
            }
        }
    }
    return states;
}

// Tính nước đi tốt nhất trong các nước còn lại
pair<int, State> getScore(State s)
{
    if (isFinalState(s)) {
        return make_pair(getScoreFinalState(s), s);
    }
    char player = getNextPlayer(s);
    vector<State> states = getNextStates(s);
    int bestScore = -2;
    State bestState;
    for (State ns: states) {
        pair<int, State> p = getScore(ns);
        int score = p.first;
        if (player == 'X') {
            if (bestScore < score || bestScore == -2) {
                bestScore = score;
                bestState = ns;
            }
        } else {
            if (bestScore > score || bestScore == -2) {
                bestScore = score;
                bestState = ns;
            }
        }
    }
    return make_pair(bestScore, bestState);
}


pair<int, int> getComputerPlay(State s)
{
    pair<int, State> p = getScore(s);
    State newState = p.second;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (newState[i][j] != s[i][j]) return make_pair(i, j);
        }
    }
    return make_pair(0, 0);
}

// kiểm tra lỗi trong việc nhập tọa độ đánh dấu ô
bool checkError(State s, int m, int n)
{
    if (m < 0 || m > 2 || n < 0 || n > 2)
        return true;
    else {
        if (s[m][n] == ' ')
            return false;
        else
            return true;
    }
}

int main()
{
    State s = State(3, vector<char>(3, ' '));
    char humanPlayer = ' ', currentPlayer = 'X';
    bool check = false;
    do {
        cout << "Choose your player (X or O): ";
        cin >> humanPlayer;
        if (humanPlayer == 'X' || humanPlayer == 'x') {
            humanPlayer = 'X';
            cout << "You are " << humanPlayer << " player" << endl;
            check = false;
        } else if (humanPlayer == 'O' || humanPlayer == 'o') {
            humanPlayer = 'O';
            cout << "You are " << humanPlayer << " player" << endl;
            check = false;
        } else {
            cout << "Invalid" << endl;
            check = true;
        }
    } while (check);
    
    renderGame(s);
    while (true) {
        int i, j;
        if (humanPlayer == currentPlayer) {
            do {
                cout << "Enter your play: ";
                cin >> i >> j;
                if (checkError(s, i, j))
                    cout << "Invalid" << endl;
                else
                    cout << "You play (" << i << ", " << j << ")" << endl;
            } while (checkError(s, i, j));
            // TODO check legal move
        } else {
            pair<int, int> move = getComputerPlay(s);
            i = move.first;
            j = move.second;
            cout << "I play (" << i << ", " << j << ")" << endl;
        }
        
        s = play(s, i, j, currentPlayer);
        renderGame(s);

        currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
        if (isFinalState(s)) {
            int score = getScoreFinalState(s);
            if (score > 0) {
                cout << "X won" << endl;
            } else if (score < 0) {
                cout << "X lost" << endl;
            } else {
                cout << "A draw" << endl;
            }
            break;
        }
    }
    return 0;
}
