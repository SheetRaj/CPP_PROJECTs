#include <bits/stdc++.h>
using namespace std;

enum class Color
{
    WHITE,
    BLACK
};

class Piece
{
protected:
    Color color;

public:
    Piece(Color color) : color(color) {}

    virtual ~Piece() = default;

    Color getColor() const
    {
        return color;
    }

    virtual char getSymbol() const = 0;

    virtual bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece *>> &board) const = 0;
};

class Pawn : public Piece
{
public:
    using Piece::Piece;

    char getSymbol() const override
    {
        return color == Color::WHITE ? 'P' : 'p';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece *>> &board) const override
    {
        int direction = (color == Color::WHITE) ? -1 : 1;

        // Move forward
        if (startX + direction == endX && startY == endY && !board[endX][endY])
        {
            return true;
        }

        // Capture diagonally
        if (startX + direction == endX && (startY + 1 == endY || startY - 1 == endY) && board[endX][endY] && board[endX][endY]->getColor() != color)
        {
            return true;
        }

        // Initial two-square move
        if ((color == Color::WHITE && startX == 6 || color == Color::BLACK && startX == 1) && startX + 2 * direction == endX && startY == endY && !board[endX][endY] && !board[startX + direction][startY])
        {
            return true;
        }

        return false;
    }
};

class Rook : public Piece
{
public:
    using Piece::Piece;

    char getSymbol() const override
    {
        return color == Color::WHITE ? 'R' : 'r';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece *>> &board) const override
    {
        if (startX != endX && startY != endY)
        {
            return false;
        }

        if (startX == endX)
        {
            int minY = min(startY, endY);
            int maxY = max(startY, endY);

            for (int y = minY + 1; y < maxY; y++)
            {
                if (board[startX][y])
                {
                    return false;
                }
            }
        }
        else
        {
            int minX = min(startX, endX);
            int maxX = max(startX, endX);

            for (int x = minX + 1; x < maxX; x++)
            {
                if (board[x][startY])
                {
                    return false;
                }
            }
        }
        return true;
    }
};

class Knight : public Piece
{
public:
    using Piece::Piece;
    char getSymbol() const override
    {
        return color == Color::WHITE ? 'N' : 'n';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece *>> &board) const override
    {
        int dx = abs(startX - endX);
        int dy = abs(startY - endY);

        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }
};

class Bishop : public Piece
{
public:
    using Piece::Piece;
    char getSymbol() const override
    {
        return color == Color::WHITE ? 'B' : 'b';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece *>> &board) const override
    {
        if (abs(startX - endX) != abs(startY - endY))
        {
            return false;
        }

        int dx = (endX > startX) ? 1 : -1;
        int dy = (endY > startY) ? 1 : -1;

        int x = startX + dx;
        int y = startY + dy;

        while (x != endX && y != endY)
        {
            if (board[x][y])
            {
                return false;
            }

            x += dx;
            y += dy;
        }
        return true;
    }
};

class Queen : public Piece
{
public:
    using Piece::Piece;

    char getSymbol() const override
    {
        return color == Color::WHITE ? 'Q' : 'q';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece *>> &board) const override
    {
        if (startX == endX || startY == endY)
        {
            return Rook(color).isValidMove(startX, startY, endX, endY, board);
        }
        else if (abs(startX - endX) == abs(startY - endY))
        {
            return Bishop(color).isValidMove(startX, startY, endX, endY, board);
        }
        return false;
    }
};

class King : public Piece
{
public:
    using Piece::Piece;

    char getSymbol() const override
    {
        return color == Color::WHITE ? 'K' : 'k';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, const vector<vector<Piece *>> &board) const override
    {
        return std::abs(startX - endX) <= 1 && std::abs(startY - endY) <= 1;
    }
};

class Board
{
private:
    vector<vector<Piece *>> board;

    // Delete copy constructor and assignment operator
    Board(const Board &) = delete;
    Board &operator=(const Board &) = delete;

public:
    Board()
    {
        reset();
    }

    ~Board()
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                delete board[i][j];
            }
        }
    }

    // Move constructor
    Board(Board &&other) noexcept : board(std::move(other.board)) {}

    // Move assignment operator
    Board &operator=(Board &&other) noexcept
    {
        if (this != &other)
        {
            board = std::move(other.board);
        }
        return *this;
    }

    void reset()
    {
        // Delete existing pieces
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                delete board[i][j];
                board[i][j] = nullptr;
            }
        }

        // Allocate new pieces
        board = vector<vector<Piece *>>(8, vector<Piece *>(8, nullptr));

        for (int i = 0; i < 8; i++)
        {
            board[1][i] = new Pawn(Color::WHITE);
            board[6][i] = new Pawn(Color::BLACK);
        }

        board[0][0] = new Rook(Color::WHITE);
        board[0][7] = new Rook(Color::WHITE);
        board[7][0] = new Rook(Color::BLACK);
        board[7][7] = new Rook(Color::BLACK);

        board[0][1] = new Knight(Color::WHITE);
        board[0][6] = new Knight(Color::WHITE);
        board[7][1] = new Knight(Color::BLACK);
        board[7][6] = new Knight(Color::BLACK);

        board[0][2] = new Bishop(Color::WHITE);
        board[0][5] = new Bishop(Color::WHITE);
        board[7][2] = new Bishop(Color::BLACK);
        board[7][5] = new Bishop(Color::BLACK);

        board[0][3] = new Queen(Color::WHITE);
        board[7][3] = new Queen(Color::BLACK);

        board[0][4] = new King(Color::WHITE);
        board[7][4] = new King(Color::BLACK);
    }

    void display() const
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j])
                {
                    cout << board[i][j]->getSymbol() << ' ';
                }
                else
                {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    bool movePiece(int startX, int startY, int endX, int endY, Color playerColor)
    {
        if (startX < 0 || startX >= 8 || startY < 0 || startY >= 8 || endX < 0 || endX >= 8 || endY < 0 || endY >= 8)
        {
            return false;
        }

        if (board[startX][startY] && board[startX][startY]->getColor() == playerColor && board[startX][startY]->isValidMove(startX, startY, endX, endY, board))
        {
            if (board[endX][endY] && board[endX][endY]->getColor() == playerColor)
            {
                return false;
            }

            board[endX] = std::move(board[endX]);
            board[endX][endY] = std::move(board[startX][startY]);
            return true;
        }
        return false;
    }
};

class Game
{
private:
    Board board;
    Color currentPlayer;

    void switchPlayer()
    {
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }

public:
    Game() : board(), currentPlayer(Color::WHITE) {}

    void start()
    {
        while (true)
        {
            board.display();
            cout << (currentPlayer == Color::WHITE ? "White" : "Black") << "'s turn: ";

            int startX, startY, endX, endY;

            cin >> startX >> startY >> endX >> endY;

            if (board.movePiece(startX, startY, endX, endY, currentPlayer))
            {
                switchPlayer();
            }
            else
            {
                cout << "Invalid move. Try again" << endl;
            }
        }
    }
};

int main()
{
    Game game;
    game.start();

    return 0;
}
