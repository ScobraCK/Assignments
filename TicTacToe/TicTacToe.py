import tkinter as tk
from tkinter import messagebox

class TicTacToe:
    def __init__(self):
        # Initialize the game board as a 1-dimensional list
        self.game_board = [' ', ' ', ' ',
                           ' ', ' ', ' ',
                           ' ', ' ', ' ']
        self.depth=0
        self.current_player = 'X'  # track current player in class

    def reset(self):
        self.game_board = [' ', ' ', ' ',
                           ' ', ' ', ' ',
                           ' ', ' ', ' ']
        self.depth=0
        self.current_player = 'X'

    def empty_cells(self):
        # Find and return a list of empty cells on the board
        cells = []
        for x, cell in enumerate(self.game_board):
            if cell == ' ':
                cells.append(x)
        return cells

    def valid_move(self, x):
        # Check if a move is valid in the current game state
        return x in self.empty_cells()

    def move(self, x):
        # Make a move on the board at position x with the given player
        if self.valid_move(x):
            self.game_board[x] = self.current_player
            self.depth += 1
            self.current_player = ('O' if self.current_player == 'X' else 'X')
            return True
        return False

    def draw(self):
        # Display the current game board
        for i, cell in enumerate(self.game_board):
            if i % 3 == 0:
                print('\n----------------')
            print('|', cell, '|', end='')
        print('\n----------------')

    def evaluate(self):
        # Evaluate the current game board and return a score
        if self.check_win('X'):
            score = 10
        elif self.check_win('O'):
            score = -10
        else:
            score = 0
        return score

    def check_win(self, player):
        # Check if the player has won the game
        win_conf = [
            [self.game_board[0], self.game_board[1], self.game_board[2]],
            [self.game_board[3], self.game_board[4], self.game_board[5]],
            [self.game_board[6], self.game_board[7], self.game_board[8]],
            [self.game_board[0], self.game_board[3], self.game_board[6]],
            [self.game_board[1], self.game_board[4], self.game_board[7]],
            [self.game_board[2], self.game_board[5], self.game_board[8]],
            [self.game_board[0], self.game_board[4], self.game_board[8]],
            [self.game_board[2], self.game_board[4], self.game_board[6]],
        ]
        return [player, player, player] in win_conf
    
    def is_full(self):
        return not self.empty_cells()

    def game_over(self):
        # Check if the game is over (either player X or O has won)
        # or if draw
        return self.check_win('X') or self.check_win('O') or self.is_full()

    def minimax_ab(self, max_player, alpha=-10000, beta=10000):
        pos = -1
        if self.depth == 8 or self.game_over():  # switched depth count to go up
            return -1, self.evaluate()
        
        current = self.current_player
        
        if max_player:
            value = -10000
            for p in self.empty_cells():
                self.move(p)  # changes board while also changing current player
                _, v = self.minimax_ab(False, alpha, beta)
                self.game_board[p] = ' '  # revert board
                self.current_player = current  # revert current player
                if v > value:
                    value = v
                    pos = p  # Update pos when a better move is found
                alpha = max(alpha, value)
                if alpha >= beta:
                    break
            return pos, alpha
        else:
            value = 10000
            for p in self.empty_cells():
                self.move(p)
                _, v = self.minimax_ab(True, alpha, beta)
                self.game_board[p] = ' '
                self.current_player = current
                if v < value:
                    value = v
                    pos = p  # Update pos when a better move is found
                beta = min(beta, value)
                if alpha >= beta:
                    break
            return pos, beta

class TicTacToeGUI:
    def __init__(self, root, game):
        self.root = root
        self.root.title("Tic-Tac-Toe")
        self.game = game

        self.buttons = []
        for i in range(9):
            button = tk.Button(root, text='', width=10, height=3, command=lambda i=i: self.make_move(i))
            button.grid(row=i // 3, column=i % 3)
            self.buttons.append(button)


        self.toggle_button = tk.Button(root, text="Player Start", command=self.toggle_start)
        self.toggle_button.grid(row=3, column=1)

        self.start_button = tk.Button(root, text="New Game", command=self.start_game)
        self.start_button.grid(row=3, column=2)

        self.game_started = False  # Track whether the game has started
        self.computer_move = 'O'
        self.player_start = True
        self.last_move = 'O' # to keep track of winner
    
    def toggle_start(self):
        if not self.game_started:
            self.player_start = False if self.player_start else True
            self.computer_move = 'X' if self.player_start else 'O'
            start_text = "Player Start" if self.player_start else "Computer Start"
            self.toggle_button.config(text=start_text)

    def start_game(self):
        # Reset the game state and GUI when starting a new game
        self.game_started = False  # stops initiated game
        self.last_move = 'O'  # start is always X
        self.computer_move = 'O' if self.player_start else 'X'
        self.game.reset()  # resets TicTacToe class
        self.update_board()
        self.game_started = True

        # Enable buttons for player moves
        for button in self.buttons:
            button.config(state=tk.NORMAL)

        if not self.player_start:
            computer_move, _ = game.minimax_ab(not self.player_start)
            game.move(computer_move)
            self.update_board()


    def make_move(self, index):
        if self.game_started:
            if self.game.move(index):
                self.update_board()  # game over check in update board
                if self.game_started:  # if game ends after player moves
                    computer_move, _ = game.minimax_ab(not self.player_start)
                    game.move(computer_move)
                    self.update_board()        

    def update_board(self):
        for i in range(9):
            self.buttons[i].config(text=self.game.game_board[i])

        # Disable buttons when the game is over
        if not self.game_started or self.game.game_over():
            for button in self.buttons:
                button.config(state=tk.DISABLED)

        # only check if game has started
        if self.game_started:
            # update last move
            self.last_move = ('X' if self.last_move == 'O' else 'O')

            # check game over
            if self.game.game_over():
                if self.game.evaluate() == 0:
                    messagebox.showinfo("Tic-Tac-Toe", "It's a draw!")
                else:
                    if self.computer_move == self.last_move:
                        messagebox.showinfo("Tic-Tac-Toe", f"Computer({self.last_move}) wins!")
                    else:
                        messagebox.showinfo("Tic-Tac-Toe", f"Player({self.last_move}) wins!")
                self.game_started = False


if __name__ == "__main__":
    root = tk.Tk()
    game = TicTacToe()
    gui = TicTacToeGUI(root, game)
    root.mainloop()
