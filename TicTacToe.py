from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.widget import Widget
from kivy.lang import Builder

from kivy.uix.button import Button

from kivy.uix.label import Label

from ai.ttt import State, minimax_search

from kivy.properties import ObjectProperty

class TTTButton(Button):
    def __init__(self, x_grid, y_grid, *args, **kwargs):
        super(TTTButton, self).__init__(*args, **kwargs)
        self.x_grid = x_grid
        self.y_grid = y_grid

class TicTacToeGame(Widget):
    game_space = ObjectProperty(None)
    toolbar = ObjectProperty(None)
    ai_first_button = ObjectProperty(None)
    new_game = ObjectProperty(None)
    grid = ObjectProperty(None)

    def initialize(self, app):
        self.state = State()
        self.ai_first_button.bind(on_press = self.ai_plays_first)
        self.new_game.bind(on_press = self.start_new_game)
        self.state_to_grid()
        self.app = app

    def start_new_game(self, button):
        # TODO: avoid this method by moving TicTacToeGame beneauth a mother game method.
        self.app.stop()
        TTTApp().run() # start a new game!

    def state_to_grid(self):
        self.grid.clear_widgets()
        board = self.state.board
        for y, row in enumerate(board):
            for x, piece in enumerate(row):
                if piece == 0:
                    button = TTTButton(x, y, text=str(piece))
                    button.bind(on_press = self.human_plays)
                    self.grid.add_widget(button)
                else:
                    widget = Label(text=str(piece))
                    self.grid.add_widget(widget)

    def do_nothing(self, button):
        '''A disabled button callback used for when game is over.
        '''
        pass

    def check_win(self):
        winning_player = self.state.check_win() # returns 1 or 2 if either of these players won or returns 0 if
                                                # neither player won in the case of a tie
        if winning_player:
            self.human_plays = self.do_nothing

            self.game_over.size_hint = (1., 0.3) #

            if winning_player == self.state.min_piece:
                # if this case is true, the human won!??!
                # Wow, this should never happen! let the player know they discovered a bug in my code.
                winning_text = 'Congradulations, you won.  This should never happen and you have just discovered a bug in my code.  Please send an email to me at chetweger@gmail.com describing how this happened!'
            elif winning_player == self.state.max_piece:
                # ai won
                winning_text = 'You lost.  Better luck next time.'
            else:
                winning_text = 'The game ended in a tie.'
            self.game_over.text = winning_text
            return True
        return False

    def human_first(self):
        self.state.max_piece = 2
        self.state.min_piece = 1
        self.state.next_piece = 2

    def ai_first(self):
        self.state.max_piece = 1
        self.state.min_piece = 2
        self.state.next_piece = 1

    def human_plays(self, button):
        if self.state.max_piece == -1:
            # we know that the human is playing first!
            # therefore the AI will be player 2 (try to maximize utility of 2)
            self.toolbar.remove_widget(self.ai_first_button) # once ai plays first, AI can't play first again.
            self.human_first()

        self.state.board[button.y_grid][button.x_grid] = self.state.min_piece
        ai_won = self.check_win() # check if the human just won
        if ai_won:
            self.state_to_grid()
            return
        self.state = minimax_search(self.state)
        self.check_win() # check if the AI just won
        self.state.next_piece = self.state.max_piece
        self.state_to_grid()

    def ai_plays_first(self, button):
        self.toolbar.remove_widget(button) # once ai plays first, AI can't play first again.
        self.ai_first()
        self.state = minimax_search(self.state)
        self.state.next_piece = self.state.max_piece
        self.state_to_grid()

class TTTApp(App):
    def build(self):
        tic_tac_toe_game = TicTacToeGame()
        tic_tac_toe_game.initialize(self)
        game_space = tic_tac_toe_game.game_space
        return game_space

if __name__ == '__main__':
    TTTApp().run()
